#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <io.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "E:\sishka\4vitya/functions.h"
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

enum fild
{
    ID = 1,
    NAME,
    DAT,
    PRICE,
    STATUS,
};

enum fild2
{
    EMPLOYEE = 1,
    SUPPORT,
    STAGE,
    CUSTOMER
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
    FILE *fp = fopen("log.log", "a");
    if (fp == NULL)
    {
        setConsoleColor(black, red);
        printf("File not found");
        setConsoleColor(black, white);
        exit(1);
    }
    time_t mytime = time(NULL);
    struct tm *now = localtime(&mytime);

    fprintf(fp, "[%i.%i.%i] Added a structure with the name : %s\n", now->tm_hour, now->tm_min, now->tm_sec, (*head)->data->name);

    fclose(fp);
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

    FILE *fp = fopen("log.log", "a");
    if (fp == NULL)
    {
        setConsoleColor(black, red);
        printf("File not found");
        setConsoleColor(black, white);
        exit(1);
    }
    time_t mytime = time(NULL);
    struct tm *now = localtime(&mytime);

    fprintf(fp, "[%i.%i.%i] Output a list of structures\n", now->tm_hour, now->tm_min, now->tm_sec);

    fclose(fp);
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

    FILE *fp = fopen("log.log", "a");
    if (fp == NULL)
    {
        setConsoleColor(black, red);
        printf("File not found");
        setConsoleColor(black, white);
        exit(1);
    }
    time_t mytime = time(NULL);
    struct tm *now = localtime(&mytime);

    fprintf(fp, "[%i.%i.%i] The list of structures is saved to a file named : %s\n", now->tm_hour, now->tm_min, now->tm_sec, nam);

    fclose(fp);

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

    FILE *fp = fopen("log.log", "a");
    if (fp == NULL)
    {
        setConsoleColor(black, red);
        printf("File not found");
        setConsoleColor(black, white);
        exit(1);
    }
    time_t mytime = time(NULL);
    struct tm *now = localtime(&mytime);

    fprintf(fp, "[%i.%i.%i] The list of structures is supplemented from a file named : %s\n", now->tm_hour, now->tm_min, now->tm_sec, nam2);

    fclose(fp);
}

// bool compare()

void find_emp(struct LIST *head)
{
    if (head == NULL)
    {
        printf("The list is empty\n");
        return;
    }
    int emp;
    printf("Enter number of assigned employees : ");
    while (int_int_check(emp) || emp < 0)
    {
        setConsoleColor(black, red);
        printf("Error! Please enter the correct data\n");
        rewind(stdin);
        setConsoleColor(black, white);
    }
    int count = 0;
    struct LIST *node = head;
    while (node != NULL)
    {
        if (node->data->chose == 1 && node->data->stat.st1.employee == emp)
        {
            output_Project(node->data);
            printf("\n");
            count++;
        }
        node = node->next;
    }
    if (count == 0)
        printf("No matches\n");
}

// возвращает TRUE если  дата1 != дата2
bool data_sort2(struct data d1, struct data d2)
{
    bool res = 0;
    if (d1.year != d2.year)
    {
        res = 1;
    }
    else
    {
        if (d1.month != d2.month)
        {
            res = 1;
        }
        else if (d1.day != d2.day)
            res = 1;
    }

    return res;
}

void find_data(struct LIST *head)
{
    if (head == NULL)
    {
        printf("The list is empty\n");
        return;
    }
    printf("Enter date of product support : \n");
    data dt;
    dt = input_data(&dt);
    int count = 0;
    struct LIST *node = head;
    while (node != NULL)
    {
        if (node->data->chose == 1 && !data_sort2(node->data->stat.st1.support, dt))
        {
            output_Project(node->data);
            printf("\n");
            count++;
        }
        node = node->next;
    }
    if (count == 0)
        printf("No matches\n");
}

