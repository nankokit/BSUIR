#include "functionss.h"

void task1()
{

    printf("Enter number of array elements: ");
    int size;
    int_input(&size);
    int *array = (int *)malloc(size * sizeof(int));
    array_input(array, size);
    print_array(array, size);

    int n = 1, temp_size = 0;
    int *temp_arr = (int *)malloc(n * sizeof(int));
    int *arr_index = (int *)malloc(n * sizeof(int));
    if (size > 1)
    {
        for (int i = 0; i < size; i++)
        {
            if (array[i] % 2 == 1)
            {
                n++;
                temp_arr = (int *)realloc(temp_arr, n * sizeof(int));
                arr_index = (int *)realloc(arr_index, n * sizeof(int));
                temp_arr[temp_size] = array[i];
                arr_index[temp_size] = i;
                temp_size++;
            }
        }
    }
    printf("Sorting these elements\n");
    print_array(temp_arr, temp_size);
    mergeSortingRecursive(temp_arr, 0, temp_size);
    printf("Sorting result\n");
    print_array(temp_arr, temp_size);
    for (int i = 0; i < temp_size; i++)
    {
        array[arr_index[i]] = temp_arr[i];
    }
    printf("Result\n");
    print_array(array, size);
    printf("\n");
}

void task2()
{
    printf("Entering array. Enter n: ");
    int n, m;
    int_input(&n);
    printf("\nEnter m: ");
    int_input(&m);
    printf("\n");

    int **array = (int **)malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++)
        array[i] = (int *)malloc(m * sizeof(int));
    bivariate_array_input(array, n, m);
    print_bivariate_array(array, n, m);
    int *summ_columns = (int *)malloc(m * sizeof(int));
    for (int i = 0; i < m; i++)
        summ_columns[i] = sum_column_elements(array, n, i);
    print_array(summ_columns, m);
    printf("\n");
    // int number = m /2;
    int first = 0, last = m;
    // if (last % 2 == 1)
    //     last--;
    // last--;

    quicksort(array, first, m - 1, n, m);
    printf("Sorting\n");
    print_bivariate_array(array, n, m);
    for (int i = 0; i < m; i++)
        summ_columns[i] = sum_column_elements(array, n, i);
    print_array(summ_columns, m);
}

int main()
{
    srand((int)time(NULL));
    int n;
    while (1)
    {
        printf("Enter task number: \n");
        n = check_int_input();
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

        default:
            printf("Incorrect. Try again. \n");
            printf("\n");
        }
    }
    return 0;
}