#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>
#include <locale.h>

long long scid()
{
    long long n, K;
    char a;
    while (1)
    {
        n = scanf_s("%lld", &K);
        scanf_s("%c", &a);
        if (n != 0 && a == '\n' && K > 0)
        {
            return K;
        }
        else
        {
            printf("Wrong data \n");
            rewind(stdin);
        }
    }
}

typedef struct sNode
{
    char value;
    struct sNode *next;
} tNode;

typedef struct
{
    tNode *p_begin;
    size_t m_size;
} tStack;

void stack_push(tStack *s, char value)
{
    tNode *p = (tNode *)malloc(sizeof(tNode));
    p->value = value;
    p->next = s->p_begin;
    s->p_begin = p;
    s->m_size++;
}
char stack_pop(tStack *s)
{
    tNode *tmp = s->p_begin;
    char tmp_value = tmp->value;
    s->p_begin = s->p_begin->next;
    s->m_size--;
    free(tmp);
    return tmp_value;
}

char stack_top(const tStack *s)
{
    return s->p_begin->value;
}

int stack_is_empty(const tStack *s)
{
    return s->m_size == 0;
}

tStack stack_create()
{
    tStack new_stack = {NULL, 0};
    return new_stack;
}

int priority(char c)
{
    switch (c)
    {
    case '(':
        return 0;
    case ')':
        return 1;
    case '+':
        return 2;
    case '-':
        return 2;
    case '*':
        return 3;
    case '/':
        return 3;
    case '^':
        return 4;
    default:
        return 10;
    }
}

void get_rpn(char *s)
{
    tStack St = stack_create();
    tStack *Stack = &St;
    char res[50];
    int lenth = strlen(s), j = lenth - 1;
    res[lenth] = '\0';

    for (int i = lenth - 1; i >= 0; i--)
    {
        if (priority(s[i]) == 10)
        {
            res[j] = s[i];
            j--;
        }
        else if (priority(s[i]) == 2 && (i == 0 || (priority(s[i - 1]) != 10 && priority(s[i - 1]) != 1)))
        {
            res[strlen(res) + 1] = res[strlen(res)];
            res[strlen(res)] = ' ';
        }
        else if (stack_is_empty(Stack))
        {
            stack_push(Stack, s[i]);
        }
        else if (!stack_is_empty(Stack) && priority(s[i]) == 1)
        {
            stack_push(Stack, s[i]);
        }
        else if (!stack_is_empty(Stack) && priority(s[i]) == 0)
        {
            while (!stack_is_empty(Stack) && stack_top(Stack) != ')')
            {
                res[j] = stack_top(Stack);
                j--;
                stack_pop(Stack);
            }
            if (!stack_is_empty(Stack))
                stack_pop(Stack);
        }
        else if (!stack_is_empty(Stack) && priority(stack_top(Stack)) > priority(s[i]))
        {
            while (!stack_is_empty(Stack) && priority(stack_top(Stack)) > priority(s[i]))
            {
                res[j] = stack_top(Stack);
                j--;
                stack_pop(Stack);
            }
            stack_push(Stack, s[i]);
        }
        else
        {
            stack_push(Stack, s[i]);
        }
    }
    while (!stack_is_empty(Stack))
    {
        res[j] = stack_top(Stack);
        j--;
        stack_pop(Stack);
    }

    printf("Строка в префиксной форме:\n");
    for (int i = j + 1; i < strlen(res); i++)
    {
        printf("%c", res[i]);
    }
    printf("\n");
}

int main()
{
    // setlocale(LC_ALL, "Rus");
    // SetConsoleCP(1251);
    // SetConsoleOutputCP(1251);
    while (1)
    {
        printf("Выберите действие:\n1. Выполнить преобразование\n2. Закончить работу\n");
        int choose = scid();
        if (choose == 2)
            return 0;
        printf("Введите строку в инфиксной форме:\n");
        char inf[50], pref[50];
        fgets(inf, 50, stdin);
        inf[strlen(inf) - 1] = 0;
        get_rpn(inf);
    }
}