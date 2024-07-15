#define _XOPEN_SOURCE 500
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <malloc.h>
#include <stdint.h>
#include <semaphore.h>
#include <sys/types.h>
#include <time.h>
#include <pthread.h>
#include <fcntl.h>
#include "queue.h"
#include <errno.h>

sem_t *items;
sem_t *free_space;

typedef struct
{
    pthread_t id;
    pthread_cond_t *condvar;
    bool running;
} thread_t;

thread_t *consumers = NULL;
int consumers_cnt = 0;

thread_t *producers = NULL;
int producers_cnt = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

Queue msqueue;

typedef struct message
{
    uint8_t type;
    uint16_t hash;
    uint8_t size;
    uint8_t data[];
} Message;

uint16_t hashCode(int length, char bytes[])
{
    uint16_t res = 0;
    for (int i = 0; i < length; i++)
    {
        res ^= bytes[i];
    }
    return res;
}

Message *randomMessage()
{
    uint8_t size = rand() % 256;
    Message *message = malloc(sizeof(Message) + size);
    *message = (Message){
        .type = rand(),
        .hash = 0,
        .size = size,
    };
    for (int i = 0; i < size; i++)
        message->data[i] = rand();
    message->hash = hashCode(sizeof(Message) + size, (char *)message);
    return message;
}