void find_stage(struct LIST *head)
{
    if (head == NULL)
    {
        printf("The list is empty\n");
        return;
    }
    printf("Enter remaining stages of development :\n1 -- Planning\n2 -- Analysis\n3 -- Design\n4 -- Implementation\n5 -- Testing\n6 -- Maintenance\n");
    stages st;
    while (scanf("%i", &st) != 1 || getchar() != '\n' || st <= 0 || st > 6)
    {
        setConsoleColor(black, red);
        printf("Error! Please enter the correct data\n");
        rewind(stdin);
        setConsoleColor(black, white);
    }

    int count = 0;
    struct LIST *node = head;
    while (node != NULL)
    {
        if (node->data->chose == 2 && node->data->stat.st2.stage <= st)
        {
            output_Project(node->data);
            printf("\n");
            count++;
        }
        node = node->next;
    }
    if (count == 0)
        printf("No matches\n");
}

void find_cust(struct LIST *head)
{
    if (head == NULL)
    {
        printf("The list is empty\n");
        return;
    }
    printf("Enter the customer : \n");
    char *temp = (char *)calloc(1, 1);
    get_str(&temp);
    int count = 0;
    struct LIST *node = head;
    while (node != NULL)
    {
        // char* temp2 = (char*)calloc(1, 1);
        // str_copy(node->data->stat.st2.customer, &temp2);
        // printf("%s\n", node->data->stat.st2.customer);
        if (node->data->chose == 2 && strcmp(node->data->stat.st2.customer, temp) == 0)
        {
            output_Project(node->data);
            printf("\n");
            count++;
        }
        node = node->next;
    }
    if (count == 0)
        printf("No matches\n");
}

void find(struct LIST *head)
{
    printf("Selct the search parameters :\n1 -- Number of assigned employees\n2 -- Date of product support\n3 -- Remaining stages of development\n4 -- The customer\n");
    int c;
    while (int_int_check(c) || c <= 0 || c > 4)
    {
        setConsoleColor(black, red);
        printf("Error! Please enter the correct data\n");
        rewind(stdin);
        setConsoleColor(black, white);
    }

    switch (c)
    {
    case 1:
    {
        find_emp(head);
        break;
    }
    case 2:
    {
        find_data(head);
        break;
    }
    case 3:
    {
        find_stage(head);
        break;
    }
    case 4:
    {
        find_cust(head);
        break;
    }
    }
    FILE *fp = fopen("log.log", "a");
    if (fp == NULL)
    {
        setConsoleColor(black, red);
        printf("File not found");
        setConsoleColor(black, white);
        exit(1);
    }
    time_t mytime = time(NULL);
    struct tm *now = localtime(&mytime);

    switch (c)
    {
    case 1:
    {
        fprintf(fp, "[%i.%i.%i] Performed a search by the number of employees\n", now->tm_hour, now->tm_min, now->tm_sec);
        break;
    }
    case 2:
    {
        fprintf(fp, "[%i.%i.%i] Performed a search by the end date of support\n", now->tm_hour, now->tm_min, now->tm_sec);
        break;
    }
    case 3:
    {
        fprintf(fp, "[%i.%i.%i] A search was conducted for the remaining stages of development\n", now->tm_hour, now->tm_min, now->tm_sec);
        break;
    }
    case 4:
    {
        fprintf(fp, "[%i.%i.%i] A search was conducted by the customer's name\n", now->tm_hour, now->tm_min, now->tm_sec);
        break;
    }
    }

    fclose(fp);
}

// возвращает TRUE если  дата1 > дата2
bool data_sort(struct data d1, struct data d2)
{
    bool res = 0;
    if (d1.year != d2.year)
    {
        if (d1.year > d2.year)
            res = 1;
    }
    else
    {
        if (d1.month != d2.month)
        {
            if (d1.month > d2.month)
                res = 1;
        }
        else if (d1.day > d2.day)
            res = 1;
    }

    return res;
}

