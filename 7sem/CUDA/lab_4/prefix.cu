#include <iostream>
#include <cuda_runtime.h>
#include <curand.h>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iomanip>

#include "utils.h"
#include "prefix.h"

using namespace std;

const int MOD = 1e9 + 7;

const int BLOCK_SIZE = 512;

void prefixSum(const int *src, int n, int *dest)
{
    auto start_cpu = chrono::high_resolution_clock::now();

    for (int i = 0; i < n - 1; i++)
        dest[i + 1] = (dest[i] + src[i]) % MOD;

    auto end_cpu = chrono::high_resolution_clock::now();
    auto duration_cpu = chrono::duration_cast<chrono::microseconds>(end_cpu - start_cpu).count();

    cout << "CPU computation time: " << duration_cpu << " microseconds\n";
}

// Ядро для выполнения сканирования внутри блока
__global__ void scanKernel(int *inData, int *outData, int *blockSums, int n)
{
    __shared__ int temp[2 * BLOCK_SIZE];

    int tid = threadIdx.x;
    int blockOffset = blockIdx.x * 2 * BLOCK_SIZE;
    int offset = 1;
    int nBlock = 2 * BLOCK_SIZE;

    int ai = blockOffset + tid;
    int bi = blockOffset + tid + BLOCK_SIZE;

    temp[tid] = (ai < n) ? inData[ai] : 0;
    temp[tid + BLOCK_SIZE] = (bi < n) ? inData[bi] : 0;

    for (int d = nBlock >> 1; d > 0; d >>= 1)
    {
        __syncthreads();
        if (tid < d)
        {
            int ai = offset * (2 * tid + 1) - 1;
            int bi = offset * (2 * tid + 2) - 1;
            temp[bi] = (temp[bi] + temp[ai]) % MOD;
        }
        offset <<= 1;
    }

    // Сохраняем сумму блока и обнуляем последний элемент
    if (tid == 0)
    {
        if (blockSums != NULL)
            blockSums[blockIdx.x] = temp[nBlock - 1] % MOD;
        temp[nBlock - 1] = 0;
    }

    // ---- DOWNSWEEP ----
    for (int d = 1; d < nBlock; d <<= 1)
    {
        offset >>= 1;
        __syncthreads();
        if (tid < d)
        {
            int ai = offset * (2 * tid + 1) - 1;
            int bi = offset * (2 * tid + 2) - 1;
            int t = temp[ai];
            temp[ai] = temp[bi];
            temp[bi] = (temp[bi] + t) % MOD;
        }
    }

    __syncthreads();

    if (ai < n)
        outData[ai] = temp[tid];
    if (bi < n)
        outData[bi] = temp[tid + BLOCK_SIZE];
}

__global__ void scanDistribute(int *data, int *sums, int n)
{
    int blockOffset = (blockIdx.x + 1) * (2 * BLOCK_SIZE);
    int idx = blockOffset + threadIdx.x;

    if (idx < n)
        data[idx] = (data[idx] + sums[blockIdx.x + 1]) % MOD;
}

int scan(int *inData, int *outData, int n)
{
    int numBlocks = (n + 2 * BLOCK_SIZE - 1) / (2 * BLOCK_SIZE);

    if (numBlocks <= 1)
    {
        dim3 threads(BLOCK_SIZE, 1, 1);
        dim3 blocks(1, 1, 1);
        scanKernel<<<blocks, threads>>>(inData, outData, NULL, n);
        cudaDeviceSynchronize();
        return 0;
    }

    int *sums1 = NULL;
    int *sums2 = NULL;
    cudaMalloc((void **)&sums1, numBlocks * sizeof(int));
    cudaMalloc((void **)&sums2, numBlocks * sizeof(int));

    dim3 threads(BLOCK_SIZE, 1, 1);
    dim3 blocks(numBlocks, 1, 1);

    scanKernel<<<blocks, threads>>>(inData, outData, sums1, n);
    cudaDeviceSynchronize();

    scan(sums1, sums2, numBlocks);

    dim3 threads2(2 * BLOCK_SIZE, 1, 1);
    dim3 blocks2(numBlocks - 1, 1, 1);
    scanDistribute<<<blocks2, threads2>>>(outData, sums2, n);
    cudaDeviceSynchronize();

    cudaFree(sums1);
    cudaFree(sums2);
    return 0;
}

// Функция prefixSumCUDA с интегрированным scan
void prefixSumCUDA(const int *src, int n, int *dest)
{
    int *d_src = nullptr, *d_dest = nullptr;
    int size_src = n * sizeof(int);
    int size_dest = n * sizeof(int);

    cudaMalloc(&d_src, size_src);
    cudaMalloc(&d_dest, size_dest);
    cudaMemcpy(d_src, src, size_src, cudaMemcpyHostToDevice);

    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);

    // Начало замера времени
    cudaEventRecord(start);

    // Вызов функции scan
    scan(d_src, d_dest, n);

    // Окончание замера времени
    cudaEventRecord(stop);
    cudaEventSynchronize(stop);

    float milliseconds = 0;
    cudaEventElapsedTime(&milliseconds, start, stop);

    cudaMemcpy(dest, d_dest, size_dest, cudaMemcpyDeviceToHost);

    cudaEventDestroy(start);
    cudaEventDestroy(stop);
    cudaFree(d_src);
    cudaFree(d_dest);

    std::cout << "CUDA computation time: " << milliseconds * 1000 << " microseconds\n";
}