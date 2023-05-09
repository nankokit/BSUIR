#include "library.h"

// Структура узла дерева
typedef struct treeNode
{
    char *data;
    int frequency; // частота
    struct treeNode *left, *right;
} TreeNode;

// Структура элемента списка
typedef struct listNode
{
    TreeNode *treeNode;
    int value;
    struct listNode *next;
} ListNode;

// новый узел дерева
TreeNode *newNode(char *data, int freq, TreeNode *left, TreeNode *right)
{
    TreeNode *temp = (TreeNode *)malloc(sizeof(TreeNode));
    temp->left = left;
    temp->right = right;
    temp->data = data;
    temp->frequency = freq;
    return temp;
}

// новый элемент списка
ListNode *newList(TreeNode *data, int value)
{
    ListNode *temp = (ListNode *)malloc(sizeof(ListNode));
    temp->treeNode = data;
    temp->value = value;
    temp->next = NULL;
    return temp;
}

void insertBegin(ListNode **head, TreeNode *treeNode, int value)
{
    ListNode *list = newList(treeNode, value);
    if (*head == NULL)
    {
        *head = list;
    }
    else
    {
        list->next = *head;
        // (*head)->prev = list;
        *head = list;
    }
}

void insertEnd(ListNode **head, TreeNode *treeNode, int value)
{
    listNode *list = newList(treeNode, value);
    if (*head == NULL)
    {
        *head = list;
    }
    else
    {
        listNode *temp = *head;
        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = list;
        // list->prev = current;
    }
}

void insertMid(ListNode **head, TreeNode *treeNode, int value)
{
    if (*head == NULL)
    {
        insertBegin(head, treeNode, value);
    }
    else
    {
        ListNode *list = newList(treeNode, value);
        ListNode *current = *head;
        int i;
        while (current != NULL && list->value < current->value)
        {
            current = current->next;
        }
        if (current == NULL)
        {
            insertEnd(head, treeNode, value);
        }
        else
        {
            list->next = current->next;
            // current->next->prev = node;
            current->next = list;
            // node->prev = current;
        }
    }
}

void HuffmanCodes(char *alphabet, int alphabetSize, int *frequencies)
{
    TreeNode *temp = newNode(&(alphabet[0]), frequencies[0], NULL, NULL);
    ListNode *tempList = newList(temp, frequencies[0]);
    ListNode *head = NULL;
    insertBegin(&head, temp, frequencies[0]);
    for (int i = 1; i < alphabetSize; i++)
    {
        temp = newNode(&(alphabet[i]), frequencies[i], NULL, NULL);
        tempList = newList(temp, frequencies[i]);
        insertEnd(&head, temp, frequencies[i]);
    }
    tempList = head;
    for (int i = 0; i < alphabetSize; i++)
    {
        printf("%c - %d \n", tempList->treeNode->data, tempList->treeNode->frequency);
        tempList = tempList->next;
    }
}

// Тестирование
int main()
{

    char text[255], alphabet[255];
    int *frequencies = (int *)(malloc(255 * sizeof(int)));
    int size = 0, alphabetSize = 0;
    getData(text, frequencies, alphabet, &size, &alphabetSize);

    // char data[] = {'a', 'b', 'c', 'd', 'e', 'f'};
    // int freq[] = {5, 9, 12, 13, 16, 45};
    HuffmanCodes(alphabet, alphabetSize, frequencies);
    return 0;
}