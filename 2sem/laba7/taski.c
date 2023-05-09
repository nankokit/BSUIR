#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_WORDS 1000
#define MAX_LENGTH 50

typedef struct Node
{
    char *key;
    char *value;
    struct Node *left;
    struct Node *right;
} Node;

Node *create_node(char *key, char *value)
{
    Node *node = (Node *)malloc(sizeof(Node));
    node->key = key;
    node->value = value;
    node->left = NULL;
    node->right = NULL;
    return node;
}

void insert_node(Node **root, char *key, char *value)
{
    if (*root == NULL)
    {
        *root = create_node(key, value);
        return;
    }
    int cmp = strcmp(key, (*root)->key);
    if (cmp < 0)
    {
        insert_node(&((*root)->left), key, value);
    }
    else if (cmp > 0)
    {
        insert_node(&((*root)->right), key, value);
    }
    else
    {
        free((*root)->value);
        (*root)->value = value;
    }
}

Node *search_node(Node *root, char *key)
{
    if (root == NULL || strcmp(key, root->key) == 0)
    {
        return root;
    }
    if (strcmp(key, root->key) < 0)
    {
        return search_node(root->left, key);
    }
    else
    {
        return search_node(root->right, key);
    }
}

void print_tree(Node *root)
{
    if (root == NULL)
    {
        return;
    }
    print_tree(root->left);
    printf("%s=%s\n", root->key, root->value);
    print_tree(root->right);
}

void getData(char *filename, Node **root)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Unable to open file %s\n", filename);
        exit(-1);
    }

    char words[MAX_WORDS][MAX_LENGTH];
    int num_words = 0;

    char line[MAX_LENGTH * 2 + 2];
    while (fgets(line, sizeof(line), file))
    {
        line[strcspn(line, "\n")] = '\0';    // strcspn возвращает длину сегмента без указанных символов
        char *delimiter = strchr(line, '='); // указатель на найденное первое вхождение
        if (delimiter == NULL)
        {
            printf("Invalid line in file %s: %s\n", filename, line);
            continue;
        }
        *delimiter = '\0'; // \0 вместо ==
        char *word1 = line;
        char *word2 = delimiter + 1;
        if (strlen(word1) >= MAX_LENGTH || strlen(word2) >= MAX_LENGTH)
        {
            printf("Word too long in file %s: %s\n", filename, line);
            continue;
        }
        strcpy(words[num_words], word1);
        strcpy(words[num_words + 1], word2);
        insert_node(root, words[num_words], words[num_words + 1]);
        num_words += 2;
        if (num_words >= MAX_WORDS)
        {
            printf("Too many words in file %s\n", filename);
            break;
        }
    }

    fclose(file);
}

void task1()
{
    Node *root = NULL;
    char filename[] = "source.txt";
    getData(filename, &root);
    // print_tree(root);
    int check = 0;
    char *word = (char *)malloc(50 * sizeof(char));
    Node *temp;
    while (!check)
    {
        printf("Enter word (0 - exit): ");
        fgets(word, 50, stdin);
        word[strcspn(word, "\n")] = '\0';
        temp = search_node(root, word);
        if (temp != NULL)
            printf("Translation: %s\n", temp->value);
        else
            printf("Translation: not found\n");
        if (word[0] == '0')
            check = 1;
    }
}

char *getCommonSubstring(char *str1, char *str2)
{
    int len1 = strlen(str1);
    int len2 = strlen(str2);
    int **table = (int **)calloc(len1 + 1, sizeof(int *));
    for (int i = 0; i <= len1; i++)
    {
        table[i] = (int *)calloc(len2 + 1, sizeof(int));
    }
    int max_len = 0;
    int end_pos = 0;
    for (int i = 1; i <= len1; i++)
    {
        for (int j = 1; j <= len2; j++)
        {
            if (str1[i - 1] == str2[j - 1])
            {
                table[i][j] = table[i - 1][j - 1] + 1;
                if (table[i][j] > max_len)
                {
                    max_len = table[i][j];
                    end_pos = i;
                }
            }
            else
            {
                table[i][j] = 0;
            }
        }
    }
    char *substring = (char *)malloc(max_len + 1);
    strncpy(substring, str1 + end_pos - max_len, max_len);
    substring[max_len] = '\0';
    for (int i = 0; i <= len1; i++)
    {
        free(table[i]);
    }
    free(table);
    return substring;
}

char *getSubstringArray(char **str, int size)
{
    if (size == 0)
    {
        return NULL;
    }
    char *common_substring = str[0];
    for (int i = 1; i < size; i++)
    {
        common_substring = getCommonSubstring(common_substring, str[i]);
    }
    return common_substring;
}

void task3()
{
    printf("Enter string number:\n");
    int size;
    scanf("%d", &size);
    char **str = (char **)calloc(size, sizeof(char *));
    char buf;
    rewind(stdin);
    for (int i = 0; i < size; i++)
    {
        str[i] = (char *)calloc(50, sizeof(char));
        printf("Enter %d string: ", i + 1);
        fgets(str[i], 50, stdin);
    }

    char *common_substring = getSubstringArray(str, size);
    printf("%s\n", common_substring);
    free(common_substring);
}

int main()
{
    printf("Task 1\n");
    task1();
    printf("Task 3\n");
    task3();
    return 0;
}