#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "string.h"

int check_int_input()
{
    int readinput;
    int input;
    char inputchar;

    while (1)
    {
        readinput = scanf_s("%d", &input);
        scanf_s("%c", &inputchar);
        if (readinput != 0 && (inputchar == '\n' || inputchar == '\0') && (int)input == input)
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

void input_file(FILE *fp)
{
    int n = 0, count = 0;
    do
    {
        printf("Enter the count of numbers: ");
        count = check_int_input();
    } while (count <= 0);

    printf("Enter the input way (1 - keyboard, 2 - random): ");
    n = check_int_input();
    while ((n != 1) && (n != 2))
    {
        printf("Try again... \n");
        n = check_int_input();
    }

    char temp;
    switch (n)
    {
    case 1:
        for (int i = 0; i < count; i++)
        {
            printf("Enter %d number: ", i + 1);
            n = check_int_input();
            // temp = n + '0';
            // fprintf(fp, "%c", temp);
            fprintf(fp, "%d ", n);
        }
        break;
    case 2:
        for (int i = 0; i < count; i++)
        {
            n = rand() % 201;
            // temp = n + '0';
            // fprintf(fp, "%c", temp);
            fprintf(fp, "%d ", n);
        }
        break;
    default:
        break;
    }
}

int print_file(char *name)
{
    FILE *fp = fopen(name, "r");
    if (fp == 0)
        exit(-1);
    else
    {
        int temp;
        while (!feof(fp))
        {
            fscanf(fp, "%d ", &temp);
            // if (fscanf(fp, "%d ", temp))
            //     break;
            printf("%d ", temp);
        }
        printf("\n");
        fclose(fp);
        return 1;
    }
}

int modul(int a)
{
    if (a < 0)
        return -a;
    else
        return a;
}

int digits(int a)
{
    int i = 0;
    do
    {
        a /= 10;
        i++;
    } while (modul(a) > 0);

    return i;
}

void arr_digit(int num, int digit, int *mas)
{
    for (int i = 0; i < digit; i++)
    {
        mas[digit - i - 1] = num % 10;
        num /= 10;
    }
    // for (int i = 0; i < digit; i++)
    // {
    //     printf("%d", mas[i]);
    // }
}

void print_whitespaces(FILE *fp, int n)
{
    for (int i = 0; i < n; i++)
    {
        fprintf(fp, " ");
    }
}

void print_underlining(FILE *fp, int n)
{
    for (int i = 0; i < n; i++)
    {
        fprintf(fp, "-");
    }
    fprintf(fp, "\n");
}

void task1(char *name)
{
    printf("\nStarting 1 task... \n");

    FILE *fp = fopen(name, "w+t");
    if (fp == 0)
        exit(-1);
    else
    {
        input_file(fp);
        fclose(fp);

        if ((fp = fopen(name, "r")) == NULL)
        {
            printf("File not found\n");
        }

        printf("\nFile information: ");
        print_file(name);
    }
}

void task2(char *name)
{
    printf("\nStarting 2 task... \n");

    FILE *fp = fopen(name, "r+t");
    if (fp == 0)
        exit(-1);
    else
    {
        int temp;
        int num = 0;
        while (!feof(fp))
        {
            fscanf(fp, "%d ", &temp);
            if (temp % 2 == 0)
                printf("%d ", temp);
        }
        printf("\n");
        fclose(fp);
    }
}

int task3(char *name)
{
    printf("\nStarting 3 task... \n");

    FILE *fp = fopen(name, "r+t");
    if (fp == 0)
        exit(-1);
    else
    {
        int num1 = 0, num2 = 0, digit1 = 0, digit2 = 0;
        fscanf(fp, "%d ", &num1);
        fscanf(fp, "%d ", &num2);
        if (num1 < 0 || num2 < 0)
        {
            // fprintf(fp, "%s", "\nSorry I'm a stupid lazy fish so I don't wanna consider nums < 0\n");
            num1 = modul(num1);
            num2 = modul(num2);
        }
        digit1 = digits(num1);
        digit2 = digits(num2);

        int *mas1 = malloc(sizeof(int) * digit1);
        arr_digit(num1, digit1, mas1);
        int *mas2 = malloc(sizeof(int) * digit2);
        arr_digit(num2, digit2, mas2);

        // printf("\nFirst num has %d digits and is ", digit1);
        // for (int i = 0; i < digit1; i++)
        //     printf("%d", mas1[i]);
        // printf("\nSecond num has %d digits and is ", digit2);
        // for (int i = 0; i < digit2; i++)
        //     printf("%d", mas2[i]);

        fseek(fp, 0, SEEK_END);
        // fputs("\0\nMultiplication:\n", fp);
        fprintf(fp, "%s", "\nMultiplication:\n");
        print_whitespaces(fp, digit2 + 1);
        for (int i = 0; i < digit1; i++)
            fprintf(fp, "%d", mas1[i]);
        fprintf(fp, "%s", "\nx\n");

        print_whitespaces(fp, digit1 + 1);
        for (int i = 0; i < digit2; i++)
            fprintf(fp, "%d", mas2[i]);
        fprintf(fp, "\n");

        int width = digit1 + digit2 + 1;
        print_underlining(fp, width);

        int resdigit;
        if (digit1 < digit2)
            resdigit = digit2 + 1;
        else
            resdigit = digit1 + 1;

        int temp = 0, i;

        for (i = 0; i < digit2; i++)
        {
            temp = num1 * mas2[digit2 - i - 1];
            int *result = calloc(sizeof(char), (resdigit));
            for (int j = 0; j < resdigit; j++)
            {
                result[resdigit - j - 1] = temp % 10;
                temp /= 10;
            }

            // if (i == digit2 / 2 || (digit2 == 1))
            // {
            //     fprintf(fp, "+");
            //     print_whitespaces(fp, width - resdigit - i - 1);
            // }
            // else
            print_whitespaces(fp, width - resdigit - i);

            if (result[0] == 0)
                fprintf(fp, " ");
            else
                fprintf(fp, "%d", result[0]);
            for (int j = 1; j < resdigit; j++)
                fprintf(fp, "%d", result[j]);

            fprintf(fp, "\n");
        }
        if (i > 1)
        {
            print_underlining(fp, width);
            temp = num1 * num2;
            int *result = calloc(sizeof(char), (width));
            for (int j = 0; j < width; j++)
            {
                result[width - j - 1] = temp % 10;
                temp /= 10;
            }
            int j = 0;
            while (result[j] == 0 && j < width)
            {
                fprintf(fp, " ");
                j++;
            }
            for (j; j < width; j++)
                fprintf(fp, "%d", result[j]);
        }
    }
}

/*int main(int argc, char **argv)
{
    srand((int)time(NULL));
    // if (argc != 2)
    // {
    //     printf("Where is the file name?)\n");
    //     exit(-1);
    // }
    // if (strlen(argv[1]) > 255)
    //     exit(-1);
    // char *name = argv[1];
    int temp;
    char *name = "testing";
    task1(name);
    task2(name);
    task3(name);
    return 0;
}*/

int main()
{
    char *name1 = "file1";
    char *name2 = "file2";
    char *name3 = "file3";
    FILE *fp1 = fopen(name1, "a+b");
    FILE *fp2 = fopen(name2, "a+b");
    // input_file(fp1);
    // input_file(fp2);
    FILE *fp3 = fopen(name3, "a+b");
    fseek(fp2, (int)((-1) * sizeof(int)), SEEK_END);
    int temp1 = 0, temp2 = 0;
    fread(&temp1, sizeof(int), 1, fp1);
    fread(&temp2, sizeof(int), 1, fp2);
    while (feof(fp1) && ftell(fp2) != 0)
    {
        if (temp1 < temp2)
        {
            fprintf(fp3, "%d ", temp2);
            fseek(fp2, (int)((-2) * sizeof(int)), SEEK_CUR);
            fread(&temp2, sizeof(int), 1, fp2);
        }
        else
        {
            fprintf(fp3, "%d ", temp1);
            fread(&temp1, sizeof(int), 1, fp1);
        }
    }
}