#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include "funckions.h"
#include "functionss.h"
#include "task1.h"
#include "task2.h"
#include "task3.h"
#include "проверка.h"

int main()
{
    int taskNumber = 0;
    taskNumber = rigthInput();
    while (taskNumber)
    {
        printf("Enter task number (4 - exit): \n");
        taskNumber = check_int_input();
        printf("\n");
        switch (taskNumber)
        {
        case 1:
            printf("Task 1 \n\n");
            task1();
            printf("\n");
            break;

        case 2:
            printf("Task 2 \n\n");
            int n1, n2;
            char *str;
            char *str1;
            char *str2;
            char test[1];
            n1 = rigthInput();
            printf("Введите N2: ");
            n2 = rigthInput();
            rewind(stdin);

            printf("Введите S1: ");
            char *str = (char *)calloc(255, 1);
            fgets(str, 255, stdin);
            if (lengthStr(str) == 254)
            {
                printf("To big di...\n");
                break;
            }

            printf("Введите S2: ");
            str2 = (char *)calloc(255, 1);
            fgets(str2, 255, stdin);
            rewind(stdin);
            if (lengthStr(str) == 254)
            {
                printf("To big di...\n");
                break;
            }
            else
            {
                task2(n1, str, n2, str2);
            }
            break;

        case 3:
            printf("Task 2 \n");
            printf("\n");
            task3();
            printf("\n");
            break;

        case 4:
            printf("Exit\n");
            exit(0);
            break;
        default:
            printf("Incorrect. Try again. \n");
            printf("\n");
        }
    }
    return 0;
}
