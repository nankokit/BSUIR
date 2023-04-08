#include "проверка.h"

int lengthStr(char *str)
{
    int len = 0;
    while (str[len] != '\0')
    {
        if (str[len] < 0)
        {
            // printf("Ogo vi iz Russia. Ban\n");
            return (-1);
        }
        len++;
    }
    return len;
}

int proverkaSwitch(int taskNumber)
{
    while (taskNumber != 1 && taskNumber != 2 && taskNumber != 3 && taskNumber != 4)
    {
        printf("F: ");
        taskNumber = rigthInput();
    }
    return taskNumber;
}

void menu()
{
    printf("1 - первая задача\n");
    printf("2 - вторая задача\n");
    printf("3 - третья задача\n");
    printf("4 - выход из программы\n");
    printf("Все остальное - ошибка\n\n");
}

void get_string(char *string)
{
    printf("String input: \n");
    int c, i = 0;
    while ((c = getchar()) != EOF && c != '\n')
    {
        string[i++] = c;
        string = realloc(string, (i + 1) * sizeof(int));
    }
    string[i] = '\0';
}

int string_length(char *string)
{
    int i = 0;
    while (string[i] != '\0')
        i++;
    return i;
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

void reverse(char *string)
{
    int i = 0, n = string_length(string);
    while (string[i] != '\0')
    {
        if ((i == 0 && if__little_letter(string[i]) == 1) || (if_letter(string[i - 1]) == 0 && if__little_letter(string[i]) == 1))
        {
            string[i] = 'A' - ('a' - string[i]);
        }
        i++;
    }
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
