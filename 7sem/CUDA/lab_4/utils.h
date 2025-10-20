#pragma once

void fillRandomCUDA(int *arr, size_t size);
void fillRandom(int *arr, int size, int minVal = 1, int maxVal = 9);
bool compareMatrices(const int *a, const int *b, int n, int m);
void printMatrix(const int *matrix, int n, int m);
