#pragma once

void prefixSum(const int *arr, int n, int *prefix_arr);

__global__ void scanKernel(int *inData, int *outData, int *blockSums, int n);
__global__ void scanDistribute(int *data, int *sums, int n);
int scan(int *inData, int *outData, int n);
void prefixSumCUDA(const int *src, int n, int *dest);