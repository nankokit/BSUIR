#define _GNU_SOURCE

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/select.h>
#include <pthread.h>
#include <dirent.h>
#include <string.h>

int num_client = 0;
int end = 0;
int socket_data;

char *echo(char buffer[4096])
{
    char *echo_buffer = malloc(4096);
    strcpy(echo_buffer, buffer + sizeof("ECHO"));
    return echo_buffer;
}

char *info()
{
    char response[] = "*SOME INFO ABOUT SERVER*\n";
    char *info = malloc(256);
    strcpy(info, response);
    return info;
}

char *list(char *dir)
{
    struct dirent **entries;
    int count = scandir(dir, &entries, NULL, NULL);
    if (count < 0)
        printf("Error scan dir");
    int name_count = 0;
    char *names = NULL;
    for (int i = 0; i < count; i++)
    {
        struct dirent *entry = entries[i];
        free(entries[i]);
        if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, ".."))
            continue;
        char name[256];
        strcpy(name, entry->d_name);
        if (entry->d_type == DT_DIR)
            strcat(name, "/");
        if (entry->d_type == DT_LNK)
        {
            char temp[256];
            int len = readlink(name, temp, sizeof(temp) - 1);
            if (len == -1)
            {
                printf("Error readlink");
                continue;
            }
            strcat(name, " ~> ");
            strcat(name, temp);
        }
        strcat(name, "\n");
        names = (char *)realloc(names, 256 * ++name_count);
        strcat(names, name);
    }
    return names + 1;
}

char *cd(char *dir, char *buffer)
{
    char *dir_name = calloc(256, 1);
    strcpy(dir_name, buffer + sizeof("CD"));

    char *buff = calloc(256, 1);
    chdir(dir_name);
    getcwd(buff, 256);
    if (strlen(buff) < strlen("/"))
    {
        printf("Error path\n");
        chdir(dir);
        getcwd(buff, 256);
    }

    return buff;
}

void *clientHandler(void *arg)
{
    int client = *(int *)arg;

    char *dir = calloc(256, sizeof(char));
    char buffer[4096];
    int byte_count;
    getcwd(dir, 256);

    write(client, info(), strlen(info()));
    sleep(1);
    write(client, dir, strlen(dir));
    while ((byte_count = read(client, buffer, 4096)) > 0)
    {
        buffer[byte_count] = '\0';
        printf("%s\n", buffer);
        if (strstr(buffer, "ECHO"))
            write(client, echo(buffer), strlen(buffer));
        else if (strstr(buffer, "QUIT"))
        {
            printf("Client disconnect\n");
            num_client--;
            end = 1;
            break;
        }
        else if (strstr(buffer, "INFO"))
            write(client, info(), strlen(info()));
        else if (strstr(buffer, "CD"))
        {
            dir = cd(dir, buffer);
            strcpy(buffer, dir);
            write(client, buffer, strlen(buffer));
        }
        else if (strstr(buffer, "LIST"))
            write(client, list(dir), strlen(list(dir)));
        else
        {
            char response[] = "Unknown request";
            write(client, response, strlen(response));
        }
    }
    close(client);
    return NULL;
}

int main()
{
    pthread_t threads[5];
    int server = socket(AF_INET, SOCK_STREAM, 0);
    if (server == -1)
    {
        printf("Error socket");
        exit(1);
    }
    struct sockaddr_in adr = {
        .sin_family = AF_INET,
        .sin_port = htons(8080)};
    if (bind(server, (struct sockaddr *)&adr, sizeof adr) == -1)
    {
        printf("Error bind");
        exit(1);
    }
    if (listen(server, 5) == -1)
    {
        printf("Error listen");
        exit(1);
    }
    printf("Server start\n");
    do
    {
        struct sockaddr_in client_adr;
        socklen_t client_adrlen = sizeof(client_adr);
        int client = accept(server, (struct sockaddr *)&client_adr, &client_adrlen);
        if (client == -1)
        {
            printf("Error accept");
            continue;
        }
        if (num_client < 5)
        {
            int *socket_data = (int *)malloc(sizeof(int));
            *socket_data = client;
            pthread_create(&threads[num_client], NULL, clientHandler, (void *)socket_data);
            num_client++;
            printf("Client connected\n");
        }
        else
        {
            printf("Max clients\n");
            close(client);
        }
    } while (num_client);
    sleep(5);
    close(server);
    return 0;
}
