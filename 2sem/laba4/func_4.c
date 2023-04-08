#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <Windows.h>
typedef struct list LIST;

enum log_condition
{
    STARTING,
    NEW_RECORD,
    MOD_RECORD,
    DEL_RECORD,
    PRINT_LIST,
    SORTING_LIST,
    SEARCH_RECORD,
    END
};

typedef struct date
{
    int day;
    int month;
    int year;
} DATEE;

typedef enum stages
{
    planning = 1,
    analysis,
    design,
    implementation,
    testing,
    maintenance,
} STAGES;

typedef struct status1
{
    int count_staff;
    DATEE date_support;
} STATUS1;

typedef struct status2
{
    STAGES stage;
    char customer[40];
} STATUS2;

typedef union ustatus
{
    STATUS1 st1;
    STATUS2 st2;
} USTATUS;

typedef struct itcompany
{
    int id;
    char name[40];
    DATEE date_deadline;
    int cost;
    int type_status;
    USTATUS status;
    // STATUS1 status1;
    // STATUS2 status2;
} ITCOMPANY;

struct list
{
    LIST *next;
    ITCOMPANY exemplar;
};

void log_record(int event, ITCOMPANY exemplar)
{
    FILE *fp = fopen("log.log", "a+");
    if (fp == NULL)
    {
        printf("File not found");
        exit(1);
    }

    SYSTEMTIME st;
    GetLocalTime(&st);

    switch (event)
    {
    case STARTING:
    {
        fprintf(fp, "[Record of %02d.%02d.%04d]\n", st.wDay, st.wMonth, st.wYear);
        fprintf(fp, "[%02i.%02i.%02i] -- Start of execution\n", st.wHour, st.wMinute, st.wSecond);
        break;
    }
    case NEW_RECORD:
    {
        fprintf(fp, "[%i.%i.%i] -- New list element [id-%d]\n", st.wHour, st.wMinute, st.wSecond, exemplar.id);
        break;
    }
    case MOD_RECORD:
    {
        fprintf(fp, "[%i.%i.%i] -- Modification list element [id-%d]\n", st.wHour, st.wMinute, st.wSecond, exemplar.id);
        break;
    }
    case DEL_RECORD:
    {
        fprintf(fp, "[%i.%i.%i] -- Deletion list element [id-%d]\n", st.wHour, st.wMinute, st.wSecond, exemplar.id);
        break;
    }
    case PRINT_LIST:
    {
        fprintf(fp, "[%i.%i.%i] -- Output list \n", st.wHour, st.wMinute, st.wSecond);
        break;
    }
    case SORTING_LIST:
    {
        fprintf(fp, "[%i.%i.%i] -- Sorting \n", st.wHour, st.wMinute, st.wSecond);
        break;
    }
    case SEARCH_RECORD:
    {
        fprintf(fp, "[%i.%i.%i] -- Search %s \n", st.wHour, st.wMinute, st.wSecond);
        break;
    }
    case END:
    {
        fprintf(fp, "[%i.%i.%i] -- Exit  \n", st.wHour, st.wMinute, st.wSecond);
        break;
    }
    default:
        break;
    }

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

int input_bord(int more, int less)
{
    int temp = input_int();
    while (temp < more || temp > less)
    {
        printf("Incorrect. try again \n");
        temp = input_int();
    }
}

LIST *go_to_n(LIST *head, int n)
{
    int count = 0;
    while (count < n && head)
    {
        head = head->next;
        count++;
    }
    return head;
}

void record_name(ITCOMPANY *exemplar)
{
    char str[255];
    printf("Enter project name: \n");
    input_string(str);
    strcpy(exemplar->name, str);
}

void record_date(DATEE *exemplar)
{
    int temp = 0;

    printf("Enter year deadline: \n");
    temp = input_bord(1970, 2038);
    exemplar->year = temp;

    printf("Enter month deadline: \n");
    temp = input_bord(1, 12);
    exemplar->month = temp;

    printf("Enter day deadline: \n");
    if (exemplar->month == 2)
        temp = input_bord(1, 28);
    else if ((exemplar->month < 8 && (exemplar->month % 2) == 1) || (exemplar->month > 7 && (exemplar->month % 2) == 0))
        temp = input_bord(1, 31);
    else
        temp = input_bord(1, 30);
    exemplar->day = temp;
}

void record_date_deadline(ITCOMPANY *exemplar)
{
    printf("Enter deadline date: \n");
    DATEE deadline;
    record_date(&deadline);
    exemplar->date_deadline = deadline;
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

void record_status1(STATUS1 *exemplar)
{
    int temp = 0;
    printf("Enter count of staff: \n");
    temp = input_int();
    while (temp <= 0)
    {
        printf("Incorrect. try again \n");
        temp = input_int();
    }
    exemplar->count_staff = temp;
    printf("Enter end date of product support: \n");
    DATEE ending;
    record_date(&ending);
    exemplar->date_support = ending;
}

void record_status2(STATUS2 *exemplar)
{
    int temp = 0;
    printf("Enter stage number(1-planning, 2-analysis, 3-design, 4-implementation, 5-testing, 6-maintenance): \n");
    temp = input_bord(1, 6);
    exemplar->stage = temp;
    char str[40];
    printf("Enter customer name: \n");
    input_string(str);
    strcpy(exemplar->customer, str);
}

void record_status(ITCOMPANY *exemplar)
{
    float temp = 0;
    printf("Enter status(1 or 2): \n");
    temp = input_int();
    while (temp != 1 && temp != 2)
    {
        printf("Incorrect. try again \n");
        temp = input_int();
    }
    exemplar->type_status = temp;
    if (temp == 1)
    {
        STATUS1 temp_stat;
        record_status1(&temp_stat);
        exemplar->status.st1 = temp_stat;
    }
    if (temp == 2)
    {
        STATUS2 temp_stat;
        record_status2(&temp_stat);
        exemplar->status.st2 = temp_stat;
    }
}

void new_record(ITCOMPANY *exemplar, int last_id)
{
    exemplar->id = ++last_id;
    record_name(exemplar);
    record_date_deadline(exemplar);
    record_cost(exemplar);
    record_status(exemplar);
}

void print_stage(STAGES stage)
{
    switch (stage)
    {
    case planning:
        printf("planning ");
        break;
    case analysis:
        printf("analysis ");
        break;
    case design:
        printf("design ");
        break;
    case implementation:
        printf("implementation ");
        break;
    case testing:
        printf("testing ");
        break;
    case maintenance:
        printf("maintenance ");
        break;
    default:
        break;
    }
}

void print_exemplar(ITCOMPANY *exemplar)
{
    printf("Project: %s\n", exemplar->name);
    printf("ID: %d\n", exemplar->id);
    printf("deadline: %02d.%02d.%4d \n", exemplar->date_deadline.day, exemplar->date_deadline.month, exemplar->date_deadline.year);
    printf("cost: %d\n", exemplar->cost);
    if (exemplar->type_status == 1)
    {
        printf("count staff: %d\n", exemplar->status.st1.count_staff);
        printf("end date of support: %02d.%02d.%4d\n", exemplar->status.st1.date_support.day,
               exemplar->status.st1.date_support.month, exemplar->status.st1.date_support.year);
    }
    if (exemplar->type_status == 2)
    {
        STAGES remain = exemplar->status.st2.stage;
        print_stage(remain);
        if (remain != 6)
        {
            printf("remaining stages: ");
            remain++;
            for (remain; remain <= 6; remain++)
            {
                print_stage(remain);
                printf(" -> ");
            }
            printf("end\n");
        }
        printf("customer name: %s\n", exemplar->status.st2.customer);
    }
    printf("_____________________________________________________\n");
}

void push(LIST **head, ITCOMPANY new_exemplar, int *count)
{
    LIST *tmp = (LIST *)malloc(sizeof(LIST));
    tmp->exemplar = new_exemplar;
    tmp->next = *head;
    *head = tmp;
    (*count)++;
    log_record(MOD_RECORD, new_exemplar);
}

LIST *getLast(LIST *head)
{
    if (head == NULL)
    {
        return NULL;
    }
    while (head->next)
    {
        head = head->next;
    }
    return head;
}

void pushBack(LIST **head, ITCOMPANY new_exemplar, int *count)
{
    LIST *tmp = calloc(1, sizeof(LIST));
    tmp->exemplar = new_exemplar;
    if (*head == NULL)
    {
        tmp->next = NULL;
        *head = tmp;
    }
    else
    {
        LIST *new = *head;
        while (new->next != NULL)
            new = new->next;
        new->next = tmp;
    }
    (*count)++;
    log_record(NEW_RECORD, new_exemplar);
}

ITCOMPANY pop(LIST *head, int *count)
{
    if (head != NULL)
    {
        LIST *to_removed = head;
        ITCOMPANY removed = to_removed->exemplar;
        head = to_removed->next;
        (*count)--;
        free(to_removed);
        return removed;
    }
    else
        printf("Empty list\n");
}

void print_list(LIST *head)
{
    if (head != NULL)
    {
        LIST *temp = head;
        ITCOMPANY temp_exemplar;
        while (temp != NULL)
        {
            temp_exemplar = temp->exemplar;
            print_exemplar(&temp_exemplar);
            temp = temp->next;
        }

        log_record(PRINT_LIST, temp_exemplar);
    }
    else
        printf("Empty list\n");
}

ITCOMPANY *go_to_ex(LIST *head, int num)
{
    if (head != NULL)
    {
        LIST *temp = head;
        for (int i = 0; i < num; i++)
        {
            temp = temp->next;
        }
        return &(temp->exemplar);
    }
}

void modification(LIST *head, int count)
{
    if (head != NULL)
    {
        LIST *temp = head;
        printf("Select project number\n");
        for (int i = 1; temp != NULL; i++)
        {
            printf("%d - %s\n", i, temp->exemplar.name);
            temp = temp->next;
        }
        int projnum = input_int() - 1;
        temp = head;
        for (int i = 0; i < projnum; i++)
        {
            temp = temp->next;
        }
        print_exemplar(&(temp->exemplar));
        // ITCOMPANY *temporary;
        // temporary = go_to_ex(head, projnum);
        // print_exemplar(temporary);
        log_record(MOD_RECORD, temp->exemplar);
        printf("Select record field:\n");
        printf("1 - all\n2 - name\n3 - data deadline\n4 - cost\n5 - status\n");
        int num = input_int();
        switch (num)
        {
        case 1:
        {
            new_record(&(temp->exemplar), temp->exemplar.id - 1);
            break;
        }
        case 2:
        {
            record_name(&(temp->exemplar));
            break;
        }
        case 3:
        {
            record_date_deadline(&(temp->exemplar));
            break;
        }
        case 4:
        {
            record_cost(&(temp->exemplar));
            break;
        }
        case 5:
        {
            record_status(&(temp->exemplar));
            break;
        }
        default:
        {
            printf("Incorrect input. Try again. \n\n");
            break;
        }
        }
    }
    else
        printf("Empty list\n");
}

void deletion(LIST **head, int count)
{
    if (head != NULL)
    {
        LIST *temp = *head;
        printf("Select project number\n");
        for (int i = 1; temp != NULL; i++)
        {
            printf("%d - %s\n", i, temp->exemplar.name);
            temp = temp->next;
        }
        int num = input_int() - 1;
        temp = go_to_n(*head, num - 1);
        if (temp->next != NULL)
            temp->next = temp->next->next;
        else
            *head = NULL;
        count--;
        log_record(DEL_RECORD, temp->exemplar);
    }
}

int sort_date(DATEE a, DATEE b)
{
    if (a.year == b.year)
    {
        if (a.month == b.month)
        {
            return (a.day < b.day);
        }
        else
            return (a.month < b.month);
    }
    else
        return (a.year < b.year);
}

int sort_condition(ITCOMPANY a, ITCOMPANY b, int condition)
{
    switch (condition)
    {
    case 1:
        return (a.id < b.id);
        break;
    case 2:
        return (strcmp(a.name, b.name) > 0);
        break;
    case 3:
        return sort_date(a.date_deadline, b.date_deadline);
        break;
    case 4:
        return (a.cost < b.cost);
        break;
    case 5:
    {
        if (a.type_status == 1 && b.type_status == 1)
            return (a.status.st1.count_staff < b.status.st1.count_staff);
        else
            return a.type_status == 1;
        break;
    }
    case 6:
    {
        if (a.type_status == 1 && b.type_status == 1)
            return sort_date(a.status.st1.date_support, b.status.st1.date_support);
        else
            return a.type_status == 1;
        break;
    }
    case 7:
    {
        if (a.type_status == 2 && b.type_status == 2)
            return (a.status.st2.stage < b.status.st2.stage);
        else
            return a.type_status == 2;
        break;
    }
    case 8:
    {
        if (a.type_status == 2 && b.type_status == 2)
            return (strcmp(a.status.st2.customer, b.status.st2.customer) > 0);
        else
            return a.type_status == 2;
        break;
    }
    default:
        break;
    };
}

LIST *sort(LIST *head, int condition)
{
    LIST *new_root = NULL;

    while (head != NULL)
    {
        LIST *prev = head;
        head = head->next;

        if (new_root == NULL || sort_condition(prev->exemplar, new_root->exemplar, condition))
        {
            prev->next = new_root;
            new_root = prev;
        }
        else
        {
            LIST *current = new_root;
            while (current->next != NULL && !sort_condition(prev->exemplar, current->next->exemplar, condition))
            {
                current = current->next;
            }

            prev->next = current->next;
            current->next = prev;
        }
    }
    log_record(SORTING_LIST, new_root->exemplar);
    return new_root;
}

int equal_date(DATEE a, DATEE b)
{
    if (a.year == b.year)
    {
        if (a.month == b.month)
        {
            return (a.day == b.day);
        }
    }
    return 0;
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
        return equal_date(a.date_deadline, b.date_deadline);
        break;
    case 4:
        return (a.cost == b.cost);
        break;
    case 5:
    {
        if (a.type_status == 1 && b.type_status == 1)
            return (a.status.st1.count_staff == b.status.st1.count_staff);
        else
            return 0;
        break;
    }
    case 6:
    {
        if (a.type_status == 1 && b.type_status == 1)
            return sort_date(a.status.st1.date_support, b.status.st1.date_support);
        else
            return 0;
        break;
    }
    case 7:
    {
        if (a.type_status == 2 && b.type_status == 2)
            return (a.status.st2.stage == b.status.st2.stage);
        else
            return 0;
        break;
    }
    case 8:
    {
        if (a.type_status == 2 && b.type_status == 2)
            return (strcmp(a.status.st2.customer, b.status.st2.customer) == 0);
        else
            return 0;
        break;
    }
    default:
        break;
    };
}

void search(LIST *head, int condition)
{
    ITCOMPANY find;
    log_record(MOD_RECORD, find);
    switch (condition)
    {
    case 1:
    {
        do
        {
            printf("Enter search id: ");
            find.id = input_int();
        } while (find.id < 0);
        break;
    }
    case 2:
    {
        record_name(&find);
        break;
    }
    case 3:
    {
        record_date_deadline(&find);
        break;
    }
    case 4:
    {
        record_cost(&find);
        break;
    }
    case 5:
    {
        printf("Enter search count of staff: ");
        find.type_status = 1;
        find.status.st1.count_staff = input_int();
        while (find.status.st1.count_staff <= 0)
        {
            printf("Incorrect. try again \n");
            find.status.st1.count_staff = input_int();
        }
        break;
    }
    case 6:
    {
        printf("Enter search end date of product support: \n");
        DATEE ending;
        find.type_status = 1;
        record_date(&ending);
        find.status.st1.date_support = ending;
        break;
    }
    case 7:
    {
        printf("Enter stage number(1-planning, 2-analysis, 3-design, 4-implementation, 5-testing, 6-maintenance): \n");
        find.type_status = 2;
        find.status.st2.stage = input_bord(1, 6);
        break;
    }
    case 8:
    {
        printf("Enter customer name: \n");
        char str[40];
        input_string(str);
        find.type_status = 2;
        strcpy(find.status.st2.customer, str);
        break;
    }
    default:
        break;
    }

    while (head != NULL)
    {
        if (equal_condition(head->exemplar, find, condition))
            print_exemplar(&(head->exemplar));
        head = head->next;
    }
}

// ITCOMPANY pop(LIST **head)
// {
//     LIST *to_removed = NULL;
//     ITCOMPANY removed;
//     if (head == NULL)
//     {
//         exit(-1);
//     }
//     to_removed = (*head);
//     removed = to_removed->exemplar;
//     (*head) = (*head)->next;
//     free(to_removed);
//     return removed;
// }
