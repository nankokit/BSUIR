
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int check_int_input();
void int_input(int *n);
void array_input_random(int *array, int size);
void array_input(int *array, int size);
void print_array(int *array, int size);
void bivariate_array_input_random(int **array, int n, int m);
void bivariate_array_input(int **array, int n, int m);
void swap_colums(int **array, int n, int f, int l);
void print_bivariate_array(int **array, int n, int m);
int sum_column_elements(int **array, int n, int num_column);
void mergeSorting(int *array, int left, int mid, int right);
void mergeSortingRecursive(int *array, int left, int right);
void quicksorting(int **array, int first, int last, int n, int m);
int partitions(int **array, int first, int last, int n, int m);
void quicksort(int **a, int start, int end, int n, int m);

int check_int_input()
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

void int_input(int *n)
{
    *n = check_int_input();
    while (*n <= 0 || *n >= 2147483647)
    {
        printf("Incorrect input. Try again. \nEnter number of array elements:");
        *n = check_int_input();
    }
}

void array_input_random(int *array, int size)
{
    for (int i = 0; i < size; i++)
        array[i] = rand() % 200 - 100;
}

void array_input(int *array, int size)
{
    // selection input method
    printf("Select array input method:\n1 - random input\n2 - keyboard input\n");
    int check = -1, method;
    int_input(&method);
    if (method == 1)
        array_input_random(array, size);
    else
    {
        if (method == 2)
        {
            printf("Enter array elements \n");
            for (int i = 0; i < size; i++)
            {
                printf("array[%d] = ", i);
                array[i] = check_int_input();
            }
        }
        else
        {
            printf("Incorrect. Try again. \n");
            rewind(stdin);
        }
    }
}

void print_array(int *array, int size)
{
    for (int i = 0; i < size; i++)
        printf("%3d ", array[i]);
    printf("\n");
}

void bivariate_array_input_random(int **array, int n, int m)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            array[i][j] = rand() % 100 - 50;
        }
    }
}

void bivariate_array_input(int **array, int n, int m)
{
    // selection input method
    printf("Select array input method:\n1 - random input\n2 - keyboard input\n");
    int check = -1, method;
    int_input(&method);
    if (method == 1)
        bivariate_array_input_random(array, n, m);
    else
    {
        if (method == 2)
        {
            printf("Enter array elements \n");
            for (int i = 0; i < n; i++)
            {
                for (int j = 0; j < m; j++)
                {
                    printf("array[%d][%d] = ", i, j);
                    array[i][j] = check_int_input();
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

void swap_colums(int **array, int n, int f, int l)
{
    // f is the smallest
    int count;
    for (int i = 0; i < n; i++)
    {
        count = array[i][l];
        array[i][l] = array[i][f];
        array[i][f] = count;
    }
}

void print_bivariate_array(int **array, int n, int m)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            printf("%3d ", array[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int sum_column_elements(int **array, int n, int num_column)
{
    int summa = 0;
    for (int i = 0; i < n; i++)
    {
        summa += array[i][num_column];
    }
    return summa;
}

void mergeSorting(int *array, int left, int mid, int right)
{
    int step_left = 0, step_right = 0;
    int *result = (int *)malloc((right - left + 1) * sizeof(int));
    while (left + step_left < mid && mid + step_right < right)
    {
        if (array[left + step_left] < array[mid + step_right])
        {
            result[step_left + step_right] = array[left + step_left];
            step_left++;
        }
        else
        {
            result[step_left + step_right] = array[mid + step_right];
            step_right++;
        }
    }
    while (left + step_left < mid)
    {
        result[step_left + step_right] = array[left + step_left];
        step_left++;
    }
    while (mid + step_right < right)
    {
        result[step_left + step_right] = array[mid + step_right];
        step_right++;
    }
    for (int i = 0; i < step_left + step_right; i++)
        array[left + i] = result[i];
    free(result);
}

void mergeSortingRecursive(int *array, int left, int right)
{
    if (left + 1 >= right)
        return;
    int mid = (left + right) / 2;
    mergeSortingRecursive(array, left, mid);
    mergeSortingRecursive(array, mid, right);
    mergeSorting(array, left, mid, right);
}

int partitions(int **array, int first, int last, int n, int m)
{
    if ((first + 1) % 2 == 1 && (first + 1) <= last)
        first++;
    if ((last + 1) % 2 == 1 && (last - 1) >= first)
        last--;
    int pivot_num = last;
    int pivot = sum_column_elements(array, n, pivot_num);
    int index = first;
    int i = first;
    for (i; i < last; i += 2)
    {
        if (sum_column_elements(array, n, i) >= pivot)
        {
            swap_colums(array, n, i, index);
            index += 2;
        }
    }
    swap_colums(array, n, index, last);
    return index;
}

void quicksort(int **a, int start, int end, int n, int m)
{
    if (start >= end)
        return;
    int pivot = partitions(a, start, end, n, m);
    quicksort(a, start, pivot - 2, n, m);
    quicksort(a, pivot + 2, end, n, m);
}
