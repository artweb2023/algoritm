#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <set>

int main()
{
    // Инициализация генератора случайных чисел
    std::srand(static_cast<unsigned int>(std::time(0)));

    // Открытие файла для записи
    std::ofstream outputFile("random_numbers.txt");

    // Проверка, удалось ли открыть файл
    if (!outputFile)
    {
        std::cerr << "Не удалось открыть файл для записи." << std::endl;
        return 1; // Возврат с кодом ошибки
    }

    std::set<int> generatedNumbers; // Множество для отслеживания уникальных чисел

    // Генерация 100,000 уникальных случайных чисел и запись их в файл
    while (generatedNumbers.size() < 100000)
    {
        int randomNumber = std::rand() % 100001; // Генерация числа от 0 до 100,000

        // Проверка на уникальность числа
        if (generatedNumbers.find(randomNumber) == generatedNumbers.end())
        {
            generatedNumbers.insert(randomNumber); // Добавление уникального числа в множество
            outputFile << randomNumber << std::endl; // Запись числа в файл
        }
    }

    // Закрытие файла
    outputFile.close();

    std::cout << "100,000 уникальных случайных чисел успешно записаны в файл random_numbers.txt." << std::endl;

    return 0; // Возврат без ошибок
}
