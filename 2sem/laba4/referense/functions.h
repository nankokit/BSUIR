#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <time.h>
#pragma once
#define DEL 0.001


enum Color
{
    black = 0,
    dark_blue,
    green,
    blue,
    red,
    magenta,
    yellow,
    white,
    grey,
    light_dark_blue,
    light_green,
    light_blue,
    light_red,
    light_magenta,
    light_yellow,
};

void setConsoleColor(Color background, Color foreground);

//const float DEL = 0.001;

bool float_check(float& a);

bool int_check(float& a);

bool int_int_check(int& a);

template<typename T>
T abs_m(T a)
{
    return a > 0 ? a : -a;
}//модуль

int fact(int n);

float pow(float a, int n);

float cos(float a);

float sin(float x);

float ln(float x);

//float exp(float x);
//
//float pow_exp(int n, float x);
//
//float sqrt(float x);


int size_array();
//Вводим количество элементов массива

template<typename T>
void input_array(T* arr, int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("Enter %i elements: ", (i + 1));
        while (int_check(arr[i]))
        {
            setConsoleColor(black, red);
            printf("Error! Please enter the correct data\nEnter %i elements(number):\t", (i + 1));
            rewind(stdin);
            setConsoleColor(black, white);
        }
        rewind(stdin);
    }
}
// Вводим массив

template<typename T>
void ran_input_array(T* arr, int n)
{
    srand(static_cast<unsigned int>(time(0)));
    for (int i = 0; i < n; i++)
    {
        arr[i] = float((rand() % 200) - 100);
    }
}
// Вводим массив случайно

template<typename T>
void ran_input_float_array(T* arr, int n)
{
    srand(static_cast<unsigned int>(time(0)));
    for (int i = 0; i < n; i++)
    {
        arr[i] = ((float)((rand() % 20000) - 10000)) / 100;
    }
}
//Вводим массив float случайно

template<typename T>
void input_ran_or_user_array(T* arr, int n)
{
    printf("\nSelect the input method:\n1) automatic input\n2) user input\n");
    float in;
    while (int_check(in) || in <= 0 || in > 2)
    {
        setConsoleColor(black, red);
        printf("Error! Please enter the correct data\nSelect the input method:\n1) automatic input\n2) user input\n ");
        rewind(stdin);
        setConsoleColor(black, white);
    }
    rewind(stdin);

    switch ((int)in)
    {
    case 1:
    {
        ran_input_array(arr, n);
    }
    break;
    case 2:
    {
        input_array(arr, n);
    }
    break;
    }
}
// Выбор ввода массива

template<typename T>
void input_ran_or_user_array_float(T* arr, int n)
{
    printf("\nSelect the input method:\n1) automatic input\n2) user input\n");
    float in;
    while (int_check(in) || in <= 0 || in > 2)
    {
        setConsoleColor(black, red);
        printf("Error! Please enter the correct data\nSelect the input method:\n1) automatic input\n2) user input\n ");
        rewind(stdin);
        setConsoleColor(black, white);
    }
    rewind(stdin);

    switch ((int)in)
    {
    case 1:
    {
        ran_input_float_array(arr, n);
    }
    break;
    case 2:
    {
        input_array(arr, n);
    }
    break;
    }
}
// Выбор ввода массива float

template<typename T>
void output_array(T* arr, int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("%.0f ", arr[i]);
    }
}
// Выводим массив

template<typename T>
void output_array_float(T* arr, int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("%.2f ", arr[i]);
    }
}
// Выводим массив

template<typename T>
T max_array(T* arr, int n)
{
    int max = arr[0];
    for (int i = 0; i < n; i++)
    {
        if (arr[i] > max)
            max = arr[i];
    }
    return max;
}
// максимум в массиве

template<typename T>
T min_array(T* arr, int n)
{
    int min = arr[0];
    for (int i = 0; i < n; i++)
    {
        if (arr[i] < min)
            min = arr[i];
    }
    return min;
}
// минимум в массиве


// Ищет максимальный по модулю элемент
template<typename T>
T max_abs_array(T* arr, int n)
{
    int max = arr[0];
    bool fl = 0;

    for (int i = 0; i < n; i++)
    {
        if (arr[i] < 0)
        {
            arr[i] *= -1;
            if (max < arr[i])
            {
                max = arr[i];
                fl = 1;
            }
            arr[i] *= -1;
        }
        else
        {
            if (max < arr[i])
            {
                max = arr[i];
                fl = 0;
            }
        }
    }
    if (fl == 1)
        max *= -1;
    return max;
}

template<typename T>
T sum_bet_1pos_and_2pos(T* arr, int n)
{
    float sum = 0;
    for (int i = 0; i < n; i++)
    {
        if (arr[i] > 0)
        {
            i++;
            while (arr[i] <= 0)
            {
                sum += arr[i];
                i++;
                if (i = n-1)
                {
                    sum = 0;
                    break;
                }
            }
            break;
        }

    }
    return sum;
}
// Ищет сумму между первым и вторым положительными элементами

