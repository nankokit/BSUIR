#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

FILE *fp;

typedef struct bucket
{
    size_t hash;
    char *key, *value;
    struct bucket *next;
} bucket;

typedef struct hashmap
{
    bucket **buckets;
    unsigned int size, capacity;
} hashmap;

unsigned int hash_create(char *);
unsigned int position(size_t, unsigned int);
void find_in_map(hashmap *, char *);
bucket *find_bucket(hashmap *, char *);
void map_input(hashmap *, char *, char *);
void increase_capacity(hashmap *);
void delete_in_map(hashmap *, char *);
void get_from_file(hashmap *);

unsigned int hash_create(char *key)
{
    const int p = 31;
    const int m = 1e9 + 9;
    long long hash_value = 0;
    long long p_pow = 1;
    for (int i = 0; i < (int)strlen(key); i++)
    {
        hash_value = (hash_value + (key[i] - 'a' + 1) * p_pow) % m;
        p_pow = (p_pow * p) % m;
    }
    return hash_value;
}

unsigned int position(size_t hash, unsigned int capacity)
{
    return hash % capacity;
}

void find_in_map(hashmap *map, char *key)
{
    size_t hash = hash_create(key);
    unsigned int idx = position(hash, map->capacity);
    bucket *temp = map->buckets[idx];
    while (temp != NULL)
    {
        if (strcmp(temp->key, key) == 0)
        {
            printf("IP address: %s\n", temp->value);
            return;
        }
        temp = temp->next;
    }
    printf("There is no value with key %s in the map\n", key);
}

bucket *find_bucket(hashmap *map, char *key)
{
    size_t hash = hash_create(key);
    unsigned int idx = position(hash, map->capacity);
    bucket *temp = map->buckets[idx];
    while (temp != NULL)
    {
        if (strcmp(temp->key, key) == 0)
            return temp;
        temp = temp->next;
    }
    return NULL;
}

void map_input(hashmap *map, char *key, char *value)
{
    bucket *bkt;
    if ((bkt = find_bucket(map, key)) != NULL)
        strcpy(bkt->value, value);
    else
    {
        map->size++;
        size_t hash = hash_create(key);
        unsigned int idx = position(hash, map->capacity);
        bkt = (bucket *)malloc(sizeof(bucket));
        bkt->key = (char *)malloc(255);
        bkt->value = (char *)malloc(255);
        bkt->hash = hash;
        strcpy(bkt->key, key);
        strcpy(bkt->value, value);
        bkt->next = map->buckets[idx];
        map->buckets[idx] = bkt;
        if (map->size == map->capacity)
            increase_capacity(map);
    }
}

void increase_capacity(hashmap *map)
{
    bucket **oldbuckets = map->buckets;
    bucket **newbuckets;
    unsigned int oldcapacity = map->capacity;
    newbuckets = (bucket **)calloc(map->capacity * 2, sizeof(bucket *));
    map->buckets = newbuckets;
    map->capacity *= 2;
    map->size = 0;
    for (unsigned int i = 0; i < oldcapacity; i++)
    {
        while (oldbuckets[i] != NULL)
        {
            map_input(map, oldbuckets[i]->key, oldbuckets[i]->value);
            oldbuckets[i] = oldbuckets[i]->next;
        }
        free(oldbuckets[i]);
    }
    free(oldbuckets);
}

void delete_in_map(hashmap *map, char *key)
{
    unsigned int idx = position(hash_create(key), map->capacity);
    bucket *deleted = find_bucket(map, key);
    if (deleted == NULL)
    {
        printf("There is no value with the key %s in the map\n", key);
        return;
    }
    bucket *parent = map->buckets[idx];
    while (parent->next != deleted)
        parent = parent->next;
    parent->next = deleted->next;
    free(deleted);
}

void get_from_file(hashmap *map)
{
    char *key, *value;
    key = (char *)malloc(255);
    value = (char *)malloc(255);
    while (!feof(fp))
    {
        fscanf(fp, "%s%s", key, value);
        map_input(map, key, value);
    }
}

/* void task1(hashmap *map)
{
    int n;
    printf("How much strings do you want download from file?\n");
    scanf("%i", &n);
    get_from_file(map, n);
}

void task2(hashmap *map)
{
    printf("Domain: ");
    char *key;
    key = (char *)malloc(255);
    rewind(stdin);
    scanf("%s", key);
    find_in_map(map, key);
}

void task3(hashmap *map)
{
    char *key, *value;
    key = (char *)malloc(255);
    value = (char *)malloc(255);
    printf("Domain: ");
    rewind(stdin);
    scanf("%s", key);
    printf("IP address: ");
    rewind(stdin);
    scanf("%s", value);
    map_input(map, key, value);
}

void task4(hashmap *map)
{
    char *key;
    key = (char *)malloc(255);
    printf("Domain: ");
    scanf("%s", key);
    delete_in_map(map, key);
} */

int main()
{
    hashmap *map;
    map = (hashmap *)malloc(sizeof(hashmap));
    map->capacity = 1;
    map->size = 0;
    map->buckets = (bucket **)calloc(1, sizeof(bucket *));
    double time_spent = 0.0;
    fp = fopen("top-1m.txt", "r");
    clock_t begin = clock();
    get_from_file(map);
    clock_t end = clock();
    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("%lf\n", time_spent);
    fclose(fp);
    return 0;
}
