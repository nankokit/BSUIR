#include <iostream>
#include <cuda_runtime.h>
#include <curand.h>
#include <chrono>
#include <cstdlib>
#include <ctime>

using namespace std;

__global__ void tripleColumnsKernel(const int* src, int n, int m, int* dest)
{
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;
    if (row < n && col < m)
    {
        int val = src[row * m + col];
        int new_m = m * 3;
        for (int k = 0; k < 3; k++)
        {
           dest[row * new_m + col * 3 + k] = val;
        }
    }
}

void fillRandomCUDA(int* arr, size_t size)
{
    int* d_arr;
    cudaMalloc(&d_arr, size * sizeof(int));

    curandGenerator_t gen;
    curandCreateGenerator(&gen, CURAND_RNG_PSEUDO_DEFAULT);
    curandSetPseudoRandomGeneratorSeed(gen, (unsigned long long)time(nullptr));
    curandGenerate(gen, (unsigned int*)d_arr, size);

    int* tmp = new int[size];
    cudaMemcpy(tmp, d_arr, size * sizeof(int), cudaMemcpyDeviceToHost);
    for (size_t i = 0; i < size; ++i)
    {
        arr[i] = (tmp[i] % 9) + 1;
    }
    delete[] tmp;
    curandDestroyGenerator(gen);
    cudaFree(d_arr);
}

void tripleColumnsCUDA(const int* src, int n, int m, int* dest)
{
    int* d_src = nullptr, * d_dest = nullptr;
    int size_src = n * m * sizeof(int);
    int new_m = m * 3;
    int size_dest = n * new_m * sizeof(int);

    cudaMalloc(&d_src, size_src);
    cudaMalloc(&d_dest, size_dest);
    cudaMemcpy(d_src, src, size_src, cudaMemcpyHostToDevice);

    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);

    dim3 block(16, 16);
    dim3 grid((m + block.x - 1) / block.x, (n + block.y - 1) / block.y);

    cudaEventRecord(start, 0);

    tripleColumnsKernel <<<grid, block>>> (d_src, n, m, d_dest);

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

void printMatrix(const int* matrix, int n, int m)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
            cout << matrix[i * m + j] << " ";
        cout << endl;
    }
}

void fillRandom(int* arr, int size, int minVal = 1, int maxVal = 9)
{
    for (int i = 0; i < size; i++)
        arr[i] = minVal + rand() % (maxVal - minVal + 1);
}

void tripleColumns(int* src, int n, int m, int* dest)
{
    int new_m = m * 3;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
        {
            int val = src[i * m + j];
            for (int k = 0; k < 3; k++)
                dest[i * new_m + j * 3 + k] = val;
        }
}

bool compareMatrices(const int* a, const int* b, int n, int m)
{
    for (int i = 0; i < n * m; ++i)
        if (a[i] != b[i])
            return false;
    return true;
}

int main(int argc, char* argv[])
{
    if (argc != 3) {
        cout << "Usage: " << argv[0] << " N M" << endl;
        return 1;
    }

    int N = atoi(argv[1]);
    int M = atoi(argv[2]);

    if (N <= 0 || M <= 0) {
        cout << "N and M must be positive integers." << endl;
        return 1;
    }

    srand(time(nullptr));

    int* matrix = new int[N * M];
    int* new_matrix = new int[N * M * 3];
    int* new_matrix_cuda = new int[N * M * 3];

    fillRandomCUDA(matrix, N * M);

    auto start_cpu = chrono::high_resolution_clock::now();
    tripleColumns(matrix, N, M, new_matrix);
    auto end_cpu = chrono::high_resolution_clock::now();
    auto duration_cpu = chrono::duration_cast<chrono::microseconds>(end_cpu - start_cpu).count();

    cout << "CPU computation time: " << duration_cpu << " microseconds\n";

    tripleColumnsCUDA(matrix, N, M, new_matrix_cuda);

    bool equal = compareMatrices(new_matrix, new_matrix_cuda, N, M * 3);
    cout << "Equal matrix: " << (equal ? "Yes" : "No") << "\n";


    if (N <= 12 || M < 12)
    {
        cout << "\nOriginal matrix:" << endl;

        printMatrix(matrix, N, M);

        cout << "\nCPU result matrix:" << endl;
        printMatrix(new_matrix, N, M * 3);

        cout << "\nCUDA result matrix:" << endl;
        printMatrix(new_matrix_cuda, N, M * 3);
    }

    delete[] matrix;
    delete[] new_matrix;
    delete[] new_matrix_cuda;
    return 0;
}