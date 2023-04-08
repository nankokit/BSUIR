#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <windows.h>

int check_int_input()
{
    int readinput;
    int input;
    char inputchar;

    while (1)
    {
        readinput = scanf_s("%d", &input);
        scanf_s("%c", &inputchar);
        if (readinput != 0 && inputchar == '\n' && (int)input == input && input < 2147483647)
        {
            return input;
        }
        else
        {
            printf("Incorrect input. Try again. \n");
            rewind(stdin);
        }
    }
}

void get_string(char *string, int *stop)
{
    SetConsoleCP(1251);       // установка кодовой страницы win-cp 1251 в поток ввода
    SetConsoleOutputCP(1251); // установка кодовой страницы win-cp 1251 в поток вывода
    printf("String input: \n");
    int c, i = 0;
    *stop = 0;
    while (*stop == 0 && ((c = getchar()) != EOF && c != '\n'))
    {
        string = (char *)realloc(string, (i + 1) * sizeof(char *));
        string[i++] = c;
        if (c > 192)
        {
            printf("Russian is not supported.\n");
            *stop = 1;
            rewind(stdin);
            free(string);
            string = (char *)malloc(1 * sizeof(char *));
        }
    }
    string[i] = '\0';
    rewind(stdin);
}

int if_letter(char string)
{
    if ((string >= 'a' && string <= 'z') || (string >= 'A' && string <= 'Z'))
        return 1;
    return 0;
}

int if__little_letter(char string)
{
    if ((string >= 'a' && string <= 'z'))
        return 1;
    return 0;
}

int if__little_letter_rus(char string)
{
    if ((string >= 192 && string <= 255))
        return 1;
    return 0;
}

int lengthStr(char *str)
{
    int len = 0;
    while (str[len] != '\0')
    {
        len++;
    }
    return len;
}

void reverse(char *string)
{
    int i = 0, n;
    n = lengthStr(string);
    while (string[i] != '\0')
    {
        if ((i == 0 && if__little_letter(string[i]) == 1) || (if_letter(string[i - 1]) == 0 && if__little_letter(string[i]) == 1))
        {
            string[i] = 'A' - ('a' - string[i]);
        }
        /*else
        {
            if((i == 0 && if__little_letter_rus(string[i]) == 1) || (if_letter(string[i - 1]) == 0 && if__little_letter_rus(string[i]) == 1))
                string[i] = string[i] - 32;
        }*/
        i++;
    }
}

void print_string(char *string)
{
    int i = 0;
    while (string[i] != '\0')
    {
        printf("%c", string[i]);
        i++;
    }
    printf("\n");
}

int maxLength(char *str)
{
    int len = lengthStr(str);
    if (len == (-1))
    {
        return (-1);
    }

    int j = 0;
    int check = 0, maxLen = 0, ans = 0;
    while ((str[j] <= 122 && str[j] >= 97) || (str[j] <= 90 && str[j] >= 65))
    {
        maxLen++;
        j++;
    }

    if (maxLen > ans)
    {
        ans = maxLen;
    }

    maxLen = 0;

    for (int i = 0; i < len - 1; i++)
    {
        check = 0;

        if ((str[i] > 122 || (str[i] < 97 && str[i] > 90) || str[i] < 65))
        {
            j = i + 1;
            while ((str[j] <= 122 && str[j] >= 97) || (str[j] <= 90 && str[j] >= 65))
            {
                j++;
                maxLen++;
            }
            if (maxLen > ans)
            {
                ans = maxLen;
            }
            maxLen = 0;
        }
    }
    return ans;
}

void swap(int *n1, int *n2)
{
    int swap;
    swap = *n1;
    *n1 = *n2;
    *n2 = swap;
}

void charSwap(char **c1, char **c2)
{
    char *swap = *c1;
    *c1 = *c2;
    *c2 = swap;
}

int partition(int *maxSize, char **str, int start, int pivot)
{
    int i = start;
    while (i < pivot)
    {
        if (maxSize[i] > maxSize[pivot] && i == pivot - 1)
        {
            swap(&maxSize[i], &maxSize[pivot]);
            charSwap(&str[i], &str[pivot]);
            pivot--;
        }

        else if (maxSize[i] > maxSize[pivot])
        {
            swap(&maxSize[pivot - 1], &maxSize[pivot]);
            swap(&maxSize[i], &maxSize[pivot]);

            charSwap(&str[pivot - 1], &str[pivot]);
            charSwap(&str[i], &str[pivot]);

            pivot--;
        }
        else
            i++;
    }
    return pivot;
}

void quickSort(int *maxSize, char **str, int start, int end)
{
    if (start < end)
    {
        int pivot = partition(maxSize, str, start, end);
        quickSort(maxSize, str, start, pivot - 1);
        quickSort(maxSize, str, pivot + 1, end);
    }
}

void max_word(char *str, int *maxSize, int index)
{
    int max = 0, i = 0, lenght;
    while (str[i] != '\0')
    {
        lenght = 0;
        if ((i == 0 && if_letter(str[i]) == 1) || (if_letter(str[i - 1]) == 0 && if_letter(str[i]) == 1))
            do
            {
                lenght++;
                i++;
            } while (if_letter(str[i]) != 0);
        if (lenght > max)
        {
            max = lenght;
        }
        i++;
    }
    maxSize[index] = max;
}