// параметр сортировки (возвращает TRUE если Jтый элемент > J+1)
bool par_sort(int cmp, struct Project *s1, struct Project *s2)
{
    bool res = 0;

    switch (cmp)
    {
    case 1:
    {
        if (s1->chose > s2->chose)
            return 1;
        if (s1->chose < s2->chose)
            return 0;
        if (s1->chose == 2 && s2->chose == 2)
            return 0;
        res = (s1->stat.st1.employee > s2->stat.st1.employee);
        break;
    }
    case 2:
    {
        if (s1->chose > s2->chose)
            return 1;
        if (s1->chose < s2->chose)
            return 0;
        if (s1->chose == 2 && s2->chose == 2)
            return 0;
        res = data_sort(s1->stat.st1.support, s2->stat.st1.support);
        break;
    }
    case 3:
    {
        if (s1->chose < s2->chose)
            return 1;
        if (s1->chose > s2->chose)
            return 0;
        if (s1->chose == 1 && s2->chose == 1)
            return 0;
        res = (s1->stat.st2.stage > s2->stat.st2.stage);
        break;
    }
    case 4:
    {
        if (s1->chose < s2->chose)
            return 1;
        if (s1->chose > s2->chose)
            return 0;
        if (s1->chose == 1 && s2->chose == 1)
            return 0;
        res = (strcmp(s1->stat.st2.customer, s2->stat.st2.customer) > 0);
        break;
    }
    }

    return res;
}

// сортировка по любому 1 параметру
void struct_sort(struct LIST **head)
{
    printf("Selct the sort option :\n1 -- Number of assigned employees\n2 -- Date of product support\n3 -- Remaining stages of development\n4 -- The customer\n\n");
    int cmp;
    while (scanf("%i", &cmp) != 1 || getchar() != '\n' || cmp <= 0 || cmp > 4)
    {
        setConsoleColor(black, red);
        printf("Error! Please enter the correct data\n");
        rewind(stdin);
        setConsoleColor(black, white);
    }
    int n = 0;
    struct LIST *node = *head;
    while (node != NULL)
    {
        n++;
        node = node->next;
    }
    // printf("n : %i\n", n);

    struct Project *stemp;
    struct LIST *node1 = *head;
    // struct LIST* node2 = *head;
    for (int i = 0; i < n - 1; i++)
    {
        while (node1->next != NULL)
        {
            if (par_sort(cmp, node1->data, node1->next->data))
            {
                stemp = node1->data;
                node1->data = node1->next->data;
                node1->next->data = stemp;
            }
            node1 = node1->next;
        }
        node1 = *head;
        // node2 = node2->next;
    }

    FILE *fp = fopen("log.log", "a");
    if (fp == NULL)
    {
        setConsoleColor(black, red);
        printf("File not found");
        setConsoleColor(black, white);
        exit(1);
    }
    time_t mytime = time(NULL);
    struct tm *now = localtime(&mytime);

    switch (cmp)
    {
    case 1:
    {
        fprintf(fp, "[%i.%i.%i] Sorting was carried out by the number of employees\n", now->tm_hour, now->tm_min, now->tm_sec);
        break;
    }
    case 2:
    {
        fprintf(fp, "[%i.%i.%i] Sorting was carried out by the end date of support\n", now->tm_hour, now->tm_min, now->tm_sec);
        break;
    }
    case 3:
    {
        fprintf(fp, "[%i.%i.%i] Sorting was carried out by the remaining stages of development\n", now->tm_hour, now->tm_min, now->tm_sec);
        break;
    }
    case 4:
    {
        fprintf(fp, "[%i.%i.%i] Sorting was carried out by the customer's name\n", now->tm_hour, now->tm_min, now->tm_sec);
        break;
    }
    }

    fclose(fp);
}

