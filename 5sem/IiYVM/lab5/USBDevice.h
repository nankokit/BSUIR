#pragma once
#pragma comment(lib, "setupapi.lib")
#include <Windows.h>
#include <Setupapi.h>
#include <iostream>
#include <Cfgmgr32.h>
#include <vector>
#include <string>
#include <dbt.h>
#include <initguid.h>
#include <Usbiodef.h>
#include <memory>

class USBDevice {
private:
    std::wstring HARDWARE_ID;
    std::wstring name;
    std::wstring devicePath;
    std::wstring pid;
    bool ejectable;
    DEVINST devInst;
    HDEVNOTIFY notificationHandle;
    void filterNotifications(HWND hWnd);

public:
    static std::vector<USBDevice> devices;
    void unregister() const;

    USBDevice(HDEVINFO deviceList, SP_DEVINFO_DATA deviceInfo, HWND hWnd);
    USBDevice(const USBDevice& other);

    bool operator==(const USBDevice& other) const;

    void setupDeviceInterface(HDEVINFO deviceList, SP_DEVINFO_DATA deviceInfo, HWND hWnd);
    bool isEjectable() const;
    bool eject() const;
    std::wstring getName() const;
    std::wstring getNameAndPid() const;
};

