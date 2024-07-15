#define _XOPEN_SOURCE
#define _GNU_SOURCE
#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <termios.h>
#include <unistd.h>

pid_t *childs = NULL;
int childCount = 0;

bool forkNew()
{
    pid_t child = fork();
    if (child == 0)
    {
        execve("./child", NULL, NULL);
    }
    printf("Fork new: %d\n", child);
    childCount++;
    childs = realloc(childs, sizeof(pid_t) * childCount);
    childs[childCount - 1] = child;
    return true;
}

bool killLast()
{
    if (childCount == 0)
        return true;
    childCount--;
    printf("Kill PID: %d\n", childs[childCount]);
    kill(childs[childCount], SIGKILL);
    waitpid(childs[childCount], NULL, 0);
    return true;
}

bool killAll()
{
    while (childCount)
        killLast();
    free(childs);
    childs = NULL;
    return true;
}

bool list()
{
    system("pstree `pidof parent`");
    return true;
}

bool silent(int id)
{
    if (!(0 <= id && id < childCount))
    {
        printf("Silent: child %d not exists\n", id);
        return true;
    }
    kill(childs[id], SIGUSR1);
    printf("Silent: child %d can't talk\n", id);
    return true;
}

bool silentAll()
{
    for (int i = 0; i < childCount; i++)
        silent(i);
    return true;
}

bool grant(int id)
{
    if (!(0 <= id && id < childCount))
    {
        printf("Grant: child %d not exists\n", id);
        return true;
    }
    kill(childs[id], SIGUSR2);
    printf("Grant: child %d can talk\n", id);
    return true;
}

bool grantAll()
{
    alarm(0);
    for (int i = 0; i < childCount; i++)
        grant(i);
    return true;
}

bool priority(int id)
{
    silentAll();
    grant(id);
    alarm(5);
    return true;
}

bool handleOpt(char opt[256])
{
    if (!strcmp(opt, "+"))
        return forkNew();
    if (!strcmp(opt, "-"))
        return killLast();
    if (!strcmp(opt, "l"))
        return list();
    if (!strcmp(opt, "k"))
        return killAll();
    if (!strcmp(opt, "s"))
        return silentAll();
    if (!strcmp(opt, "g"))
        return grantAll();
    int id = 0;
    if (sscanf(opt, "s%d", &id))
        return silent(id);
    if (sscanf(opt, "g%d", &id))
        return grant(id);
    if (sscanf(opt, "p%d", &id))
        return priority(id);
    if (!strcmp(opt, "q"))
        return false;
    return true;
}

int main()
{
    struct sigaction sa = {
        .sa_flags = 0,
        .sa_handler = grantAll};
    sigaction(SIGALRM, &sa, NULL);

    char opt[256];
    printf("Start program\n");
    while (scanf("%s", opt))
    {
        if (!handleOpt(opt))
            break;
    }
    killAll();
    return 0;
}
