#include <stdio.h>
#include <malloc.h>
#include <time.h>
#include <stdlib.h>

long double int_input()
{
    int readinput;
    long double input;
    char inputchar;

    while (1)
    {
        readinput = scanf("%Lf", &input);
        scanf("%c", &inputchar);
        if (readinput != 0 && inputchar == '\n' && (int)input == input)
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

void task1()
{
    int *array;
    int i, size;
    printf("Enter number of array elements: ");
    size = int_input();
    while (size <= 0)
    {
        printf("Incorrect input. Try again. \n");
        size = int_input();
    }

    array = (int *)malloc(size * sizeof(int));

    srand((int)time(NULL));

    // selection input method
    printf("Select array input method:\n1 - random input\n2 - keyboard input\n");
    int check = -1, method;
    while (check == -1)
    {
        method = int_input();
        if (method == 1)
        {
            check = 1;
            for (int i = 0; i < size; i++)
            {
                printf("array[%d] = ", i);
                array[i] = rand() % 201 - 100;
                printf("%d\n", array[i]);
            }
        }
        else
        {
            if (method == 2)
            {
                check = 1;
                printf("Enter array elements \n");
                for (int i = 0; i < size; i++)
                {
                    printf("array[%d] = ", i);
                    array[i] = int_input();
                }
            }
            else
            {
                printf("Incorrect. Try again. \n");
                rewind(stdin);
            }
        }
    }
    for (i = 0; i < size; i++)
    {
        if (array[i] < 0)
        {
            for (int j = i; j < size - 1; j++)
            {
                array[j] = array[j + 1];
            }
            size--;
            i = -1;
        }
    }
    for (i = 0; i < size; i++)
        if (size > 0)
            printf("%d ", array[i]);
        else
            printf("Empty array \n ");
    free(array);
}

void task2()
{
    int size, i, j, inputint;
    printf("Enter the number of strings: ");
    size = int_input();
    // size = int_input();
    int **array = (int **)malloc(size * sizeof(int *));
    int *number_in_string = (int *)malloc(size * sizeof(int));
    for (i = 0; i < size; i++)
    {
        j = 0;
        number_in_string[i] = 0;
        array[i] = (int *)malloc(sizeof(int) * 1);
        // while (scanf("%d", &inputint) != 0)
        do
        {
            printf("\nArray[%d][%d]: ", i, j);
            inputint = int_input();
            number_in_string[i]++;
            array[i] = (int *)realloc(array[i], sizeof(int) * number_in_string[i]);
            array[i][j] = inputint;
            j++;
        } while (inputint != 0);
        /*number_in_string[i]++;
        array[i][j] = 0;*/
    }
    for (i = 0; i < size; i++)
    {
        printf("\nString %d :", i);
        for (j = 0; j < number_in_string[i]; j++)
        {
            printf(" %d", array[i][j]);
        }
    }
    printf("\n");

    int min, check;
    for (i = 1; i < size; i += 2)
    {
        min = array[i][0];
        check = -1;
        for (j = 0; j < number_in_string[i]; j++)
        {
            if (min > array[i][j])
            {
                min = array[i][j];
                check = j;
            }
        }
        if (check == -1)
        {
            number_in_string[i] = 0;
        }
        else
        {
            number_in_string[i]--;
            for (j = check; j < number_in_string[i]; j++)
            {
                array[i][j] = array[i][j + 1];
            }
        }
    }

    for (i = 0; i < size; i++)
    {
        printf("\nString %d :", i);
        if (number_in_string[i] == 0)
            printf(" Empty array ");
        else
        {
            for (j = 0; j < number_in_string[i]; j++)
            {
                printf(" %d", array[i][j]);
            }
        }
    }
}

void task3()
{
    int i, size;
    printf("Enter number of array elements: ");
    size = int_input();
    while (size <= 0)
    {
        printf("Incorrect input. Try again. \n");
        size = int_input();
    }
    int **array = (int **)malloc(size * sizeof(int *));

    // selection input method
    printf("Select array input method:\n1 - random input\n2 - keyboard input\n");
    int check = -1, method;
    while (check == -1)
    {
        method = int_input();
        if (method == 1)
        {
            check = 1;
            for (int i = 0; i < size; i++)
            {
                array[i] = (int *)malloc(size * sizeof(int));
                for (int j = 0; j < size; j++)
                {
                    printf("array[%d][%d] = ", i, j);
                    array[i][j] = rand() % 201 - 100;
                    printf("%d\n", array[i][j]);
                }
            }
        }
        else
        {
            if (method == 2)
            {
                check = 1;
                printf("Enter array elements \n");
                int j = 0;
                for (int i = 0; i < size; i++)
                {
                    array[i] = (int *)malloc(size * sizeof(int));
                    for (j; j < size; j++)
                    {
                        printf("array[%d][%d] = ", i, j);
                        array[i][j] = int_input();
                    }
                }
            }
            else
            {
                printf("Incorrect. Try again. \n");
                rewind(stdin);
            }
        }
    }

    for (i = 0; i < size; i++)
    {
        printf("\nString %d :", i);
        for (int j = 0; j < size; j++)
        {
            printf(" %3d", array[i][j]);
        }
    }

    int reserve;
    for (int j = 0; j < size; j++)
    {
        reserve = array[0][j];
        for (i = 0; i < size - 1; i++)
        {
            array[i][j] = array[i + 1][j];
        }
        array[size - 1][j] = reserve;
    }
    printf("\n");
    for (i = 0; i < size; i++)
    {
        printf("\nString %d :", i);
        for (int j = 0; j < size; j++)
        {
            printf(" %3d", array[i][j]);
        }
    }
    for (i = 0; i < size; i++)
    {
        free(array[i]);
    }
    free(array);
}

int main()
{
    int n;
    while (1)
    {
        printf("\nEnter task number: ");
        n = int_input();
        printf("\n");
        switch (n)
        {
        case 1:
            printf("Task 1 \n");
            printf("\n");
            task1();
            printf("\n");
            break;

        case 2:
            printf("Task 2 \n");
            printf("\n");
            task2();
            printf("\n");
            break;

        case 3:
            printf("Task 3 \n");
            printf("\n");
            task3();
            printf("\n");
            break;

        default:
            printf("Incorrect. Try again. \n");
            printf("\n");
        }
    }
}