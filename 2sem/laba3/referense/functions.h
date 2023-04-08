#include <stdio.h>
#include <stdlib.h>
#include "struct_functions.h"
#pragma once

void save(school* p, int size);
void load(school** p, int* size);
void load_previously_entered_data(school** p, int* size);
int cmp_last_name(char* i_name, char* j_name);
int sort_condition(school bubble, school pj, char choose);
void insertion_sort(school* p, int size, char choose);