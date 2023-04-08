#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include <string.h>

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

FILE *file_open(char *fname, char *mode)
{
    FILE *fp = fopen(fname, mode);
    if (fp == NULL)
    {
        printf("File error\n");
        return 0;
    }
    return fp;
}

int valid_file_open(char *fname)
{
    FILE *fp = fopen(fname, "r+b");
    if (fp == NULL)
    {
        printf("File error\n");
        return 0;
    }
    return 1;
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

    switch (n)
    {
    case 1:
        for (int i = 0; i < count; i++)
        {
            printf("Enter %d number: ", i + 1);
            n = check_int_input();
            fwrite(&n, sizeof(n), 1, fp);
        }
        break;
    case 2:
        for (int i = 0; i < count; i++)
        {
            n = rand() % 21 - 10;
            fwrite(&n, sizeof(n), 1, fp);
        }
        break;
    default:
        break;
    }
}

int print_file(char *name)
{
    // FILE *fp = file_open(name, 'r+b');
    // if (fp == 0)
    //     return 0;
    FILE *fp = fopen(name, "r+b");
    if (valid_file_open(name) == 0)
        exit(-1);
    else
    {
        int temp = 0;
        while (!feof(fp))
        {
            if (fread(&temp, sizeof(int), 1, fp) != 1)
                break;
            printf("%d ", temp);
        }
        printf("\n");
        fclose(fp);
        return 1;
    }
}

int sizeoff(FILE *fp)
{
    fseek(fp, 0, SEEK_END); // seek to end of file
    int size = ftell(fp);   // get current file pointer
    fseek(fp, 0, SEEK_SET);
    return size;
}

int module(int a)
{
    if (a < 0)
        return -a;
    else
        return a;
}

void counting(FILE *fp, int *count_chet, int *count_nechet, int *count_null)
{
    int temp1 = 0, count = sizeoff(fp) / sizeof(int);
    fsetpos(fp, SEEK_SET);
    for (int i = 0; i < count; i++)
    {
        fseek(fp, (int)sizeof(int) * i, SEEK_SET);
        fread(&temp1, (int)sizeof(int), 1, fp);
        if (temp1 == 0)
            (*count_null)++;
        else
        {
            if (temp1 % 2 == 0)
                (*count_chet)++;
            else
                (*count_nechet)++;
        }
    }
}

void task1(char *name)
{
    printf("\nStarting 1 task... \n");

    FILE *fp = fopen(name, "w+b");
    // if (fp == 0)
    //     return 0;
    if (valid_file_open(name) == 0)
        exit(-1);
    else
    {
        input_file(fp);
        fclose(fp);

        if ((fp = freopen(name, "r", fp)) == NULL)
        {
            printf("File not found\n");
            return;
        }

        printf("\nFile information: ");
        print_file(name);
    }
}

void task2(char *name)
{
    printf("\nStarting 2 task... \n");
    // if (fp == 0)
    //     return 0;
    FILE *fp = fopen(name, "r+b");
    if (valid_file_open(name) == 0)
        exit(-1);
    int temp = 0, count = 1, a, max = 0;
    fread(&a, sizeof(int), 1, fp);
    temp = a;

    while (!feof(fp))
    {
        fread(&a, sizeof(int), 1, fp);
        if (temp != a)
        {
            temp = a;
        }
        else
        {
            while (temp == a && !feof(fp))
            {
                count++;
                fread(&a, sizeof(int), 1, fp);
            }
            if (count > max)
                max = count;
            count = 1;
            temp == a;
            if (!feof(fp))
                fseek(fp, (int)(sizeof(int) * (-1)), SEEK_CUR);
        }
    }

    printf("Max count: %i\n", max);
    fclose(fp);
}

