#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main()
{
    char buffer[4096];
    int byte_count;
    char *dir = calloc(256, 1);
    int cd_fl = 0;
    int client = socket(AF_INET, SOCK_STREAM, 0);
    if (client == -1)
    {
        printf("Error socket");
        exit(1);
    }

    struct sockaddr_in adr = {
        .sin_family = AF_INET,
        .sin_addr.s_addr = inet_addr("127.0.0.1"),
        .sin_port = htons(8080)};
    if (connect(client, (struct sockaddr *)&adr, sizeof(adr)) == -1)
    {
        printf("Error connect");
        exit(1);
    }
    if (byte_count = read(client, buffer, 4096))
    {
        buffer[byte_count] = '\0';
        printf("%s", buffer);
    }
    if (byte_count = read(client, buffer, 4096))
    {
        buffer[byte_count] = '\0';
        strcpy(dir, buffer);
    }
    while (1)
    {
        do
        {
            printf("\n%s > ", dir);
            fgets(buffer, 4096, stdin);
        } while (!strcmp(buffer, "\n"));
        buffer[strcspn(buffer, "\n")] = '\0';

        if (strstr(buffer, "@"))
        {
            char *file_name = calloc(256, 1);
            strcpy(file_name, buffer + 1);
            FILE *file = fopen(file_name, "r");
            if (!file)
            {
                printf("Error open");
                continue;
            }
            while (fgets(buffer, sizeof(buffer), file))
            {
                printf("\n%s > %s\n", dir, buffer);
                if (strstr(buffer, "\n"))
                {
                    int i = 0;
                    while (buffer[i] != '\n')
                        i++;
                    buffer[i] = '\0';
                }
                write(client, buffer, strlen(buffer));
                if (strstr(buffer, "CD"))
                    cd_fl = 1;
                byte_count = read(client, buffer, 4096);
                if (byte_count <= 0)
                {
                    printf("Connection closed by server");
                    break;
                }
                buffer[byte_count] = '\0';
                if (cd_fl)
                {
                    strcpy(dir, buffer);
                    dir[strlen(dir)] = '\0';
                    cd_fl = 0;
                }
                else
                    printf("%s", buffer);
                usleep(100);
            }
            fclose(file);
            continue;
        }

        write(client, buffer, strlen(buffer));
        if (strstr(buffer, "CD"))
            cd_fl = 1;
        byte_count = read(client, buffer, 4096);
        if (byte_count <= 0)
        {
            printf("Connection closed by server");
            break;
        }
        buffer[byte_count] = '\0';
        if (cd_fl)
        {
            strcpy(dir, buffer);
            dir[strlen(dir)] = '\0';
            cd_fl = 0;
        }
        else
            printf("%s", buffer);
        if (!strcmp(buffer, "QUIT"))
            break;
    }
    close(client);
    free(dir);
    return 0;
}
