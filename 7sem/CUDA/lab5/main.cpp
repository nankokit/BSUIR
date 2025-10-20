#include <iostream>
#include <cuda_runtime.h>
#include <curand.h>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <opencv2/opencv.hpp>

#include "emboss.h"

using namespace std;

bool compare(unsigned char *a, unsigned char *b, size_t size)
{
    for (int i = 0; i < size; ++i)
        if (a[i] != b[i])
            return false;
    return true;
}

int main()
{
    const char *inPath = INPUT_IMAGE;
    const char *outPathCpu = OUTPUT_CPU_IMAGE;
    const char *outPathCuda = OUTPUT_CUDA_IMAGE;

    // Read with OpenCV (BGR order)
    cv::Mat img = cv::imread(inPath, cv::IMREAD_COLOR);
    if (img.empty())
    {
        std::cerr << "Error loading image: " << inPath << "\n";
        return 1;
    }

    if (!img.isContinuous())
        img = img.clone();

    int width = img.cols;
    int height = img.rows;
    int channels = img.channels();
    if (channels != 3)
    {
        std::cerr << "Only 3-channel color images supported.\n";
        return 1;
    }

    // Convert BGR (OpenCV) to RGB expected by CUDA kernel
    cv::Mat img_rgb;
    cv::cvtColor(img, img_rgb, cv::COLOR_BGR2RGB);

    std::vector<unsigned char> cpu_dest(width * height * 3);

    emboss(img_rgb.data, cpu_dest.data(), width, height);

    cv::Mat cpu_out_rgb(height, width, CV_8UC3, cpu_dest.data());
    cv::Mat cpu_out_bgr;
    cv::cvtColor(cpu_out_rgb, cpu_out_bgr, cv::COLOR_RGB2BGR);

    std::vector<unsigned char> cuda_dest(width * height * 3);

    embossCuda(img_rgb.data, cuda_dest.data(), width, height);

    cv::Mat cuda_out_rgb(height, width, CV_8UC3, cuda_dest.data());
    cv::Mat cuda_out_bgr;
    cv::cvtColor(cuda_out_rgb, cuda_out_bgr, cv::COLOR_RGB2BGR);

    bool equal = compare(cpu_dest.data(), cuda_dest.data(), width * height * 3);
    cout << "Equal image: " << (equal ? "Yes" : "No") << "\n";

    if (!cv::imwrite(outPathCpu, cpu_out_bgr))
    {
        std::cerr << "Error saving image: " << outPathCpu << "\n";
        return 1;
    }

    if (!cv::imwrite(outPathCuda, cuda_out_bgr))
    {
        std::cerr << "Error saving image: " << outPathCuda << "\n";
        return 1;
    }

    return 0;
}
