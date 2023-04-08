#include "func_4.h"
/*
IT компания. Содержаться данные о проектах — id, название, дата дедлайна, стоимость
заказа, статус.
+ Усложняется статус проекта: 1 — количество закрепленных сотрудников, дата окончания
поддержки продукта; 2 — оставшиеся этапы разработки, заказчик.

Реализовать операцию взятия остатка от деления на число используя битовые поля.
Число передается через командную строку.
Реализовать функции поиска и сортировки информации по одному из новых свойств.

6-8 Использование встроенных/самописных динамических структур данных. Включая
функции сортировки, поиска и тд.

9-10 Обосновать выбор динамической структуры данных. Создание .log файлов
*/

int main()
{
    LIST *head = NULL;
    int count = 0;
    int active = 1;
    ITCOMPANY arg;
    log_record(STARTING, arg);
    {
        ITCOMPANY pr1 = {1001, "Project1", 3, 8, 2023, 2500, 2, 3};
        ITCOMPANY *temp = &pr1;
        strcpy(temp->status.st2.customer, "Bogdan\0");
        pushBack(&head, pr1, &count);
        ITCOMPANY pr2 = {1002, "Project2", 16, 3, 2022, 6800, 1, 10, 10, 7, 2023};
        pushBack(&head, pr2, &count);
        ITCOMPANY pr3 = {1003, "Project3", 30, 12, 2022, 9800, 1, 2, 1, 8, 2023};
        pushBack(&head, pr3, &count);
        ITCOMPANY pr4 = {1004, "Project4", 12, 7, 2024, 1200, 2, 5};
        temp = &pr4;
        strcpy(temp->status.st2.customer, "Sidorovich\0");
        pushBack(&head, pr4, &count);
    }

    while (active)
    {
        printf("Select an action: \n");
        printf("1 - add a new list item \n");
        printf("2 - modification of an existing record\n");
        printf("3 - deletion of an existing record\n");
        printf("4 - list output\n");
        printf("5 - sorted output\n");
        printf("6 - search\n");
        printf("7 - exit\n");
        int task = input_bord(1, 7);

        switch (task)
        {
        case 1:
        {
            ITCOMPANY exemplar;
            new_record(&exemplar, 1000 + count);
            pushBack(&head, exemplar, &count);
            break;
        }
        case 2:
        {
            modification(head, count);
            break;
        }
        case 3:
        {
            deletion(&head, count);
            break;
        }
        case 4:
        {
            print_list(head);
            break;
        }
        case 5:
        {
            printf("Enter critery:\n1 - id\n2 - name\n3 - deadline\n4 - cost\n5 - count_staff\n6 - date_support\n7 - stage\n8 - customer name\n");
            int condition = input_bord(1, 8);
            head = sort(head, condition);
            print_list(head);
            break;
        }
        case 6:
        {
            printf("Enter search critery:\n1 - id\n2 - name\n3 - deadline\n4 - cost\n5 - count_staff\n6 - date_support\n7 - stage\n8 - customer name\n");
            int condition = input_bord(1, 8);
            search(head, condition);
            break;
        }
        case 7:
        {
            log_record(END, arg);
            active = 0;
            break;
        }

        default:
            printf("Incorrect input. Try again. \n\n");
            break;
        }
    }
    return 0;
}