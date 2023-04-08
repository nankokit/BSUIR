#include "library.h"
#include <windows.h>
#include <locale.h>
/*
1. Дана строка. Преобразовать в ней все первые буквы каждого слова в прописные.
2. Даны целые положительные числа N1 и N2 и строки S1 и S2. Получить из этих строк новую строку, содержащую первые N1 символов строки S1
и последние N2 символов строки S2 (в указанном порядке).
3. Отсортировать методом Хоара массив строк по убыванию длины максимального слова в каждой строки
*/

void task1()
{
    //  SetConsoleCP(1251);
    //  SetConsoleOutputCP(1251);
    int stop = 1;
    char *string = (char *)malloc(1 * sizeof(int));
    while (stop != 0)
        get_string(string, &stop);
    reverse(string);
    print_string(string);
}

void task2()
{
    int n1, n2, stop = 1;
    printf("Enter S1: ");
    char *str1 = (char *)calloc(1, 1 * sizeof(char));
    while (stop != 0)
        get_string(str1, &stop);
    printf("Enter S2: ");
    char *str2 = (char *)calloc(1, 1 * sizeof(char));
    stop = 1;
    while (stop != 0)
        get_string(str2, &stop);

    printf("Enter N1: ");
    n1 = check_int_input();
    int len;
    while (n1 > (len = lengthStr(str1)))
    {
        printf("N1 is more than the number of elements of the string1. \nEnter N1 <= %d: ", lengthStr(str1));
        n1 = check_int_input();
    }
    printf("Enter N2: ");
    n2 = check_int_input();
    while (n2 > (len = lengthStr(str2)))
    {
        printf("N2 is more than the number of elements of the string2. \nEnter N2 <= %d: ", lengthStr(str2));
        n2 = check_int_input();
    }

    char *ans = (char *)calloc(n1 + n2 + 1, sizeof(int));
    for (int i = 0; i < n1; i++)
    {
        ans[i] = str1[i];
    }
    for (int i = 0; i < n2; i++)
    {
        ans[i + n1] = str2[lengthStr(str2) - n2 + i];
    }
    print_string(ans);
}

void task3()
{
    int size = 0, stop;
    char **str;
    int *maxSize;

    printf("Enter the number of strings: ");
    size = check_int_input();
    rewind(stdin);

    maxSize = (int *)calloc(size, sizeof(int));
    str = (char **)calloc(size, sizeof(char *));
    for (int i = 0; i < size; i++)
    {
        stop = 1;
        printf("%d -", i);
        str[i] = (char *)calloc(1, sizeof(char));
        while (stop != 0)
            get_string(str[i], &stop);
    }
    // for (int i = 0; i < size; i++)
    // {
    //     print_string(str[i]);
    // }
    int lenght = 0, max = 0, j;
    char c;
    for (int i = 0; i < size; i++)
    {
        j = 0, max = 0;
        while (str[i][j] != '\0')
        {
            lenght = 0;
            if ((i == 0 && if_letter(str[i][j]) == 1) || (if_letter(str[i][j - 1]) == 0 && if_letter(str[i][j]) == 1))
                do
                {
                    lenght++;
                    j++;
                } while (if_letter(str[i][j]) != 0);
            if (lenght > max)
            {
                max = lenght;
            }
            j++;
        }
        maxSize[i] = max;
    }

    printf("\nString number and lenght of max word:\n");
    for (int i = 0; i < size; i++)
    {
        printf("%d - %d\n", i, maxSize[i]);
    }

    quickSort(maxSize, str, 0, size - 1);
    printf("Sorted strings:\n");
    j = 0;
    for (int i = size - 1; i >= 0; i--)
    {
        // arrSize[i] = maxLength(str[i]);
        printf("%d   ", maxSize[i]);
        printf("%s", str[i]);
        printf("\n");
    }
    printf("\n");
}

int main()
{
    int taskNumber = 0;
    while (1)
    {
        printf("Enter task number 1 - 3 (4 - exit): \n");
        taskNumber = check_int_input();
        printf("\n");
        switch (taskNumber)
        {
        case 1:
            printf("Task 1 \n\n");
            task1();
            printf("\n");
            break;

        case 2:
            printf("Task 2 \n\n");
            task2();
            break;

        case 3:
            printf("Task 3 \n");
            printf("\n");
            task3();
            printf("\n");
            break;

        case 4:
            printf("Exit\n");
            exit(0);
            break;
        default:
            printf("Incorrect. Try again. \n");
            printf("\n");
        }
    }
    return 0;
}