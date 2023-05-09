#include "library.h"

TreeNode *buildHuffmanTree(char *alphabet, int *freq, int size)
{
    TreeNode *left, *right, *top;
    ListNode *listNode = createList(size);
    for (int i = 0; i < size; ++i)
        insertListNode(listNode, newTreeNode(alphabet[i], freq[i], NULL, NULL));
    while (!isSizeOne(listNode))
    {
        left = extractMin(listNode);
        right = extractMin(listNode);
        top = newTreeNode('$', left->frequency + right->frequency, NULL, NULL);
        top->left = left;
        top->right = right;
        insertListNode(listNode, top);
    }
    return extractMin(listNode);
}

void getCodes(TreeNode *root, int arr[], int top, char *alphabet, int alphabetSize, char **translation)
{
    if (root->left)
    {
        arr[top] = 0;
        getCodes(root->left, arr, top + 1, alphabet, alphabetSize, translation);
    }
    if (root->right)
    {
        arr[top] = 1;
        getCodes(root->right, arr, top + 1, alphabet, alphabetSize, translation);
    }
    if (!root->left && !root->right)
    {
        int index = getIndex(alphabet, alphabetSize, root->alphabet);
        char *temp = (char *)malloc((top + 1) * sizeof(char));
        for (int i = 0; i < top; ++i)
            temp[i] = arr[i] + '0';
        temp[top] = '\r';
        translation[index] = temp;

        // printf("in index %d get address %d letter %c: ", index, temp, root->alphabet);
        // printString(translation[index]);
        // printf("\n");
    }
}

char *encode(char *text, int size, char *alphabet, int alphabetSize, char **translation)
{
    char *encoded = (char *)malloc(255 * sizeof(char));
    int index, encodedSize = 0, k;
    for (int i = 0; i < size; i++)
    {
        index = getIndex(alphabet, alphabetSize, text[i]);
        k = 0;
        while (translation[index][k] != '\r')
            encoded[encodedSize++] = translation[index][k++];
    }
    encoded[encodedSize] = '\r';
    return encoded;
}

char *decode(char *encoded, char *alphabet, int alphabetSize, char **translation, int size)
{
    char *decoded = (char *)malloc(255 * sizeof(char));
    int decodedSize = 0, i = 0, k = 0, pointer = 0, check = 0;
    for (decodedSize; decodedSize != size; decodedSize++)
    {
        for (i = 0; i < alphabetSize && !check; i++)
        {
            k = 0;
            while (translation[i][k] == encoded[pointer + k] && translation[i][k] != '\r')
            {
                k++;
            }
            if (translation[i][k] == '\r')
                check = 1;
        }
        decoded[decodedSize] = alphabet[i - 1];
        pointer += k;
        k = 0;
        check = 0;
    }
    decoded[decodedSize] = '\n';
    return decoded;
}

int main()
{
    char text[255], alphabet[255];
    int *freq = (int *)(malloc(255 * sizeof(int)));
    int size = 0, alphabetSize = 0;
    getData(text, freq, alphabet, &size, &alphabetSize);
    TreeNode *root = buildHuffmanTree(alphabet, freq, alphabetSize);
    int arr[100], top = 0;
    char **translation = (char **)malloc(alphabetSize * sizeof(char *));
    getCodes(root, arr, top, alphabet, alphabetSize, translation);
    printf("Compliance table\n");
    for (int i = 0; i < alphabetSize; i++)
    {
        printf("%c - ", alphabet[i]);
        printString(translation[i]);
        printf("\n");
    }
    char *encoded = encode(text, size, alphabet, alphabetSize, translation);
    printf("Encoded text: ");
    printString(encoded);
    printf("\n");
    char *decoded = decode(encoded, alphabet, alphabetSize, translation, size);
    printf("Decoded text: ");
    printString(decoded);
    return 0;
}

/*
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

treeNode *createHuffmanTree(TreeNode **nodes, int n)
{
    while (n > 1)
    {
        // поиск двух узлов с наименьшими частотами
        int min1 = 0, min2 = 1;
        if (nodes[min1]->frequency > nodes[min2]->frequency)
        {
            min1 = 1;
            min2 = 0;
        }
        for (int i = 2; i < n; i++)
        {
            if (nodes[i]->frequency < nodes[min1]->frequency)
            {
                min2 = min1;
                min1 = i;
            }
            else if (nodes[i]->frequency < nodes[min2]->frequency)
            {
                min2 = i;
            }
        }
        // создание нового узла
        TreeNode *newNode = malloc(sizeof(TreeNode));
        newNode->frequency = nodes[min1]->frequency + nodes[min2]->frequency;
        newNode->symbol = 0;
        newNode->left = nodes[min1];
        newNode->right = nodes[min2];
        // удаление двух старых узлов и добавление нового
        nodes[min1] = newNode;
        nodes[min2] = nodes[n - 1];
        n--;
    }
    return nodes[0]; // корень дерева
}

void HuffmanCodes(char *alphabet, int alphabetSize, int *frequencies)
{
    TreeNode *temp = newTreeNode(&(alphabet[0]), frequencies[0], NULL, NULL);
    ListNode *tempList = newList(temp, frequencies[0]);
    ListNode *head = NULL;
    insertBegin(&head, temp, frequencies[0]);
    for (int i = 1; i < alphabetSize; i++)
    {
        temp = newTreeNode(&(alphabet[i]), frequencies[i], NULL, NULL);
        tempList = newList(temp, frequencies[i]);
        insertEnd(&head, temp, frequencies[i]);
    }
    tempList = head;
    for (int i = 0; i < alphabetSize; i++)
    {
        printf("%c - %d \n", tempList->treeNode->alphabet, tempList->treeNode->frequency);
        tempList = tempList->next;
    }
}

// Тестирование
int main()
{

    char text[255], alphabet[255];
    int *frequencies = (int *)(malloc(255 * sizeof(int)));
    int size = 0, alphabetSize = 0;
    getData(text, frequencies, alphabet, size, &alphabetSize);

    // char alphabet[] = {'a', 'b', 'c', 'd', 'e', 'f'};
    // int frequency[] = {5, 9, 12, 13, 16, 45};
    HuffmanCodes(alphabet, alphabetSize, frequencies);
    return 0;
}
*/