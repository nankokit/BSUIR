#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <io.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "functions.h"
// #include "D:/program/my_functions/Struct.h"

struct data
{
    int day;
    int month;
    int year;
};

enum stages
{
    planning = 1,
    analysis,
    design,
    implementation,
    testing,
    maintenance,
};

struct status1
{
    int employee;
    struct data support;
};

struct status2
{
    stages stage;
    char *customer = (char *)calloc(1, 1);
};

union ustatus
{
    status1 st1;
    status2 st2;
};

struct Project
{
    int id;
    char *name = (char *)calloc(1, 1);
    struct data deadline;
    float price;
    int chose;
    ustatus stat;
};

struct LIST
{
    struct Project *data;
    // struct LIST* prev;
    struct LIST *next;
};

struct data input_data(struct data *dt)
{
    printf("Year :\t");

    while (int_int_check((*dt).year) || (*dt).year <= 0)
    {
        setConsoleColor(black, red);
        printf("Error! Please enter the correct data\n");
        rewind(stdin);
        setConsoleColor(black, white);
    }

    printf("Month : ");
    while (int_int_check((*dt).month) || (*dt).month <= 0 || (*dt).month > 12)
    {
        setConsoleColor(black, red);
        printf("Error! Please enter the correct data\n");
        rewind(stdin);
        setConsoleColor(black, white);
    }

    int fy = 0;
    if ((*dt).year % 4 == 0)
    {
        if ((*dt).year % 100 == 0)
        {
            if ((*dt).year % 400 == 0)
                fy = 1;
            else
                fy = 0;
        }
        else
            fy = 1;
    }
    else
        fy = 0;

    int fm = 0;
    if ((*dt).month == 4 || (*dt).month == 6 || (*dt).month == 9 || (*dt).month == 11)
        fm = 1;
    if ((*dt).month == 2)
        fm = 2;

    printf("Day :\t");
    while (int_int_check((*dt).day) || (*dt).day <= 0 || (fm == 0 && (*dt).day > 31) || (fm == 1 && (*dt).day > 30) || (fy == 1 && fm == 2 && (*dt).day > 29) || (fy == 0 && fm == 2 && (*dt).day > 28))
    {
        setConsoleColor(black, red);
        printf("Error! Please enter the correct data\n");
        rewind(stdin);
        setConsoleColor(black, white);
    }

    return *dt;
}

struct Project input_Project(struct Project *pr1)
{
    char *temp = (char *)calloc(1, 1);

    printf("New project :\n");
    printf("Id :\t");
    while (int_int_check((*pr1).id) || (*pr1).id <= 0)
    {
        setConsoleColor(black, red);
        printf("Error! Please enter the correct data\n");
        rewind(stdin);
        setConsoleColor(black, white);
    }

    printf("Name :\t");
    get_str(&temp);
    str_copy(temp, &(*pr1).name);

    printf("Deadline :\n");
    (*pr1).deadline = input_data(&(*pr1).deadline);

    printf("Price :\t\t");
    while (float_check((*pr1).price) || (*pr1).price < 0)
    {
        setConsoleColor(black, red);
        printf("Error! Please enter the correct data\n");
        setConsoleColor(black, white);
        rewind(stdin);
    }

    printf("Status :\n1 -- number of assigned employees and date of product support\n2 -- the remaining stages of development, the customer\n");

    while (int_int_check((*pr1).chose) || (*pr1).chose <= 0 || (*pr1).chose > 2)
    {
        setConsoleColor(black, red);
        printf("Error! Please enter the correct data\n");
        rewind(stdin);
        setConsoleColor(black, white);
    }

