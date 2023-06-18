#include <string.h>
#include <malloc.h>
#include <time.h>
#define MAP_SIZE 10000

typedef struct map_node
{
    char *key;
    int value;
    struct map_node *next;
} map_node;

typedef struct unordered_map
{
    map_node *map[MAP_SIZE];
} unordered_map;

// Функция хеширования
unsigned int hash(char *str)
{
    unsigned int hash = 0;
    for (int i = 0; str[i] != '\0'; i++)
    {
        hash = hash * 31 + str[i];
    }
    return hash % MAP_SIZE;
}

// Добавление элемента в карту
void map_put(unordered_map *map, char *key, int value)
{
    unsigned int index = hash(key);
    map_node *node = map->map[index];
    while (node != NULL)
    {
        if (strcmp(node->key, key) == 0)
        {
            node->value = value;
            return;
        }
        node = node->next;
    }
    node = (map_node *)malloc(sizeof(map_node));
    node->key = key;
    node->value = value;
    node->next = map->map[index];
    map->map[index] = node;
}

// Получение значения по ключу
int map_get(unordered_map *map, char *key)
{
    unsigned int index = hash(key);
    map_node *node = map->map[index];
    while (node != NULL)
    {
        if (strcmp(node->key, key) == 0)
        {
            return node->value;
        }
        node = node->next;
    }
    return -1;
}

// Удаление элемента по ключу
void map_remove(unordered_map *map, char *key)
{
    unsigned int index = hash(key);
    map_node *node = map->map[index];
    if (node == NULL)
    {
        return;
    }
    if (strcmp(node->key, key) == 0)
    {
        map->map[index] = node->next;
        free(node);
        return;
    }
    while (node->next != NULL)
    {
        if (strcmp(node->next->key, key) == 0)
        {
            map_node *temp = node->next;
            node->next = temp->next;
            free(temp);
            return;
        }
        node = node->next;
    }
}

// Очистка карты
void map_clear(unordered_map *map)
{
    for (int i = 0; i < MAP_SIZE; i++)
    {
        map_node *node = map->map[i];
        while (node != NULL)
        {
            map_node *temp = node;
            node = node->next;
            free(temp);
        }
        map->map[i] = NULL;
    }
}

int main()
{
    double time_spent = 0.0;
    clock_t begin = clock();

    clock_t end = clock();

    time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
    printf("The elapsed time is %f seconds", time_spent);

    return 0;
}