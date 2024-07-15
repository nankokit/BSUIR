#define _XOPEN_SOURCE
#define _GNU_SOURCE
#include <memory.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <stddef.h>
#include <sys/types.h>

bool silent = false;

struct Variables
{
    int a, b;
} variables = {0};

struct Statistic
{
    int zero_zero, zero_one, one_zero, one_one;
} statistic = {0};

void reset()
{
    memset(&variables, 0, sizeof(variables));
    memset(&statistic, 0, sizeof(statistic));
}

void collect()
{
    struct Variables vars = variables;
    if (vars.a && vars.b)
        statistic.one_one++;
    if (vars.a && !vars.b)
        statistic.one_zero++;
    if (!vars.a && vars.b)
        statistic.zero_one++;
    if (!vars.a && !vars.b)
        statistic.zero_zero++;
}

static void startTimer(void (*handler)(), int usec)
{
    timer_t timer = NULL;
    struct sigevent sev = {
        .sigev_notify = SIGEV_THREAD,
        .sigev_notify_function = handler,
        .sigev_value = {.sival_int = 0}};
    timer_create(CLOCK_REALTIME, &sev, &timer);

    struct itimerspec its = {
        .it_value.tv_sec = usec / 1000000,
        .it_value.tv_nsec = usec % 1000000,
        .it_interval.tv_sec = usec / 1000000,
        .it_interval.tv_nsec = usec % 1000000};

    timer_settime(timer, 0, &its, NULL);
}

void usr1() { silent = true; }
void usr2() { silent = false; }

int main()
{

    struct sigaction sa1 = {
        .sa_flags = 0,
        .sa_handler = usr1};
    sigaction(SIGUSR1, &sa1, NULL);
    struct sigaction sa2 = {
        .sa_flags = 0,
        .sa_handler = usr2};
    sigaction(SIGUSR2, &sa2, NULL);

    startTimer(collect, 1000);

    while (true)
    {
        reset();
        for (int i = 0; i < 100000000; i++)
        {
            variables.a = 0;
            variables.b = 0;
            variables.a = 1;
            variables.b = 1;
        }
        if (!silent)
        {
            struct Statistic stat = statistic;
            printf(
                "PPID:%d PID:%5d ZZ:%4d ZO:%4d OZ:%4d OO:%4d\n",
                getppid(),
                getpid(),
                stat.zero_zero,
                stat.zero_one,
                stat.one_zero,
                stat.one_one);
        }
    }
    return 0;
}