void task3(char *name)
{
    printf("\nStarting 3 task... \n");
    // FILE *fp = file_open(name, 'r+b');
    // if (fp == 0)
    //     return 0;
    FILE *fp = fopen(name, "r+b");
    if (valid_file_open(name) == 0)
        exit(-1);
    int a = 0;
    int count = sizeoff(fp) / sizeof(int);
    fsetpos(fp, SEEK_SET);
    for (int i = 0; i < count; i += 2)
    {
        fseek(fp, (int)sizeof(int) * i, SEEK_SET);
        fwrite(&a, (int)sizeof(int), 1, fp);
    }
    fclose(fp);
    print_file(name);
}

void task4(char *name)
{
    printf("\nStarting 4 task... \n");
    // FILE *fp = file_open(name, 'r+b');
    // if (fp == 0)
    //     return 0;
    FILE *fp = fopen(name, "r+b");
    if (valid_file_open(name) == 0)
        exit(-1);

    int temp1 = 0, temp2 = 0;
    int count = sizeoff(fp) / sizeof(int);

    int count_chet = 0, count_nechet = 0, count_null = 0;
    counting(fp, &count_chet, &count_nechet, &count_null);

    fseek(fp, 0, SEEK_SET);
    int set_1 = 0, end = 0, i = 0;

    while (end != (count_chet + count_null - 1))
    {
        i = 0;
        fseek(fp, i * sizeof(int), SEEK_SET);
        fread(&temp1, sizeof(int), 1, fp);
        i++;

        while (module(temp1) % 2 == 0)
        {
            fread(&temp1, sizeof(int), 1, fp);
            i++;
        }
        end = i - 1; // before first nechet after all chet

        if (end != (count_chet + count_null))
        {
            do
            {
                fread(&temp1, sizeof(int), 1, fp);
                i++;
            } while (module(temp1) % 2 == 1);
            set_1 = i - 1; // before active chet

            while (set_1 != end)
            {
                set_1--;
                fseek(fp, (set_1) * (int)sizeof(int), SEEK_SET);
                fread(&temp1, sizeof(int), 1, fp);
                fread(&temp2, sizeof(int), 1, fp);
                fseek(fp, (set_1) * (int)sizeof(int), SEEK_SET);
                fwrite(&temp2, (int)sizeof(int), 1, fp);
                fwrite(&temp1, (int)sizeof(int), 1, fp);
            }
        }

        if ((fp = freopen(name, "r+b", fp)) == NULL)
        {
            printf("File not found\n");
            return;
        }
        // print_file(name);
    }
    // printf("\nStep 1... \n");
    // print_file(name);
    // printf("\nStep 2... \n");
    while (end != (count_chet - 1))
    {
        i = 0;
        fseek(fp, i * sizeof(int), SEEK_SET);
        fread(&temp1, sizeof(int), 1, fp);
        i++;

        while (temp1 != 0)
        {
            fread(&temp1, sizeof(int), 1, fp);
            i++;
        }
        end = i - 1; // before first null after all chet

        if (end != count_chet)
        {
            do
            {
                fread(&temp1, sizeof(int), 1, fp);
                i++;
            } while (module(temp1) == 0);
            set_1 = i - 1; // before active chet

            while (set_1 != end)
            {
                set_1--;
                fseek(fp, (set_1) * (int)sizeof(int), SEEK_SET);
                fread(&temp1, sizeof(int), 1, fp);
                fread(&temp2, sizeof(int), 1, fp);
                fseek(fp, (set_1) * (int)sizeof(int), SEEK_SET);
                fwrite(&temp2, (int)sizeof(int), 1, fp);
                fwrite(&temp1, (int)sizeof(int), 1, fp);
            }
        }

        if ((fp = freopen(name, "r+b", fp)) == NULL)
        {
            printf("File not found\n");
            return;
        }
        // printf("Result: \n");
        // print_file(name);
    }
    // printf("Result: \n");
    print_file(name);
}

int main(int argc, char **argv)
{
    srand((int)time(NULL));
    if (argc != 2)
    {
        printf("Where is the file name?)\n");
        exit(-1);
    }
    if (strlen(argv[1]) > 255)
        exit(-1);
    char *name = argv[1];

    // char *name = "testing.bin";
    task1(name);
    task2(name);
    task3(name);
    task4(name);
    return 0;
}