void *Producer(void *arg)
{
    srand(time(0));
    int number = producers_cnt - 1;
    while (producers[number].running)
    {
        struct timespec wait_time;
        clock_gettime(CLOCK_MONOTONIC, &wait_time);
        wait_time.tv_nsec += 5000000;
        if (sem_timedwait(free_space, &wait_time) == 0 && errno != ETIMEDOUT)
        {
            pthread_mutex_lock(&mutex);
            Message *message = randomMessage();
            push(&msqueue, message);
            printf("----------------------------------------------------\n"
                   "Producer %lu sent message with type %hX and hash %hX\n"
                   "Added messages for all time: %d\n"
                   "----------------------------------------------------\n",
                   pthread_self(), message->type, message->hash, msqueue.added_cnt);
            pthread_mutex_unlock(&mutex);
            sem_post(items);
        }
        pthread_mutex_lock(&mutex);
        struct timespec pause;
        clock_gettime(CLOCK_MONOTONIC, &pause);
        pause.tv_sec += 3;
        pthread_cond_timedwait(producers[number].condvar, &mutex, &pause);
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(NULL);
}

void *Consumer(void *arg)
{
    int number = consumers_cnt - 1;
    while (consumers[number].running)
    {
        struct timespec wait_time;
        clock_gettime(CLOCK_MONOTONIC, &wait_time);
        wait_time.tv_nsec += 5000000;
        if (sem_timedwait(items, &wait_time) == 0 && errno != ETIMEDOUT)
        {
            pthread_mutex_lock(&mutex);
            Message *message = pop(&msqueue);
            printf("----------------------------------------------------\n"
                   "Consumer %lu get message with type %hX and hash %hX\n"
                   "Extracted messages for all time: %d\n"
                   "----------------------------------------------------\n",
                   pthread_self(), message->type, message->hash, msqueue.extracted_cnt);
            pthread_mutex_unlock(&mutex);
            sem_post(free_space);
        }
        pthread_mutex_lock(&mutex);
        struct timespec pause;
        clock_gettime(CLOCK_MONOTONIC, &pause);
        pause.tv_sec += 3;
        pthread_cond_timedwait(consumers[number].condvar, &mutex, &pause);
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(NULL);
}

void create_thread(char type)
{
    pthread_condattr_t condattr;
    pthread_condattr_init(&condattr);
    pthread_condattr_setclock(&condattr, CLOCK_MONOTONIC);
    if (type == 'p')
    {
        producers_cnt++;
        producers = (thread_t *)realloc(producers, sizeof(thread_t) * producers_cnt);
        producers[producers_cnt - 1].running = true;
        producers[producers_cnt - 1].condvar = (pthread_cond_t *)malloc(sizeof(pthread_cond_t));
        pthread_cond_init(producers[producers_cnt - 1].condvar, &condattr);
        pthread_create(&producers[producers_cnt - 1].id, NULL, Producer, NULL);
        printf("New producer is created with ID: %lu\n", producers[producers_cnt - 1].id);
    }
    if (type == 'c')
    {
        consumers_cnt++;
        consumers = (thread_t *)realloc(consumers, sizeof(thread_t) * consumers_cnt);
        consumers[consumers_cnt - 1].running = true;
        consumers[consumers_cnt - 1].condvar = (pthread_cond_t *)malloc(sizeof(pthread_cond_t));
        pthread_cond_init(consumers[consumers_cnt - 1].condvar, &condattr);
        pthread_create(&consumers[consumers_cnt - 1].id, NULL, Consumer, NULL);
        printf("New consumer is created with ID: %lu\n", consumers[consumers_cnt - 1].id);
    }
    pthread_condattr_destroy(&condattr);
}

void kill_last(char type)
{
    if (type == 'p')
    {
        if (producers_cnt > 0)
        {
            producers_cnt--;
            producers[producers_cnt].running = false;
            pthread_cond_signal(producers[producers_cnt].condvar);
            pthread_join(producers[producers_cnt].id, NULL);
            printf("Producer with ID %lu is killed\n", producers[producers_cnt].id);
            pthread_cond_destroy(producers[producers_cnt].condvar);
            producers = (thread_t *)realloc(producers, sizeof(thread_t) * producers_cnt);
        }
    }
    if (type == 'c')
    {
        if (consumers_cnt > 0)
        {
            consumers_cnt--;
            consumers[consumers_cnt].running = false;
            pthread_cond_signal(consumers[consumers_cnt].condvar);
            pthread_join(consumers[consumers_cnt].id, NULL);
            printf("Consumer with ID %lu is killed\n", consumers[consumers_cnt].id);
            pthread_cond_destroy(consumers[consumers_cnt].condvar);
            consumers = (thread_t *)realloc(consumers, sizeof(thread_t) * consumers_cnt);
        }
    }
}

void kill_all(char type)
{
    if (type == 'p')
    {
        while (producers_cnt)
            kill_last(type);
    }
    if (type == 'c')
    {
        while (consumers_cnt)
            kill_last(type);
    }
}

void change_queue_size(char type)
{
    pthread_mutex_lock(&mutex);
    if (type == '+')
        sem_post(free_space);
    else if (type == '-')
    {
        int semvalue;
        sem_getvalue(free_space, &semvalue);
        if (semvalue == 0)
        {
            sem_getvalue(items, &semvalue);
            if (semvalue)
            {
                pop(&msqueue);
                sem_wait(items);
            }
            else
                printf("Queue size is null\n");
        }
        else
            sem_wait(free_space);
    }
    pthread_mutex_unlock(&mutex);
}

int main()
{
    printf("Start program\n");
    msqueue = (Queue){.size = 0,
                      .added_cnt = 0,
                      .extracted_cnt = 0,
                      .head = NULL,
                      .tail = NULL};
    sem_unlink("/items");
    sem_unlink("/free_space");
    items = sem_open("/items", O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR, 0);
    free_space = sem_open("/free_space", O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR, 5);
    char opt[256];
    while (scanf("%s", opt))
    {
        if (!strcmp(opt, "p"))
            create_thread('p');
        if (!strcmp(opt, "c"))
            create_thread('c');
        if (!strcmp(opt, "kp"))
            kill_last('p');
        if (!strcmp(opt, "kc"))
            kill_last('c');
        if (!strcmp(opt, "kap"))
            kill_all('p');
        if (!strcmp(opt, "kac"))
            kill_all('c');
        if (!strcmp(opt, "ka"))
        {
            kill_all('p');
            kill_all('c');
        }
        if (!strcmp(opt, "s"))
            printf("----------------------------------------------------\n"
                   "Added: %d\nExtracted: %d\n"
                   "----------------------------------------------------\n",
                   msqueue.added_cnt, msqueue.extracted_cnt);
        if (!strcmp(opt, "s+"))
            change_queue_size('+');
        if (!strcmp(opt, "s-"))
            change_queue_size('-');
        if (!strcmp(opt, "q"))
            break;
    }
    kill_all('p');
    kill_all('c');
    pthread_mutex_destroy(&mutex);
    sem_unlink("/items");
    sem_unlink("/free_space");
    qfree(&msqueue);
    return 0;
}