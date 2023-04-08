#include <iostream>
#include <stdio.h>
#include <cstdlib>

long double scdint()
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

int inputarray(int rows, int columns, int mas[100][100])
{
	//int mas[100][100];
	int a = 0;
	srand((int)time(NULL));
	//selection input method
	printf("Select array input method:\n1 - random input\n2 - keyboard input\n");
	int check = -1;
	while (check == -1)
	{
		a = scdint();
		if (a == 1)
		{
			check = 1;
			for (int i = 0; i < rows; i++)
			{
				for (int j = 0; j < columns; j++)
				{
					//printf("mas[%d][%d] = ", i + 1, j + 1);
					mas[i][j] = rand() % 2001 - 1000;
				}
			}
			printf("\n");
		}
		else
		{
			if (a == 2)
			{
				check = 1;
				printf("Enter array elements \n");
				for (int i = 0; i < rows; i++)
				{
					for (int j = 0; j < columns; j++)
					{
						printf("mas[%d][%d] = ", i + 1, j + 1);
						mas[i][j] = scdint();
					}
				}
				printf("\n");
			}
			else
			{
				printf("Incorrect. Try again. \n");
				rewind(stdin);
			}
		}
	}
	/*for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			printf(" %d ", mas[i][j]);
		}
		printf("\n");
	}*/
	return mas[100][100];
}

void task1()
{
	int rows, columns;
	int mas[100][100];

	printf("Enter the number of array rows ( <100 ) \n");
	rows = scdint();
	while (rows >= 100 || rows == 0 || rows < 0)
	{
		printf("Incorrect. Try again. \n");
		rewind(stdin);
		rows = scdint();
	}
	printf("Enter the number of array columns ( <100 ) \n");
	columns = scdint();
	while (columns >= 100 || columns == 0 || columns < 0)
	{
		printf("Incorrect. Try again. \n");
		rewind(stdin);
		columns = scdint();
	}
	
	inputarray(rows, columns, mas);
	
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			printf(" %4d ", mas[i][j]);
		}
		printf("\n");
	}

	int check_column = 0, result = 0;
	for (int j = 0; j < columns; j++)
	{
		for (int i = 0; i < rows; i++)
		{
			if (mas[i][j] == 0)
			{
				check_column++;
				i = rows;
			}
		}
		if (check_column == 0) 
			result++;
		check_column = 0;
		printf("\n");
	}
	printf("Count columns without 0 = %d\n", result);
}

void task2()
{
	int rows, columns;
	int mas[100][100];

	printf("Enter the number of array rows ( <100 ) \n");
	rows = scdint();
	while (rows >= 100 || rows == 0 || rows < 0)
	{
		printf("Incorrect. Try again. \n");
		rewind(stdin);
		rows = scdint();
	}
	printf("Enter the number of array columns ( <100 ) \n");
	columns = scdint();
	while (columns >= 100 || columns == 0 || columns < 0)
	{
		printf("Incorrect. Try again. \n");
		rewind(stdin);
		columns = scdint();
	}
	
	inputarray(rows, columns, mas);

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			printf(" %4d ", mas[i][j]);
		}
		printf("\n");
	}
	printf("\n");
	int index = -1;

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			if (mas[i][j] > 0)
			{
				index = i + 1;
				for (int k = 0; k < columns; k++)
				{
					mas[i - 1][k] *= (-1);
				}
				printf("Row number %d \n", index);
				j = columns;
				for (int l = 0; l < rows; l++)
				{
					for (int n = 0; n < columns; n++)
					{
						printf(" %4d ", mas[l][n]);
					}
					printf("\n");
				}
				printf("\n");
			}
		}
	}
	printf("\n");
	if(index == -1) printf("There are no such rows \n");
	//else printf("Row number %d \n", index);
}

void task3()
{
	int rows, columns;
	int mas[100][100];

	printf("Enter the number of array rows = columns ( <100 ):  \n");
	rows = scdint();
	while (rows >= 100 || rows == 0 || rows < 0)
	{
		printf("Incorrect. Try again. \n");
		rewind(stdin);
		rows = scdint();
	}
	columns = rows;

	inputarray(rows, columns, mas);

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			printf(" %4d ", mas[i][j]);
		}
		printf("\n");
	}

	int max = mas[0][0], j = 0;
	for (int i = 0; i < rows / 2 + 1; i++)
	{
		j = i;
		for (j ; j < columns; j++)
		{
			if (max < mas[i][j])	max = mas[i][j];
		}
		columns--;		
	}
	printf("Max element at 1st field = %d ", max);
	printf("\n");
}

int main()
{
	int n;
	while (1)
	{
		printf("Enter task number: \n");
		n = scdint();
		printf("\n");
		switch (n)
		{
		case 1:
			printf("Task 1 \n");
			printf("\n");
			task1();
			printf("\n");
			break;

		case 2:
			printf("Task 2 \n");
			printf("\n");
			task2();
			printf("\n");
			break;

		case 3:
			printf("Task 3 \n");
			printf("\n");
			task3();
			printf("\n");
			break;

		default:
			printf("Incorrect. Try again. \n");
			printf("\n");
		}
	}
}
