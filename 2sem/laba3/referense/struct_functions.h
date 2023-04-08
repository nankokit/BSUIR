#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#pragma once

enum options
{
    ADD = '1', PRINT, UPDATE, KILL, SORT, RETARDED, ONE_FAMILY, QUIT,
};
typedef struct
{
    unsigned short int id;
    char full_name[64];
    short int age;
    float gpa;
    char address[64];
    int telephone;
} school;

void add(school* p,int size);
void print(school* p, int i);
void update(school* p, int size);
void kill(school* p, int* size);
void sort(school* p, int size);
void retarded(school* p, int size);
void one_family(school* p, int size);

