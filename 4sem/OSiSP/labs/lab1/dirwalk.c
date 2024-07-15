#define _XOPEN_SOURCE 500 // for lstat
#define _POSIX_SOURCE
#define _XOPEN_SOURCE_EXTENDED
#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <dirent.h>
#include <sys/stat.h>
#include <linux/limits.h>
#include <errno.h>
#include <getopt.h>

typedef struct options
{
    bool symlinks;
    bool dirs;
    bool files;
    bool sort;
} options;

void scanning(char *path, options currentOptions)
{
    struct stat stat;
    if (lstat(path, &stat) == -1)
        printf("Error create stat struct %s: %s\n", path, strerror(errno));

    if ((currentOptions.dirs && S_ISDIR(stat.st_mode)) || (currentOptions.files && S_ISREG(stat.st_mode)) || (currentOptions.symlinks && S_ISLNK(stat.st_mode)))
        printf("%s\n", path);

    if (S_ISDIR(stat.st_mode))
    {
        DIR *dir = opendir(path);
        if (dir == NULL)
        {
            printf("Error opening %s: %s\n", path, strerror(errno));
            return;
        }
        struct dirent **containedDirs;
        int count = 0;
        if (currentOptions.sort)
            count = scandir(path, &containedDirs, NULL, alphasort);
        else
            count = scandir(path, &containedDirs, NULL, NULL);

        if (count < 0)
            printf("Error scan dir %s: %s", path, strerror(errno));

        for (int i = 0; i < count; i++)
        {
            struct dirent *containedDir = containedDirs[i];
            free(containedDirs[i]);
            if ((strcmp(containedDir->d_name, ".") == 0) || (strcmp(containedDir->d_name, "..") == 0))
                continue;
            char newPath[256];
            strcat(strcat(strcpy(newPath, path), "/"), containedDir->d_name);
            scanning(newPath, currentOptions);
        }
        closedir(dir);
    }
}

char *parsingArgs(int argc, char *argv[], options *currentOptions, char **path)
{
    int opt;
    while ((opt = getopt(argc, argv, "ldfs")) != EOF)
    {
        switch (opt)
        {
        case 'l':
            currentOptions->symlinks = true;
            break;
        case 'd':
            currentOptions->dirs = true;
            break;
        case 'f':
            currentOptions->files = true;
            break;
        case 's':
            currentOptions->sort = true;
            break;
        case '?':
            printf("unknown option : %c\n", optopt);
            break;
        }
    }

    if (!currentOptions->dirs && !currentOptions->files && !currentOptions->symlinks)
    {
        currentOptions->dirs = true;
        currentOptions->files = true;
        currentOptions->symlinks = true;
    }
    if (optind < argc)
        path = &(argv[optind]);
    return *path;
}

int main(int argc, char *argv[])
{
    int opt;
    char *path = ".";
    options currentOptions = {0};
    path = parsingArgs(argc, argv, &currentOptions, &path);
    scanning(path, currentOptions);
    return 0;
}