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

void save(char *fname, ITCOMPANY *exemplar, int size)
{
    FILE *fp = fopen(fname, "wb");
    if (size != 0)
    {
        fwrite(&size, sizeof(int), 1, fp);
        fwrite(exemplar, sizeof(ITCOMPANY), size, fp);
    }
    fclose(fp);
}

int struct_count_file()
{
    FILE *fp = fopen("loaded_data.bin", "rb");
    int temp;
    fread(&temp, sizeof(int), 1, fp);
    fclose(fp);
    return temp;
}

void load(char *fname, ITCOMPANY **exemplar, int *size)
{
    FILE *fp = fopen(fname, "rb");
    fread(&(*size), sizeof(int), 1, fp);
    (*exemplar) = realloc((*exemplar), (*size) * sizeof(ITCOMPANY));
    fread((*exemplar), sizeof(ITCOMPANY), (*size), fp);
    fclose(fp);
}

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
    str[i] = '\0';
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

int sort_condition(ITCOMPANY a, ITCOMPANY b, int condition)
{
    switch (condition)
    {
    case 1:
        return (a.id > b.id);
        break;
    case 2:
        return (strcmp(a.name, b.name) > 0);
        break;
    case 3:
    {
        if (a.year_deadline == b.year_deadline)
        {
            if (a.month_deadline == b.month_deadline)
            {
                return (a.day_deadline > b.day_deadline);
            }
            else
                return (a.month_deadline > b.month_deadline);
        }
        else
            return (a.year_deadline > b.year_deadline);
        break;
    }
    case 4:
        return (a.cost > b.cost);
        break;
    case 5:
        return (a.status > b.status);
        break;
    default:
        break;
    };
}

int equal_condition(ITCOMPANY a, ITCOMPANY b, int condition)
{
    switch (condition)
    {
    case 1:
        return (a.id == b.id);
        break;
    case 2:
        return (strcmp(a.name, b.name) == 0);
        break;
    case 3:
    {
        if (a.year_deadline == b.year_deadline)
        {
            if (a.month_deadline == b.month_deadline)
            {
                return (a.day_deadline == b.day_deadline);
            }
            else
                return 0;
        }
        else
            return 0;
        break;
    }
    case 4:
        return (a.cost == b.cost);
        break;
    case 5:
        return (a.status == b.status);
        break;
    default:
        return 0;
        break;
    };
}

int other_conditions(ITCOMPANY a, ITCOMPANY b, int *condition, int count_conditions)
{
    int match = 0;
    for (int i = 0; i < count_conditions; i++)
    {
        match += equal_condition(a, b, condition[i]);
    }
    if (match == count_conditions)
        return 1;
    else
        return 0;
}

void sorting_first(ITCOMPANY *exemplar, int count, int condition)
{
    ITCOMPANY temporary;
    for (int i = 1; i < count; i++)
    {
        int j = i;
        while (j > 0 && sort_condition(exemplar[j - 1], exemplar[j], condition))
        {
            temporary = exemplar[j - 1];
            exemplar[j - 1] = exemplar[j];
            exemplar[j] = temporary;
            j--;
        }
    }
}

void sorting_second(ITCOMPANY *exemplar, int count, int *condition, int count_conditions)
{
    sorting_first(exemplar, count, condition[0]);
    ITCOMPANY temporary;

    printf("After 1 sort\n");
    for (int i = 0; i < count; i++)
    {
        print_exemplar(&exemplar[i]);
    }
    printf("\n\n");
    for (int i = 1; i < count; i++)
    {
        int j = i;
        while (equal_condition(exemplar[j - 1], exemplar[j], condition[0]) && sort_condition(exemplar[j - 1], exemplar[j], condition[1]) && j > 0)
        {
            temporary = exemplar[j - 1];
            exemplar[j - 1] = exemplar[j];
            exemplar[j] = temporary;
            j--;
        }
    }
}

void sorting_third(ITCOMPANY *exemplar, int count, int *condition, int count_conditions)
{
    sorting_first(exemplar, count, condition[0]);
    ITCOMPANY temporary;

    printf("After 1 sort\n");
    for (int i = 0; i < count; i++)
    {
        print_exemplar(&exemplar[i]);
    }
    printf("\n\n");
    for (int i = 1; i < count; i++)
    {
        int j = i;
        while (equal_condition(exemplar[j - 1], exemplar[j], condition[0]) &&
               equal_condition(exemplar[j - 1], exemplar[j], condition[1]) &&
               sort_condition(exemplar[j - 1], exemplar[j], condition[2]) && j > 0)
        {
            temporary = exemplar[j - 1];
            exemplar[j - 1] = exemplar[j];
            exemplar[j] = temporary;
            j--;
        }
    }
}

void sorting_many(ITCOMPANY *exemplar, int count, int *condition, int count_conditions)
{
    sorting_first(exemplar, count, condition[0]);
    ITCOMPANY temporary;

    printf("After 1 sort\n");
    for (int i = 0; i < count; i++)
    {
        print_exemplar(&exemplar[i]);
    }
    printf("\n\n");
    for (int k = 1; k < count_conditions; k++)
    {
        for (int i = 1; i < count; i++)
        {
            int j = i;
            while (other_conditions(exemplar[j - 1], exemplar[j], condition, k) && j > 0)
            {
                sorting_first(exemplar, count, condition[k]);
                j--;
            }
        }
    }
}