#include "stdio.h"
#include "malloc.h"

int** alloc(int x, int y);
int** alloc2(int n);
int getint(int min, int max);
int checkinput(int a);
int checkinputk(int k);
int* writenumber(int number, int* size);
int* elements(int* a, int pos, int size);
void delete(int a, int* sizes_arr, int n, int k);
void summa(int arr, int y, int x);
int** alloc2(int n);
int checkinputx(int x);
int checkinputy(int y);
int getintt(int min);
void randarr(int** arr, int x, int y, int min, int max);
void userarr(int** arr, int x, int y, int min);
int symmetr(int** a, int n);
void newarr(int** a, int n, bool sim);
void print_array_sec(int** arr, int x, int y);

void task1() 
{
    printf("Write width: ");
    int x = 0, y = 0;
    x = checkinputx(x);
    printf("Write height: ");
    y = checkinputy(y);
    int** arr = alloc(x, y);
    printf("0 for keyboard input and 1 for random input\n ");
    int rand = getint(0, 1);
    if (rand == 1)
        randarr(arr, x, y, 1, 100);
    else
        userarr(arr, x, y, 1);
    summa(arr, y, x);
    print_array_sec(arr, x, y);
    free(arr);
}

void task2()
{
    int n = 0;
    printf("enter row and column: ");
    n = checkinput(n);
    int** a = alloc2(n);
    bool k = 0;
    k = symmetr(a, n);
    newarr(a, n, k);
    for (int i = 0; i < n; i++) {
        free(a[i]);
    }
    free(a);
}

void task3() 
{
    int n = 0, k = 0;
    printf("enter rows: ");
    n = checkinput(n);
    printf("enter number k: ");
    k = checkinput(k);
    int* sizes_arr = (int*)malloc(n * sizeof(int));
    int** a = (int**)malloc(n * sizeof(int*));
    delete(a, sizes_arr, n, k);
    free(a);
    free(sizes_arr);
}

int get_variant(int count) 
{
    int variant;
    char s[100];
    scanf("%s", s);
    while (sscanf(s, "%d", &variant) != 1  variant < 1  variant > count || getchar() != '\n') 
    {
        printf("Incorrect input. Try again: \n");
        scanf("%s", s);
    }
    return variant;
}

int main()
{
    int variant;
    do {
        printf("Enter number of task (1-3)\n");
        printf("Enter 4 for exit\n");
        variant = get_variant(4);

        switch (variant)
        {
        case 1:
            task1();
            break;

        case 2:
            task2();
            break;

        case 3:
            task3();
            break;
        }
        if (variant != 4)
            system("pause");
    } while (variant != 4);

    return 0;
}

int checkinput(int a)
{
    while (scanf("%d", &a) != 1  getchar() != '\n') 
    {
        printf("Pls enter a number!\n");
        rewind(stdin);
    }
    return a;
}

int checkinputk(int k)
{
    while (scanf("%d", &k) != 1  getchar() != '\n'  k < 1) 
    {
        printf("Pls enter a number!\n");
        rewind(stdin);
    }
    return k;
}

int checkinputx(int x) 
{
    while (scanf("%d", &x) != 1  getchar() != '\n'  x < 1) 
    {
        printf("Pls enter a number!\n");
        rewind(stdin);
    }
    return x;
}

void summa(int** arr, int y, int x)
{
    for (int i = 0; i < x; i++)
    {
        for (int j = 0; j < y; j++)
        {
            int get = arr[i][j];
            int sum = 0;

            for (int j1 = 0; j1 < y; j1++)
            {
                if (j == j1) continue;
                sum += arr[i][j1];
            }
            if (sum < get)
            {
                printf("number %d at row %d , sum = %d\n", get, i, sum);
            }
        }
        printf("\n");
    }
}

int checkinputy(int y)
{
    while (scanf("%d", &y) != 1  getchar() != '\n'  y < 1) {
        printf("Pls enter a number!\n");
        rewind(stdin);
    }
    return y;
}

int** alloc(int x, int y)
{
    int** arr = (int**)(malloc(x * sizeof(int*)));
    for (int i = 0; i < x; i++) 
    {
        arr[i] = (int*)(malloc(y * sizeof(int)));
    }
    return arr;
}

