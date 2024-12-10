#include "USBManager.h"

std::vector<USBDevice> USBDevice::devices;

USBManager::USBManager(HWND hwnd, std::atomic<bool>& ejectFlag, std::atomic<bool>& exitFlag)
    : hWnd(hwnd), safeEject(ejectFlag), exitFlag(exitFlag) {
    DEV_BROADCAST_DEVICEINTERFACE_A filter;
    filter.dbcc_size = sizeof(filter);
    filter.dbcc_classguid = GUID_DEVINTERFACE_USB_DEVICE;
    filter.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
    RegisterDeviceNotificationA(hWnd, &filter, DEVICE_NOTIFY_WINDOW_HANDLE);
}

USBDevice USBManager::createUSBDevice(PDEV_BROADCAST_DEVICEINTERFACE info) {
    HDEVINFO deviceList = SetupDiCreateDeviceInfoList(nullptr, nullptr);
    SetupDiOpenDeviceInterfaceW(deviceList, (LPCWSTR)info->dbcc_name, NULL, NULL);

    SP_DEVINFO_DATA deviceInfo;
    ZeroMemory(&deviceInfo, sizeof(SP_DEVINFO_DATA));
    deviceInfo.cbSize = sizeof(SP_DEVINFO_DATA);

    SetupDiEnumDeviceInfo(deviceList, 0, &deviceInfo);
    return USBDevice(deviceList, deviceInfo, hWnd);
}

void USBManager::ejectDeviceByIndex(int index) {
    if (index >= 0 && index < USBDevice::devices.size()) {
        USBDevice& device = USBDevice::devices[index];
        ejectDevice(device);
        Sleep(100);
    }
    else {
        std::cout << "Invalid device index." << std::endl;
    }
}

bool USBManager::ejectDevice(USBDevice& device) {
    if (device.isEjectable()) {
        safeEject = true;
        if (!device.eject()) {
            safeEject = false;
            system("CLS");
            std::wstring message = L"Failed to eject the device safely.";
            printMenu(message);
            return false;
        }
        Sleep(100);
        safeEject = false;
        return true;
    }
    else {
        system("CLS");
        std::wstring message = L"Device isn't removable.";
        printMenu(message);
        return false;
    }
}

void USBManager::removeDevice(const USBDevice& device) {
    auto it = std::remove_if(USBDevice::devices.begin(), USBDevice::devices.end(),
        [&device](const USBDevice& d) { return d == device; });

    if (it != USBDevice::devices.end()) {
        it->unregister();
        USBDevice::devices.erase(it, USBDevice::devices.end());
    }
}

void USBManager::handleDeviceConnected(PDEV_BROADCAST_DEVICEINTERFACE checkGUID) {
    if (checkGUID->dbcc_classguid != GUID_DEVINTERFACE_USB_DEVICE)
        return;

    USBDevice device = createUSBDevice(checkGUID);
    if (device.getName().empty())
        return;

    if (std::find(USBDevice::devices.begin(), USBDevice::devices.end(), device) != USBDevice::devices.end()) {
        return;
    }

    USBDevice::devices.push_back(device);
    system("CLS");
    std::wstring message = L"Device connected: " + USBDevice::devices.back().getNameAndPid();
    printMenu(message);
    printDevices();
}

void USBManager::handleDeviceDisconnected(PDEV_BROADCAST_DEVICEINTERFACE checkGUID) {
    using namespace std::literals::string_literals;

    USBDevice device = createUSBDevice(checkGUID);
    if (device.getName().empty())
        return;

    removeDevice(device);
    system("CLS");
    std::wstring message = L"Device disconnected "s + (safeEject ? L"safely" : L"unsafely") + L": " + device.getNameAndPid();
    printMenu(message);
    printDevices();
}

void USBManager::handleDeviceSafelyRemove() {
    system("CLS");
    std::wstring message = L"Trying to remove safely... ";
    printMenu(message);
    printDevices();
}

void USBManager::handleDeviceSafelyRemoveFailed() {
    system("CLS");
    std::wstring message = L"Failed to remove safely";
    printMenu(message);
    printDevices();
}

void USBManager::setupDevicesList() {
    SP_DEVINFO_DATA devInfoData;
    const HDEVINFO deviceInfoSet = SetupDiGetClassDevsA(&GUID_DEVINTERFACE_USB_DEVICE, nullptr, nullptr, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);
    if (deviceInfoSet == INVALID_HANDLE_VALUE) {
        std::cout << "Cannot retrieve device information set" << std::endl;
        exitFlag = true;
        return;
    }

    for (int i = 0; ; i++) {
        devInfoData.cbSize = sizeof(devInfoData);
        if (SetupDiEnumDeviceInfo(deviceInfoSet, i, &devInfoData) == FALSE)
            break;

        USBDevice device(deviceInfoSet, devInfoData, hWnd);
        if (device.isEjectable()) {
            USBDevice::devices.push_back(device);
        }
    }
    system("CLS");
    printMenu();
    printDevices();
    SetupDiDestroyDeviceInfoList(deviceInfoSet);
}

void USBManager::printDevices() {
    for (size_t i = 0; i < USBDevice::devices.size(); i++) {
        std::cout << i + 1 << ". ";
        std::wcout << (USBDevice::devices[i].getNameAndPid()) << L"\n";
        std::cout << std::endl;
    }
}

void USBManager::cleanupDevices() {
    for (const USBDevice& device : USBDevice::devices) {
        removeDevice(device);
    }
}