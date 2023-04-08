#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <time.h>

#define MAX_STR_LEN 255

void task101()
{
	int n;
	int **mat;
	scanf_s("%d", &n);
	mat = (int **)calloc(n, sizeof(int *));
	for (int i = 0; i < n; i++)
	{
		mat[i] = (int *)calloc(n, sizeof(int));
	}

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			mat[i][j] = rand() % 201 - 100;
			printf("%4d", mat[i][j]);
		}
		printf("\n");
	}
	printf("Sorted arr\n");
	int i = 0, j = 0, temp;
	while (i < n)
	{
		while (j < n - 1 - i)
		{
			temp = mat[i][j];
			mat[i][j] = mat[n - 1 - j][n - 1 - i];
			mat[n - 1 - j][n - 1 - i] = temp;
			j++;
		}
		i++;
		j = 0;
	}
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			printf("%4d", mat[i][j]);
		}
		printf("\n");
	}
	for (int i = 0; i < n; i++)
	{
		free(mat[i]);
	}
	free(mat);
}

void task102()
{
	int n, m, num = 0, l1, max = 0;
	char **str;
	int l = 0;
	scanf_s("%d%d", &n, &m);
	m++;
	str = (char **)calloc(n, sizeof(char *));
	rewind(stdin);
	for (int i = 0; i < n; i++)
	{
		str[i] = (char *)calloc(m, sizeof(char));
		fgets(str[i], m, stdin);
		rewind(stdin);
	}
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{

			if (((str[i][j]) != ' ') && ((str[i][j]) != '\0') && ((str[i][j]) != '\n'))
			{
				l++;
			}
			else
			{
				if (max < l)
				{
					max = l;
				}
				l = 0;
			}
		}
	}
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{

			if (((str[i][j]) != ' ') && ((str[i][j]) != '\0') && ((str[i][j]) != '\n'))
			{
				l++;
			}
			else
			{
				if (max == l)
				{
					l1 = j - l;
					num = i;
				}
				l = 0;
			}
		}
	}
	for (int i = l1; i < (l1 + max); i++)
	{
		printf("%c", str[num][i]);
	}
	/*int n, lenght = 0, max;
	char c, i = 0, j = 0;
	printf("Enter n: ");
	scanf("%d", &n);
	char **mas = (char **)calloc(n, sizeof(char *));
	int *max_lenght = (int *)calloc(n, sizeof(int));
	for (int i = 0; i < n; i++)
	{
		j = 0;
		*mas[i] = (char *)calloc(1, sizeof(char));
		while ((c = getchar()) != '\n' && c != EOF)
		{
			*mas[i] = (char *)realloc(mas[i], (j + 1) * sizeof(char));
			mas[i][j] = c;
			j++;
		}
		mas[i][j] = '\n';
		j = 0;
		while (mas[i][j] != '\n')
		{
			lenght = 0;
			if (mas[i][j] == ' ')
			{
				do
				{
					lenght++;
					j++;
				} while (mas[i][j] != ' ' || mas[i][j] != '\n');
				if(lenght > max) max = lenght;
				else lenght = 0;
			}
			j++;
		}
	}

	printf("Text:\n");
	for (int i = 0; i < n; i++)
	{
		while (mas[i][j] != '\n')
		{
			printf("%c", mas[i][j]);
			j++;
		}
		printf("\n");
	}*/
}

void task201()
{
	int n, m, k;
	int **mat1, **mat2;
	printf("Enter m & n of string 1(m*n) ");
	scanf_s("%d %d", &m, &n);
	printf("N is the same. Enter k of string 2(n*k) ");
	scanf_s("%d", &k);
	mat1 = (int **)calloc(m, sizeof(int *));
	for (int i = 0; i < m; i++)
	{
		mat1[i] = (int *)calloc(n, sizeof(int));
	}
	mat2 = (int **)calloc(n, sizeof(int *));
	for (int i = 0; i < n; i++)
	{
		mat2[i] = (int *)calloc(k, sizeof(int));
	}
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
		{
			mat1[i][j] = rand() % 21 - 10;
			printf("%4d", mat1[i][j]);
		}
		printf("\n");
	}
	printf("\n");
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < k; j++)
		{
			mat2[i][j] = rand() % 21 - 10;
			printf("%4d", mat2[i][j]);
		}
		printf("\n");
	}
	printf("\n");
	int **mat_new = (int **)calloc(m, sizeof(int *));
	for (int i = 0; i < n; i++)
	{
		mat_new[i] = (int *)calloc(k, sizeof(int));
	}
	int a1 = 0, a2 = 0, b2 = 0;
	while (a1 < m)
	{
		while (b2 < k)
		{
			while (a2 < n)
			{
				mat_new[a1][b2] += mat1[a1][a2] * mat2[a2][b2];
				a2++;
			}
			a2 = 0;
			b2++;
		}
		b2 = 0;
		a1++;
	}

	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < k; j++)
		{
			printf("%4d", mat_new[i][j]);
		}
		printf("\n");
	}
}

