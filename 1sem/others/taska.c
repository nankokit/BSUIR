#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include <time.h>

long double scd()
{
	int readinput;
	long double input;
	char inputchar;

	while (1)
	{
		readinput = scanf_s("%Lf", &input);
		scanf_s("%c", &inputchar);
		if (readinput != 0 && inputchar == '\n' && (int)input == input)
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

int main()
{ 
	while (1)
	{
		srand((int)time(NULL));
		int a = 0, row, column;
		int mas[100][100];

		//entering number of elements
		printf("Enter the number of rows: ");
		row = scd();
		while (row >= 100 || row == 0 || row < 0 || row % 2 != 0)
		{
			printf("Incorrect. Try again. \n");
			rewind(stdin);
			row = scd();
		}
		printf("\n");

		printf("Enter the number of columns: ");
		column = scd();
		while (column >= 100 || column == 0 || column < 0 || column % 2 != 0)
		{
			printf("Incorrect. Try again. \n");
			rewind(stdin);
			column = scd();
		}
		printf("\n");


		//selection input method
		printf("Select array input method:\n1 - random input\n2 - keyboard input\n");
		int check = -1;
		while (check == -1)
		{
			a = scd();
			if (a == 1)
			{
				check = 1;
				for (int i = 0; i < row; i++)
				{
					for (int j = 0; j < column; j++)
					{
						printf("mas[%d][%d] = ", i + 1, j + 1);
						mas[i][j] = rand() % 201 - 100;
						printf("%d\n", mas[i][j]);
					}
				}
			}
			else
			{
				if (a == 2)
				{
					check = 1;
					printf("Enter array elements \n");
					for (int i = 0; i < row; i++)
					{
						for (int j = 0; j < column; j++)
						{
							printf("mas[%d][%d] = ", i + 1, j + 1);
							mas[i][j] = scd();
						}
					}
				}
				else
				{
					printf("Incorrect. Try again. \n");
					rewind(stdin);
				}
			}
		}
		printf("\n");

		int sum1 = 0, sum2 = 0, sum3 = 0, sum4 = 0;
		// 1
		for (int i = 0; i < row / 2; i++)
		{
			for (int j = 0; j < column / 2; j++)
			{
				sum1 += mas[i][j];
			}
		}
		//2
		for (int i = 0; i < row / 2; i++)
		{
			for (int j = column / 2; j < column; j++)
			{
				sum2 += mas[i][j];
			}
		}
		// 3
		for (int i = row / 2; i < row; i++)
		{
			for (int j = 0; j < column / 2; j++)
			{
				sum3 += mas[i][j];
			}
		}

		//4
		for (int i = row / 2; i < row; i++)
		{
			for (int j = column / 2; j < column; j++)
			{
				sum4 += mas[i][j];
			}
		}

		for (int i = 0; i < row; i++)
		{
			for (int j = 0; j < column; j++)
			{
				printf("%d ", mas[i][j]);
			}
			printf(" \n");
		}

		printf("Summa 1 = %d\n", sum1);
		printf("Summa 2 = %d\n", sum2);
		printf("Summa 3 = %d\n", sum3);
		printf("Summa 4 = %d\n", sum4);
		printf(" \n");
	}
}
