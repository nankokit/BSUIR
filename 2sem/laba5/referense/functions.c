#include "functions.h"

int var(char c)
{
    if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) return 1;
    return 0;
}
int number(char c)
{
    if(c >= '0' && c <= '9') return 1;
    return 0;
}
int sign(char c)
{
    if (c == '*' || c == '/' || c == '+' || c == '-') return 1;
    return 0;
}
void check_first_element(char c)
{
    if(!sign(c))
    {
        printf(" ! First entered element isn't a sign\n");
        exit(-1);
    }
}
void check_2_last_elements(char* expression)
{
    if( !var(expression[strlen(expression) - 2]) && !number(expression[strlen(expression) - 2]) ||
    !var(expression[strlen(expression) - 3]) && !number(expression[strlen(expression) - 3]))
    {
        printf(" ! Last two entered elements aren't vars/signs\n");
        exit(-1);
    }
}
void check_ratio_of_signs_and_vars_numbers(char *expression)
{
    int signs_count = 0;
    int vars_numbers_count = 0;

    for(int i = 0; expression[i] != '\n' && expression[i] != '\0'; i++)
    {
        if(sign(expression[i])) signs_count++;
        else if(var(expression[i]) || number(expression[i])) vars_numbers_count++;
        else
        {
            printf(" ! Some element isn't sign/var/number\n");
            exit(-1);
        }
    }
    if(signs_count != vars_numbers_count - 1)
    {
        printf(" ! Ratio of signs and vars/numbers isn't correct\n");
        exit(-1);
    }
}

void transformation(char* expression, char* new_expression)
{
    check_first_element( expression[0]);
    check_2_last_elements(expression);
    check_ratio_of_signs_and_vars_numbers(expression);

    list *sample;
    int new_expression_pos = 0;

    char* secret_developments_of_the_c_language = malloc(0);
    free(secret_developments_of_the_c_language);

    for (int i = 0; expression[i] != '\n' && expression[i] != '\0'; i++)
    {
        if (sign(expression[i]))
        {
            push(&sample, expression[i]);
        }
        else
        {
            new_expression[new_expression_pos] = expression[i];
            new_expression_pos++;

            sample->sign_status++;
            int checkbox = 0;
            while(checkbox == 0)
            {
                if(peek_status(sample) == 2)
                {
                    new_expression[new_expression_pos] = pop(&sample);
                    new_expression_pos++;

                    if(sample != NULL)sample->sign_status++;
                }
                if(sample == NULL) checkbox = 1;
                else if(peek_status(sample) != 2) checkbox = 1;
            }
        }
    }
}
char* input(int* size)
{
    char* expression = malloc(0);
    printf("\n * Enter your expression:\n");

    char symbol = 0;
    (*size) = 1;
    for(; symbol != '\n'; (*size)++)
    {
        symbol = getchar();
        expression = realloc(expression, (*size));
        expression[*size - 1] = symbol;
    }
    expression[*size-1] = '\0';
    return expression;
}
void output(char* new_expression)
{
    printf("\n * Your new expression:\n");
    fputs(new_expression, stdout);
}