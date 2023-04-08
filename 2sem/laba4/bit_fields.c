#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct bit_field
{
    unsigned int lastbit : 1;
};

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

int pow_two(int num)
{
    int check = 1;
    while (check < num)
        check *= 2;
    if (check == num)
        return 1;
    return 0;
}

void bit_remains(int argc, char *argv[])
{
    if (argc != 3)
        exit(-1);

    // printf("S1 %s s2 %s\n", argv[1], argv[2]);
    int divisible = atoi(argv[1]), divisor = atoi(argv[2]), remains = 0;

    // printf("Enter divisible: ");
    // int divisible = input_int();
    // printf("Enter divisor: ");
    // int divisor = input_int(), remains = 0;
    while (divisible < 0)
    {
        printf("Enter the divisible again\n");
        divisible = input_int();
    }
    while (!pow_two(divisor))
    {
        printf("Enter the divider being the power of two again\n");
        divisor = input_int();
    }

    struct bit_field *pointer; // объявление указателя на структуру

    for (int i = 1; i != divisor; i *= 2)
    {
        pointer = (struct bit_field *)&divisible; // указателю pointer явно присваивается адрес переменной
        if (pointer->lastbit)                     // если младший бит равен 1,
            remains += 1 * i;                     // остаток будет равен 1
        else                                      // если младший бит не равен 1,
            remains += 0 * i;                     // остаток будет равен 0
        divisible /= 2;
    }

    printf("Result: %d \n", remains);

    // printf("Num1 - %d, divisor - %d \n", divisible, divisor);
    // printf("Result: %d \n", (divisible & (divisor - 1))); // the remains of the division
}

int main(int argc, char *argv[])
{
    bit_remains(argc, argv);
    return 0;
}