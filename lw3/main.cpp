/*
   13. Имеются  расписания вылетов самолетов в  ряде  аэропор-
тов.  Требуется по  начальному  и  конечному  пунктам  методом
поиска  в глубину сформировать и выдать дерево возможных путей.
Проиллюстрировать этапы поиска (9).
    Студент ИПС: Милочкин А.А
    macOS 13.5.2
    Среда : VS code version 1.82
    Коомпилятор: gcc c++
*/

#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>

struct Flight
{
    std::string from;
    std::string to;
    Flight(const std::string &from, const std::string &to) : from(from), to(to) {}
};

struct Tree
{
    std::string airport;
    int level;
    Tree *parent;
    std::vector<Tree *> child;
};

std::vector<std::vector<std::string> >
dfs(std::map<std::string, std::vector<Flight> > &schedule,
    const std::string &currentAirport, const std::string &destination,
    std::set<std::string> &visited, std::vector<std::string> &currentPath,
    std::vector<std::vector<std::string> > &allPaths)
{
    visited.insert(currentAirport);
    currentPath.push_back(currentAirport);
    std::cout << "Текущий аэропорт: " << currentAirport << std::endl;
    if (currentAirport == destination)
    {
        std::cout << "Путь найден: ";
        allPaths.push_back(currentPath);
        for (std::vector<std::string>::const_iterator it = currentPath.begin();
            it != currentPath.end(); ++it)
        {
            const std::string &airport = *it;
            std::cout << airport << " ";
        }
        std::cout << std::endl;
    }
    else
    {
        std::cout << "Путь не найден" << std::endl;
        for (std::vector<Flight>::const_iterator it =
            schedule[currentAirport].begin();
            it != schedule[currentAirport].end(); ++it)
        {
            const Flight &flight = *it;
            if (visited.find(flight.to) == visited.end())
            {
                if (currentAirport != destination)
                {
                    std::cout << "Исследование вылета из: " << flight.to << std::endl;
                }
                dfs(schedule, flight.to, destination, visited, currentPath, allPaths);
            }
        }
    }
    visited.erase(currentAirport);
    currentPath.pop_back();
    return allPaths;
}

std::map<std::string, std::vector<Flight> >
buildScheduleGraph(const std::string &fileName)
{
    std::map<std::string, std::vector<Flight> > schedule;
    std::string line;
    std::ifstream F(fileName);
    if (!F)
    {
        std::cerr << "Файл не найден или не может быть открыт." << std::endl;
        return schedule;
    }
    while (std::getline(F, line))
    {
        std::stringstream ss(line);
        std::string from, to;
        ss >> from >> to;
        schedule[from].emplace_back(from, to);
    }
    F.close();
    return schedule;
}

std::map<std::string, std::vector<Flight> > inputSchedules()
{
    std::string fileName;
    std::map<std::string, std::vector<Flight> > schedule;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    while (true)
    {
        std::cout << "Введите название файла (или 'q' для выхода в меню): ";
        std::getline(std::cin, fileName);
        if (fileName[0] == 'q')
        {
            break;
        }
        schedule = buildScheduleGraph(fileName);
        break;
    }
    return schedule;
}

void printPaths(const std::vector<std::vector<std::string> > &allPaths)
{
    if (allPaths.empty())
    {
        std::cout << "Пути не найдены." << std::endl;
        return;
    }
    else
    {
        std::cout << "Все возможные пути:" << std::endl;
        for (std::vector<std::vector<std::string> >::const_iterator it =
            allPaths.begin();
            it != allPaths.end(); ++it)
        {
            const std::vector<std::string> &path = *it;
            for (std::vector<std::string>::const_iterator innerIt = path.begin();
                innerIt != path.end(); ++innerIt)
            {   
                const std::string &airport = *innerIt;
                std::cout << airport << " ";
            }
            std::cout << std::endl;
        }
    }
}

Tree *createNode(const std::string &airport, int level, Tree *parent)
{
    Tree *node = new Tree;
    node->airport = airport;
    node->level = level;
    node->parent = parent;
    return node;
}

void createTreePath(const std::vector<std::vector<std::string> >& allPaths, Tree* root)
{
    if (allPaths.empty())
    {
        std::cout << "Пути не найдены." << std::endl;
        return;
    }
    else
    {
        for (std::vector<std::vector<std::string> >::const_iterator it = allPaths.begin(); it != allPaths.end(); ++it)
        {
            const std::vector<std::string>& path = *it;
            Tree* currentNode = root;
            for (std::vector<std::string>::const_iterator innerIt = path.begin(); innerIt != path.end(); ++innerIt)
            {
                const std::string& airport = *innerIt;
                Tree* child = nullptr;
                if (currentNode->airport == airport && currentNode == root)
                {
                    child = currentNode;
                }
                else
                {
                    for (std::vector<Tree*>::const_iterator i = currentNode->child.begin(); i != currentNode->child.end(); ++i)
                    {
                        if ((*i)->airport == airport)
                        {
                            child = *i;
                            break;
                        }
                    }
                }
                if (!child)
                {
                    child = createNode(airport, currentNode->level + 1, currentNode);
                    currentNode->child.push_back(child);
                }
                currentNode = child;
            }
        }
    }
}


void printTree(const Tree* node, int level = 0)
{
    if (node)
    {
        for (int i = 0; i < level; i++)
        {
            std::cout << ".";
        }
        std::cout << node->airport << std::endl;
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
    int answer = 0;
    Tree *root = nullptr;
    std::set<std::string> visited;
    std::vector<std::string> currentPath;
    std::string startAirport;
    std::string endAirport;
    std::map<std::string, std::vector<Flight> > schedule;
    std::vector<std::vector<std::string> > allPaths;
    while (answer != 5)
    {
        std::cout << "1. Введите расписание" << std::endl;
        std::cout << "2. Поиск пути" << std::endl;
        std::cout << "3. Показать все возможные пути" << std::endl;
        std::cout << "4. Распечатать дерево возможные пути" << std::endl;
        std::cout << "5. Выход" << std::endl;
        answer = safeInputInt();
        switch (answer)
        {
            case 1:
                schedule = inputSchedules();
                break;
            case 2:
                std::cout << "Введите пункт отправления:";
                std::cin >> startAirport;
                std::cout << "Введите пункт назначения:";
                std::cin >> endAirport;
                dfs(schedule, startAirport, endAirport, visited, currentPath, allPaths);
                cleanTree(root);
                break;
            case 3:
                printPaths(allPaths);
                break;
            case 4:
                root = createNode(startAirport, 0, nullptr);
                createTreePath(allPaths, root);
                printTree(root);
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
