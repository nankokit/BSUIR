#include <iostream>
#include <cuda_runtime.h>
#include <curand.h>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iomanip>

using namespace std;

void emboss(const unsigned char *src, unsigned char *dst, int width, int height)
{
    auto start_cpu = chrono::high_resolution_clock::now();

    int kernel[3][3] = {
        {0, 1, 0},
        {1, 0, -1},
        {0, -1, 0}};

    // 3-channel RGB, uchar*, interleaved
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            for (int c = 0; c < 3; ++c)
            {
                int sum = 0;
                for (int ky = -1; ky <= 1; ++ky)
                {
                    int ny = y + ky;
                    if (ny < 0)
                        ny = 0;
                    if (ny >= height)
                        ny = height - 1;
                    for (int kx = -1; kx <= 1; ++kx)
                    {
                        int nx = x + kx;
                        if (nx < 0)
                            nx = 0;
                        if (nx >= width)
                            nx = width - 1;
                        int src_idx = (ny * width + nx) * 3 + c;
                        sum += src[src_idx] * kernel[ky + 1][kx + 1];
                    }
                }
                sum += 128;
                if (sum < 0)
                    sum = 0;
                if (sum > 255)
                    sum = 255;
                int dst_idx = (y * width + x) * 3 + c;
                dst[dst_idx] = static_cast<unsigned char>(sum);
            }
        }
    }

    auto end_cpu = chrono::high_resolution_clock::now();
    auto duration_cpu = chrono::duration_cast<chrono::microseconds>(end_cpu - start_cpu).count();
    cout << "CPU computation time: " << duration_cpu << " microseconds\n";
}

__global__ void embossKernel(
    const unsigned char *src,
    unsigned char *dst,
    int width, int height, int pitch)
{
    int pixelIndex4 = (blockIdx.x * blockDim.x + threadIdx.x) * 4;
    int totalPixels = width * height;

    if (pixelIndex4 >= totalPixels)
        return;

    for (int i = 0; i < 4; ++i)
    {
        int idx = pixelIndex4 + i;
        if (idx >= totalPixels)
            break;

        int x = idx % width;
        int y = idx / width;

        int offset = y * pitch + x * 3;

        int x_left = max(x - 1, 0);
        int x_right = min(x + 1, width - 1);
        int y_top = max(y - 1, 0);
        int y_bot = min(y + 1, height - 1);

        int offset_top = y_top * pitch + x * 3;
        int offset_bot = y_bot * pitch + x * 3;
        int offset_l = y * pitch + x_left * 3;
        int offset_r = y * pitch + x_right * 3;

        int r = 128 + int(src[offset_top + 0]) + int(src[offset_l + 0]) - int(src[offset_r + 0]) - int(src[offset_bot + 0]);
        int g = 128 + int(src[offset_top + 1]) + int(src[offset_l + 1]) - int(src[offset_r + 1]) - int(src[offset_bot + 1]);
        int b = 128 + int(src[offset_top + 2]) + int(src[offset_l + 2]) - int(src[offset_r + 2]) - int(src[offset_bot + 2]);

        dst[offset + 0] = (r < 0) ? 0 : (r > 255 ? 255 : r);
        dst[offset + 1] = (g < 0) ? 0 : (g > 255 ? 255 : g);
        dst[offset + 2] = (b < 0) ? 0 : (b > 255 ? 255 : b);
    }
}

void embossCuda(const unsigned char *src,
                unsigned char *dst,
                int width, int height)
{
    size_t pitch;
    unsigned char *d_src = nullptr, *d_dst = nullptr;
    cudaMallocPitch(&d_src, &pitch, width * 3, height);
    cudaMallocPitch(&d_dst, &pitch, width * 3, height);

    cudaMemcpy2D(d_src, pitch, src, width * 3, width * 3, height, cudaMemcpyHostToDevice);

    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);

    dim3 block(256);
    dim3 grid((width * height / 4 + block.x - 1) / block.x);

    cudaEventRecord(start, 0);

    embossKernel<<<grid, block>>>(d_src, d_dst, width, height, pitch);
    cudaDeviceSynchronize();

    cudaEventRecord(stop, 0);
    cudaEventSynchronize(stop);

    float milliseconds = 0;
    cudaEventElapsedTime(&milliseconds, start, stop);

    cudaMemcpy2D(dst, width * 3, d_dst, pitch, width * 3, height, cudaMemcpyDeviceToHost);

    cudaEventDestroy(start);
    cudaEventDestroy(stop);

    cudaFree(d_src);
    cudaFree(d_dst);

    cout << "CUDA computation time: " << milliseconds * 1000 << " microseconds\n";
}
