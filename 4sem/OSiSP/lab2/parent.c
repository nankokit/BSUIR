#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <limits.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <unistd.h>
#include <locale.h>

extern char **environ;
int childId = 0;

void sort(char **str)
{
    int count = 0;
    while (str[count++])
        ;

    count--;
    for (int i = 0; i < count; i++)
        for (int j = i + 1; j < count; j++)
            if (strcoll(str[i], str[j]) > 0)
            {
                char *temp = str[i];
                str[i] = str[j];
                str[j] = temp;
            }
}

void child(char *childPath, char *fileWithEnvList, char opt)
{
    if (!childPath)
    {
        printf("Child path not set\n");
        return;
    }
    char childName[256];
    sprintf(childName, "child_%02d", childId++);
    char fullPath[256];
    sprintf(fullPath, "%s/child", childPath);
    int pid = fork();
    if (pid == -1)
    {
        printf("Error forking: %s\n", strerror(errno));
        return;
    }
    if (pid == 0)
    {
        char *args[] = {childName, fileWithEnvList, &opt, NULL};
        execve(fullPath, args, environ);
    }
}

char *getEnvVal(char **environ, char *variable)
{
    int lenght = strlen(variable);
    for (int i = 0; environ[i] != NULL; i++)
        if (strncmp(environ[i], variable, lenght) == 0 && environ[i][lenght] == '=')
            return &environ[i][lenght + 1];
    return NULL;
}

int main(int argc, char **argv, char **envp)
{
    setenv("LC_COLLATE", "C", 1);
    if (argc != 2)
    {
        printf("Error! File is not provided\n");
        exit(1);
    }
    printf("Start programm\n");

    sort(envp);
    int count = 0;
    while (envp[count])
        printf("%s\n", envp[count++]);

    char c;
    printf("Enter '+' or '*' or '&' :\n");
    while ((c = getchar()) != EOF)
    {
        switch (c)
        {
        case '+':
            child(getenv("CHILD_PATH"), argv[1], c);
            break;
        case '*':
            child(getEnvVal(envp, "CHILD_PATH"), argv[1], c);
            break;
        case '&':
            child(getEnvVal(environ, "CHILD_PATH"), argv[1], c);
            break;
        case 'q':
            exit(0);
        case '\n':
            continue;
        default:
            printf("Unknown option\n");
            break;
        }
    }
    return 0;
}