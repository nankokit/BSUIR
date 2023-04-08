#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//#include "stepen.h"

int stepen(int number, int step)
{
    int ans = number;
    for (int j = 0; j < step; j++)
    {
        ans = ans * number;
    }
    return ans;
}

double checker(char mas[])
{
    int* mas_before; //указатели для динамических массивов
    int* mas_after;
    int size_mas = 0;   // переменная, для подсчета длины строки
    int dot_check = 0;   // переменная, которая хранит кол-во точек в строке
    int minus_check = 0;   // переменная, проверяющая, отрицательное ли число
    int size_before = 0;   // переменная, хранящая размер массива до точки
    //gets_s(mas);   // вводим массив

    if (mas[0] == '-')
    {
        for (int i = 0; mas[i] != 0; i++)
        {
            mas[i] = mas[i + 1];
        }
        minus_check = 1;
    }

    // ПРОВЕРКА НА ВВОД ЧИСЛА
    for (int i = 0; mas[i] != 0; i++) // цикл, пока не дойдем о конца строки
    {
        if (mas[i] <= 43 || mas[i] > 58 || mas[i] == 47 || mas[i] == 45) // если элемент строки не является 0123456789., то выводим "Wrong input"
        {
            //printf("Wrong input\n");
            return 0;
        }

        if (mas[i] == ',' || mas[i] == '.') // заменяем запятые на точки и считаем кол-во точек в строке
        {
            mas[i] = '.';
            dot_check++; // +1 к кол-ву точек в строке
            if (dot_check > 1) // если точек >1 то выводим "Wrong input"
            {
                printf("Wrong input\n");
                return 0;
            }
        }

        if (dot_check != 1) // если мы не дошли до запятой, то +1 к длине до запятой
        {
            size_before++;
        }

        size_mas++; // +1 к длине строки за каждый символ
    }


    // ПРЕОБРАЗОВАНИЕ МАССИВА ЧАРОВ В ЧИСЛО
    int size_after = size_mas - size_before - 1;   // cчитаем размер массива чисел после запятой
    mas_before = (int*)malloc(size_before + 1 * sizeof(int));   // выделяем память для массива чисел до запятой
    mas_after = (int*)malloc(size_after * sizeof(int));   // выделяем память для массива чисел после запятой

    double num_before = 0;   // число до запятой
    double num_after = 0;   // число после запятой

    int stepen_x = size_before - 1;   // степень цифры до заяптой
    for (int i = 0; i < size_before; i++)   // на примере 123 будет так 1*100+2*10+3*1
    {
        mas_before[i] = mas[i] - '0';   // получаем число, отнимая ASKII код 0
        num_before = num_before + mas_before[i] * stepen(10, stepen_x);
        stepen_x--;
    }


    if (dot_check == 1)   // если есть точка, то считаем часть после точки
    {
        int contrstep = 1;
        int j = 0;
        double num = 0;
        for (int i = size_before + 1; i < size_mas; i++)   // хз, что тут происходит, но оно работает
        {
            mas_after[j] = mas[i] - '0';
            num = mas_after[j];
            for (int check = 0; check < contrstep; check++)
            {
                num = num / 10;
            }
            j++;
            contrstep++;
            num_after = num_after + num;
        }
    }

    double VICTORY = num_before + num_after;   // gg wp

    if (minus_check == 1)   // если число было отрицательным, то умножаем на -1
    {
        VICTORY = VICTORY * (-1);
    }
    //printf("Number = %lf", VICTORY);
    return VICTORY;
}

int rigthInput()
{
    double number = 0;
    int check = 0;
    char mas[1000];
    int mas_size = 0;

    gets(mas);

    number = checker(mas);

    while (number == 0)
    {
        for (int i = 0; mas[i] != 0; i++)
        {
            mas_size++;
        }
        if (mas_size == 1 && mas[0] == '0')
        {
            number = 1;
            break;
        }
        else
        {
            printf("Ошибка\n");
            printf("Введите ещё раз: ");
            gets(mas);
            number = checker(mas);
        }
        mas_size = 0;
    }

        number = number / 10;

    return number;
}