template<typename T>
void del_ident_elem(T* arr, int &n)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            if (arr[i] == arr[j])
            {
                for (int k = j; k < n - 1; k++)
                    arr[k] = arr[k + 1];
                n--;
                j--;
            }
        }
    }

}
// Удаляет повторяющиеся элементы массива

template<typename T>
void del_ident_elem_float(T* arr, int &n)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            if (abs(arr[i] - arr[j]) < DEL)
            {
                for (int k = j;k < n - 1;k++)
                    arr[k] = arr[k + 1];
                n--;
                j--;
            }
        }
    }
    //return T(n);
}
// Удаляет повторяющиеся элементы массива (float)







int number_line();

int number_columns();

int size_sq_matrix();

template<typename T>
void input_matrix(T mat[100][100], int m, int n)
{ 
    printf("\n");
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            printf("Enter %i %i elements: ", (i + 1), (j + 1));
            while (int_check(mat[i][j]))
            {
                setConsoleColor(black, red);
                printf("Error! Please enter the correct data\nEnter %i %i elements: ", (i + 1), (j + 1));
                rewind(stdin);
                setConsoleColor(black, white);
            }
            rewind(stdin);
        }
    }
}
// Вводим матрицу


// Случайно вводим матрицу
template<typename T>
void ran_input_matrix(T mat[100][100], int m, int n)
{
    srand(static_cast<unsigned int>(time(0)));
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            mat[i][j] = float((rand() % 198) - 99);
        }
    }
}

// Выводим матрицу
template<typename T>
void output_matrix(T mat[100][100], int m, int n)
{
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
            printf("%4.0f",mat[i][j]);
        printf("\n");
    }
}


// выбор ввода матрицы
template<typename T>
void input_ran_or_user_matrix(T mat[100][100], int m, int n)
{
    printf("\nSelect the input method:\n1) automatic input\n2) user input\n");
    float in;
    while (int_check(in) || in <= 0 || in > 2)
    {
        setConsoleColor(black, red);
        printf("Error! Please enter the correct data\nSelect the input method:\n1) automatic input\n2) user input\n ");
        rewind(stdin);
        setConsoleColor(black, white);
    }
    rewind(stdin);

    switch ((int)in)
    {
    case 1:
    {
        ran_input_matrix(mat, m, n);
    }
    break;
    case 2:
    {
        input_matrix(mat, m, n);
    }
    break;
    }
}


// сортировка строк по характеристике от min до max
// хариктеристика-- сумма положительных четных элементов
template<typename T>
void sotr_char(T mat[100][100], int m, int n)
{
    int cha[100] = {};
    int inter, inter_m;
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (mat[i][j] > 0 && (int)mat[i][j] % 2 == 0)
                cha[i] += (int)mat[i][j];
        }
        if (i > 0)
        {
            inter = cha[i];
            int k = i - 1;
            while (k >= 0 && inter < cha[k])
            {
                cha[k + 1] = cha[k];
                cha[k] = inter;
                for (int t = 0; t < n; t++)
                {
                    inter_m = mat[k][t];
                    mat[k][t] = mat[k + 1][t];
                    mat[k + 1][t] = inter_m;
                }
                k--;
            }
        }
    }
}


// проверяет есть ли в матрице хотябы одна строка выдае её номер
// и делит столбец с этим номером на 2
template<typename T>
void check_line_neg_elem_and_div_col_on_2(T mat[100][100], int m, int n)
{
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (mat[i][j] < 0)
            {
                printf("The number of the line with a negative element:\t%i\n", (i + 1));
                for (int k = 0; k < n; k++)
                {
                    mat[k][i] /= 2;
                }
                break;
            }
        }
    }
}

// минимальный элемент в левом треугольнике
template<typename T>
float min_left_triangle(T mat[100][100], int m, int n)
{
    float min = mat[n - 1][n - 1];
    for (int i = n / 2; i < n; i++)
    {
        for (int j = n - 1; j >= i; j--)
        {
            if (mat[i][j] < min)
                min = mat[i][j];
            //printf("%i %i");
        }
    }
    // идем вниз
    for (int i = (n / 2) - 1; i >= 0; i--)
    {
        for (int j = n - 1; j >= n - i - 1; j--)
        {
            if (mat[i][j] < min)
                min = mat[i][j];
        }
    }
    // идем вверх

    return min;
}



