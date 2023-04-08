#include "stack.h"

void push(list **head, char sign)
{
    list *sample = malloc(sizeof(list));
    if (sample == NULL) exit(STACK_OVERFLOW);

    sample->next = (*head);
    sample->sign = sign;
    sample->sign_status = 0;

    (*head) = sample;
}
char pop(list **head)
{
    list *sample;
    char sign;
    if (*head == NULL) exit(STACK_UNDERFLOW);
    sample = *head;
    *head = (*head)->next;
    sign = sample->sign;
    free(sample);
    return sign;
}
int peek_status(list* head)
{
    if (head == NULL) exit(STACK_UNDERFLOW);
    return head->sign_status;
}