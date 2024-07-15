#include "index.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <fcntl.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>

int blocksCount;
int threadsCount;
int blockSize;
uint64_t recordsCount;

pthread_barrier_t barrier;
pthread_mutex_t mutex;
pthread_mutex_t numberMutex;

struct index_hdr_s *buffer;
int start = 0;
int *blockStatus;
record *begin;
uint64_t count = 0;

int cmp(const void *a, const void *b)
{
    struct index_s num1 = *((struct index_s *)a);
    struct index_s num2 = *((struct index_s *)b);
    if (num1.timeMark < num2.timeMark)
        return -1;
    else if (num1.timeMark > num2.timeMark)
        return 1;
    else
        return 0;
}

int mergeBlocks(int step)
{
    int recordNumber = -1;
    pthread_mutex_lock(&mutex);
    for (int i = 0; i < blocksCount / step; i++)
        if (blockStatus[i])
        {
            blockStatus[i] = 0;
            recordNumber = i;
            break;
        }
    pthread_mutex_unlock(&mutex);
    return recordNumber;
}

void merge(int arg)
{
    int step = 1;
    while (step <= blocksCount)
    {
        int recordNumber = 1;
        step *= 2;
        while (recordNumber >= 0)
        {
            recordNumber = mergeBlocks(step);
            if (recordNumber == -1)
                break;
            struct index_s *temp = malloc(step * blockSize * sizeof(struct index_s));
            memcpy(temp, &(begin[recordNumber * blockSize * step]), step * blockSize * sizeof(struct index_s));
            int i = 0, j = step * blockSize / 2, k = step * recordNumber * blockSize;
            while (i < step * blockSize / 2 && j < step * blockSize)
            {
                if (cmp(temp + i, temp + j) == 1)
                    begin[k++] = temp[j++];
                else
                    begin[k++] = temp[i++];
            }
            while (j < step * blockSize)
                begin[k++] = temp[j++];
            while (i < step * blockSize / 2)
                begin[k++] = temp[i++];
            free(temp);
        }
        pthread_barrier_wait(&barrier);
        if (arg == 0)
            for (int i = 0; i < blocksCount; i++)
                blockStatus[i] = 1;
        pthread_barrier_wait(&barrier);
    }
}

int nextBlock()
{
    int recordNumber = -1;
    pthread_mutex_lock(&mutex);
    for (int i = 0; i < blocksCount; i++)
        if (!blockStatus[i])
        {
            blockStatus[i] = 1;
            recordNumber = i;
            break;
        }
    pthread_mutex_unlock(&mutex);
    return recordNumber;
}

void *sort(void *arg)
{
    int argument = *(int *)arg;
    int recordNumber = argument;
    pthread_barrier_wait(&barrier);
    while (recordNumber >= 0)
    {
        struct index_s *temp = &(begin[recordNumber * blockSize]);
        qsort(temp, blockSize, sizeof(struct index_s), cmp);
        recordNumber = nextBlock();
        if (recordNumber == -1)
            break;
    }
    pthread_barrier_wait(&barrier);
    merge(argument);
    return NULL;
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Error parametrs\n");
        return 1;
    }
    blocksCount = atoi(argv[1]);
    if (blocksCount > 256 || !(blocksCount > 0 && (blocksCount & (blocksCount - 1)) == 0))
    {
        printf("Invalid blocks count (must be power of 2) \n");
        return 1;
    }
    threadsCount = atoi(argv[2]);
    if (threadsCount < 12 || threadsCount > 32 || blocksCount < threadsCount)
    {
        printf("Invalid threads count (must be between 12 and 32)\n");
        return 1;
    }

    pthread_mutex_init(&mutex, NULL);
    pthread_barrier_init(&barrier, NULL, threadsCount);

    FILE *file = fopen("file", "rb+");
    if (!file)
        printf("Error open file");

    fread(&recordsCount, sizeof(uint64_t), 1, file);
    blockSize = recordsCount / blocksCount;

    struct stat st;
    if (stat("file", &st) < 0)
        return -1;
    int fd = fileno(file);
    begin = (record *)((uint8_t *)mmap(
                           NULL,
                           st.st_size,
                           PROT_READ | PROT_WRITE,
                           MAP_SHARED,
                           fd,
                           0) +
                       sizeof(uint64_t));

    blockStatus = (int *)calloc(blocksCount, sizeof(int));
    for (int i = 0; i < threadsCount; i++)
        blockStatus[i] = 1;

    pthread_t *threads = (pthread_t *)malloc(threadsCount * sizeof(pthread_t));
    int *arg = (int *)malloc(threadsCount * sizeof(int));
    for (int i = 0; i < threadsCount; i++)
    {
        arg[i] = i;
        pthread_create(&threads[i], NULL, sort, &arg[i]);
    }
    free(arg);

    for (int i = 0; i < threadsCount; i++)
        pthread_join(threads[i], 0);

    free(threads);
    free(blockStatus);
    fclose(file);
    munmap(begin - sizeof(uint64_t), st.st_size);
    pthread_barrier_destroy(&barrier);
    pthread_mutex_destroy(&mutex);
    return 0;
}
