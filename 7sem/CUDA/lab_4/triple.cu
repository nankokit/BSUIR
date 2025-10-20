#include <iostream>
#include <cuda_runtime.h>
#include <curand.h>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iomanip>

#include "triple.h"

using namespace std;

void tripleColumns(int *src, int h, int w, int *dest)
{
    auto start_cpu = chrono::high_resolution_clock::now();

    int new_w = w * 3;
    for (int i = 0; i < h; i++)
        for (int j = 0; j < w; j++)
        {
            int val = src[i * w + j];
            for (int k = 0; k < 3; k++)
                dest[i * new_w + j * 3 + k] = val;
        }

    auto end_cpu = chrono::high_resolution_clock::now();
    auto duration_cpu = chrono::duration_cast<chrono::microseconds>(end_cpu - start_cpu).count();

    cout << "CPU computation time: " << duration_cpu << " microseconds\n";
}

__global__ void tripleColumnsKernel(const int *src, int h, int w, int *dest)
{
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;

    int new_w = w * 3;
    if (row < h && col < w)
    {
        int val = src[row * w + col];
        for (int k = 0; k < 3; k++)
            dest[row * new_w + col * 3 + k] = val;
    }
}

void tripleColumnsCUDA(const int *src, int h, int w, int *dest)
{
    int *d_src = nullptr, *d_dest = nullptr;
    int size_src = h * w * sizeof(int);
    int new_w = w * 3;
    int size_dest = h * new_w * sizeof(int);

    cudaMalloc(&d_src, size_src);
    cudaMalloc(&d_dest, size_dest);
    cudaMemcpy(d_src, src, size_src, cudaMemcpyHostToDevice);

    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);

    dim3 block(16, 16);
    dim3 grid((w + block.x - 1) / block.x, (h + block.y - 1) / block.y);

    cudaEventRecord(start, 0);

    tripleColumnsKernel<<<grid, block>>>(d_src, h, w, d_dest);

    cudaEventRecord(stop, 0);
    cudaEventSynchronize(stop);

    float milliseconds = 0;
    cudaEventElapsedTime(&milliseconds, start, stop);

    cudaMemcpy(dest, d_dest, size_dest, cudaMemcpyDeviceToHost);

    cudaEventDestroy(start);
    cudaEventDestroy(stop);
    cudaFree(d_src);
    cudaFree(d_dest);

    cout << "CUDA computation time: " << milliseconds * 1000 << " microseconds\n";
}