    switch ((*pr1).chose)
    {
    case 1:
    {
        printf("Enter number of assigned employees : ");
        while (int_int_check((*pr1).stat.st1.employee) || (*pr1).stat.st1.employee < 0)
        {
            setConsoleColor(black, red);
            printf("Error! Please enter the correct data\n");
            rewind(stdin);
            setConsoleColor(black, white);
        }

        printf("Enter date of product support : \n");
        (*pr1).stat.st1.support = input_data(&(*pr1).stat.st1.support);
        break;
    }
    case 2:
    {
        printf("Enter stages of development :\n1 -- Planning\n2 -- Analysis\n3 -- Design\n4 -- Implementation\n5 -- Testing\n6 -- Maintenance\n");
        stages st;
        while (scanf("%i", &st) != 1 || getchar() != '\n' || st <= 0 || st > 6)
        {
            setConsoleColor(black, red);
            printf("Error! Please enter the correct data\n");
            rewind(stdin);
            setConsoleColor(black, white);
        }
        (*pr1).stat.st2.stage = st;

        printf("Enter the customer : \n");
        get_str(&temp);
        str_copy(temp, &(*pr1).stat.st2.customer);
        break;
    }
    }

    return *pr1;
}

void output_Project(struct Project *pr1)
{
    printf("Project ");
    setConsoleColor(black, yellow);
    printf("%s", (*pr1).name);
    setConsoleColor(black, white);
    printf(" :\n");

    printf("Id :\t\t%i\n", (*pr1).id);

    ////printf("name :\t\t%s\n", pr1.name);

    printf("Deadline data : %i\\%i\\%i\n", (*pr1).deadline.day, (*pr1).deadline.month, (*pr1).deadline.year);

    printf("Price :\t\t%.2f ", (*pr1).price);
    setConsoleColor(black, green);
    printf("$\n");
    setConsoleColor(black, white);

    printf("Status :\n");

    switch ((*pr1).chose)
    {
    case 1:
    {
        printf("Number of assigned employees : %i\n", (*pr1).stat.st1.employee);
        printf("Date of product support :  %i\\%i\\%i\n", (*pr1).stat.st1.support.day, (*pr1).stat.st1.support.month, (*pr1).stat.st1.support.year);
        break;
    }
    case 2:
    {
        printf("Remaining stages of development : ");
        switch ((*pr1).stat.st2.stage)
        {
        case planning:
        {
            printf("Planning, Analysis, Design, Implementation, Testing, Maintenance\n");
            break;
        }
        case analysis:
        {
            printf("Analysis, Design, Implementation, Testing, Maintenance\n");
            break;
        }
        case design:
        {
            printf("Design, Implementation, Testing, Maintenance\n");
            break;
        }
        case implementation:
        {
            printf("Implementation, Testing, Maintenance\n");
            break;
        }
        case testing:
        {
            printf("Testing, Maintenance\n");
            break;
        }
        case maintenance:
        {
            printf("Maintenance\n");
            break;
        }
        }
        printf("Customer : %s\n", (*pr1).stat.st2.customer);
        break;
    }
    }
}

// void push_after(struct LIST** head, struct Project* x)
//{
//     struct LIST* temp = (struct LIST*)calloc(1, (sizeof(struct LIST)));
//     temp->data = x;
//     temp->next = NULL/*(*ptr)->next;*/;
//     if (*head == NULL) {
//         temp->prev = NULL;
//         *head = temp;
//         return;
//     }
//     struct LIST* next_ptr = *head;
//     while (next_ptr->next != NULL)
//         next_ptr = next_ptr->next;
//     next_ptr->next = temp;
//     temp->prev =  next_ptr;
//
// }

void push_af(struct LIST **head)
{
    struct LIST *temp = (struct LIST *)calloc(1, (sizeof(struct LIST)));
    struct Project *com = (Project *)calloc(1, sizeof(Project));
    *com = input_Project(com);
    temp->data = com;
    if (*head == NULL)
    {
        temp->next = NULL;
        *head = temp;
        // return;
    }
    else
    {
        struct LIST *next_p = *head;
        while (next_p->next != NULL)
            next_p = next_p->next;
        next_p->next = temp;
    }
}

void push(struct LIST **head)
{
    struct LIST *temp = (struct LIST *)calloc(1, (sizeof(struct LIST)));
    struct Project *com = (Project *)calloc(1, sizeof(Project));
    *com = input_Project(com);
    temp->data = com;
    if (*head == NULL)
    {
        temp->next = NULL;
        *head = temp;
        return;
    }
    temp->next = *head;
    //(*head)->next = temp->next;
    *head = temp;
}

