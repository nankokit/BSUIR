void swap(int* n1, int* n2)
{
    int* swap = *n1;
    *n1 = *n2;
    *n2 = swap;
}

void charSwap(char** c1, char** c2)
{
	char* swap = *c1;
	*c1 = *c2;
	*c2 = swap;
}

int partition(int* list, char** str, int start, int pivot)
{
    int i = start;
    while(i < pivot)
	{
		if(list[i] > list[pivot] && i == pivot-1)
		{
			swap(&list[i], &list[pivot]);
            charSwap(&str[i], &str[pivot]);
			pivot--;
		}
		
		else if(list[i] > list[pivot])
		{
			swap(&list[pivot - 1], &list[pivot]);
			swap(&list[i], &list[pivot]);

            charSwap(&str[pivot - 1], &str[pivot]);
			charSwap(&str[i], &str[pivot]);

			pivot--;
		}
		else i++;
	}
	return pivot;
}

void quickSort(int* list, char** str, int start, int end)
{
	if(start < end)
	{
		int pivot = partition(list, str, start, end);
		quickSort(list, str, start, pivot - 1);
		quickSort(list, str, pivot + 1, end);
	}
}