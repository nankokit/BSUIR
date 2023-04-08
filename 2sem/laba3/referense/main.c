#include "struct_functions.h"
#include "functions.h"

int main()
{
    int size = 0;
    school* p = malloc(size * sizeof(school));

    load_previously_entered_data(&p, &size);

    while(1)
    {
        printf("|----------------------------------|\n"
               "| 1. Add pupil                     |\n"
               "| 2. Print all pupils              |\n"
               "| 3. Update information about pupil|\n"
               "| 4. Remove pupil                  |\n"
               "| 5. Sort pupils                   |\n"
               "| 6. Find retarded pupil(-s)       |\n"
               "| 7. Find students from one family |\n"
               "| 8. Quit                          |\n"
               "|----------------------------------|\n");
        char choose;
        while (!scanf("%c", &choose) || choose > '8' || choose < '1' || getchar() != '\n') {
            printf("invalid input\n");
            rewind(stdin);
        }

        if      (choose == ADD)
        {
            p = realloc(p, (++size) * sizeof(school) );
            add(p, size);
            printf(" * New pupil has been added * \n");
        }
        else if (choose == PRINT)
        {
            if(size == 0) printf(" * List of pupils is clear \n");
            for (int i = 0; i < size; i++) print(p, i);
        }
        else if (choose == UPDATE) update(p, size);
        else if (choose == KILL) kill(p, &size);
        else if (choose == SORT) sort(p, size);
        else if (choose == RETARDED) retarded(p, size);
        else if (choose == ONE_FAMILY) one_family(p, size);
        else if (choose == QUIT)
        {
            save(p, size);
            if(size != 0) free(p);
            return 0;
        }
    }
}
