#include <stdio.h>
#include <stdlib.h>
#include "funckions.h"
#include "task1.h"
#include "task2.h"
#include "task3.h"
#include "проверка.h"

int main()
{
    int taskNumber = 0, n1, n2, tester = 0;
    char* str;   
    char* str1;
    char* str2;
    char test[1];

    menu();

    printf("Введите номер задачи: ");
    taskNumber = rigthInput();
    taskNumber = proverkaSwitch(taskNumber);

    switch (taskNumber)
    {
    case 1:
        printf("Задача 1\nДана строка. Преобразовать в ней все первые буквы каждого слова в прописные.\nВведите строку: ");
        str = (char*)calloc(1,1);   
        fgets(str,255,stdin);
        rewind(stdin);   
        if (lengthStr(str) == 254)
        {
            printf("To big di...\n");
            break;
        }
        else
        {
            task1(str);
        }   
        break;
    case 2:
        printf("Задача 2\nN1 первых символов первой строки и N2 символов второй строкиъ\nВведите N1: ");
        n1 = rigthInput();
        printf("Введите N2: ");
        n2 = rigthInput();
        rewind(stdin);

        printf("Введите S1: ");
        str = (char*)calloc(255,1);
        fgets(str,255,stdin);
        if (lengthStr(str) == 254)
        {
            printf("To big di...\n");            
            break;
        }
        

        printf("Введите S2: ");
        str2 = (char*)calloc(255,1);
        fgets(str2,255,stdin);
        rewind(stdin);   
        if (lengthStr(str) == 254)
        {
            printf("To big di...\n");    
            break;
        }
        else
        {
            task2(n1, str, n2, str2);
        }
        break;
    case 3:
        printf("Задача 3\nОтсортировать методом Хаора массив строк по убыванию длины максимального слова в каждой строке\n");
        task3();
        break;
    default:
        printf("Gg\n");
        return 0;
    }

    printf("\n\nВведите что-нибудь, чтобы продолжить....");
    gets(test);
    rewind(stdin);
    printf("\e[1;1H\e[2J");
    exit(main());
}