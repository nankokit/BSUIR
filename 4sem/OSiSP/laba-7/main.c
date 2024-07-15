#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "record.h"

int recordsCount;

void lock(FILE *file, int id)
{
    int fd = fileno(file);
    struct flock lock = {
        .l_type = F_WRLCK,
        .l_whence = SEEK_SET,
        .l_start = id * sizeof(struct record_s),
        .l_len = sizeof(struct record_s)};
    fcntl(fd, F_SETLKW, &lock);
}

void unlock(FILE *file, int id)
{
    int fd = fileno(file);
    struct flock lock = {
        .l_type = F_UNLCK,
        .l_whence = SEEK_SET,
        .l_start = id * sizeof(struct record_s),
        .l_len = sizeof(struct record_s)};
    fcntl(fd, F_SETLKW, &lock);
}

struct record_s readRecord(FILE *file, int id)
{
    struct record_s temp;
    fseek(file, id * sizeof(struct record_s), SEEK_SET);
    fread(&temp, sizeof(struct record_s), 1, file);
    return temp;
}

void list(FILE *file)
{
    struct record_s temp;
    for (int i = 0; i < recordsCount; i++)
    {
        temp = readRecord(file, i);
        printf("№ %d\t%s\t%s\t%d\n",
               i,
               temp.name,
               temp.address,
               temp.semester);
    }
}
void getRecord(FILE *file, int id)
{
    if (id >= recordsCount)
    {
        printf("Bad id\n");
        return;
    }
    struct record_s temp = readRecord(file, id);
    printf("№ %d\t%s\t%s\t%d\n",
           id,
           temp.name,
           temp.address,
           temp.semester);
}

void putRecord(FILE *file, int id, struct record_s record)
{
    fseek(file, id * sizeof(struct record_s), SEEK_SET);
    fwrite(&record, sizeof(struct record_s), 1, file);
}

void modifyRecord(FILE *file, int id)
{
    if (id >= recordsCount)
    {
        printf("Bad id\n");
        return;
    }
    struct record_s temp;
    lock(file, id);
    printf("New name: ");
    scanf("%79s", temp.name);
    printf("New address: ");
    scanf("%79s", temp.address);
    printf("New semester: ");
    scanf("%d", &temp.semester);
    putRecord(file, id, temp);
    unlock(file, id);
}

void newRecord(FILE *file)
{
    struct record_s temp;
    printf("New name: ");
    scanf("%79s", temp.name);
    printf("New address: ");
    scanf("%79s", temp.address);
    printf("New semester: ");
    scanf("%d", &temp.semester);
    putRecord(file, recordsCount++, temp);
}

int main()
{
    FILE *file = fopen("file", "r+");
    if (!file)
    {
        printf("Error open file\n");
        exit(1);
    }
    struct stat st;
    if (stat("file", &st) == -1)
    {
        printf(" Error stat file\n");
        exit(1);
    }
    recordsCount = st.st_size / sizeof(struct record_s);
    fclose(file);
    char opt[256];
    printf("Start program (l - list, n - new, g - get, m - modify, q - quit)\n");
    while (scanf("%s", opt))
    {
        file = fopen("file", "r+");
        if (!file)
        {
            printf("Error open file\n");
            exit(1);
        }
        if (!strcmp(opt, "l"))
            list(file);
        if (!strcmp(opt, "n"))
            newRecord(file);
        if (!strcmp(opt, "q"))
            break;
        int id = 0;
        if (sscanf(opt, "g%d", &id))
            getRecord(file, id);
        if (sscanf(opt, "m%d", &id))
            modifyRecord(file, id);
        fclose(file);
    }
    return 0;
}
