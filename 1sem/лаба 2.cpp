#include <iostream>
#include <stdio.h>

/*int getFloat(float* S)
{
	char temp;
	while ((temp = getchar()) != '\n') 
	{
		if ((temp > '9') || (temp < '0') || (temp != '.'))
		{
			printf("Некорректный ввод данных \n");
			fflush(stdin);
			return -2;
			break;
		}
		else return 1;
	}
}*/

void task1() 
{
	int i = 1, sum = 10000;
	float result=10000.0;
	for (i; result < sum *2; i++)
	{
		result = 1.03 * result;
	}
	if (i%10 == 1)
			{
				printf("Сумма удвоится через %d год \n", i);
				printf("\n");
			}
	else
		{
			if (i % 10 == 2 && i % 10 ==3 && i % 10 == 4)
				{
				printf("Сумма удвоится через %d года \n", i);
				printf("\n");
				}
			else
				{
				printf("Сумма удвоится через %d лет \n", i);
				printf("\n");
				}
		}
	}

void task2() 
{
	int a, b, c, d;
	for (int i=99; i < 1000; i++)
	{
		a = i / 100;
		b = (i%100) / 10;
		c = i % 10;
		d = (int)sqrt(i);
		if (a < b && b < c && d*d == i)
		{
			printf("%d; ", i);
		}
	}
	printf("\n");
	printf("\n");	
}

void task3() 
{
	{float A, B;
	printf("Введите A и В (A > B): \n");
	while (scanf_s("%f%f", &A, &B) != 2 || A <= 0.0 || B <= 0.0 || A <= B)
	{
		printf("Некорректно введенные данные, попробуйте еще раз \n Введите A и В (A > B)\n");
		rewind(stdin);
	}
	if (A >= B && A > 0 && B > 0)
	{
		A = 10000 * A; //для того, чтобы произошел сдвиг
		B = 10000 * B;
		while (A >= B)
		{
			A = A - B;
		}
		printf("Длина незанятой части А: %.3f \n", A);
		printf("\n");
	}
	else
	{
		printf("Невозможно рассчитать \n");
		printf("\n");
	}
	printf("\n");	}

}

int main()
{
	setlocale(0, "rus");
	int n;
	while(1)
	{ 
		printf("Введите номер задания: ");
		scanf_s("%d", &n);
		printf("\n");
		switch (n)
		{
		case 1:
			printf("Задание 1 \n");
			printf("\n");
			task1();
			break; 
		case 2:
			printf("Задание 2 \n");
			printf("\n");
			task2();
			break; 
		case 3:
			printf("Задание 3 \n");
			printf("\n");
			task3();
			break; 
		default:
			printf("Некорректно введенные данные, попробуйте еще раз  \n");
			rewind(stdin);
		
		}
	}
}




/*do
	{
		fflush(stdin);
		scanf_s("%f", &A);
	} while (getFloat(&A) != 1);*/


/*int code = scanf_s("%f", S);
	if (code != 1)
		return code;*/

		/*
			printf("Введите B: \n");
			do
			{
				fflush(stdin);
				scanf_s("%f", &B);
			} while (getFloat(&B) != 1);*/

/*


	float A, B;
	int check=0;
	printf("Введите A: \n");
	scanf_s("%f", &A);
	char numb =0;
		while ((numb = getchar()) != '\n')
		{
			numb = getchar();
			if ((numb > '9') || (numb < '0') || (numb != '.'))
			{
				printf("Некорректный ввод данных1 \n");
				printf("\n");
				
				check++;
				break;
			}
		} 
	numb = 0;	
	if(A>=0)
	{ 
		
		if (check == 0) 
		{
			printf("Введите B: \n");
			scanf_s("%f", &B);
			if (B >= 0)
			{
				while ((numb = getchar()) != '\n')
				{
					if (B < 0 || ((numb > '9') && (numb < '0') || (numb != '.')))
					{
						printf("Некорректный ввод данных \n");
						printf("\n");
						check++;
					}
				}

				if (check == 0)
				{
					if (A >= B)
					{
						while (A >= B)
						{
							A = A - B;

							printf("Длина незанятой части А: %.0f \n", A);
							printf("\n");
						}
					}
					else
					{
						printf("Невозможно рассчитать \n");
						printf("\n");
					}
				}
			}
			else printf("Некорректный ввод данных \n");
		}
	}
	else printf("Некорректный ввод данных \n");

	printf("\n");*/





/*float checkfloat(float* S)
{
	int amount = scanf_s("%f", S);
	printf("%d", amount);
	if (amount != 1)
	{
		printf("Некорректный ввод данных \n");
		return -2;
	}
	char numb;
	while ((numb = getchar()) != '\n')
	{
		if ((numb > '9') || (numb < '0') || (numb != '.'))
		{
			printf("Некорректный ввод данных \n");
			return -2;
		}
		continue;
	}
	return 1;
}*/


/*float A, B;
do
{
	fflush(stdin);
	printf("Введите A: \n");

} while (checkfloat(&A) != 1);
do
{
	fflush(stdin);
	printf("Введите B: \n");
} while (checkfloat(&B) != 1);





/*
float A, B;
do
{
	//fflush(stdin);
	printf("Введите A: \n");

} while (checkfloat(&A) != 1);
do
{
	//fflush(stdin);
	printf("Введите B: \n");
} while (checkfloat(&B) != 1);

*/



/*int amount1 = scanf_s("%f", A);
printf("%d \n", amount1);
int amount2 = scanf_s("%f", B);
printf("%d \n", amount2);
if (amount1 != 1 && amount2 != 1)
{
	printf("Некорректный ввод данных");
}
else */

