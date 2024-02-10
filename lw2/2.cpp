/*
    1. Изделие задано  с  помощью  дерева.  В  листьях  указаны
    значения  массы  соответствующих деталей.  Масса сборного узла
    определяется как сумма масс составляющих деталей. Требуется:
        1) рассчитать массу всего изделия;
        2) организовать  обход  листьев,  запрашивая новые значения
    массы и сообщая, как при этом меняется масса изделия (8).
    Студент ИПС: Милочкин А.А
    macOS 13.5.2
    Среда : VS code version 1.82
    Коомпилятор: gcc c++
*/

#include <iostream>
#include <fstream>

struct Tree
{
    std::string name;
    int weight;
    int level;
    Tree *parent;
    std::vector<Tree *> child;
};

void updateTotalWeight(Tree *node)
{
    for (size_t i = 0; i < node->child.size(); i++)
    {
        Tree *child = node->child[i];
        updateTotalWeight(child);
        node->weight += child->weight;
    }
}

int inputTree(std::ifstream &F, Tree *&r)
{
    std::string line;
    Tree *currentNode = nullptr;
    int rootLevel = -1;
    while (std::getline(F, line))
    {
        int level = 0;
        while (line[level] == '.')
        {
            level++;
        }
        std::string name;
        int weight = 0;
        size_t pos = line.find_first_not_of('.');
        if (pos != std::string::npos)
        {
            std::string data = line.substr(pos);
            size_t spacePos = data.find(' ');
            if (spacePos != std::string::npos)
            {
                name = data.substr(0, spacePos);
                weight = std::stoi(data.substr(spacePos + 1));
            }
            else
            {
                name = data;
                weight = 0;
            }
        }
        Tree *node = new Tree;
        node->name = name;
        node->weight = weight;
        node->level = level;
        if (level == 0)
        {
            r = node;
        }
        else
        {
            if (level > rootLevel)
            {
                currentNode->child.push_back(node);
            }
            else
            {
                while (currentNode && currentNode->level >= level)
                {
                    currentNode = currentNode->parent;
                }
                if (currentNode)
                {
                    currentNode->child.push_back(node);
                }
            }
        }
        node->parent = currentNode;
        currentNode = node;
        rootLevel = level;
    }
    updateTotalWeight(r);
    return 0;
}

int inputFile(Tree *&r)
{
    std::string fileName;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    while (true)
    {
        std::cout << "Введите название файла (или 'q' для выхода в меню): ";
        std::getline(std::cin, fileName);
        std::ifstream inputFile(fileName);
        if (fileName[0] == 'q')
        {
            break;
        }
        if (!inputFile)
        {
            std::cerr << "Файл не найден или не может быть открыт." << std::endl;
            inputFile.clear();
            continue;
        }
        else if (!inputFile.is_open())
        {
            std::cerr << "Ошибка открытия файла." << std::endl;
            inputFile.clear();
            continue;
        }
        else
        {
            inputTree(inputFile, r);
            break;
        }
    }
    return 0;
}

void printTree(const Tree *node, int level = 0)
{
    if (node)
    {
        for (int i = 0; i < level; i++)
        {
            std::cout << ".";
        }
        std::cout << node->name << " " << node->weight << std::endl;
        for (size_t i = 0; i < node->child.size(); i++)
        {
            printTree(node->child[i], level + 1);
        }
    }
}

void cleanTree(Tree *node)
{
    if (node)
    {
        for (size_t i = 0; i < node->child.size(); i++)
        {
            Tree *child = node->child[i];
            cleanTree(child);
        }
        delete node;
    }
}

void totalSumWeight(const Tree *root)
{
    if (root)
    {
        int totalWeight = root->weight;
        std::cout << "Масса всего изделия: " << totalWeight << std::endl;
    }
    else
    {
        std::cout << "Дерево пустое." << std::endl;
    }
}

void inputLeaves(Tree *node, Tree *root)
{
    if (node != nullptr)
    {
        if (node->child.empty())
        {
            int newWeight;
            std::cout << "Введите новую массу для " << node->name << ": ";
            std::cin >> newWeight;
            int oldWeight = node->weight;
            node->weight = newWeight;
            Tree *parent = node->parent;
            while (parent)
            {
                parent->weight += (newWeight - oldWeight);
                parent = parent->parent;
            }
            totalSumWeight(root);
        }
        for (size_t i = 0; i < node->child.size(); i++)
        {
            inputLeaves(node->child[i], root);
        }
    }
    else
    {
        std::cout << "Дерево пустое" << std::endl;
    }
}

int safeInputInt()
{
    int value;
    while (true)
    {
        if (std::cin >> value)
        {
            break;
        }
        else
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Неверный ввод. Введите целое число: ";
        }
    }
    return value;
}

int main()
{
    int answer;
    Tree *root = nullptr;
    answer = 0;
    while (answer != 5)
    {
        std::cout << "1. Введите дерево" << std::endl;
        std::cout << "2. Распечатать массу изделия" << std::endl;
        std::cout << "3. Ввести новое значение массы деталей или узлов" << std::endl;
        std::cout << "4. Распечатать дерево" << std::endl;
        std::cout << "5. Выход" << std::endl;
        answer = safeInputInt();
        switch (answer)
        {
        case 1:
            inputFile(root);
            break;
        case 2:
            totalSumWeight(root);
            break;
        case 3:
            inputLeaves(root, root);
            break;
        case 4:
            if (root)
            {
                printTree(root);
            }
            else
            {
                std::cout << "Дерево пустое." << std::endl;
            }
            break;
        case 5:
            cleanTree(root);
            break;
        default:
            std::cout << "Неверный номер, введите повторно" << std::endl;
            break;
        }
    }
    return 0;
}