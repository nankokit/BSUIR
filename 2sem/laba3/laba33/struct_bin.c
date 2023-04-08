#include "funcs.h"

int main()
{
    char fname[] = "loaded_data.bin";
    ITCOMPANY new[] = {{1000, "Project1", 3, 8, 2023, 2500, 0},
                       {1001, "Project2", 3, 8, 2023, 6000, 1},
                       {1002, "Project3", 15, 10, 2022, 4500, 1},
                       {1003, "Project4", 30, 3, 2024, 8030, 0},
                       {1004, "Project5", 1, 5, 2021, 1250, 0},
                       {1005, "Project6", 1, 5, 2021, 1300, 1},
                       {1006, "Project7", 1, 5, 2021, 1400, 0},
                       {1007, "Project8", 1, 5, 2021, 1250, 0}};

    for (int i = 0; i < 8; i++)
    {
        save(fname, new, 8);
    }
    int count = struct_count_file();
    ITCOMPANY *exemplar = malloc(count * sizeof(ITCOMPANY));

    printf("\nThis data has been stored in a file: \n");
    load(fname, &exemplar, &count);
    for (int i = 0; i < count; i++)
    {
        print_exemplar(&exemplar[i]);
    }
}

/*

void add_from_file(ITCOMPANY *exemplar, int *count)
{
    (*count) = struct_count_file();
    exemplar = (ITCOMPANY *)realloc(exemplar, (*count) * sizeof(ITCOMPANY));
    load(&exemplar, &(*count));
}


void load_previously_entered_data(ITCOMPANY **exemplar, int *size)
{
    printf("\n * Do you want to load previously entered data? (enter your decision)\n"
           " * 1. yes\n"
           " * 2. no\n");

    char load_choose;
    while (!scanf("%c", &load_choose) || load_choose > '2' || load_choose < '1' || getchar() != '\n')
    {
        printf("invalid input\n");
        rewind(stdin);
    }

    if (load_choose == '1')
    {
        FILE *fp = fopen("bd_data.bin", "rb");
        fseek(fp, 0, SEEK_END);
        if (ftell(fp) == 0)
        {
            printf(" * There are no previously entered data\n");
            fclose(fp);
        }
        else
        {
            fclose(fp);
            load(&(*exemplar), (&(*size)));
        }
    }
}

*/

// void save(ITCOMPANY *exemplar, int size)
// {
//     FILE *fp = fopen("new_bd.bin", "wb");
//     if (size != 0)
//     {
//         fwrite(&size, sizeof(int), 1, fp);
//         fwrite(exemplar, sizeof(ITCOMPANY), size, fp);
//     }
//     fclose(fp);
// }

// int load_size()
// {
//     FILE *fp = fopen("new_bd.bin", "rb");
//     int temp;
//     fread(&temp, sizeof(int), 1, fp);
//     fclose(fp);
//     return temp;
// }

// void load(ITCOMPANY **exemplar, int *size)
// {
//     FILE *fp = fopen("new_bd.bin", "rb");
//     fread(&(*size), sizeof(int), 1, fp);
//     (*exemplar) = realloc((*exemplar), (*size) * sizeof(ITCOMPANY));
//     fread((*exemplar), sizeof(ITCOMPANY), (*size), fp);
//     fclose(fp);
// }
