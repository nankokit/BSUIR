#include <opencv2/opencv.hpp>
#include <iostream>
#include <chrono>
#include <string>
#include <atomic>
#include <iomanip>
#include <sstream>
#include <Windows.h>

using namespace cv;
using namespace std;

std::atomic<bool> cameraRunning(true);
bool isWindowVisible = true;
bool isRecording = false;
VideoCapture camera;
HWND consoleWindow;

std::string getCurrentTime()
{
    auto now = std::chrono::system_clock::now();
    auto timeT = std::chrono::system_clock::to_time_t(now);
    std::tm tm = *std::localtime(&timeT);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%H-%M-%S_%d-%m-%Y");
    return oss.str();
}

void makePhoto(Mat &frame)
{
    std::string image_name = (isWindowVisible ? "photo_" : "secret_photo_") + getCurrentTime() + ".jpg";
    if (!imwrite(image_name, frame))
    {
        std::cout << "Photo not saved" << std::endl;
    }
    else
    {
        std::cout << "Photo saved as: " << image_name << std::endl;
    }
}

void recordVideo()
{
    int width = static_cast<int>(camera.get(CAP_PROP_FRAME_WIDTH));
    int height = static_cast<int>(camera.get(CAP_PROP_FRAME_HEIGHT));
    std::string filename = (isWindowVisible ? "video_" : "secret_video_") + getCurrentTime() + ".avi";

    VideoWriter videoWriter(filename, VideoWriter::fourcc('M', 'J', 'P', 'G'), 30, Size(width, height));
    if (!videoWriter.isOpened())
    {
        std::cerr << "Error: Could not open the video writer." << std::endl;
        return;
    }

    std::cout << "Recording video... Press 'R' to stop." << std::endl;

    Mat frame;
    while (isRecording)
    {
        camera >> frame;
        if (frame.empty())
            break;

        videoWriter.write(frame);
        if (isWindowVisible)
        {
            imshow("Camera View", frame);
        }

        char key = static_cast<char>(waitKey(1));
    }

    videoWriter.release();
    std::cout << "Video saved as: " << filename << std::endl;
}

LRESULT CALLBACK keyboardHook(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode == HC_ACTION)
    {
        KBDLLHOOKSTRUCT *pKbdHook = reinterpret_cast<KBDLLHOOKSTRUCT *>(lParam);
        if (wParam == WM_KEYDOWN)
        {
            switch (pKbdHook->vkCode)
            {
            case VK_ESCAPE:
                cameraRunning = false;
                isRecording = false;
                std::cout << "Exiting..." << std::endl;
                break;
            case 'P':
            case 'p':
            {
                Mat frame;
                camera >> frame;
                if (!frame.empty())
                {
                    makePhoto(frame);
                }
            }
            break;
            case 'H':
            case 'h':
                isWindowVisible = !isWindowVisible;
                if (!isWindowVisible)
                {
                    destroyAllWindows();
                    ShowWindow(consoleWindow, SW_HIDE);
                }
                else
                {
                    ShowWindow(consoleWindow, SW_SHOW);
                    namedWindow("Camera View", WINDOW_AUTOSIZE);
                }
                break;
            case 'R':
            case 'r':
                if (isRecording)
                {
                    isRecording = false;
                }
                else
                {
                    isRecording = true;
                    recordVideo();
                }
                break;
            }
        }
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

void displayCameraDetails(cv::VideoCapture &camera)
{
    uint32_t codec = camera.get(cv::CAP_PROP_FOURCC);
    std::string codecString = {char(codec), char(codec >> 8), char(codec >> 16), char(codec >> 24)};

    std::cout << "------------------\nCamera info\n------------------\n";
    std::cout << "Resolution: " << camera.get(cv::CAP_PROP_FRAME_WIDTH) << "x"
              << camera.get(cv::CAP_PROP_FRAME_HEIGHT) << std::endl;
    std::cout << "Frames per second: " << camera.get(cv::CAP_PROP_FPS) << std::endl;
    std::cout << "Contrast: " << camera.get(cv::CAP_PROP_CONTRAST) << std::endl;
    std::cout << "Saturation: " << camera.get(cv::CAP_PROP_SATURATION) << std::endl;
    std::cout << "FOURCC Code: " << codecString << std::endl;
    std::cout << "Built-in Microphone: " << (camera.get(cv::CAP_PROP_AUDIO_BASE_INDEX) >= 0 ? "Yes" : "No") << std::endl;
    std::cout << "------------------\n";
}

int main()
{
    HHOOK keyboardHookHandle = SetWindowsHookEx(WH_KEYBOARD_LL, keyboardHook, NULL, 0);

    camera.open(0);
    if (!camera.isOpened())
    {
        std::cout << "Camera open error" << std::endl;
        return -1;
    }
    consoleWindow = GetForegroundWindow();

    displayCameraDetails(camera);

    std::cout << "Press 'Esc' to quit." << std::endl;
    std::cout << "Press 'H' to hide/show camera window." << std::endl;
    std::cout << "Press 'P' to make photo." << std::endl;
    std::cout << "Press 'R' to start/stop recording." << std::endl;

    Mat frame;
    namedWindow("Camera View", WINDOW_AUTOSIZE);

    while (cameraRunning)
    {
        camera >> frame;
        if (frame.empty())
        {
            std::cerr << "Captured empty frame. Exiting camera thread." << std::endl;
            break;
        }

        if (isWindowVisible)
        {
            imshow("Camera View", frame);
        }

        char key = static_cast<char>(waitKey(30));
    }

    UnhookWindowsHookEx(keyboardHookHandle);
    camera.release();
    destroyAllWindows();

    return 0;
}