//  вставляет квадрат после каждого элемента
template<typename T>
void insert_squar(T** arr, int &n)
{
    for (int i = 0; i < n; i += 2)
    {
        //printf("i: %i\tn bef rea: %i\t", i, n);
        //T* temp = realloc(arr, ++n * sizeof(T));
        //if (temp == NULL) {
        //    free(arr);
        //    //assert("memory fatal error");
        //}
        //else {
        //    arr = temp;
        //}
        *arr = (T*)realloc(*arr, ++n * sizeof(T));
        //printf("n aft rea: %i\n", n);

        for (int j = n - 1; j >= i + 2; j--)
        {
            (* arr)[j] = (* arr)[j - 1];
        }
        (* arr)[i + 1] =
            ( * arr)[i] * (* arr)[i];
        //output_array(*arr, n);
        //printf("\n\n");
    }
}

// Вводи матрицу(динамика)
template<typename T>
void dyn_input_matrix(T** mat, int n, int m)
{
    printf("\n");
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            printf("Enter %i %i elements: ", (i + 1), (j + 1));
            while (int_check(mat[i][j]))
            {
                setConsoleColor(black, red);
                printf("Error! Please enter the correct data\nEnter %i %i elements: ", (i + 1), (j + 1));
                rewind(stdin);
                setConsoleColor(black, white);
            }
            rewind(stdin);
        }
    }
}

// Случайно вводим матрицу(динамика)
template<typename T>
void dyn_ran_input_matrix(T** mat, int n, int m)
{
    srand(static_cast<unsigned int>(time(0)));
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            mat[i][j] = float((rand() % 198) - 99);
        }
    }
}

// выбор ввода матрицы
template<typename T>
void dyn_input_ran_or_user_matrix(T** mat, int n, int m)
{
    printf("\nSelect the input method:\n1) automatic input\n2) user input\n");
    float in;
    while (int_check(in) || in <= 0 || in > 2)
    {
        setConsoleColor(black, red);
        printf("Error! Please enter the correct data\nSelect the input method:\n1) automatic input\n2) user input\n ");
        rewind(stdin);
        setConsoleColor(black, white);
    }
    rewind(stdin);

    switch ((int)in)
    {
    case 1:
    {
        dyn_ran_input_matrix(mat, n, m);
    }
    break;
    case 2:
    {
        dyn_input_matrix(mat, n, m);
    }
    break;
    }
}

// Выводим матрицу(диамика)
template<typename T>
void dyn_output_matrix(T** mat, int n, int m)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
            printf("%4.0f", mat[i][j]);
        printf("\n");
    }
}

template<typename T>
void dyn_del_onle_neg_col(T*** mat, int &n, int &m)
{
    //printf("ABOBA");
    for (int i = 0; i < m; i++)
    {
        int fl = 0;
        for (int j = 0; j < n; j++)
        {
            if ((*mat)[j][i] >= 0)
            {
                fl = 1;
                break;
            }
        }
        if (fl == 0)
        {
            for (int j = 0; j < n; j++)
                for (int k = i; k < m; k++)
                    (*mat)[j][k] = (*mat)[j][k + 1];
            m--;
            for (int j = 0; j < 0; j++)
               (* mat)[j] = (T*)realloc((*mat)[j], m * sizeof(T));
            i--;
        }
    }
}

// Сортировка пузырьком
template<typename T>
void bubble_sort(T* arr, int n)
{
    T inter = 0;
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - 1 - i; j++)
        {
            if (arr[j] > arr[j + 1])
            {
                inter = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = inter;
            }
        }
    }
}


// Специальная сортировка пузырьком
// после максималного кратных К
template<typename T>
void special_bubble_sort(T** arr, int n, T k, int i_max) 
{
    //printf("ABOBA\n");
    int ind_1;
    for (int i = i_max + 1; i < n; i++)
        if ((int)(*arr)[i] % (int)k == 0)
        {
            ind_1 = i;
            break;
        }
    //printf("ind_1 : %i\n", ind_1);

    T inter = 0;
    for (int i = ind_1 + 1; i < n; i++)
    {
        //printf("i : %i\n", i);
        int ind = ind_1;
        for (int j = ind_1; j < n; j++)
        {
            //printf("j : %i\tind : %i\n", j, ind);
            if ((int)(*arr)[j] % (int)k == 0)
            {
                if ((*arr)[ind] > (*arr)[j])
                {
                    inter = (*arr)[ind];
                    (*arr)[ind] = (*arr)[j];
                    (*arr)[j] = inter;
                    //printf("I in IF\nj : %i\tind : %i\nChanging :", j, ind);
                    //for (int p = 0; p < n; p++)
                    //{
                    //    if (p == i_max)
                    //    {
                    //        setConsoleColor(black, green);
                    //        printf("%4.0f ", arr[p]);
                    //        setConsoleColor(black, white);
                    //    }
                    //    else
                    //    {
                    //        if (p > i_max && (int)arr[p] % (int)k == 0)
                    //        {
                    //            setConsoleColor(black, yellow);
                    //            printf("%4.0f ", arr[p]);
                    //            setConsoleColor(black, white);
                    //        }
                    //        else
                    //            printf("%4.0f ", arr[p]);
                    //    }
                    //}
                    //printf("\n");
                }
                ind = j;
            }
        }
        //for (int p = 0; p < n; p++)
        //{
        //    if (p == i_max)
        //    {
        //        setConsoleColor(black, green);
        //        printf("%4.0f ", arr[p]);
        //        setConsoleColor(black, white);
        //    }
        //    else
        //    {
        //        if (p > i_max && (int)arr[p] % (int)k == 0)
        //        {
        //            setConsoleColor(black, yellow);
        //            printf("%4.0f ", arr[p]);
        //            setConsoleColor(black, white);
        //        }
        //        else
        //            printf("%4.0f ", arr[p]);
        //    }
        //}
        //printf("\n");
    }
}