int** alloc2(int n)
{
    int** a = (int**)malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++)
    {
        a[i] = (int*)malloc(n * sizeof(int));
        for (int j = 0; j < n; j++)
        {
            printf("a[%d][%d] = ", i, j);
            a[i][j] = checkinput(a[i][j]);
        }
    }
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            printf("%5d ", a[i][j]);
        }
        printf("\n");
    }
    return a;
}

    int getint(int min, int max) {
        int number;
        while (!scanf_s("%d", &number)  number > max  number < min  getchar() != '\n') {
            printf_s("Invalid number!\n");
            rewind(stdin);
        }
        return number;
    }

    int getintt(int min) {
        int number;
        while (!scanf_s("%d", &number)  number < min  getchar() != '\n') {
            printf_s("Invalid number!\n");
            rewind(stdin);
        }
        return number;
    }

    void randarr(int** arr, int x, int y, int min, int max) {
        for (int i = 0; i < x; i++) {
            for (int j = 0; j < y; j++) {
                arr[i][j] = -20 + rand() % 80;
            }
        }
    }

    void userarr(int** arr, int x, int y, int min) {
        for (int i = 0; i < x; i++) {
            for (int j = 0; j < y; j++) {
                printf("\na[%d][%d] = ", i, j);
                arr[i][j] = getintt(min);
            }
        }
    }


    void print_array_sec(int** arr, int x, int y) {
        for (int i = 0; i < x; i++) {
            for (int j = 0; j < y; j++) {
                printf_s("\na[%d][%d] = %d", i, j, arr[i][j]);
            }
        }
        printf_s("\n");
        for (int i = 0; i < x; i++) {
            for (int j = 0; j < y; j++) {
                printf_s("%d\t", arr[i][j]);
            }
            printf_s("\n");
        }
    }


        int symmetr(int** a, int n) {
        bool sim = true;
        for (int j = 0; j < n; j++)
        {
            for (int k = 0; k < n; k++)
            {
                if (a[j][k] != a[n - 1 - k][n - 1 - j])
                {
                    sim = false;
                    break;
                }
            }
            if (!sim)
            {
                break;
            }
        }
        return sim;
    }
    void newarr(int** a, int n, bool sim) {
        int** b = (int**)malloc(n * sizeof(int*));
        for (int i = 0; i < n; i++)
        {
            b[i] = (int*)malloc((n - 1) * sizeof(int));
        }
        if (sim == true) {
            printf("arrays sim\n");
            for (int i = 0; i < n; i++)
            {
                int counter = 0;
                for (int j = 0; j < n; j++)
                {
                    if (i == n - j - 1) {
                        counter++;
                        continue;
                    }
                    b[i][j - counter] = a[i][j];
                }
            }
        }
        else {
            printf("not sim\n");
            for (int i = 0; i < n; i++) {
                int counter = 0;
                for (int j = 0; j < n; j++) {
                    if (i == j) {
                        counter++;
                        continue;
                    }
                    b[i][j - counter] = a[i][j];
                }
            }
        }
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n - 1; j++)
            {
                printf("%5d ", b[i][j]);
            }
            printf("\n");
        }
        for (int i = 0; i < n; i++) {
            free(b[i]);
        }
        free(b);
    }

    int* elements(int* arr, int pos, int size) {
        int* temp = (int*)malloc((size - 1) * sizeof(int));
        memmove(temp, arr, (pos) * sizeof(int));
        memmove(temp + pos, (arr)+(pos + 1), (size - pos) * sizeof(int));
        return temp;
    }

    void delete(int** a, int* sizes_arr, int n, int k) {
        for (int i = 0; i < n; i++) {
            printf_s("row: %d\n", i);
            int size;
            a[i] = writenumber(0, &size);
            sizes_arr[i] = size;
        }
        for (int i = 0; i < n; i++) {
            int size = sizes_arr[i];
            int n = 0;
            int rem = 0;
            int* to_rem = (int*)malloc(size * sizeof(int));
            while (n < size) {
                if (a[i][n] < k && n % 2 == 0) {
                    to_rem[rem] = n;
                    rem++;
                }
                n++;
            }
            for (int j = 0; j < rem; j++) {
                a[i] = elements(a[i], to_rem[j] - j, size);
                size--;
            }
            sizes_arr[i] = size;
        }
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < sizes_arr[i]; j++) {
                printf("%d ", a[i][j]);
            }
            printf("\n");
        }
    }

    int* writenumber(int number, int* size) {
        *size = 0;
        int* temp = (int*)malloc(sizeof(int));
        int i = 0;
        while (true) {
            int entered;
            printf_s("element: ");
            while (!scanf_s("%d", &entered)  (entered < 1 && entered != number)  getchar() != '\n') {
                printf_s("\ntry again\n");
                rewind(stdin);
            }
            if (entered == number) break;
            temp[i] = entered;
            (*size)++;
            i++;
            temp = (int*)realloc(temp, (i + 1) * sizeof(int));
        }
        return temp;
    }

  
    