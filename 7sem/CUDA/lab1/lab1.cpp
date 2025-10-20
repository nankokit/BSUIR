#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <cmath>
#include <immintrin.h>
#include <algorithm>

using namespace std;

using Matrix = vector<float>;

constexpr auto L_SIZE = 12;
constexpr auto B_SIZE = 256;
constexpr auto BLOCK_SIZE = L_SIZE * L_SIZE;
constexpr auto L3_SIZE = 8 * 1024 * 1024;

// using LitMatrix = float[LIT_MATRIX_SIZE][LIT_MATRIX_SIZE];
// using BigMatrix = LitMatrix[B_SIZE][B_SIZE];

void create_big_matrix(Matrix &result)
{
    size_t size = B_SIZE * B_SIZE * BLOCK_SIZE;

    result.resize(size, 0.0f);

    mt19937 r{random_device{}()};
    uniform_real_distribution<float> dist(0.0f, 10.0f);

    for (int i = 0; i < size; i++)
        result[i] = dist(r);
}

__attribute__((optimize("no-tree-vectorize"))) void mul_normal(const Matrix &A, const Matrix &B, Matrix &C)
{
    C.resize(B_SIZE * B_SIZE * BLOCK_SIZE, 0.0f);
    for (int i = 0; i < B_SIZE; i++)
        for (int j = 0; j < B_SIZE; j++)
        {
            float *C_temp = &C[(i * B_SIZE + j) * BLOCK_SIZE]; // элемент большой матрицы C[i][j]
            for (int k = 0; k < B_SIZE; k++)
            {
                const float *A_temp = &A[(i * B_SIZE + k) * BLOCK_SIZE];
                const float *B_temp = &B[(k * B_SIZE + j) * BLOCK_SIZE];
                for (int x = 0; x < L_SIZE; x++)
                    for (int y = 0; y < L_SIZE; y++)
                    {
                        float sum = 0.0;
                        for (int z = 0; z < L_SIZE; z++)
                            sum += A_temp[x * L_SIZE + z] * B_temp[z * L_SIZE + y];
                        C_temp[x * L_SIZE + y] += sum;
                    }
            }
        }
}

inline void matmul_add_simd(const float *A, const float *B, float *C)
{
    for (int i = 0; i < L_SIZE; i++)
    {
        __m256 acc_0 = _mm256_setzero_ps(); // 0..7
        __m128 acc_1 = _mm_setzero_ps();    // 8..11

        for (int j = 0; j < L_SIZE; j++)
        {
            __m256 va_0 = _mm256_set1_ps(A[i * L_SIZE + j]);
            __m128 va_1 = _mm_set1_ps(A[i * L_SIZE + j]);

            __m256 vb_0 = _mm256_loadu_ps(&B[j * L_SIZE]);
            __m128 vb_1 = _mm_loadu_ps(&B[j * L_SIZE + 8]);

            acc_0 = _mm256_fmadd_ps(va_0, vb_0, acc_0);
            acc_1 = _mm_fmadd_ps(va_1, vb_1, acc_1);
        }

        __m256 c_0 = _mm256_loadu_ps(&C[i * L_SIZE]);
        __m128 c_1 = _mm_loadu_ps(&C[i * L_SIZE + 8]);

        c_0 = _mm256_add_ps(c_0, acc_0);
        c_1 = _mm_add_ps(c_1, acc_1);

        _mm256_storeu_ps(&C[i * L_SIZE], c_0);
        _mm_storeu_ps(&C[i * L_SIZE + 8], c_1);
    }
}

void mul_simd(const Matrix &A, const Matrix &B, Matrix &C)
{
    C.resize(B_SIZE * B_SIZE * BLOCK_SIZE, 0.0f);
    for (int i = 0; i < B_SIZE; i++)
        for (int j = 0; j < B_SIZE; j++)
        {
            float *C_temp = &C[(i * B_SIZE + j) * BLOCK_SIZE]; // элемент большой матрицы C[i][j]
            for (int k = 0; k < B_SIZE; k++)
            {
                const float *A_temp = &A[(i * B_SIZE + k) * BLOCK_SIZE];
                const float *B_temp = &B[(k * B_SIZE + j) * BLOCK_SIZE];
                matmul_add_simd(A_temp, B_temp, C_temp);
            }
        }
}

