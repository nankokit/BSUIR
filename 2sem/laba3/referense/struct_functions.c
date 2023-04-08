#include "struct_functions.h"
#include "functions.h"
void add(school* p, int size)
{
//id
    p[size - 1].id = time(NULL);
//full name
    printf(" * Enter full name of this child (to 63 symbols) (Please, write in correct form):");
    fgets(p[size - 1].full_name, 64, stdin);
    rewind(stdin);
//age
    printf(" * Enter age of this child (from 0 to to 100):");
    while (!scanf("%d", &p[size - 1].age) || p[size - 1].age > 100 || p[size - 1].age < 0 || getchar() != '\n')
    {
        printf("invalid input\n");
        rewind(stdin);
    }
//gpa
    printf(" * Enter gpa of this child (from 0 to to 10):");
    while (!scanf("%f", &p[size - 1].gpa) || p[size - 1].gpa > 10 || p[size - 1].gpa < 0 || getchar() != '\n')
    {
        printf("invalid input\n");
        rewind(stdin);
    }
//char* address
    printf(" * Enter address of this child (to 63 symbols):");
    fgets(p[size - 1].address, 64, stdin);
    rewind(stdin);
//telephone
    printf(" * Enter telephone of this child:");
    while (!scanf("%d", &p[size - 1].telephone) || p[size - 1].telephone < 1 || getchar() != '\n')
    {
        printf("invalid input\n");
        rewind(stdin);
    }
}
void print(school* p, int i)
{
        printf("    |------------------------------\n"
               "    | 1. id: %d\n"
               "    | 2. full name: %s"
               "    | 3. age: %d\n"
               "    | 4. gpa: %f\n"
               "    | 5. address: %s"
               "    | 6. telephone: %d\n"
               "    |------------------------------\n", p[i].id, p[i].full_name, p[i].age,
               p[i].gpa, p[i].address, p[i].telephone);
}
void update(school* p, int size)
{
    unsigned short int id;
    printf(" * Enter id of this pupil: ");
    while (!scanf("%d", &id) || id > 65535 || id < 0 || getchar() != '\n')
    {
        printf("invalid input\n");
        rewind(stdin);
    }
    if (size == 0) printf("\n * There no one pupil with id.%d\n", id);
    for(int i = 0; i < size; i++)
    {
        if(id == p[i].id)
        {
            add(p, i+1);
            printf("\n * All info about this pupil has been updated.\n");
            break;
        }
        else printf("\n * There no one pupil with id.%d\n", id);

    }

}
void kill(school* p, int* size)
{
    unsigned short int id;
    int kill_num = -1;
    printf("\n * Enter id of this pupil: ");
    while (!scanf("%d", &id) || getchar() != '\n')
    {
        printf("invalid input\n");
        rewind(stdin);
    }

    for(int i = 0; i < (*size); i++)
        if(id == p[i].id) kill_num = i;

    if(kill_num == -1) printf(" * There no one pupil with id.%d\n", id);
    else
    {
        for (int i = kill_num; i < (*size) - 1; i++)
            p[i] = p[i + 1];

        (*size)--;
        realloc(p, (*size) * sizeof(school));
        printf("\n * This pupil has been removed \n");
        fflush(stdin);
    }
}
void sort(school* p, int size)
{
    if(size < 2) printf(" * too few pupils. Please, try next time\n");
    else
    {
        printf("    |By-what-parameter-do-you-want-to-sort?\n"
               "    | 1. id         \n"
               "    | 2. full name  \n"
               "    | 3. age        \n"
               "    | 4. gpa        \n"
               "    | 5. address    \n"
               "    | 6. telephone  \n"
               "    |------------------------------\n");
        char choose;
        while (!scanf("%c", &choose) || choose < '1' || choose > '6' || getchar() != '\n')
        {
            printf("invalid input\n");
            rewind(stdin);
        }
        insertion_sort(p, size, choose);

        printf(" * Sorting was successful\n");
    }
}

void retarded(school* p, int size)
{
    printf("\n * Enter minimum of normal gpa :");
    float min_gpa;
    while (!scanf("%f", &min_gpa) || min_gpa < 0 || min_gpa > 10)
    {
        printf("invalid input\n");
        rewind(stdin);
    }
    for(int i = 0; i < size; i++)
        if(p[i].gpa < min_gpa)
            print(p, i);

}
void one_family(school* p, int size)
{
    if(size < 2)
    {
        printf(" * There are no 2+ pupils added\n");
    }
    else
    {
        int check_one_family = 0;
        for (int i = 0; i < size - 1; i++)
        {
            for (int j = i + 1; j < size; j++)
            {
                if (cmp_last_name(p[i].full_name, p[j].full_name))
                {
                    check_one_family = 1;
                    printf(" * id%d and id%d are from one family\n", p[i].id, p[j].id);
                }
            }
        }
        if (check_one_family == 0) printf(" * There are no pupils from one family\n");
    }
}