#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <Windows.h>

typedef struct itcompany
{
    int id;
    char name[255];
    int day_deadline;
    int month_deadline;
    int year_deadline;
    int cost;
    char status;
} ITCOMPANY;

int input_int()
{
    char c;
    int num, input = scanf("%d", &num);
    if (input == 0 || (c = getchar()) != '\n' || (int)input != input)
    {
        printf("Incorrect input. Try again. \n");
        rewind(stdin);
    }
    else
        return num;
}

void input_string(char *str)
{
    char c;
    int i = 0;
    while ((c = getchar()) != '\n')
    {
        str[i] = c;
        i++;
    }
    str[i + 1] = '\0';
}

void record_name(ITCOMPANY *exemplar)
{
    char str[255];
    printf("Enter project name: \n");
    input_string(str);
    strcpy(exemplar->name, str);
}

void record_day_deadline(ITCOMPANY *exemplar)
{
    float temp = 0;
    printf("Enter day deadline [0;31]: \n");
    temp = input_int();
    while (temp <= 0 || temp > 31)
    {
        printf("Incorrect. try again \n");
        temp = input_int();
    }
    exemplar->day_deadline = temp;
}

void record_month_deadline(ITCOMPANY *exemplar)
{
    float temp = 0;
    printf("Enter month deadline: \n");
    temp = input_int();
    while (temp <= 0 || temp > 12)
    {
        printf("Incorrect. try again \n");
        temp = input_int();
    }
    exemplar->month_deadline = temp;
}

void record_year_deadline(ITCOMPANY *exemplar)
{
    float temp = 0;
    printf("Enter year deadline: \n");
    temp = input_int();
    while (temp < 1970 || temp > 2038)
    {
        printf("Incorrect. try again \n");
        temp = input_int();
    }
    exemplar->year_deadline = temp;
}

void record_cost(ITCOMPANY *exemplar)
{
    int temp = 0;
    printf("Enter cost: \n");
    temp = input_int();
    while (temp <= 0)
    {
        printf("Incorrect. try again \n");
        temp = input_int();
    }
    exemplar->cost = temp;
}

void record_status(ITCOMPANY *exemplar)
{
    float temp = 0;
    printf("Enter status(0 or 1): \n");
    temp = input_int();
    while (temp != 0 && temp != 1)
    {
        printf("Incorrect. try again \n");
        temp = input_int();
    }
    exemplar->status = temp;
}

void new_record(ITCOMPANY *exemplar)
{
    float temp = 0;
    (exemplar->id) = 1000 + rand() % 501;
    record_name(exemplar);
    record_day_deadline(exemplar);
    record_month_deadline(exemplar);
    record_year_deadline(exemplar);
    record_cost(exemplar);
    record_status(exemplar);
}

void print_exemplar(ITCOMPANY *exemplar)
{
    // printf("Project: %s\n", exemplar->name);
    // printf("ID: %d\n", exemplar->id);
    // printf("deadline: %02d.%02d.%4d \n", exemplar->day_deadline, exemplar->month_deadline, exemplar->year_deadline);
    // printf("cost: %d\n", exemplar->cost);
    // if (exemplar->status)
    //     printf("ready\n");
    // else
    //     printf("not ready\n");
    char *str1;
    if (exemplar->status)
        str1 = "ready";
    else
        str1 = "not ready";
    printf("|%12s  |%6d|%02d.%02d.%4d|%5d |%10s| \n", exemplar->name, exemplar->id, exemplar->day_deadline,
           exemplar->month_deadline, exemplar->year_deadline, exemplar->cost, str1);
    printf("_____________________________________________________\n");
}

void print_struct(ITCOMPANY **exemplar, int count)
{
    for (int i = 0; i < count; i++)
    {
        // print_exemplar(*exemplar[i]);
        ;
    }
}

void print_string(char *str)
{
    int i = 0;
    while (str[i] != '\0')
    {
        printf("%c", str[i++]);
    }
    printf("\n");
}

int find_deadline(int year, int month, int day)
{
    SYSTEMTIME st;
    GetSystemTime(&st);
    // printf("year:%d\nmonth:%d\nday:%d\n", st.wYear, st.wMonth, st.wDay);
    if (st.wYear > year)
        return 1;
    else if (st.wYear == year && st.wMonth > month)
        return 1;
    else if (st.wMonth == month && st.wDay >= day)
        return 1;
    else
        return 0;
}