#include "functions.h"

int cmp_last_name(char* i_name, char* j_name)
{
    for(int i = 0; i_name[i] != ' ' && i_name[i] != '\n'; i++)
    {
        if(i_name[i] != j_name[i] && i_name[i] - 32 != j_name[i] && i_name[i] != j_name[i] - 32) return 0;
    }
    return 1;
}

int sort_condition(school bubble, school pj, char choose)
{
    if(choose == '1') return (pj.id > bubble.id);
    else if(choose == '2') return(strcmp(pj.full_name, bubble.full_name) > 0);
    else if(choose == '3') return(pj.age > bubble.age);
    else if(choose == '4') return(pj.gpa > bubble.gpa);
    else if(choose == '5') return(strcmp(pj.address, bubble.address) > 0);
    else if(choose == '6') return(pj.telephone > bubble.telephone);
}
void insertion_sort(school* p, int size, char choose)
{
    for(int m = 0; m < size; m++)
    {
        int j;
        school bubble = p[m];
        for(j = m - 1; j >= 0 && sort_condition(bubble, p[j], choose); j--)
        {
            p[j + 1] = p[j];
        }
        p[j + 1] = bubble;
    }
}

void save(school* p, int size)
{
    FILE* fp = fopen("bd_data.bin", "wb");
    if(size != 0)
    {
        fwrite(&size, sizeof(int), 1, fp);
        fwrite(p, sizeof(school), size, fp);
    }
    fclose(fp);
}
void load(school** p, int *size)
{

    FILE* fp = fopen("bd_data.bin", "rb");
    fread(&(*size), sizeof(int), 1, fp);
    (*p) = realloc((*p), (*size) * sizeof(school));
    fread((*p), sizeof(school), (*size), fp);
    fclose(fp);
}
void load_previously_entered_data(school** p, int* size)
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

    if(load_choose == '1')
    {
        FILE* fp = fopen("bd_data.bin", "rb");
        fseek(fp, 0, SEEK_END);
        if(ftell(fp) == 0)
        {
            printf(" * There are no previously entered data\n");
            fclose(fp);
        }
        else
        {
            fclose(fp);
            load(&(*p), &(*size));
        }
    }
}