void task_sidorovich1()
{
	int m, n;
	printf("Enter m elements of array 1:\n");
	scanf_s("%d", &m);
	printf("Enter n elements of array 2:\n");
	scanf_s("%d", &n);
	int *arr1 = (int *)calloc(m, sizeof(int));
	printf("\nArray 1 :");
	for (int i = 0; i < m; i++)
	{
		arr1[i] = rand() % 21 - 10;
		printf("%4d", arr1[i]);
	}
	int *arr2 = (int *)calloc(n, sizeof(int));
	printf("\nArray 2 :");
	for (int i = 0; i < n; i++)
	{
		arr2[i] = rand() % 21 - 10;
		printf("%4d", arr2[i]);
	}
}

void task_sidorovich2()
{
	int n, m;
	printf("Enter m - number of strings & n - number of elements of string:\n");
	scanf_s("%d%d", &m, &n);
	n++;
	char **str = (char **)calloc(m, sizeof(char *));
	rewind(stdin);
	for (int i = 0; i < m; i++)
	{
		str[i] = (char *)calloc(n, sizeof(char));
		fgets(str[i], n, stdin);
		rewind(stdin);
	}
	char *string = (char *)calloc(n, sizeof(char));
	printf("Enter string:\n");
	fgets(string, n, stdin);

	printf("Text:\n");
	for (int i = 0; i < m; i++)
	{
		printf("%s", str[i]);
		printf("\n");
	}
	printf("Your string:\n");
	printf("%s\n", string);

	int counter = 0;
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
			if (str[i][j] == string[j])
				counter++;
		if (counter == n)
		{
			int k = i;
			free(str[k]);
			for (k; k < m - 1; k++)
			{
				str[k] = realloc(str[k + 1], n * sizeof(char));
			}
			m--;
			str = (char **)realloc(str, m * sizeof(char *));
			i--;
		}
		counter = 0;
	}

	printf("\nResult:\n");
	for (int i = 0; i < m; i++)
	{
		printf("%s", str[i]);
		printf("\n");
	}
}

int modul(int a)
{
	if (a < 0)
		return (-1 * a);
	else
		return a;
}

void task1()
{
	int **mat, n, m;
	scanf_s("%d %d", &n, &m);
	mat = (int **)calloc(n, sizeof(int *));
	for (int i = 0; i < n; i++)
	{
		mat[i] = (int *)calloc(m, sizeof(int));
	}

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			mat[i][j] = rand() % 201 - 100;
			printf("%4d", mat[i][j]);
		}
		printf("\n");
	}
	int max = mat[0][0], temp;
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m, (i + j) < n; j++)
		{
			temp = modul(mat[i][j]);
			if (temp > max)
				max = mat[i][j];
		}
	}
	printf("%d\n", max);
}

void task2()
{
	int n, m, counter, entering = 0;
	scanf_s("%d %d", &n, &m);
	char *string1 = (char *)calloc(n, sizeof(char));
	printf("Enter string:\n");
	fgets(string1, n, stdin);
	char *string2 = (char *)calloc(m, sizeof(char));
	printf("Enter string:\n");
	fgets(string2, m, stdin);
	for (int i = 0; i < n; i++)
	{
		if (string1[i] == string2[0])
		{
			counter = 0;
			for (int j = 0; j < m - 1; j++)
			{
				if (string1[i + j] == string2[j])
					counter++;
			}
			if (counter == m - 1)
				entering++;
		}
	}
}

