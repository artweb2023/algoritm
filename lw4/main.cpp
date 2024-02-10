/*
    8. Имеется  текстовый  файл  записей  с некоторым  ключевым
    полем. Выполнить в оперативной памяти турнирную сортировку (9).
    Студент ИПС: Милочкин А.А
    macOS 13.5.2
    Среда : VS code version 1.82
    Коомпилятор: gcc c++
*/

#include <fstream>
#include <iostream>
#include <chrono>

const int INF = INT_MAX;

int winner(int pos1, int pos2, int k, std::vector<int> &temp)
{
    int selectPos1 = (pos1 >= k) ? pos1 : temp[pos1];
    int selectPos2 = (pos2 >= k) ? pos2 : temp[pos2];

    return (temp[selectPos1] <= temp[selectPos2]) ? selectPos1 : selectPos2;
}

void processTournament(int &value, int k, std::vector<int> &temp, int i)
{
    if (i <= 1)
    {
        value = temp[temp[1]];
        temp[temp[1]] = INF;
    }
    else
    {
        int j, idx = i / 2;  // Вычисляется индекс для родительского узла и j для потомка текущего узла.
        if (i % 2 == 0 && i < 2 * k - 1)
            j = i + 1;
        else
            j = i - 1;

        temp[idx] = winner(i, j, k, temp);  // определения победителя между текущим узлом и его потомком
        processTournament(value, k, temp, idx);
    }
}

void tournamentSort(std::vector<int> &arr)
{
    int value;
    int k = arr.size();
    std::vector<int> temp(2 * k);
    std::move(arr.begin(), arr.end(), temp.begin() + k);
    for (int i = 2 * k - 1; i > 1; i -= 2)
    {
        int idx = i / 2;
        int j = i - 1;
        temp[idx] = winner(i, j, k, temp);
    }
    for (int i = 0; i < k; i++)
    {
        processTournament(value, k, temp, temp[1]);
        arr[i] = value;     // Текущий победитель добавляется в выходной массив
        int idx = temp[1];  // сохранение корня турнирного дерева
        while (idx > 1)     // проход по  турнирному дереву
        {
            int j, parentIdx = idx / 2;  // индекс для родительского узла
            if (idx % 2 == 0 && idx < 2 * k - 1)
                j = idx + 1;        // левому потомк текущего узла.
            else
                j = idx - 1;        // правому потомк текущего узла.
            temp[parentIdx] = winner(idx, j, k, temp);
            idx = parentIdx;   // Индекс текущего узла обновляется до индекса его родителя
        }
    }
}

void inputFile(std::vector<int> &arr)
{
    std::string fileName;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    while (true)
    {
        std::cout << "Введите название файла (или 'q' для выхода в меню): ";
        std::getline(std::cin, fileName);
        if (fileName == "q")
        {
            break;
        }
        std::ifstream inputFile(fileName);
        if (!inputFile.is_open())
        {
            std::cerr << "Ошибка открытия файла." << std::endl;
            continue;
        }
        int number;
        while (inputFile >> number)
        {
            arr.push_back(number);
        }
        break;
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

void saveInFile(const std::vector<int> &arr)
{
    std::ofstream outputFile("output.txt");
    for (int i = 0; i < arr.size(); ++i)
    {
        outputFile << arr[i] << "\n";
    }
    std::cout << "Результат успешно сохранен в файл: output.txt" << std::endl;
}

int main()
{
    int answer = 0;
    std::vector<int> arr;
    while (answer != 3)
    {
        std::cout << "1. Ввести данные" << std::endl;
        std::cout << "2. Сортировка" << std::endl;
        std::cout << "3. Выход" << std::endl;
        answer = safeInputInt();
        switch (answer)
        {
        case 1:
            inputFile(arr);
            break;
        case 2:
        {
            std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
            tournamentSort(arr);
            std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> duration = std::chrono::duration_cast<std::chrono::duration<double> >(end - start);
            std::cout << "Время выполнения tournamentSort: " << duration.count() << " секунд" << std::endl;
            saveInFile(arr);
            break;
        }
        case 3:
            break;
        default:
            std::cout << "Неверный номер, введите повторно" << std::endl;
            break;
        }
    }
    return 0;
}
