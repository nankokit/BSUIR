#pragma once

#include "menu.h"
#include "USBDevice.h"
#include <conio.h>
#include <initguid.h>
#include <Usbiodef.h>
#include <iostream>
#include <thread>
#include <atomic>
#include <vector>
#include <string>

class USBManager {
public:
    USBManager(HWND hwnd, std::atomic<bool>& ejectFlag, std::atomic<bool>& exitFlag);
    USBDevice createUSBDevice(PDEV_BROADCAST_DEVICEINTERFACE info);
    void ejectDeviceByIndex(int index);
    bool ejectDevice(USBDevice& device);
    void removeDevice(const USBDevice& device);
    void handleDeviceConnected(PDEV_BROADCAST_DEVICEINTERFACE checkGUID);
    void handleDeviceDisconnected(PDEV_BROADCAST_DEVICEINTERFACE checkGUID);
    void handleDeviceSafelyRemove();
    void handleDeviceSafelyRemoveFailed();
    void setupDevicesList();
    void printDevices();
    void cleanupDevices();
private:
    HWND hWnd;
    std::atomic<bool>& safeEject; 
    std::atomic<bool>& exitFlag; 
};