// Сортировка Шелла
template<typename T>
void shell_sort(T* arr, int n)
{
    for (int gap = n / 2; gap > 0; gap /= 2)
    {
        int fl = 0;
        do
        {
            fl = 0;
            for (int i = 0, j = gap; j < n; i++, j++)
            {

                if (arr[i] > arr[j])
                {
                    int inter = arr[i];
                    arr[i] = arr[j];
                    arr[j] = inter;
                    fl = 1;
                }
            }
        } while (fl == 1);
    }
}

// Специальная сортировка Шелла
// сортирует строки матрицы по сумме положительных элементов
template<typename T>
void special_shell_sort(T *** mat, int n, int m) 
{
    printf("ABOBA\n");
    for (int gap = n / 2 ; gap > 0; gap /= 2)
    {
        //printf("gap : %i\n", gap);
        int fl = 0;
        do
        {

                fl = 0;
                for (int i = 0, j = gap; j < n; i++, j++)
                {
                        float sum_i = 0, sum_j = 0;
                        for (int k = 0; k < m; k++)
                        {
                            if ((*mat)[i][k] > 0)
                            sum_i += (*mat)[i][k];
                            if ((*mat)[j][k] > 0)
                            sum_j += (*mat)[j][k];
                        }
                        //printf("i : %i\tj : %i\tsum_i : %.0f\tsum_j : %.0f\n", i, j, sum_i, sum_j);
                        if (sum_i > sum_j)
                        {
                            //printf("I in if\n");
                            for (int k = 0; k < m; k++)
                            {
                                T temp = (*mat)[i][k];
                                (*mat)[i][k] = (*mat)[j][k];
                                (*mat)[j][k] = temp;
                            }
                            fl = 1;
                        }
                   
                }
            
        } while (fl);
    }
}


// Слияние
// (Массив в каторый сливаем, его размер, 1 массив, его размер, 2 массиав, его размер)
template<typename T>
void merge(T** arr, int n, T** lef, int j, T** rig, int k)
{
    int cn = 0, cj = 0, ck = 0;
    while (cj < j && ck < k)
    {
        if ((*lef)[cj] <= (*rig)[ck])
            (*arr)[cn++] = (*lef)[cj++];
        else
            (*arr)[cn++] = (*rig)[ck++];
    }
    while (cj < j)
        (*arr)[cn++] = (*lef)[cj++];
    while (ck < k)
        (*arr)[cn++] = (*rig)[ck++];
    //printf("Merge : ");
    //output_array((*arr), n);
    //printf("\n");
}

// Сортировка слиянием
template<typename T>
void merge_sort(T** arr, int n)
{
    if (n < 2)
        return;
    T* lef = (T*)malloc(n / 2 * sizeof(T));
    T* rig = (T*)malloc((n / 2  + n % 2 ) * sizeof(T));

    for (int i = 0; i < n / 2; i++)
        lef[i] = (*arr)[i];
    for (int i = 0, j = n / 2 /*+ n % 2*/; j < n;i++, j++)
        rig[i] = (*arr)[j];

    //printf("lef : ");
    //output_array(lef, n / 2);
    //printf("\n");

    //printf("rig : ");
    //output_array(rig, n / 2 + n % 2);
    //printf("\n");

    merge_sort(&lef, n / 2);
    merge_sort(&rig, n / 2 + n % 2);
    merge(arr, n, &lef, n / 2, &rig, n / 2 + n % 2);
}


// ввод строки 
void get_str(char** str);

//длинна строки
int str_len(char* str);

// проверяет буква ли
int letter(char s);

int number(char s);

// находит К слово
int find_k_word(char* str, int k);

// удаляет слово
void del_word(char** str,int &dl, int start);

// выводит строку
void output_str(char* str);

// вставляет строку 'str0' в строку 'str' после символа 'c'  
//(стока str, длинна str, сторка str0, символ c)
void inst_str(char** str, int& dl, char* str0, char c);

// копирует сроку
void str_copy(char* str1, char** str2);