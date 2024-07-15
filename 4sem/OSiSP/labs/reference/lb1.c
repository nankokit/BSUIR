// #include <sys/types.h>
// #include <dirent.h>
// #include <stdio.h>
#define _POSIX_SOURCE
#define _XOPEN_SOURCE 500
#define _XOPEN_SOURCE_EXTENDED
#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <linux/limits.h>
#include <sys/stat.h>
#include <dirent.h>
#include <getopt.h>
#include <stdbool.h>

typedef struct options
{
    bool dirs;
    bool files;
    bool symlinks;
    bool sort;
} options;

void walk(char *path, options params)
{
    struct stat stat;
    if (lstat(path, &stat) == -1)
        printf("Error statting %s: %s\n", path, strerror(errno));

    if ((params.dirs && S_ISDIR(stat.st_mode)) || (params.files && S_ISREG(stat.st_mode)) || (params.symlinks && S_ISLNK(stat.st_mode)))
        printf("%s\n", path);

    if (S_ISDIR(stat.st_mode))
    {
        DIR *dir = opendir(path);
        if (dir == NULL)
        {
            printf("Error opening %s: %s\n", path, strerror(errno));
            return;
        }
        struct dirent **entries;
        int count = 0;
        if (params.sort)
            count = scandir(path, &entries, NULL, alphasort);
        else
            count = scandir(path, &entries, NULL, NULL);

        if (count < 0)
            printf("Error scan dir %s: %s", path, strerror(errno));

        for (int i = 0; i < count; i++)
        {
            struct dirent *entry = entries[i];
            free(entries[i]);
            if ((strcmp(entry->d_name, ".") == 0) || (strcmp(entry->d_name, "..") == 0))
                continue;
            char childPath[256];
            strcpy(childPath, path);
            strcat(childPath, "/");
            strcat(childPath, entry->d_name);

            walk(childPath, params);
        }

        closedir(dir);
    }
}

int main(int argc, char *argv[])
{
    int opt;
    char *path = ".";
    options params = {0};
    while ((opt = getopt(argc, argv, "ldfs")) != EOF)
    {
        switch (opt)
        {
        case 'l':
            params.symlinks = true;
            break;
        case 'd':
            params.dirs = true;
            break;
        case 'f':
            params.files = true;
            break;
        case 's':
            params.sort = true;
            break;
        case '?':
            printf("unknown option : %c\n", optopt);
            break;
        }
    }

    if (!params.dirs && !params.files && !params.symlinks)
    {
        params.dirs = true;
        params.files = true;
        params.symlinks = true;
    }

    if (optind < argc)
        path = argv[optind];
    walk(path, params);
    return 0;
}