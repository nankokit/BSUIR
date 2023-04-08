#define _CRT_SECURE_NO_WARNINGS
//#include "functions.h"
#include <stdio.h>
#include <Windows.h>
#include "D:/program/my_functions/functions.h"

void setConsoleColor(Color background = black, Color foreground = white)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (((background << 4) | foreground)));
}


bool float_check(float& a)
{
    return (scanf("%f", &a) != 1 || getchar() != '\n');
}
// порверка на float

bool int_check(float& a)
{
    return (float_check(a) || a != (int)a);
}
// проверка на int

bool int_int_check(int& a) {
    return (scanf("%i", &a) != 1 || getchar() != '\n');
}

int size_array()
{
    float n;
    printf("Enter the number of elements: ");
    while (int_check(n) || n <= 0)
    {
        printf("Enter the number of elements(positiv number): ");
        rewind(stdin);
    }
    rewind(stdin);

    return (int)n;
}
//Вводим количество элементов массива

int number_line()
{
    float m;
    printf("Enter the number of line: ");
    while (int_check(m) || m < 0)
    {
        printf("Error! Please enter the correct data\nEnter the number of line(positiv number): ");
        rewind(stdin);
    }
    return (int)m;
}

int number_columns()
{
    float n;
    printf("Enter the number of columns: ");
    while (int_check(n) || n < 0)
    {
        printf("Error! Please enter the correct data\nEnter the number of columns(positiv number): ");
        rewind(stdin);
    }
    return (int)n;
}

int size_sq_matrix()
{
    float n;
    printf("Enter the number of the matrix: ");
    while (int_check(n) || n < 0)
    {
        printf("Error! Please enter the correct data\nEnter the order of the matrix(positiv number): ");
        rewind(stdin);
    }
    return (int)n;

}

// факториал
int fact(long int n)
{
    if (n == 0 || n == 1) 
        return 1;
    return n * fact(n - 1);
}

// степень
float pow(float a, int n)
{
    float p = 1.0;

    if(n >= 0)
    {
        for (int i = 1; i <= n; i++)
            p *= a;
        return p;
    }
    else
    {
        n *= -1;
        for (int i = 1; i <= n; i++)
            p *= a;
        return 1/p;
    }
}

// косинус
float cos(float x)
{
    static const float pi = 3.1415926535;
    if (x >= 0)
    {
        while (x > 2 * pi)
        {
            x -= 2 * pi;
        }
    }
    else
    {
        while (x < 2 * pi)
        {
            x += 2 * pi;
        }
    }
    //printf("counting cos\n");
    static const float eps = 0.000001;
    float current = 1.0;
    float ans = 0;
    float next = current;
    int i = 0;
    int sign = 1;

   // printf("x is %f\n", x);
   do {
        ans += sign * next;
        current = next;
        next = current * x * x / (i + 1) / (i + 2);
        i += 2;
        sign *= -1;

       // printf("temp ans is %f\t next %f\n", ans, next);
   } while (abs_m(abs_m(current) - abs_m(next)) >= eps);

    return ans;
}

// синус 
float sin(float x)
{
    static const float pi = 3.1415926535;
    if (x >= 0)
    {
        while (x > 2 * pi)
        {
            x -= 2 * pi;
        }
    }
    else
    {
        while (x < 2 * pi)
        {
            x += 2 * pi;
        }
    }
    //printf("counting sin\n");
    static const float eps = 0.000001;
    float current = x;
    float ans = 0;
    float next = current;
    int i = 0;
    int sign = 1;

    // printf("x is %f\n", x);
    do {
        ans += sign * next;
        current = next;
        next = current * x * x / (i + 2) / (i + 3);
        i += 2;
        sign *= -1;

        // printf("temp ans is %f\t next %f\n", ans, next);
    } while (abs_m(abs_m(current) - abs_m(next)) >= eps);

    return ans;
}

