#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
using namespace std;

typedef struct treeNode
{
    char alphabet;
    int frequency; // частота
    struct treeNode *left, *right;
} TreeNode;

typedef struct listNode
{
    unsigned size;
    unsigned capacity;
    TreeNode **treeNodeArray;
} ListNode;

TreeNode *newTreeNode(char alphabet, int frequency, TreeNode *left, TreeNode *right)
{
    TreeNode *temp = (TreeNode *)malloc(sizeof(TreeNode));
    temp->left = left;
    temp->right = right;
    temp->alphabet = alphabet;
    temp->frequency = frequency;
    return temp;
}

ListNode *createList(unsigned capacity)
{
    ListNode *listNode = (ListNode *)malloc(sizeof(ListNode));
    listNode->size = 0;
    listNode->capacity = capacity;
    listNode->treeNodeArray = (TreeNode **)malloc(listNode->capacity * sizeof(TreeNode *));
    return listNode;
}

void swapTreeNode(TreeNode **a, TreeNode **b)
{
    TreeNode *t = *a;
    *a = *b;
    *b = t;
}

void TreeNodeSort(ListNode *listNode, int idx)
{
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;
    if (left < listNode->size && listNode->treeNodeArray[left]->frequency < listNode->treeNodeArray[smallest]->frequency)
        smallest = left;
    if (right < listNode->size && listNode->treeNodeArray[right]->frequency < listNode->treeNodeArray[smallest]->frequency)
        smallest = right;
    if (smallest != idx)
    {
        swapTreeNode(&listNode->treeNodeArray[smallest], &listNode->treeNodeArray[idx]);
        TreeNodeSort(listNode, smallest);
    }
}

int isSizeOne(ListNode *listNode)
{
    return (listNode->size == 1);
}

TreeNode *extractMin(ListNode *listNode)
{
    TreeNode *temp = listNode->treeNodeArray[0];
    if (listNode->size > 0)
    {
        listNode->treeNodeArray[0] = listNode->treeNodeArray[listNode->size - 1];
        --listNode->size;
    }
    TreeNodeSort(listNode, 0);
    return temp;
}

void insertListNode(ListNode *listNode, TreeNode *treeNode)
{
    ++listNode->size;
    int i = listNode->size - 1;
    while (i && treeNode->frequency < listNode->treeNodeArray[(i - 1) / 2]->frequency)
    {
        listNode->treeNodeArray[i] = listNode->treeNodeArray[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    listNode->treeNodeArray[i] = treeNode;
}

int getIndex(char *alphabet, int alphabetSize, char letter)
{
    int i = 0, check = 0;
    while (i != alphabetSize && !check)
        check = (alphabet[i++] == letter);
    if (check)
        return --i;
    else
        return -1;
}

void printString(char *str)
{
    int i = 0;
    while (str[i] != '\n' && str[i] != '\r' && str[i] != '\0')
        printf("%c", str[i++]);
}

string createAlphabet(char *text, int size, char *alphabet, int *alphabetSize)
{
    bool exit_flag = 0;
    alphabet[0] = text[0];
    *alphabetSize = 1;
    for (int i = 1; i < size; i++)
    {
        exit_flag = 0;
        for (int j = 0; j < *alphabetSize; j++)
        {
            if (alphabet[j] == text[i]) // Если в нашем алфавите уже есть такая буква
            {
                exit_flag = 1; // Выходим, если нашли такую букву
                break;
            }
        }
        if (exit_flag == 1) // Если мы находили букву ранее
        {
            continue; // Переходим на следующую итерацию для след. буквы
        }
        else // Если буква найдена не была
        {
            alphabet[(*alphabetSize)] = text[i];
            (*alphabetSize)++;
            continue;
        }
    }
    return alphabet;
}

void get_frequency(char *text, int *frequencies, char *alphabet, int size, int alphabetSize)
{
    for (int i = 0; i < alphabetSize; i++)
    {
        frequencies[i] = 0;
    }
    for (int j = 0; j < alphabetSize; j++)
    {
        for (int i = 0; i < size; i++)
        {
            if (alphabet[j] == text[i])
            {
                frequencies[j]++;
            }
        }
    }
    alphabet[alphabetSize] = '\0';
}

void insertionSort(int *frequencies, int alphabetSize, char *alphabet)
{
    int temp;
    char tempchar;
    for (int i = 1; i < alphabetSize; i++)
    {
        int j = i - 1;
        while (j >= 0 && frequencies[j] > frequencies[j + 1])
        {
            temp = frequencies[j];
            frequencies[j] = frequencies[j + 1];
            frequencies[j + 1] = temp;

            tempchar = alphabet[j];
            alphabet[j] = alphabet[j + 1];
            alphabet[j + 1] = tempchar;

            j--;
        }
    }
}

void getData(char *text, int *frequencies, char *alphabet, int *size, int *alphabetSize)
{
    printf("Enter your text:\n");
    fgets(text, 255, stdin);
    (*size) = strlen(text);
    (*size)--;
    text[*size] = '\0';
    // printf("text: %s\nsize: %d\n", text, *size);

    createAlphabet(text, *size, alphabet, alphabetSize);

    get_frequency(text, frequencies, alphabet, *size, *alphabetSize);
    insertionSort(frequencies, *alphabetSize, alphabet);
    // printf("alphabet: %s\nsize: %d\n", alphabet, *alphabetSize);
    //  for (int i = 0; i < alphabetSize; i++)
    //  {
    //      printf("%c - %d\n", alphabet[i], frequencies[i]);
    //  }
}