void printList(struct LIST *head)
{
    if (head == NULL)
    {
        printf("The list is empty\n");
        return;
    }
    printf("\nForward List: \n");
    struct LIST *node = head;
    while (node != NULL)
    {
        output_Project(node->data);
        node = node->next;
    }
    printf("\n");
}

char *safe_to_file(struct LIST *head)
{
    printf("Enter the file name : ");
    char *nam = (char *)calloc(1, 1);
    get_str(&nam);
    FILE *f = fopen(nam, "wb");
    if (f == NULL)
    {
        setConsoleColor(black, red);
        printf("File not found");
        setConsoleColor(black, white);
        exit(1);
    }

    struct LIST *node = head;
    char o = '\0';
    while (node != NULL)
    {

        struct Project *temp = node->data;

        fwrite(&(*temp).id, sizeof(int), 1, f);

        fwrite((*temp).name, sizeof(char), strlen((*temp).name) + 1, f);

        fwrite(&(*temp).deadline.day, sizeof(int), 1, f);
        fwrite(&(*temp).deadline.month, sizeof(int), 1, f);
        fwrite(&(*temp).deadline.year, sizeof(int), 1, f);

        fwrite(&(*temp).price, sizeof(float), 1, f);

        fwrite(&(*temp).chose, sizeof(int), 1, f);

        switch ((*temp).chose)
        {
        case 1:
        {
            fwrite(&(*temp).stat.st1.employee, sizeof(int), 1, f);

            fwrite(&(*temp).stat.st1.support.day, sizeof(int), 1, f);
            fwrite(&(*temp).stat.st1.support.month, sizeof(int), 1, f);
            fwrite(&(*temp).stat.st1.support.year, sizeof(int), 1, f);

            break;
        }
        case 2:
        {
            fwrite(&(*temp).stat.st2.stage, sizeof(stages), 1, f);

            fwrite((*temp).stat.st2.customer, sizeof(char), strlen((*temp).stat.st2.customer) + 1, f);
            break;
        }
        }

        node = node->next;
    }

    fclose(f);

    return nam;
}

int number_str_in_file(char *nam)
{
    FILE *f = fopen(nam, "rb");
    if (f == NULL)
    {
        setConsoleColor(black, red);
        printf("File not found\n");
        setConsoleColor(black, white);
        return -1;
    }

    char ctemp;
    char *stemp = (char *)calloc(1, 1);
    int itemp;
    float ftemp;
    stages strtemp;

    int count = -1;

    fread(&itemp, sizeof(int), 1, f);

    fread(&ctemp, sizeof(char), 1, f);
    while (ctemp != '\0')
        fread(&ctemp, sizeof(char), 1, f);

    fread(&itemp, sizeof(int), 1, f);
    fread(&itemp, sizeof(int), 1, f);
    fread(&itemp, sizeof(int), 1, f);

    fread(&ftemp, sizeof(float), 1, f);

    fread(&itemp, sizeof(int), 1, f);
    switch (itemp)
    {
    case 1:
    {
        fread(&itemp, sizeof(int), 1, f);

        fread(&itemp, sizeof(int), 1, f);
        fread(&itemp, sizeof(int), 1, f);
        fread(&itemp, sizeof(int), 1, f);

        break;
    }
    case 2:
    {
        fread(&strtemp, sizeof(stages), 1, f);

        fread(&ctemp, sizeof(char), 1, f);
        while (ctemp != '\0')
            fread(&ctemp, sizeof(char), 1, f);

        break;
    }
    }

    fseek(f, 0, SEEK_SET);
    while (!feof(f))
    {

        fread(&itemp, sizeof(int), 1, f);

        fread(&ctemp, sizeof(char), 1, f);
        while (ctemp != '\0')
            fread(&ctemp, sizeof(char), 1, f);

        fread(&itemp, sizeof(int), 1, f);
        fread(&itemp, sizeof(int), 1, f);
        fread(&itemp, sizeof(int), 1, f);

        fread(&ftemp, sizeof(float), 1, f);

        fread(&itemp, sizeof(int), 1, f);
        switch (itemp)
        {
        case 1:
        {
            fread(&itemp, sizeof(int), 1, f);

            fread(&itemp, sizeof(int), 1, f);
            fread(&itemp, sizeof(int), 1, f);
            fread(&itemp, sizeof(int), 1, f);

            break;
        }
        case 2:
        {
            fread(&strtemp, sizeof(stages), 1, f);

            fread(&ctemp, sizeof(char), 1, f);
            while (ctemp != '\0')
                fread(&ctemp, sizeof(char), 1, f);

            break;
        }
        }

        count++;
    }
    // printf("counter : %i\n", count);

    return count;
}