// натуральный логорифм
float ln(float x)
{
    static const float eps = 0.000000001;
    //static const float exp = 2.7182818284590;
    float ans = 0;
    
    while (x / 2 > 1) {
        x /= 2;
        ans += ln(2);
        //printf("x is %f ans is %f\n", x, ans);
    }


    float current = x - 1;
    float next = current;
    int i = 1;
    int sign = 1;
    float mul = x - 1;

    //printf("x is %f\n", x);
    do {
        ans += sign * next;
        current = next;
        mul *= (x - 1);
        i++;
        next = mul / i;
        sign *= -1;

        //printf("sign %d i %d\n", sign, i);

        //printf("temp ans is %f\t next %f\n", ans, next);
    } while (//i < 100
        abs_m(abs_m(current) - abs_m(next)) >= eps
        );

    return ans;
}


//float exp(float x) {
//    static const float eps = 0.000000001;
//    //static const float exp = 2.7182818284590;
//    float ans = 1;
//    float current = x;
//    float next = current;
//    int i = 1;
//    long long fact = 1;
//    float mul = x;
//
//    //printf("x is %f\n", x);
//    do {
//        ans += next;
//        current = next;
//        i++;
//        fact *= i;
//        mul *= x;
//        next = mul / fact;
//        //printf("fact %i next %f\n", fact, next);
//        //printf("i : %d mul : %f fact : %i current %f next %f\n", i, mul, fact, current, next);
//        //printf("%f\n", abs_m(abs_m(current) - abs_m(next)));
//    } while (i < 10
//        //abs_m(abs_m(current) - abs_m(next)) >= eps
//        );
//
//    return ans;
//}
//
//// степень через Eхр (степень, число)
//float pow_exp(float n, float x)
//{
//    return exp(n * ln(x));
//}
//
////корень
//float sqrt(float x)
//{
//    return pow_exp(0.5, x);
//}

void get_str(char** str) {
    int c, i = 0;
    while ((c = getchar()) != EOF && c != '\n') {
        (*str)[i++] = c;   //Enter or Ctrl + Z
        (*str) = (char*)realloc((*str), i + 1);
    }
    (*str)[i] = '\0';

}

int str_len(char* str)
{
    int i = 0;
    if (str == NULL)
        printf("aboba\n");
    else {
        //int i = 0;
        while (str[i] != '\0')
            i++;
    }
    return i;
}

// проверяет буква ли
int letter(char s)
{
    if ((s >= 'a' && s <= 'z') || (s >= 'A' && s <= 'Z'))
        return 1;
    return 0;
}

int number(char s) {
    if ((s >= '0' && s <= '9'))
        return 1;
    return 0;
}

// находить К слово
int find_k_word(char* str, int k) {
    int count = 0;

    for (int i = 0; str[i] != '\0'; i++) {
        if ((i == 0 && letter(str[i]) == 1) || (letter(str[i-1]) == 0 && letter(str[i]) == 1))
            count++;
        if (count == k)
            return i;
    }
    return -1;
}

void del_word(char** str,int &dl, int start) {
     for (int i = start; letter((*str)[i]) == 1; i++) {
        for (int j = start; j < dl; j++)
            (*str)[j] = (*str)[j + 1];
        i--;
        dl--;
     }
     (*str) = (char*)realloc((*str), dl + 1);
}

// выводит строку
void output_str(char* str) {
    for (int i = 0;str[i] != '\0'; i++) 
        printf("%c", str[i]);
}

// вставляет строку 'str0' в строку 'str' после символа 'c'  
//(стока str, длинна str, сторка str0, символ c)
void inst_str(char** str, int& dl, char* str0, char c) {
    int dl0 = str_len(str0);

    for (int i = dl - 1; i >= 0; i--) {
        if ((*str)[i] == c) {
            dl += dl0;
            (*str) = (char*)realloc((*str), dl + 1);

            for (int j = dl; j > i; j--) 
                (*str)[j] = (*str)[j - dl0];
            for (int j = i + 1, k = 0; k < dl0; j++, k++) 
                (*str)[j] = str0[k];
        }
    }
}

// копирует сроку
void str_copy(char* str1, char** str2){

    //int n = str_len(str1);
    //free(*str2);
    //(*str2) = (char *)malloc(n);
    //for (int i = 0; i < n; i++)     
    //    (*str2)[i] = str1[i];

    int i = 0;
    int n = str_len(str1);
    *str2 = (char*)calloc(n, sizeof(char));
        while (str1[i] != '\0')
        {
            (* str2)[i] = str1[i];
            i++;
        }
    (* str2)[i] = '\0';
}

