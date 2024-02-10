/*
    В   некотором   компиляторе   ПАСКАЛя  текст  программы
    включает примечания,  выделенные  фигурными  скобками  '{','}'
    либо  парами  символов  '(*'  и  '*)'.  Примечания  могут быть
    вложенными друг в друга.  Если примечание открыто знаком  '{',
    то оно должно быть закрыто знаком '}'.  Аналогично примечание,
    начинающееся с символов '(*'  должно  заканчиваться  символами
    '*)'. Требуется:
        1) проверить правильность вложенности примечаний;
        2) переписать   файл   с   исходным   текстом   так,  чтобы
    отсутствовала  вложенность  комментариев  при  сохранении   их
    содержания  и  в  качестве  ограничивающих  символов  остались
    только  фигурные  скобки.   Учесть   случай,   когда   символы
    примечаний находятся в апострофах. При некорректности  указать
    номера строки и позиции первой ошибки.
    Студент ИПС: Милочкин А.А
    macOS 13.5.2
    Среда : VS code version 1.82
    Коомпилятор: gcc c++
*/

#include <iostream>
#include <fstream>
#include <string>

struct St
{
    char ch;
    int line;
    int pos;
    St *next;
};

void push(St *&ptr, char elem, int elem1, int elem2)
{
    St *top = new St;
    top->ch = elem;
    top->line = elem1;
    top->pos = elem2;
    top->next = ptr;
    ptr = top;
}
void pop(St *&ptr)
{
    if (ptr)
    {
        char elem = ptr->ch;
        St *top = ptr;
        ptr = ptr->next;
        delete top;
    }
}

void clear(St *&ptr)
{
    while (ptr)
    {
        St *top = ptr;
        ptr = ptr->next;
        delete top;
    }
}

int main()
{
    const char openBrace = '{';
    const char closeBrace = '}';
    const char openParen = '(';
    const char closeParen = ')';
    const char ast = '*';
    const char apost = '\'';

    while (true)
    {
        int i;
        int pos;
        int countOpBra;
        int countClBra;
        int countOpParen;
        int countClParen;
        int countApost;
        bool isBraCom = false;
        bool isAstComm = false;
        bool isError = false;
        bool isPrintClBra = false;
        bool isPrintClParen = false;
        int lineNum = 0;
        int errLine = 0;
        St *top = nullptr;
        std::string fileName;
        std::cout << "Введите название файла (или 'q' для выхода): ";
        std::getline(std::cin, fileName);

        if (fileName[0] == 'q')
        {
            break;
        }
        std::ifstream inputFile(fileName);
        std::ofstream outputFile("result.txt");
        if (!inputFile)
        {
            std::cerr << "Файл не найден или не может быть открыт." << std::endl;
            continue;
        }
        else if (!inputFile.is_open())
        {
            std::cerr << "Ошибка открытия файла." << std::endl;
            continue;
        }
        else
        {
            std::string line;
            pos = 0;
            countApost = 0;
            while (std::getline(inputFile, line) && !isError)
            {
                countOpBra = 0;
                countClBra = 0;
                countOpParen = 0;
                countClParen = 0;
                std::string result;
                lineNum++;
                for (i = 0; i < line.length(); i++)
                {
                    if (top == nullptr)
                    {
                        isPrintClBra = false;
                        isPrintClParen = false;
                    }
                    if (line[i] == openBrace)
                    {
                        if (countApost % 2 == 0)
                        {
                            push(top, line[i], lineNum, i);
                            isBraCom = true;
                            countOpBra++;
                            if (countOpBra <= 1 && !isAstComm)
                            {
                                result += line[i];
                            }
                        }
                        else
                        {
                            result += line[i];
                        }
                    }
                    else if (line[i] == closeBrace)
                    {
                        if (countApost % 2 == 0)
                        {
                            countClBra++;
                            if (top != nullptr)
                            {
                                if (top->ch == openBrace)
                                {
                                    pop(top);
                                    isBraCom = false;
                                    if (countClBra <= 1 && !isAstComm && !isPrintClParen)
                                    {
                                        result += line[i];
                                        isPrintClBra = true;
                                    }
                                }
                                else
                                {
                                    isError = true;
                                    errLine = lineNum;
                                    pos = i;
                                }
                            }
                            else
                            {
                                isError = true;
                                errLine = lineNum;
                                pos = i;
                            }
                        }
                        else
                        {
                            result += line[i];
                        }
                    }
                    else if (line[i] == openParen && line[i + 1] == ast)
                    {
                        if (countApost % 2 == 0)
                        {
                            countOpParen++;
                            isAstComm = true;
                            push(top, line[i], lineNum, i);
                            i++;
                            if (countOpParen <= 1 && !isBraCom)
                            {
                                result += '{';
                            }
                        }
                        else
                        {
                            result += "(*";
                            i++;
                        }
                    }
                    else if (line[i] == ast && line[i + 1] == closeParen)
                    {
                        if (countApost % 2 == 0)
                        {
                            countClParen++;
                            i++;
                            if (top != nullptr)
                            {
                                if (top->ch == openParen)
                                {
                                    pop(top);
                                    isAstComm = false;
                                    if (countClParen <= 1 && !isBraCom & !isPrintClBra)
                                    {
                                        result += '}';
                                        isPrintClParen = true;
                                    }
                                }
                            }
                            else
                            {
                                isError = true;
                                errLine = lineNum;
                                pos = i;
                            }
                        }
                        else
                        {
                            result += "*)";
                            i++;
                        }
                    }
                    else if (line[i] == apost)
                    {
                        countApost++;
                        result += line[i];
                    }
                    else
                    {
                        result += line[i];
                    }
                }
                std::string processedLine = result;
                outputFile << processedLine << '\n';
            }
            inputFile.close();
            outputFile.close();
            if (top != nullptr)
            {
                isError = true;
                errLine = top->line;
                pos = top->pos;
            }
            if (isError)
            {
                std::cerr << "Ошибка: Некорректное завершение комментариев в строке " << errLine << ", позиция " << pos + 1 << std::endl;
                std::remove("result.txt");
            }
            else
            {
                std::cout << "Программа успешно завершена.Результат сохранен в файле result.txt." << std::endl;
            }
            clear(top);
        }
    }
    return 0;
}