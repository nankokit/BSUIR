#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Преобразование выражения из инфиксной формы в префиксную.
typedef struct list LIST;
typedef struct stack STACK;

struct list
{
    LIST *next;
    char value;
};

struct stack
{
    LIST *head;
    int count;
};

void push(STACK *st, char new)
{
    LIST *tmp = (LIST *)malloc(sizeof(LIST));
    tmp->value = new;
    tmp->next = st->head;
    st->head = tmp;
    st->count++;
}

char pop(STACK *st)
{
    if (st->head == NULL)
    {
        exit(-1);
    }
    LIST *to_removed = st->head;
    char removed = to_removed->value;
    st->head = to_removed->next;
    st->count--;
    free(to_removed);
    return removed;
}

int empty_stack(STACK *st)
{
    return st->count == 0;
}

char stack_top(STACK *st)
{
    return st->head->value;
}

int values(char c)
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
    case ' ':
        return 10;
    case '\n':
        return 11;
    case '\0':
        return 12;
    default:
        return -1;
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

int input_string(char *str)
{
    char c;
    int i = 0;
    while ((c = getchar()) != '\n')
    {
        str[i] = c;
        i++;
    }
    str[i] = '\0';
    return i;
}

int not_token(char input, char *output, int *index)
{
    if (values(input) == -1)
    {
        output[*index] = input;
        (*index)--;
        return 1;
    }
    else if (values(input) == 10 || values(input) == 11 || values(input) == 12)
        return 1;
    else
        return 0;
}

int count_symbols(char *str)
{
    int i = 0, count = 0;
    while (str[i] != '\n' && str[i] != '\0')
    {
        if (str[i] != ' ' && str[i] != '(' && str[i] != ')')
            count++;
        i++;
    }
    return count;
}

int right_symbols(char *str)
{
    int i = 0, count0 = 0, count1 = 0, count2 = 0, count3 = 0, token = 0, red = 0;
    while (str[i] != '\n' && str[i] != '\0')
    {
        if (str[i] == '(')
            count0++;
        else if (str[i] == ')')
            count1++;
        else if (str[i] == '+' || str[i] == '-')
            count2++;
        else if (str[i] == '*' || str[i] == '/')
            count3++;
        else if ((str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A' && str[i] <= 'Z')) //|| (str[i] >= '0' && str[i] <= '9')
            token++;
        else if (str[i] == ' ')
            ;
        else
            red++;
        i++;
    }
    if (count0 != count1 || (count2 + count3 + 1) != token)
        red++;
    return red == 0;
}

void main()
{
    while (1)
    {
        printf("Enter infix expression:\n");
        char input[50], output[50];
        input_string(input);
        if (!right_symbols(input))
        {
            printf("Enter infix expression again:\n");
            input_string(input);
        }
        // int count = input_string(input);

        int count = strlen(input);
        // int count_s = count_symbols(input);

        int i = count - 1, j = i;
        STACK tokens = {NULL, 0};
        char temp;
        for (i; i >= 0; i--)
        {
            if (not_token(input[i], output, &j)) // all other symbols
            {
                continue;
            }

            else if (empty_stack(&tokens))
            {
                push(&tokens, input[i]);
            }

            else if (values(input[i]) == 2) // + - && (i != 0 && values(input[i - 1]) != 10)
            {
                if (stack_top(&tokens) == 2 || values(stack_top(&tokens)) == 3)
                {
                    output[j] = pop(&tokens);
                    j--;
                }
                push(&tokens, input[i]);
            }

            else if (values(input[i]) == 3) // * /
            {
                if (values(stack_top(&tokens)) == 3)
                {
                    output[j] = pop(&tokens);
                    j--;
                }
                push(&tokens, input[i]);
            }

            else if (values(input[i]) == 1) // open )
            {
                push(&tokens, input[i]);
            }

            else if (values(input[i]) == 0) // close (
            {
                char temp;
                while (!empty_stack(&tokens) && stack_top(&tokens) != ')')
                {
                    if (values(temp = pop(&tokens)) != 1)
                    {
                        printf("%c ", temp);
                        output[j] = temp;
                        j--;
                    }
                }
                pop(&tokens);
            }
        }
        while (!empty_stack(&tokens))
        {
            temp = pop(&tokens);
            output[j] = temp;
            j--;
        }
        output[count] = '\0';
        printf("Result: %s\n", output);
    }
}