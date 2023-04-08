#include <iostream>
#include <stdio.h>

// 1 / x + cos(x) - sqrt(x) функция

long double hhyperbolax(long double x)
{
	long double a = 1 / x;
	return a;
}

long double sqrtx(long double x)
{
	long double b = 0.5;
	for (int i = 0; i < 50; i++)
	{
		b = b / 2 + x / (2 * b);
	}
	return b;
}

long double cosx(long double x)
{
	long double znach;
	while (x > 6.283184)
	{
		x = x - (6.283184);
	}
	while (x < -6.283184)
	{
		x = x + (6.283184);
	}

	znach = 1;
	long double factorial = 2, num = 3;
	long double powl = -x * x;
	for (int i = 0; i < 30; i++)
	{
		znach += powl / factorial;
		factorial *= num * (num + 1);
		num += 2;
		powl *= -x * x;
	}
	return znach;
}

long double modul(long double x)
{
	if (x < 0) x *= -1;
	return x;
}

long double scd()
{
	int readinput;
	long double input;
	char inputchar;

	while (1)
	{
		readinput = scanf_s("%Lf", &input);
		scanf_s("%c", &inputchar);
		if (readinput != 0 && inputchar == '\n')
		{
			if(input > 0)
			return input;
			else
			{
				printf("Incorrect input. Try again. \n");
				rewind(stdin);
			}
		}
		else
		{
			printf("Incorrect input. Try again. \n");
			rewind(stdin);
		}
	}
}

int main()
{
	long double x, result = 0;
	while (1)
		{
			printf("Enter x (x > 0): ");
			x = scd();
			result = hhyperbolax(x) + cosx(x) - sqrtx(x);
			printf("\nResult: %lf\n", result);
		}	
	
}