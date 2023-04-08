#include "stack.h"

int var(char c);
int number(char c);
int sign(char c);
void check_first_element(char c);
void check_2_last_elements(char* expression);
void check_ratio_of_signs_and_vars_numbers(char *expression);

void transformation(char* expression, char* new_expression);
char* input(int* size);
void output(char* new_expression);