// удаляет структуру с ID
void del_struct(struct LIST **head)
{

    printf("Enter the Name of project you want to delete : ");
    int idcmp, fl = 0;
    char *temp = (char *)calloc(1, 1);
    get_str(&temp);
    struct LIST *node = *head;
    while (node->next != NULL)
    {
        if (strcmp(temp, node->data->name) == 0)
        {
            output_Project(node->data);
            printf("\n");
            fl++;
        }
        node = node->next;
    }
    if (fl == 0)
    {
        printf("No Project with Name : %s\n", temp);
        return;
    }
    if (fl == 1)
    {
        int fl1 = 0;
        struct LIST *node1 = *head, *prev = NULL;

        while (node1->next != NULL && strcmp(temp, node1->data->name) != 0)
        {
            prev = node1;
            node1 = node1->next;
        }

        if (prev == NULL)
        {
            *head = node1->next;
        }
        else
        {
            prev->next = node1->next;
        }
    }
    if (fl > 1)
    {
        fl = 0;
        printf("Enter the ID of project you want to delete : ");
        while (int_int_check(idcmp) || idcmp <= 0)
        {
            setConsoleColor(black, red);
            printf("Error! Please enter the correct data\n");
            rewind(stdin);
            setConsoleColor(black, white);
        }

        int fl1 = 0;
        struct LIST *node1 = *head, *prev = NULL;

        while (node1->next != NULL && strcmp(temp, node1->data->name) != 0 && node1->data->id == idcmp)
        {
            prev = node1;
            node1 = node1->next;
            fl = 1;
        }

        if (prev == NULL)
        {
            *head = node1->next;
        }
        else
        {
            prev->next = node1->next;
        }
    }
    if (!fl)
        printf("No Project with id : %i\n", idcmp);
    if (fl)
    {
        FILE *fp = fopen("log.log", "a");
        if (fp == NULL)
        {
            setConsoleColor(black, red);
            printf("File not found");
            setConsoleColor(black, white);
            exit(1);
        }
        time_t mytime = time(NULL);
        struct tm *now = localtime(&mytime);

        fprintf(fp, "[%i.%i.%i] Removed a structure with the name : %s\n", now->tm_hour, now->tm_min, now->tm_sec, temp);

        fclose(fp);
    }
    // inst: kaplich_
}

void laba4(struct LIST **head)
{
    printf("\nSelect a function :\n 1) Add one structure\n 2) Output structures\n 3) Add structures from a file\n 4) Save structures to a file\n 5) Delete one structure\n 6) Sort structures\n 7) Search for new data\n 8) End the program\n");

    int fun;
    while (int_int_check(fun) || fun <= 0 || fun > 8)
    {
        setConsoleColor(black, red);
        printf("Error! Please enter the correct data\n");
        rewind(stdin);
        setConsoleColor(black, white);
    }

    switch (fun)
    {
    case 1:
    {
        push_af(head);
        break;
    }
    case 2:
    {
        printList(*head);
        break;
    }
    case 3:
    {
        supplement_with_data(head);
        break;
    }
    case 4:
    {
        char *name = safe_to_file(*head);
        break;
    }
    case 5:
    {
        del_struct(head);
        break;
    }
    case 6:
    {
        struct_sort(head);
        break;
    }
    case 7:
    {
        find(*head);
        break;
    }
    case 8:
    {
        return;
        break;
    }
    }

    laba4(head);
}

union bit
{
    unsigned int num;
    unsigned int bit2 : 1;
    unsigned int bit4 : 2;
    unsigned int bit8 : 3;
    unsigned int bit16 : 4;
    // unsigned int bit32 : 5;
    // unsigned int bit64 : 6;
    // unsigned int bit128 : 7;
    // unsigned int bit256 : 8;
};

// void divide(BinaryNum dividend, BinaryNum divisor, BinaryNum* remainder) {
// BinaryNum temp = { 0 };
// int num_bits = /*sizeof(dividend.num) **/ 8; // Количество бит в числе

//// Проверка на случай деления на ноль
// if (divisor.num == 0) {
//     printf("Error: Division by zero");
//     exit(1);
// }

