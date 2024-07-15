#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv, char **envp)
{

    if (argc != 3)
    {
        printf("Error! Option is not provided\n");
        exit(1);
    }
    char *name = argv[0];
    char *filename = argv[1];
    char *opt = argv[2];

    printf("Option : %s\nName : %s\nPid = %d\nPpid = %d\n", opt, name, getpid(), getppid());

    FILE *file = fopen(filename, "r");
    while (!feof(file))
    {
        char str[256] = {0};
        fscanf(file, "%s", str);
        if (strlen(str) == 0)
            continue;
        printf("%s=%s\n", str, getenv(str));
    }
    fclose(file);
}