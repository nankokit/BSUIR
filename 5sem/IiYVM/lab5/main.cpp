#pragma once
#include "USBDevice.h"
#include <conio.h>
#include <initguid.h>
#include <Usbiodef.h>
#include <iostream>
#include <thread>
#include <string>
#include <atomic>
#include <vector>
#include "USBManager.h"
#include "menu.h"

std::atomic<bool> exitFlag(false);
std::atomic<bool> safeEject(false);
USBManager* usbManager = nullptr;


LRESULT FAR PASCAL WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

    if (message == WM_DEVICECHANGE) {
        switch (wParam) {
        case DBT_DEVICEARRIVAL: {
            usbManager->handleDeviceConnected((PDEV_BROADCAST_DEVICEINTERFACE)lParam);
            break;
        }
        case DBT_DEVICEREMOVECOMPLETE: {
            usbManager->handleDeviceDisconnected((PDEV_BROADCAST_DEVICEINTERFACE)lParam);
            break;
        }
        case DBT_DEVICEQUERYREMOVE: {
            usbManager->handleDeviceSafelyRemove();
            break;
        }
        case DBT_DEVICEQUERYREMOVEFAILED: {
            usbManager->handleDeviceSafelyRemoveFailed();
            break;
        }
        }
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

ATOM RegisterWindowClass() {
    WNDCLASSEXW wx;
    ZeroMemory(&wx, sizeof(wx));
    wx.cbSize = sizeof(WNDCLASSEX);
    wx.lpfnWndProc = (WNDPROC)WndProc;
    wx.lpszClassName = L"NONE";
    return RegisterClassExW(&wx);
}

HWND CreateMainWindow() {
    if (RegisterWindowClass()) {
        return CreateWindowA("NONE", "DevNotifWnd", WS_ICONIC, 0, 0, CW_USEDEFAULT, 0, 0, NULL, GetModuleHandle(nullptr), NULL);
    }
    return NULL;
}

void MessageLoop(HWND hWnd) {
    MSG msg;
    while (true) {
        if (exitFlag)
            break;
        if (PeekMessage(&msg, hWnd, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    delete usbManager;
}

DWORD WINAPI initialisationThread(void*) {
    HWND hWnd = CreateMainWindow();

    usbManager = new USBManager(hWnd, safeEject, exitFlag);
    usbManager->setupDevicesList();

    MessageLoop(hWnd);
    return 0;
}

int main() {
    setlocale(LC_ALL, "Russian");

    HANDLE thread = CreateThread(nullptr, 0, initialisationThread, nullptr, 0, nullptr);
    if (thread == nullptr) {
        std::cout << "Cannot create thread." << std::endl;
        return GetLastError();
    }

    while (true) {
        const char ch = _getch();
        if (ch >= '1' && ch <= '9') {
            int index = ch - '0' - 1;
            usbManager->ejectDeviceByIndex(index);
        }
        else if (ch == 27) { 
            exitFlag = true;
            WaitForSingleObject(thread, INFINITE);
            CloseHandle(thread);
            usbManager->cleanupDevices(); 
            break;
        }
    }
    return 0;
}