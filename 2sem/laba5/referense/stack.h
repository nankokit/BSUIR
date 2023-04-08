#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STACK_OVERFLOW 1488
#define STACK_UNDERFLOW 1337

#pragma once

typedef struct LIST
{
    char sign;
    int sign_status;
    struct LIST *next;
}list;

void push(list **head, char sign);
char pop(list **head);
int peek_status(list* head);

