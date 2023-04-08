#include "stack.h"
#include "functions.h"
//example +*-*2a*3dc*2b
// 2a*3d*-c*2b*+
int main()
{
    int size;
    char* expression = input(&size);
    char* new_expression = calloc(size, sizeof(char));
    transformation(expression, new_expression);
    output(new_expression);
    return 0;
}