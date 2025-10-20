#include <iostream>
#include <cuda_runtime.h>
#include <curand.h>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iomanip>

#include "utils.h"

using namespace std;

void fillRandomCUDA(int *arr, size_t size)
{
    int *d_arr;
    cudaMalloc(&d_arr, size * sizeof(int));

    curandGenerator_t gen;
    curandCreateGenerator(&gen, CURAND_RNG_PSEUDO_DEFAULT);
    curandSetPseudoRandomGeneratorSeed(gen, (unsigned long long)time(nullptr));
    curandGenerate(gen, (unsigned int *)d_arr, size);

    int *tmp = new int[size];
    cudaMemcpy(tmp, d_arr, size * sizeof(int), cudaMemcpyDeviceToHost);
    for (size_t i = 0; i < size; ++i)
    {
        arr[i] = (tmp[i] % 9) + 1;
    }
    delete[] tmp;
    curandDestroyGenerator(gen);
    cudaFree(d_arr);
}

void fillRandom(int *arr, int size, int minVal, int maxVal)
{
    for (int i = 0; i < size; i++)
        arr[i] = minVal + rand() % (maxVal - minVal + 1);
}

bool compareMatrices(const int *a, const int *b, int n, int m)
{
    for (int i = 0; i < n * m; ++i)
        if (a[i] != b[i])
            return false;
    return true;
}

void printMatrix(const int *matrix, int n, int m)
{
    int maxVal = matrix[0];
    for (int i = 1; i < n * m; ++i)
        if (matrix[i] > maxVal)
            maxVal = matrix[i];

    int width = 0;
    int temp = maxVal;
    do
    {
        temp /= 10;
        width++;
    } while (temp > 0);

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
            cout << setw(width + 1) << matrix[i * m + j];
        cout << "\n";
    }
}
