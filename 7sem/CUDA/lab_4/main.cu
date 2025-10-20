#include <iostream>
#include <cuda_runtime.h>
#include <curand.h>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iomanip>

#include "utils.h"
#include "triple.h"
#include "prefix.h"

using namespace std;

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        cout << "Usage: " << argv[0] << " <h> <w>" << endl;
        return 1;
    }

    int h = atoi(argv[1]);
    int w = atoi(argv[2]);

    if (h <= 0 || w <= 0)
    {
        cout << "row and col must be positive integers." << endl;
        return 1;
    }

    srand(time(nullptr));

    int *matrix = new int[h * w];
    // fillRandom(matrix, h * w);

    fillRandomCUDA(matrix, h * w);

    // int *trip_matrix_cpu = new int[h * w * 3];
    int *trip_matrix_cuda = new int[h * w * 3];

    // tripleColumns(matrix, h, w, trip_matrix_cpu);
    tripleColumnsCUDA(matrix, h, w, trip_matrix_cuda);

    w *= 3;

    int *prefix_matrix_cpy = new int[h * w]{};
    int *prefix_matrix_cuda = new int[h * w]{};

    prefixSum(trip_matrix_cuda, h * w, prefix_matrix_cpy);
    prefixSumCUDA(trip_matrix_cuda, h * w, prefix_matrix_cuda);

    bool equal = compareMatrices(prefix_matrix_cpy, prefix_matrix_cuda, h, w);
    cout << "Equal matrix: " << (equal ? "Yes" : "No") << "\n";

    if (h <= 12 || w < 12)
    {
        cout << "\nOriginal matrix:" << endl;
        printMatrix(matrix, h, w);

        // cout << "\nCPU result matrix:" << endl;
        // printMatrix(trip_matrix_cpu, h, w * 3);

        // cout << "\nCUDA result matrix:" << endl;
        // printMatrix(trip_matrix_cuda, h, w * 3);

        cout << "\nCPU prefix matrix:" << endl;
        printMatrix(prefix_matrix_cpy, h, w);

        cout << "\nCUDA result prefix matrix:" << endl;
        printMatrix(prefix_matrix_cuda, h, w);
    }

    delete[] matrix;
    // delete[] trip_matrix_cpu;
    delete[] trip_matrix_cuda;
    delete[] prefix_matrix_cpy;
    delete[] prefix_matrix_cuda;
    return 0;
}