void mul_cache(const Matrix &A, const Matrix &B, Matrix &C)
{
    C.resize(B_SIZE * B_SIZE * BLOCK_SIZE, 0.0f);

    const size_t blok_bytes = BLOCK_SIZE * sizeof(float);
    const float target_bytes_per_block = (float)L3_SIZE * 0.9f / 3.0f;

    size_t elems_per_block = max<size_t>(1, (size_t)(target_bytes_per_block / blok_bytes));
    size_t BS = max<size_t>(1, (size_t)floor(sqrt((double)elems_per_block)));

    size_t BS_I = min<size_t>(BS, B_SIZE);
    size_t BS_J = min<size_t>(BS, B_SIZE);
    size_t BS_K = min<size_t>(BS, B_SIZE);

    for (size_t ii = 0; ii < B_SIZE; ii += BS_I)
    {
        size_t bi = min(BS_I, B_SIZE - ii);
        for (size_t jj = 0; jj < B_SIZE; jj += BS_J)
        {
            size_t bj = min(BS_J, B_SIZE - jj);
            vector<float> C_block(bi * bj * BLOCK_SIZE);
            fill(C_block.begin(), C_block.end(), 0.0f);

            for (size_t kk = 0; kk < B_SIZE; kk += BS_K)
            {
                size_t bk = min(BS_K, B_SIZE - kk);
                for (size_t p = 0; p < bi; ++p)
                    for (size_t q = 0; q < bk; ++q)
                    {
                        const float *A_elem = &A[((ii + p) * B_SIZE + (kk + q)) * BLOCK_SIZE];
                        for (size_t r = 0; r < bj; ++r)
                        {
                            const float *B_elem = &B[((kk + q) * B_SIZE + (jj + r)) * BLOCK_SIZE];
                            float *C_elem = &C_block[(p * bj + r) * BLOCK_SIZE];
                            matmul_add_simd(A_elem, B_elem, C_elem);
                        }
                    }
            }
            for (size_t p = 0; p < bi; ++p)
                for (size_t r = 0; r < bj; ++r)
                {
                    float *dst = &C[((ii + p) * B_SIZE + (jj + r)) * BLOCK_SIZE];
                    float *src = &C_block[(p * bj + r) * BLOCK_SIZE];
                    std::copy(src, src + BLOCK_SIZE, dst);
                }
        }
    }
}

bool compare_matrices(const Matrix &C1, const Matrix &C2, float eps = 0.0001f)
{
    if (C1.size() != C2.size())
        return false;
    for (size_t i = 0; i < C1.size(); ++i)
    {
        if (std::abs(C1[i] - C2[i]) > eps)
        {
            return false;
        }
    }
    return true;
}

int main()
{
    Matrix A, B, C, C_simd, C_cache;

    create_big_matrix(A);
    create_big_matrix(B);

    auto start_normal = std::chrono::high_resolution_clock::now();
    mul_normal(A, B, C);
    auto end_normal = std::chrono::high_resolution_clock::now();
    auto duration_normal = std::chrono::duration_cast<std::chrono::milliseconds>(end_normal - start_normal).count();
    std::cout << "Normal multiplication time: " << duration_normal << " ms\n";

    auto start_simd = std::chrono::high_resolution_clock::now();
    mul_simd(A, B, C_simd);
    auto end_simd = std::chrono::high_resolution_clock::now();
    auto duration_simd = std::chrono::duration_cast<std::chrono::milliseconds>(end_simd - start_simd).count();
    std::cout << "SIMD multiplication time: " << duration_simd << " ms\n";

    bool equal_simd = compare_matrices(C, C_simd);
    std::cout << "SIMD results are equal to normal: " << (equal_simd ? "Yes" : "No") << "\n";

    auto start_cache = std::chrono::high_resolution_clock::now();
    mul_cache(A, B, C_cache);
    auto end_cache = std::chrono::high_resolution_clock::now();
    auto duration_cache = std::chrono::duration_cast<std::chrono::milliseconds>(end_cache - start_cache).count();
    std::cout << "Cache multiplication time: " << duration_cache << " ms\n";

    bool equal_cache = compare_matrices(C, C_cache);
    std::cout << "Cache results are equal to normal: " << (equal_cache ? "Yes" : "No") << "\n";

    return 0;
}
