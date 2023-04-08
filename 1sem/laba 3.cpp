#include <stdio.h>
#include <cstdlib>
#include <time.h>
//#include <iostream>

int getInt(int* S)
{
	char temp;
	while ((temp = getchar()) != '\n')
	{
		if ((temp > '9') || (temp < '0'))
		{
			//printf("Check. \n");
			//printf("Incorrect. Try again. \n");
			return -2;
			break;
		}
	}
	return 1;
}

int getFloat(float* S)
{
	char temp;
	while ((temp = getchar()) != '\n')
	{
		if ((temp > '9') || (temp < '0') || (temp != '.'))
		{
			//printf("Check. \n");
			//printf("Incorrect. Try again. \n");
			return -2;
			break;
		}
	}
	return 1;
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

void task1()
{
	srand((int)time(NULL));
	int a = 0, number;
	int mas[100];

	//entering number of elements
	printf("Enter the number of array elements \n");
	number = scd();
	while (number >= 100 || number == 0 || number < 0)
	{
		printf("Incorrect. Try again. \n");
		rewind(stdin);
		number = scd();
	}

	//selection input method
	printf("Select array input method:\n1 - random input\n2 - keyboard input\n");
	int check = -1;
	while (check == -1)
	{
		a = scd();
		if (a == 1)
		{			
			check = 1;
			for (int i = 0; i < number; i++)
			{
				printf("mas[%d] = ", i + 1);
				mas[i] = rand() % 2001 - 1000;
				printf("%d\n", mas[i]);	
			}
		}
		else
		{
			if (a == 2)
			{
				check = 1;
				printf("Enter array elements \n");
				for (int i = 0; i < number; i++)
				{
					printf("mas[%d] = ", i + 1);
					mas[i] = scd();
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

	//multiplication even array elements
	long int product = 1;
	for (int i = 0; i < number; i++)
	{
		if ((i + 1) % 2 == 0)
		{
			product = product * mas[i];
		}

	}
	printf("Product of even array elements = %ld\n", product);

	//search for null elements
	int num1, num2;
	for (int i = 0; i < number; i++)
	{
		if (mas[i] == 0)
		{
			num1 = i;
			break;
		}
		else
		{
			num1 = -1;
		}

	}
	for (int i = number; i >= 0; i--)
	{
		if (mas[i] == 0)
		{
			num2 = i;
			break;
		}
		else
		{
			num2 = -1;
		}

	}
	
	int sum0 = 0;
	if (num1 != -1 && num2 != -1)
	{
		for (int i = num1 + 1; i < num2; i++)
		{                               
			//printf("S = %f \n", mas[i]);
			sum0 += mas[i];
			//printf("Sum = %f \n", sum0);
			}
		printf("Summa = %d \n", sum0);
	}
	else
	{
		printf("No summa \n", sum0);
	}
	
	
	for (int i = 0; i < number; i++)
	{
		mas[i] = 0;
	}
}

void task2()
{
	srand((int)time(NULL));
	int a = 0, number;
	int mas[100];
	printf("Enter the number of array elements \n");
	number = scd();
	while (number >= 100 || number == 0 || number < 0)
	{
		printf("Incorrect. Try again. \n");
		rewind(stdin);
		number = scd();
	}

	//entering number of elements
	printf("Select array input method:\n1 - random input\n2 - keyboard input\n");
	int check = -1;
	while (check == -1)
	{
		a = scd();
		if (a == 1)
		{
			check = 1;
			for (int i = 0; i < number; i++)
			{
				printf("mas[%d] = ", i + 1);
				mas[i] = rand() % 2001 - 1000;
				printf("%d\n", mas[i]);
			}
		}
		else
		{
			if (a == 2)
			{
				check = 1;
				printf("Enter array elements \n");
				for (int i = 0; i < number; i++)
				{
					printf("mas[%d] = ", i + 1);
					mas[i] = scd();
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

	for (int i = 0; i < number; i++)
	{
		if (mas[i] % 2 == 0 && mas[i] != 0)
		{
			for (int j = number; j > i + 1; j--)
			{
				mas[j] = mas[j - 1];
			}
			number++;
			mas[i + 1] = 0;
		}
	}

	for (int i = 0; i < number; i++)
	{
		printf("mas[%d] = %d \n", i + 1, mas[i]);
	}

	for (int i = 0; i < number; i++)
	{
		mas[i] = 0;
	}
}

void task3()
{
	srand((int)time(NULL));
	int a = 0, number;
	int mas1[100], mas2[100];
	int check = -1;
	printf("Select array 1 input method:\n1 - random input\n2 - keyboard input\n");
	while (check == -1)
	{
		a = scd();
		if (a == 1)
		{
			check = 1;
			for (int i = 0; i < 10; i++)
			{
				printf("mas1[%d] = ", i + 1);
				mas1[i] = rand() % 2001 - 1000;
				printf("%d\n", mas1[i]);
			}
		}
		else
		{
			if (a == 2)
			{
				check = 1;
				printf("Enter array elements \n");
				for (int i = 0; i < 10; i++)
				{
					printf("mas1[%d] = ", i + 1);
					mas1[i] = scd();
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
	check = -1;
	printf("Select array 2 input method:\n1 - random input\n2 - keyboard input\n");
	while (check == -1)
	{
		a = scd();
		if (a == 1)
		{
			check = 1;
			for (int i = 0; i < 10; i++)
			{
				printf("mas2[%d] = ", i + 1);
				mas2[i] = rand() % 2001 - 1000;
				printf("%d\n", mas2[i]);
			}
		}
		else
		{
			if (a == 2)
			{
				check = 1;
				printf("Enter array elements \n");
				for (int i = 0; i < 10; i++)
				{
					printf("mas2[%d] = ", i + 1);
					mas2[i] = scd();
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

	int minPosition, tmp, cell = -1;

	for (int i = 0; i < 10; i++)
	{
		minPosition = i;
		for (int j = i + 1; j < 10; j++)
			if (mas1[minPosition] > mas1[j])
				minPosition = j;
		tmp = mas1[minPosition];
		mas1[minPosition] = mas1[i];
		mas1[i] = tmp;
	}

	/*printf("Sorted array 1:\n");
	for (int i = 0; i < 10; i++)
		printf("%d ", mas1[i]);*/
	printf("\n");

	/*
	for (int i = 0; i < 10; i++)
	{
		if (cell == -1)
		{
			for (int j = 0; j < 10; j++)
			{
				if (mas1[i] == mas2[j])
				{
					cell = i;
				}
			}
		}
		else
		{
			//printf("%d\n", cell);
			i = 10;
		}

	}
	if (cell != -1) printf("Element = %d\n", mas1[cell]);
	else printf("There no such elements");*/



	int element, checkmas = 0;
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
			{
				if ( mas1[i]!= mas2[j])
				{
					checkmas ++;
				}
			}
		if (checkmas == 10)
		{
			element = mas1[i];
			i = 10;
		}
		else checkmas = 0;
		//printf("P %d check = %d \n", i, checkmas);
	}
	if (checkmas == 10) printf("Element = %d\n", element);
	else  printf("There no such elements");


	/*


	printf("Sorted array 1:\n");
	for (int i = 0; i < 10; i++)
		printf("%d ", mas1[i]);
		
	printf("\n");
	free(mas1);*/
	

	printf("\n");
}

int main()
{
	int n;
	while (1)
	{
		printf("Enter task number: \n");
		n = scd();
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











/*while (scanf_s("%d", &a) != 1)
{
	printf("Incorrect. Try again. \n");
	rewind(stdin);
}

do
{
	scanf_s("%i", number);
} while ((getInt(&number)) == 1);

 || a != 1 || a !=2

do
{

} while ((getInt(&a)) == 1);



while (check != 0)
{
	do
	{
		scanf_s("%i", &a);
		a = getchar();
		if ((a > '9') || (a < '0'))
		{
			printf("Incorrect. Try again. \n");
			break;
		}
		else check = 0;
	} while (a != '\n');
}

do
	{

		fflush(stdin);
	} while ((getInt(&number)) != 1);

	*/



/*
* 
* scanf_s("%d", &number);
	do
	{
		printf("Incorrect. Try again. \n");
		rewind(stdin);
	} while (getInt(&number) != 1);

	
	while (scanf_s("%d", &number) != 1 || number < 0)
	{
		printf("Incorrect. Try again. \n");
		rewind(stdin);
	}

		while (getInt(&n) != 1)
		{
			printf("Incorrect. Try again. \n");
			scanf_s("%i", &n);
		}


		//scanf_s("%i", &n);



		scanf_s("%d", &n);
			while ((n = getchar()) != '\n')
		{
			if ((n > '9') || (n < '0'))
			{
				//printf("Check. \n");
				printf("Incorrect. Try again. \n");
				//rewind(stdin);
				//return -2;
				break;
			}
		}*/