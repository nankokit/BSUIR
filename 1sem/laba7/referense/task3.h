#include <stdlib.h>
#include <stdio.h>
#include "кьюсорт.h"

int maxLength(char* str)
{
    int len = lengthStr(str);
    if (len == (-1))
    {
        return (-1);
    }
    
    int j = 0;
    int check = 0, maxLen = 0, ans = 0;
    while ((str[j] <= 122 && str[j] >= 97) || (str[j] <= 90 && str[j] >= 65))
    {
        maxLen++;
        j++;
    }

    if (maxLen > ans)
    {
        ans = maxLen;
    }
    
    maxLen = 0;

    for (int i = 0; i < len - 1; i++)
    {
        check = 0;

        if ((str[i] > 122 || (str[i] < 97 && str[i] > 90) || str[i] < 65))  
        {
            j = i + 1;
            while ((str[j] <= 122 && str[j] >= 97) || (str[j] <= 90 && str[j] >= 65))
            {
                j++;
                maxLen++;
            }
            if (maxLen > ans)
            {
                ans = maxLen;
            }
            maxLen = 0;
        }
    }     
    return ans;
}

void task3()
{
    int size = 0;
    char** str;
    int* arrSize;

    printf("Введите кол-во строк: ");
    size = rigthInput();
    rewind(stdin);

    arrSize = (int*)calloc(size, sizeof(int));
    str = (char**)calloc(size, sizeof(char*));
    for (int i = 0; i < size; i++)
    {
        str[i] = (char*)calloc(255,1);
    }

    for (int i = 0; i < size; i++)
    {
        rewind(stdin);
        fgets(str[i],255,stdin);
        arrSize[i] = maxLength(str[i]);
        if (arrSize[i] == 254)
        {
            printf("To big di.....");
            return;
        }
    }
    
    printf("Выведенные строки:\n"); 

    for (int i = 0; i < size; i++)
    {
        printf("%d   ", arrSize[i]);
        printf("%s", str[i]);           
    }

    printf("\n");

    quickSort(arrSize, str, 0, size - 1);

    printf("Отсортированные строки:\n"); 

    int j = 0;

    for (int i = size - 1; i >= 0; i--)
    {
        //arrSize[i] = maxLength(str[i]);
        printf("%d   ", arrSize[i]);
        printf("%s", str[i]);
        printf("\n");  
    }
    printf("\n");
}