void merge_sort(int *a, int l, int r)
{
	int m = (l + r) / 2;
	if (l == r)
		return;
	merge_sort(a, l, m);
	merge_sort(a, m + 1, r);
	int i = l, j = m + 1, k = 0;
	int *b;
	b = (int *)malloc((r - l + 1) * sizeof(int));
	while (i <= m && j <= r)
	{
		if (a[j] < a[i])
		{
			b[k] = a[j];
			j++;
			k++;
		}
		else
		{
			b[k] = a[i];
			i++;
			k++;
		}
	}
	for (; i <= m; i++, k++)
		b[k] = a[i];
	for (; j <= r; j++, k++)
		b[k] = a[j];
	for (i = l, k = 0; i <= r; i++, k++)
		a[i] = b[k];
}

void quick_sort(int *a, int l, int r)
{
	int i = l, j = r, temp, sr = a[(l + r) / 2];
	do
	{
		while (a[i] < sr)
			i++;
		while (a[j] > sr)
			j--;
		if (i <= j)
		{
			temp = a[j];
			a[j] = a[i];
			a[i] = temp;
			i++;
			j--;
		}
	} while (i <= j);
	if (i < r)
		quick_sort(a, i, r);
	if (j > l)
		quick_sort(a, l, j);
}

void task401()
{
	int *mat, m, k;
	printf("Enter number of elements: \n");
	scanf_s("%d", &m);
	mat = (int *)calloc(m, sizeof(int));
	for (int i = 0; i < m; i++)
	{
		mat[i] = rand() % 201;
		printf("%4d", mat[i]);
	}
	printf("\n");

	printf("Enter k: \n");
	scanf_s("%d", &k);
	int temp, index_start, index_end, index, index_temp, j = 0;

	j = m - 1;
	while ((mat[j] % 2) == 1 && j >= 0)
	{
		j--;
	}
	index_end = j;
	j = 0;
	while ((mat[j] % 2) == 1 && j < m)
	{
		j++;
	}
	index_start = j;
	j++;
	if (index_start != index_end)
	{
		for (int i = 0; i < k; i++)
		{

			printf("\n\n");
			temp = mat[index_start];
			while (j <= index_end)
			{
				if ((mat[j] % 2) == 0)
				{
					index_temp = j;
					do
					{
						j--;
					} while (j >= index_start && (mat[j] % 2) == 1);
					mat[j] = mat[index_temp];
					j = index_temp + 1;
				}
				else
					j++;
			}
			mat[index_end] = temp;

			printf("Iteration %d\n", i);
			for (int i = 0; i < m; i++)
			{
				printf("%4d", mat[i]);
			}
			j = index_start + 1;
		}
	}
}

void task402()
{
	int n, m;
	printf("Enter m - number of strings & n - number of elements of string:\n");
	scanf_s("%d%d", &m, &n);
	n++;
	char **str = (char **)calloc(m, sizeof(char *));
	rewind(stdin);
	for (int i = 0; i < m; i++)
	{
		str[i] = (char *)calloc(n, sizeof(char));
		fgets(str[i], n, stdin);
		rewind(stdin);
	}
	char *string = (char *)calloc(n, sizeof(char));
}

void reversing_part(char *string, int start, int end)
{
	char buff;
	for (int i = start, j = 0; i < ((end + start) * 0.5 + 0.5); i++, j++)
	{

		buff = string[i];
		string[i] = string[end - j];
		string[end - j] = buff;
	}
}
int letter(char x)
{
	if ((x >= 'a' && x <= 'z') || (x >= 'A' && x <= 'Z'))
		return 1;
	return 0;
}
void n_m(char *string, int n, int m)
{
	int starts_count = 0;
	int ends_count = 0;

	int n_end = 0;
	int n_start = 0;
	int m_start = 0;
	int m_end = 0;

	for (int i = 0; i < '\n'; i++)
	{
		if ((i == 0 || (!letter(string[i - 1]) && i - 1 >= 0) && letter(string[i]))) // start
		{
			starts_count++;
			if (starts_count == n)
			{
				n_start = i;
			}
			if (starts_count == m)
			{
				m_start = i;
			}
		}
		if (!letter(string[i]) && letter(string[i - 1])) // word end
		{
			ends_count++;
			if (ends_count == m)
			{
				m_end = i - 1;
			}
			if (ends_count == n)
			{
				n_end = i - 1;
			}
		}
	}
	reversing_part(string, n_start, m_end);
	reversing_part(string, n_start, n_end);
	reversing_part(string, m_start, m_end);
	reversing_part(string, n_end + 1, m_start - 1);
}

int main()
{
	// srand((int)time(NULL));
	task401();
}