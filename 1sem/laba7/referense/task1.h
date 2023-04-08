#include <stdio.h>

void get_string(char *string)
{
    SetConsoleCP(1251);       // установка кодовой страницы win-cp 1251 в поток ввода
    SetConsoleOutputCP(1251); // установка кодовой страницы win-cp 1251 в поток вывода
    printf("String input: \n");
    int c, i = 0;
    while ((c = getchar()) != EOF && c != '\n')
    {
        string[i++] = c;
        string = realloc(string, (i + 1) * sizeof(int));
    }
    string[i] = '\0';
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
    if ((string >= 224 && string <= 255))
        return 1;
    return 0;
}

void reverse(char *string)
{
    int i = 0, n = string_length(string);
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

void task1()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    char *string = malloc(1 * sizeof(int));
    get_string(string);
    reverse(string);
    print_string(string);
}

int string_length(char *string)
{
    int i = 0;
    while (string[i] != '\0')
        i++;
    return i;
}

int find_k_word(char *string, int k)
{
    int i = 0, n = string_length(string), cnt = 0;
    while (string[i] != '\0')
    {
        if ((i == 0 && if_letter(string[i]) == 1) || (if_letter(string[i - 1]) == 0 && if_letter(string[i]) == 1))
            cnt++;

        if (cnt == k)
            return i;
        i++;
    }
    return -1;
}
