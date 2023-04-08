#include "funcs.h"
// #include <stdio.h>
// #include <mysql.h>
// #include <stdlib.h>

enum answer
{
    NO,
    YES
};

int main()
{
    srand(time(NULL));
    int count = 0;
    ITCOMPANY *exemplar = (ITCOMPANY *)malloc(1 * sizeof(ITCOMPANY));

    printf("Do you want to add data from bin file? (1 - YES/0 - NO) \n");
    int answ = input_int();
    if (answ == YES)
    {
        // count = struct_count_file();
        // ITCOMPANY *exemplar = malloc(count * sizeof(ITCOMPANY));
        printf("Enter file name: \n");
        // char fname[40];
        // input_string(fname);

        char fname[] = "loaded_data.bin";
        // printf("%s", fname);
        printf("\nData from file: \n");
        load(fname, &exemplar, &count);
        for (int i = 0; i < count; i++)
        {
            print_exemplar(&exemplar[i]);
        }
    }

    int active = 1;

    while (active)
    {
        printf("Select an action: \n");
        printf("1 - add a new record \n");
        printf("2 - information output\n");
        printf("3 - modification of an existing record\n");
        printf("4 - deletion of an existing record\n");
        printf("5 - sorted output(by selected fields)\n");
        printf("6 - find and output deadlines\n");
        printf("7 - find and output ready-made projects with entered cost\n");
        printf("8 - save all information in a bin file and exit\n");
        printf("9 - exit\n");
        int task = input_int();
        // int task;
        // scanf("%d", &task);
        // int task = 2;
        switch (task)
        {
        case 1:
        {
            ITCOMPANY temp;
            new_record(&temp);
            exemplar = (ITCOMPANY *)realloc(exemplar, (count + 1) * sizeof(ITCOMPANY));
            exemplar[count] = temp;
            print_exemplar(&exemplar[count]);
            count++;
            break;
        }
        case 2:
        {
            printf("_____________________________________________________\n");
            printf("| Project name |  ID  | deadline | cost |  status  |\n");
            printf("_____________________________________________________\n");
            for (int i = 0; i < count; i++)
            {
                print_exemplar(&exemplar[i]);
            }
            printf("\n");
            break;
        }
        case 3:
        {
            printf("Select project number\n");
            for (int i = 0; i < count; i++)
            {
                printf("%d - %s\n", i + 1, exemplar[i].name);
            }
            int projnum = input_int() - 1;
            printf("Select record field:\n");
            printf("1 - all\n2 - name\n3 - data deadline\n4 - cost\n5 - status\n");
            int num = input_int();
            switch (num)
            {
            case 1:
            {
                new_record(&exemplar[projnum]);
                break;
            }
            case 2:
            {
                record_name(&exemplar[projnum]);
                break;
            }
            case 3:
            {
                record_day_deadline(&exemplar[projnum]);
                record_month_deadline(&exemplar[projnum]);
                record_year_deadline(&exemplar[projnum]);
                break;
            }
            case 4:
            {
                record_cost(&exemplar[projnum]);
                break;
            }
            case 5:
            {
                record_status(&exemplar[projnum]);
                break;
            }
            default:
            {
                printf("Incorrect input. Try again. \n\n");
                break;
            }
            }
            break;
        }
        case 4:
        {
            printf("Select project number\n");
            for (int i = 0; i < count; i++)
            {
                printf("%d - %s\n", i + 1, exemplar[i].name);
            }
            int projnum = input_int() - 1;

            for (projnum; projnum < count - 1; projnum++)
                exemplar[projnum] = exemplar[projnum + 1];
            count--;
            break;
        }
        case 5:
        {
            // printf("Enter 1 critery:\n"
            //        "1 - id\n2 - name\n"
            //        "3 - deadline\n4 - cost\n5 - status\n");
            // int critery1 = input_int();
            // printf("Enter 2 critery:\n"
            //        "1 - id\n2 - name\n"
            //        "3 - deadline\n4 - cost\n5 - status\n");
            // int critery2 = input_int();

            // int critery = input_int();
            // sorting_first(exemplar, count, critery);
            int count_conditions;
            do
            {
                printf("Enter number of criterys:\n");
                count_conditions = input_int();
            } while (count_conditions < 0 || count_conditions > 5);

            int *condition = calloc(count_conditions, sizeof(int));
            for (int i = 0; i < count_conditions; i++)
            {
                printf("Enter %d critery:\n1 - id\n2 - name\n3 - deadline\n4 - cost\n5 - status\n", i + 1);
                condition[i] = input_int();
            }

            if (count_conditions == 1 || count_conditions == 2)
                sorting_second(exemplar, count, condition, count_conditions);
            if (count_conditions == 3)
                sorting_third(exemplar, count, condition, count_conditions);

            for (int i = 0; i < count; i++)
            {
                print_exemplar(&exemplar[i]);
            }
            printf("\n");
            break;
        }
        case 6:
        {
            for (int i = 0; i < count; i++)
            {
                if (find_deadline(exemplar[i].year_deadline, exemplar[i].month_deadline, exemplar[i].day_deadline) && exemplar[i].status == 0)
                {
                    printf("Deadline %d:\n", i + 1);
                    print_exemplar(&exemplar[i]);
                }
            }

            break;
        }
        case 7:
        {
            printf("Enter min cost:\n");
            int temp = input_int();
            while (temp <= 0)
            {
                printf("Incorrect. try again \n");
                temp = input_int();
            }
            for (int i = 0; i < count; i++)
            {
                if (exemplar[i].cost >= temp && exemplar[i].status == 1)
                    print_exemplar(&exemplar[i]);
            }
            break;
        }
        case 8:
        {
            printf("Enter file name: \n");
            char fname[40];
            input_string(fname);
            save(fname, exemplar, count);
            if (count != 0)
                free(exemplar);
            active = 0;
            break;
        }
        case 9:
        {
            active = 0;
            break;
        }

        default:
            printf("Incorrect input. Try again. \n\n");
            break;
        }
    }
}

/*

void input_file_name(int *count)
{
    char *str = malloc(sizeof(char));
    input_string(str);
}

*/
// ITCOMPANY new1 = {1000, "Project", 3, 8, 2023, 2500, 0};
// exemplar[0] = new1;
// ITCOMPANY new2 = {1001, "Project", 3, 8, 2023, 6000, 1};
// exemplar[1] = new2;
// ITCOMPANY new3 = {1002, "Project1", 15, 10, 2022, 4500, 1};
// exemplar[2] = new3;
// ITCOMPANY new4 = {1003, "Project", 30, 3, 2024, 8030, 0};
// exemplar[3] = new4;
// ITCOMPANY new5 = {1004, "Project", 1, 5, 2021, 1250, 0};
// exemplar[4] = new5;
// ITCOMPANY new6 = {1005, "Project", 1, 5, 2021, 1300, 0};
// exemplar[5] = new6;
// ITCOMPANY new7 = {1006, "Project1", 1, 5, 2021, 1400, 0};
// exemplar[6] = new7;
// ITCOMPANY new8 = {1007, "Project", 1, 5, 2021, 1250, 0};
// exemplar[7] = new8;

// exemplar = (ITCOMPANY *)realloc(exemplar, count * sizeof(ITCOMPANY));