void supplement_with_data(struct LIST **head)
{
    printf("Enter the file name : ");
    char *nam2 = (char *)calloc(1, 1);
    get_str(&nam2);

    int count = number_str_in_file(nam2);
    // printf("count : %i\n", count);

    FILE *f = fopen(nam2, "rb");
    if (f == NULL)
    {
        setConsoleColor(black, red);
        printf("File not found\n");
        setConsoleColor(black, white);
        return;
    }
    char ctemp;
    char *stemp = (char *)calloc(1, 1);
    int itemp;
    float ftemp;
    stages strtemp;

    for (int i = 0; i < count; i++)
    {
        struct Project *com = (Project *)calloc(1, sizeof(Project));
        fread(&itemp, sizeof(int), 1, f);
        (*com).id = itemp;

        int k = 0;
        fread(&ctemp, sizeof(char), 1, f);
        stemp[k++] = ctemp;
        while (ctemp != '\0')
        {
            fread(&ctemp, sizeof(char), 1, f);
            stemp = (char *)realloc(stemp, (k + 1) * sizeof(char));
            stemp[k++] = ctemp;
        }
        stemp[k] = '\0';
        str_copy(stemp, &(*com).name);

        fread(&itemp, sizeof(int), 1, f);
        (*com).deadline.day = itemp;
        fread(&itemp, sizeof(int), 1, f);
        (*com).deadline.month = itemp;
        fread(&itemp, sizeof(int), 1, f);
        (*com).deadline.year = itemp;

        fread(&ftemp, sizeof(float), 1, f);
        (*com).price = ftemp;

        fread(&itemp, sizeof(int), 1, f);
        (*com).chose = itemp;
        switch ((*com).chose)
        {
        case 1:
        {
            // printf("I in 1\n");
            fread(&itemp, sizeof(int), 1, f);
            (*com).stat.st1.employee = itemp;
            // printf("I scanf : %i\tI write : %i\n",itemp, (*com).stat.st1.employee);

            fread(&itemp, sizeof(int), 1, f);
            (*com).stat.st1.support.day = itemp;
            fread(&itemp, sizeof(int), 1, f);
            (*com).stat.st1.support.month = itemp;
            fread(&itemp, sizeof(int), 1, f);
            (*com).stat.st1.support.year = itemp;
            // printf("I scanf\n");
            break;
        }
        case 2:
        {
            fread(&strtemp, sizeof(stages), 1, f);
            (*com).stat.st2.stage = strtemp;

            k = 0;
            fread(&ctemp, sizeof(char), 1, f);
            stemp[k++] = ctemp;
            while (ctemp != '\0')
            {
                fread(&ctemp, sizeof(char), 1, f);
                stemp = (char *)realloc(stemp, (k + 1) * sizeof(char));
                stemp[k++] = ctemp;
            }
            stemp[k] = '\0';
            str_copy(stemp, &(*com).stat.st2.customer);

            break;
        }
        }

        struct LIST *temp = (struct LIST *)calloc(1, (sizeof(struct LIST)));
        temp->next = NULL;
        // output_Project(com);
        temp->data = com;
        // printList(temp);
        if (*head == NULL)
        {
            temp->next = NULL;
            *head = temp;
        }
        else
        {
            struct LIST *next_p = *head;
            while (next_p->next != NULL)
                next_p = next_p->next;
            next_p->next = temp;
        }

        // printf("cicle : %i\n", i+1);
    }

    fclose(f);
}

int main()
{
    struct LIST *head = NULL;

    // for (int i = 0; i < 4; i++)
    // push_af(&head);

    // printList(head);

    supplement_with_data(&head);

    printList(head);

    return 0;
}
