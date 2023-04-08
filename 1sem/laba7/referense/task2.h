#include <stdlib.h>
#include <stdio.h>

void task2(int n1, char* str1, int n2, char* str2)
{
    char* ans;
    ans = (char*)calloc(n1 + n2 + 1,1);

    int number = 0;
    int len1 = lengthStr(str1);
    int len2 = lengthStr(str2);
    if (len1 == (-1) || len2 == (-1))
    {
        printf("Ошибка!!!!\n");
        return;
    }
    
    if (len1 > n1)
    {
        for (int i = 0; i < n1; i++)
        {
            ans[i] = str1[i];
        }   
    }
    else
    {
        printf("Невозмжно\n");
        return;
    }
    if (len2 > n2)
    {
        for (int i = 0; i < n2 + 1; i++)
        {
            ans[i + n1] = str2[len2 - n2 + i - 1];
        }      
    }
    else
    {
        printf("Невозможно\n");
        return;
    }
    printf("ans = %s\n", ans);
}