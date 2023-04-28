#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node
{
    struct node *next;
    struct node *prev;
    char *value;
} NODE;

typedef struct stdeq
{
    NODE *head;
    NODE *tail;
    int count;
} DEQ;

int int_input()
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

void pushFront(DEQ *deq, char *value)
{
    NODE *new = malloc(sizeof(NODE));
    if (new == NULL)
    {
        exit(1);
    }
    new->value = value;
    new->next = deq->head;
    new->prev = NULL;
    if (deq->head != NULL)
    {
        deq->head->prev = new;
    }
    if (deq->tail == NULL)
        deq->tail = new;
    deq->head = new;
    deq->count++;
}

void pushBack(DEQ *deq, char *value)
{
    NODE *new = malloc(sizeof(NODE));
    if (new == NULL)
    {
        exit(1);
    }
    new->value = value;
    new->next = NULL;
    new->prev = deq->tail;
    if (deq->tail != NULL)
    {
        deq->tail->next = new;
    }
    if (deq->head == NULL)
        deq->head = new;
    deq->tail = new;
    deq->count++;
}

NODE *popFront(DEQ *deq)
{
    if (deq->head == NULL)
    {
        return NULL;
    }
    NODE *new;
    new = deq->head;
    deq->head = deq->head->next;
    // if (deq->head == deq->tail)
    // {
    //     deq->head = NULL;
    //     deq->tail = NULL;
    // }
    if (deq->head != NULL)
    {
        deq->head->prev = NULL;
    }
    deq->count--;
    return new;
}

NODE *popBack(DEQ *deq)
{
    if (deq->tail == NULL)
    {
        return NULL;
    }
    NODE *new = malloc(sizeof(NODE));
    new = deq->tail;
    deq->tail = deq->tail->prev;
    // if (deq->head == deq->tail)
    // {
    //     deq->head = NULL;
    //     deq->tail = NULL;
    // }
    if (deq->tail != NULL)
    {
        deq->tail->next = NULL;
    }
    deq->count--;
    return new;
}

FILE *file_open(char *fname, char *mode)
{
    FILE *fp = fopen(fname, mode);
    if (fp == NULL)
    {
        printf("File error\n");
        return 0;
    }
    return fp;
}

char *res(char *str, int *i)
{
    char *result = calloc(255, sizeof(char));
    int j = 0;
    while (str[*i] != '(')
        (*i)++;
    (*i)++;
    while (str[*i] != ')')
    {
        result[j++] = str[(*i)++];
    }
    return result;
}

void record_file(DEQ *deq, char *str)
{
    FILE *fp = file_open("new.txt", "a+b");
    fprintf(fp, "%s\n", str);
    fclose(fp);
}

int choise(char *str, char **pointer)
{
    char *func1 = "pushfront";
    char *func2 = "pushback";
    char *func3 = "popfront";
    char *func4 = "popback";
    int choise = 0;
    char *point;
    if (strstr(str, func1) != NULL)
    {
        point = strstr(str, func1);
        choise = 1;
    }
    else if (strstr(str, func2) != NULL)
    {
        point = strstr(str, func2);
        choise = 2;
    }
    else if (strstr(str, func3) != NULL)
    {
        point = strstr(str, func3);
        choise = 3;
    }
    else if (strstr(str, func4) != NULL)
    {
        point = strstr(str, func4);
        choise = 1;
    }
    *pointer = point;
    return choise;
}

void read_file(DEQ *deq)
{
    FILE *fp = file_open("sourse.txt", "r");
    char *str = calloc(255, sizeof(char));
    char *result;
    int i;
    char *pointer;
    while (!feof(fp))
    {
        i = 0;
        fgets(str, 255, fp);
        // printf("New string %s\n", str);
        switch (choise(str, &pointer))
        {
        case 1:
        {
            i = pointer - str;
            result = res(str, &i);
            pushFront(deq, result);
            break;
        }
        case 2:
        {
            i = pointer - str;
            result = res(str, &i);
            pushBack(deq, result);
            break;
        }
        case 3:
        {
            NODE *tmp = popFront(deq);
            if (tmp == NULL)
                result = "EMPTY";
            else
                result = tmp->value;
            printf("popFront %s\n", result);
            record_file(deq, result);
            break;
        }
        case 4:
        {
            NODE *tmp = popBack(deq);
            if (tmp == NULL)
                result = "EMPTY";
            else
                result = tmp->value;
            printf("popBack %s\n", result);
            record_file(deq, result);
            break;
        }
        default:
            break;
        }
    }
}

DEQ *create_deq()
{
    DEQ *deq = malloc(sizeof(DEQ));
    deq->count = 0;
    deq->head = NULL;
    deq->tail = NULL;
    return deq;
}

int main()
{
    DEQ *deq = create_deq();
    char *tmp;
    read_file(deq);
    return 0;
}

/*
char *shift_str(char *str, int i)
{
    char *result = calloc(255, sizeof(char));
    int j = 0;
    // i--;
    while (str[i] != '\0')
    {
        result[j] = str[i];
        j++;
        i++;
    }

    return result;
}
*/