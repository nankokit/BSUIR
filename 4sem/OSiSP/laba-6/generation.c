#define _GNU_SOURCE
#define MAX 60081.21597
#define MIN 15020.0
#include "index.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    uint64_t recordsCount = atoll(argv[1]);
    if (argc != 2 || recordsCount % 256)
    {
        printf("Invalid parametrs (count must be multiple of 256)\n");
        return 1;
    }

    FILE *file = fopen("file", "wb");
    struct index_hdr_s indexHeader =
        {
            .recordsCount = recordsCount,
            .index = (struct index_s *)malloc(recordsCount * sizeof(struct index_s))};

    for (uint64_t i = 0; i < recordsCount; i++)
    {
        struct index_s temp =
            {
                .recordNumber = i + 1,
                .timeMark = ((double)rand() / RAND_MAX) * (MAX - MIN) + MIN};
        indexHeader.index[i] = temp;
    }
    fwrite(&indexHeader.recordsCount, sizeof(uint64_t), 1, file);
    fwrite(indexHeader.index, sizeof(struct index_s), indexHeader.recordsCount, file);
    free(indexHeader.index);
    fclose(file);
    return 0;
}