#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <Windows.h>
#include "sorting_func.h"

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

int input_int();
void input_string(char *str);
void record_name(ITCOMPANY *exemplar);
void record_day_deadline(ITCOMPANY *exemplar);
void record_month_deadline(ITCOMPANY *exemplar);
void record_year_deadline(ITCOMPANY *exemplar);
void record_cost(ITCOMPANY *exemplar);
void record_status(ITCOMPANY *exemplar);
void new_record(ITCOMPANY *exemplar);
void print_exemplar(ITCOMPANY *exemplar);
void print_struct(ITCOMPANY **exemplar, int count);
int sort_condition(ITCOMPANY a, ITCOMPANY b, int condition);
int equal_condition(ITCOMPANY a, ITCOMPANY b, int condition);
int other_conditions(ITCOMPANY a, ITCOMPANY b, int *condition, int count_conditions);
void sorting_first(ITCOMPANY *exemplar, int count, int condition);
void sorting_second(ITCOMPANY *exemplar, int count, int *condition, int count_conditions);

int main()
{
    srand(time(NULL));
    timess();
    int count = 8;
    ITCOMPANY *exemplar = (ITCOMPANY *)calloc(count, sizeof(ITCOMPANY));

    ITCOMPANY new1 = {1000, "Project", 3, 8, 2023, 2500, 0};
    exemplar[0] = new1;
    ITCOMPANY new2 = {1001, "Project", 3, 8, 2023, 6000, 1};
    exemplar[1] = new2;
    ITCOMPANY new3 = {1002, "Project1", 15, 10, 2022, 4500, 1};
    exemplar[2] = new3;
    ITCOMPANY new4 = {1003, "Project", 30, 3, 2024, 8030, 0};
    exemplar[3] = new4;
    ITCOMPANY new5 = {1004, "Project", 1, 5, 2021, 1250, 0};
    exemplar[4] = new5;
    ITCOMPANY new6 = {1005, "Project", 1, 5, 2021, 1300, 0};
    exemplar[5] = new6;
    ITCOMPANY new7 = {1006, "Project1", 1, 5, 2021, 1400, 0};
    exemplar[6] = new7;
    ITCOMPANY new8 = {1007, "Project", 1, 5, 2021, 1250, 0};
    exemplar[7] = new8;

    // exemplar = (ITCOMPANY *)realloc(exemplar, count * sizeof(ITCOMPANY));
    //    for (int i = 0; i < count; ++i)
    //    {
    //        print_exemplar(&exemplar[i]);
    //    }
    // printf("Struct: \n");
    // print_struct(&exemplar, count);
    while (1)
    {
        printf("Select an action: \n");
        printf("1 - add a new record \n");
        printf("2 - information output\n");
        printf("3 - modification of an existing record\n");
        printf("4 - deletion of an existing record\n");
        printf("5 - sorted output(by selected fields)\n");
        int task = input_int();
        // int task;
        // scanf("%d", &task);
        // int task = 2;
        switch (task)
        {
        case 1:
        {
            ITCOMPANY temp;
            new_record(&temp);
            exemplar = (ITCOMPANY *)realloc(exemplar, (count + 1) * sizeof(ITCOMPANY));
            exemplar[count] = temp;
            print_exemplar(&exemplar[count]);
            count++;
            break;
        }
        case 2:
        {
            printf("_____________________________________________________\n");
            printf("| Project name |  ID  | deadline | cost |  status  |\n");
            printf("_____________________________________________________\n");
            for (int i = 0; i < count; i++)
            {
                print_exemplar(&exemplar[i]);
            }
            printf("\n");
            break;
        }
        case 3:
        {
            printf("Select project number\n");
            for (int i = 0; i < count; i++)
            {
                printf("%d - %s\n", i + 1, exemplar[i].name);
            }
            int projnum = input_int() - 1;
            printf("Select record field:\n");
            printf("1 - all\n2 - name\n3 - data deadline\n4 - cost\n5 - status\n");
            int num = input_int();
            switch (num)
            {
            case 1:
            {
                new_record(&exemplar[projnum]);
                break;
            }
            case 2:
            {
                record_name(&exemplar[projnum]);
                break;
            }
            case 3:
            {
                record_day_deadline(&exemplar[projnum]);
                record_month_deadline(&exemplar[projnum]);
                record_year_deadline(&exemplar[projnum]);
                break;
            }
            case 4:
            {
                record_cost(&exemplar[projnum]);
                break;
            }
            case 5:
            {
                record_status(&exemplar[projnum]);
                break;
            }
            default:
            {
                printf("Incorrect input. Try again. \n\n");
                break;
            }
            }
            break;
        }
        case 4:
        {
            printf("Select project number\n");
            for (int i = 0; i < count; i++)
            {
                printf("%d - %s\n", i + 1, exemplar[i].name);
            }
            int projnum = input_int() - 1;

            for (projnum; projnum < count - 1; projnum++)
                exemplar[projnum] = exemplar[projnum + 1];
            count--;
            break;
        }
        case 5:
        {
            // printf("Enter 1 critery:\n"
            //        "1 - id\n2 - name\n"
            //        "3 - deadline\n4 - cost\n5 - status\n");
            // int critery1 = input_int();
            // printf("Enter 2 critery:\n"
            //        "1 - id\n2 - name\n"
            //        "3 - deadline\n4 - cost\n5 - status\n");
            // int critery2 = input_int();

            // int critery = input_int();
            // sorting_first(exemplar, count, critery);
            int count_conditions;
            do
            {
                printf("Enter number of criterys:\n");
                count_conditions = input_int();
            } while (count_conditions < 0 || count_conditions > 5);

            int *condition = calloc(count_conditions, sizeof(int));
            for (int i = 0; i < count_conditions; i++)
            {
                printf("Enter %d critery:\n1 - id\n2 - name\n3 - deadline\n4 - cost\n5 - status\n", i + 1);
                condition[i] = input_int();
            }

            sorting_second(exemplar, count, condition, count_conditions);
            for (int i = 0; i < count; i++)
            {
                print_exemplar(&exemplar[i]);
            }
            printf("\n");
            break;
        }
        default:
            printf("Incorrect input. Try again. \n\n");
            break;
        }
    }
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
    for (int i = 1; i < count; i++)
    {
        int j = i;
        while (other_conditions(exemplar[j - 1], exemplar[j], condition, i))
        {
            sorting_first(exemplar, count, condition[i]);
            j++;
        }
    }
}
