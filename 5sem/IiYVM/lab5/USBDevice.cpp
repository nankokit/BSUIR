#include "USBDevice.h"

USBDevice::USBDevice(const USBDevice& other)
    : HARDWARE_ID(other.HARDWARE_ID),
    name(other.name),
    pid(other.pid),
    ejectable(other.ejectable),
    devInst(other.devInst),
    devicePath(other.devicePath),
    notificationHandle(other.notificationHandle) {
}

void USBDevice::unregister() const {
    UnregisterDeviceNotification(notificationHandle);
}

bool USBDevice::operator==(const USBDevice& other) const {
    return this->pid == other.pid;
}

bool USBDevice::isEjectable() const {
    return this->ejectable;
}

std::wstring USBDevice::getName() const {
    return name;
}

void USBDevice::filterNotifications(HWND hWnd) {
    DEV_BROADCAST_HANDLE filter = { 0 };

    HANDLE deviceHandle = CreateFile(devicePath.c_str(),
        0,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    if (deviceHandle != INVALID_HANDLE_VALUE) {
        filter.dbch_size = sizeof(filter);
        filter.dbch_devicetype = DBT_DEVTYP_HANDLE;
        filter.dbch_handle = deviceHandle;

        this->notificationHandle = RegisterDeviceNotification(hWnd, &filter, DEVICE_NOTIFY_WINDOW_HANDLE);
        CloseHandle(deviceHandle);
    }
}

USBDevice::USBDevice(HDEVINFO deviceList, SP_DEVINFO_DATA deviceInfo, HWND hWnd) {
    this->devInst = deviceInfo.DevInst;

    TCHAR buffer[1024];
    ZeroMemory(buffer, sizeof(buffer));
    SetupDiGetDeviceRegistryProperty(deviceList, &deviceInfo, SPDRP_DEVICEDESC, NULL, (BYTE*)buffer, sizeof(buffer), NULL);
    this->name = std::wstring(buffer);

    ZeroMemory(buffer, sizeof(buffer));
    SetupDiGetDeviceRegistryProperty(deviceList, &deviceInfo, SPDRP_HARDWAREID, nullptr, (BYTE*)buffer, sizeof(buffer), nullptr);
    this->HARDWARE_ID = std::wstring(buffer);

    if (!this->HARDWARE_ID.empty() && (HARDWARE_ID.find(L"PID_") != std::wstring::npos)) {
        this->pid = HARDWARE_ID.substr(HARDWARE_ID.find(L"PID_") + 4, 4);
    }

    DWORD properties;
    SetupDiGetDeviceRegistryPropertyA(deviceList, &deviceInfo, SPDRP_CAPABILITIES, NULL, (PBYTE)&properties, sizeof(DWORD), NULL);
    this->ejectable = properties & CM_DEVCAP_REMOVABLE;

    if (hWnd != nullptr && this->ejectable) {
        setupDeviceInterface(deviceList, deviceInfo, hWnd);
    }
}

void USBDevice::setupDeviceInterface(HDEVINFO deviceList, SP_DEVINFO_DATA deviceInfo, HWND hWnd) {
    SP_DEVICE_INTERFACE_DATA devInterfaceData;
    devInterfaceData.cbSize = sizeof(devInterfaceData);

    if (!SetupDiEnumDeviceInterfaces(deviceList, &deviceInfo, &GUID_DEVINTERFACE_USB_DEVICE, 0, &devInterfaceData)) {
        return;
    }

    DWORD requiredLength = 0;
    SetupDiGetDeviceInterfaceDetail(deviceList, &devInterfaceData, NULL, 0, &requiredLength, NULL);

    auto devInterfaceDetailData = (PSP_INTERFACE_DEVICE_DETAIL_DATA)malloc(requiredLength);
    if (devInterfaceDetailData == NULL) {
        return;
    }
    devInterfaceDetailData->cbSize = sizeof(SP_INTERFACE_DEVICE_DETAIL_DATA);

    if (!SetupDiGetDeviceInterfaceDetail(deviceList, &devInterfaceData, devInterfaceDetailData, requiredLength, NULL, &deviceInfo)) {
        free(devInterfaceDetailData);
        return;
    }

    devicePath = std::wstring(devInterfaceDetailData->DevicePath);
    free(devInterfaceDetailData);
    this->filterNotifications(hWnd);
}

bool USBDevice::eject() const {
    if (CM_Request_Device_EjectW(this->devInst, nullptr, nullptr, NULL, NULL) != CR_SUCCESS) {
        return false; 
    }
    return true; 
}

std::wstring USBDevice::getNameAndPid() const {
    return name + L" (pid = " + pid + L")";
}