//// Выполнение целочисленного деления столбиком
// for (int i = num_bits - 1; i >= 0; i--) {
//     temp.num <<= 1;
//     temp.bits.bit0 = dividend.bits.bit7;
//     dividend.num <<= 1;
//     if (temp.num >= divisor.num) {
//         temp.num -= divisor.num;
//     }
// }

//*remainder = temp;
//}

// void print_binary(BinaryNum num) {
//     printf("%d%d%d%d%d%d%d%d", num.bits.bit7, num.bits.bit6, num.bits.bit5, num.bits.bit4,num.bits.bit3, num.bits.bit2, num.bits.bit1, num.bits.bit0);
// }

//// Calculate the most significant bit position of a number
// int msb_position(unsigned int n) {
//     int msb = -1;
//     while (n) {
//         n >>= 1;
//         msb++;
//     }
//     return msb;
// }
//
//// Compare two integers: return 0 if a == b, 1 if a > b, -1 if a < b
// int compare(int a, int b) {
//     int result = a - b;
//     return (result > 0) - (result < 0);
// }
//
//// Generic modulo operation using bitwise operators
// int bitwise_modulo(int n, int m) {
//     if (m == 0) {
//         printf("Error: division by zero.\n");
//         return -1;
//     }
//
//     if (n < 0) {
//         n = -n;
//     }
//
//     int msb_n = msb_position(n);
//     int msb_m = msb_position(m);
//
//     for (int i = msb_n - msb_m; i >= 0; i--) {
//         if (compare(n, (m << i)) >= 0) {
//             n -= (m << i);
//         }
//     }
//
//     return n;
// }

int main(/*int argc, char** argv*/)
{
    struct LIST *head = NULL;

    FILE *fp = fopen("log.log", "w");
    if (fp == NULL)
    {
        setConsoleColor(black, red);
        printf("File not found");
        setConsoleColor(black, white);
        exit(1);
    }
    time_t mytime = time(NULL);
    struct tm *now = localtime(&mytime);

    fprintf(fp, "[%i.%i.%i] The program is running\n", now->tm_hour, now->tm_min, now->tm_sec);

    fclose(fp);

    // int result = bitwise_modulo(n, m);
    // printf("%d %% %d = %d\n", n, m, result);

    // BinaryNum dividend = {unsigned(atoi(argv[1]))};
    // BinaryNum divisor = {unsigned(atoi(argv[2]))};
    // BinaryNum remainder = {0};
    ////printf("Dividend: %i\n");
    ////print_binary(dividend);
    ////printf("Divisor: ");
    ////print_binary(divisor);
    // printf("%i %% %i = ", dividend, divisor);
    // divide(dividend, divisor, &remainder);
    // printf("%d\n", remainder.num);
    ////print_binary(remainder);

    // bit qq;
    // qq.num = { unsigned(atoi(argv[1])) };

    // if (qq.num > 0 || qq.num != 2147483647)
    //     printf("all number : %i\n2 : %i\n4 : %i\n8 : %i\n16 : %i", qq.num, qq.bit2, qq.bit4, qq.bit8, qq.bit16);
    // else
    //     printf("aboba\n");
    // fp = fopen("log.log", "a");
    // if (fp == NULL) {
    //     setConsoleColor(black, red);
    //     printf("File not found");
    //     setConsoleColor(black, white);
    //     exit(1);
    // }
    //  mytime = time(NULL);
    //  now = localtime(&mytime);

    // fprintf(fp, "[%i.%i.%i] The program has found the remainder of the number accumulation : %i\n", now->tm_hour, now->tm_min, now->tm_sec, qq.num);

    // fclose(fp);

    laba4(&head);

    fp = fopen("log.log", "a");
    if (fp == NULL)
    {
        setConsoleColor(black, red);
        printf("File not found");
        setConsoleColor(black, white);
        exit(1);
    }
    mytime = time(NULL);
    now = localtime(&mytime);

    fprintf(fp, "[%i.%i.%i] The program is completed", now->tm_hour, now->tm_min, now->tm_sec);

    fclose(fp);
    return 0;
}
