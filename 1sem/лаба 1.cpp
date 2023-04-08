#include <iostream>
#include <math.h>
#include <stdio.h>


void task1()
{
	printf("Введите длину ребра куба \n");
	float a, V, S;
	scanf_s("%f", &a);
	if (a > 0) {
		V = a * a * a;
		S = a * a * 6;
		printf("Объем: %.3f \n", V);
		printf("Площадь поверхности: %.3f \n", S);
	}
	else
		printf("Некорректные данные\n");
	printf("\n");
}

void task2() 
{
	float x, y, R2, r;
	printf("Введите x и y: \n");
	scanf_s("%f %f", &x, &y);
	R2 = pow(x, 2) + pow(y, 2);
	r = sqrt(R2);
	if (r == 1)
	{
		printf("Через точку (%.2f ; %.2f) возможно провести окружность с центром в точке координат  \n", x, y);
	}
	else
	{
		printf("Через точку (%.2f ; %.2f) невозможно провести окружность с центром в точке координат  \n", x, y);
	}
	printf("\n");
}

void task3()
{
	printf("Введите x, y, z:  \n");
	float a[3];
	int sum=0;
	for (int i = 0; i < 3; i++) 
	{
		scanf_s("%f", &a[i]);
	}
	for (int i = 0; i < 3; i++)
	{
		if (a[i] > 0)
		{
			sum++;
		}
	}
	printf("Количество положительных чисел: %d \n", sum);
	printf("\n");
}

void task4() 
{
	int num;
	printf("Введите номер месяца от 1 до 12:  \n");
	scanf_s("%d", &num);
	if ((num >= 1 && num <= 2) || num == 12)
	{
		printf("Зима \n");
	}
	else {
		if (num >= 3 && num <= 5)
		{
			printf("Весна \n");
		}
		else
		{
			if (num >= 6 && num <= 8)
			{
				printf("Лето \n");
			}
			else
			{
				if (num >= 9 && num <= 11)
				{
					printf("Осень \n");
				}
				else
				{
					printf("Некорректные данные \n");
				}
			}
		}
	}

	printf("\n");
}


int main()
{
	setlocale(0, "rus");
	int n;
	while (1) 
	{
		printf("Введите номер задания: \n");
		scanf_s("%d", &n);
		printf("\n");

		switch (n)
		{
		case 1:
		{printf("Задание 1 \n");
		printf("\n");
		task1();
		break; }
		case 2:
		{printf("Задание 2 \n");
		printf("\n");
		task2();
		break; }
		case 3:
		{printf("Задание 3 \n");
		printf("\n");
		task3();
		break; }
		case 4:
		{printf("Задание 4 \n");
		printf("\n");
		task4();
		break; }

		default: 
		{printf("Некорректно введенные данные, попробуйте еще раз  \n");
		printf("\n");
		break; }
		}
	}

	return 0;
}


