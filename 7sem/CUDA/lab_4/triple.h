
#pragma once

void tripleColumns(int *src, int h, int w, int *dest);

__global__ void tripleColumnsKernel(const int *src, int h, int w, int *dest);
void tripleColumnsCUDA(const int *src, int h, int w, int *dest);
