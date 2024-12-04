#include "PowerSupply.h"

string toTime(int second)
{
    if (second == -1)
        return "charging";
    else
    {
        int days = second / (60 * 60 * 24);
        int hours = (second % (60 * 60 * 24)) / (60 * 60);
        int minutes = (second % (60 * 60)) / 60;
        int seconds = second % 60;
        string result = "";
        if (days > 0)
            result += (to_string(days) + " days ");
        result += (to_string(hours) + " hours ");
        result += (to_string(minutes) + " minutes ");
        // result += (to_string(seconds) + " seconds ");
        return result;
    }
}
void PowerSupply::update()
{
    _SYSTEM_POWER_STATUS status;
    if (!GetSystemPowerStatus(&status))
    {
        cout << "Error while getting system power status" << std::endl;
    }
    else
    {
        this->powerSupply = getPowerSupply(status);
        this->batteryType = getBatteryType();
        this->batteryLifePercent = (int)(status.BatteryLifePercent);
        this->powerSavingMode = status.SystemStatusFlag == 0 ? "disabled" : "enabled";
        this->batteryLifeWithoutCharging = (unsigned long)status.BatteryLifeTime;
        this->fullBatteryLifeWithoutCharging = this->batteryLifeWithoutCharging == -1 ? -1 : (unsigned long)(this->batteryLifeWithoutCharging / this->batteryLifePercent * 100);
    }
}

string PowerSupply::getPowerSupply(_SYSTEM_POWER_STATUS status)
{
    if (status.ACLineStatus != 0 && status.ACLineStatus != 1)
    {
        return "error";
    }
    else
    {
        return status.ACLineStatus == 0 ? "disconnected" : "connected";
    }
}

string PowerSupply::getBatteryType()
{
    // Lion, Li-polymer, NiCad, NiMh
    string batteryType;
    HDEVINFO descriptorOfAllDeviceBatteries; // desccriptor
    descriptorOfAllDeviceBatteries = SetupDiGetClassDevs(
        &GUID_DEVCLASS_BATTERY, // type devices
        NULL,
        NULL,
        DIGCF_PRESENT | DIGCF_DEVICEINTERFACE); // only present and interface devices

    SP_DEVICE_INTERFACE_DATA batteryInterface = {0};            // struct for device interface
    batteryInterface.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA); // struct size

    SetupDiEnumDeviceInterfaces(descriptorOfAllDeviceBatteries, NULL, &GUID_DEVCLASS_BATTERY, 0, &batteryInterface); // fill data in each interface device
    DWORD requiredSize = 0;

    // getting the required size for the interface device
    SetupDiGetDeviceInterfaceDetail(descriptorOfAllDeviceBatteries, &batteryInterface, NULL, 0, &requiredSize, NULL);
    // memory is allocated for the structure that will contain the details of the device interface
    PSP_DEVICE_INTERFACE_DETAIL_DATA deviceDetail = (PSP_DEVICE_INTERFACE_DETAIL_DATA)LocalAlloc(LPTR, requiredSize);
    deviceDetail->cbSize = sizeof(*deviceDetail);

    // getting details of the device interface
    SetupDiGetDeviceInterfaceDetail(descriptorOfAllDeviceBatteries, &batteryInterface, deviceDetail, requiredSize, &requiredSize, NULL);

    // open descriptor of the battery
    HANDLE batteryHandle = CreateFile(deviceDetail->DevicePath,
                                      GENERIC_READ | GENERIC_WRITE,
                                      FILE_SHARE_READ | FILE_SHARE_WRITE,
                                      NULL,
                                      OPEN_EXISTING,
                                      FILE_ATTRIBUTE_NORMAL,
                                      NULL);

    // getting the tag of the battery
    BATTERY_QUERY_INFORMATION batteryQueryInfo = {0};
    DWORD bytesReturned = 0;
    DeviceIoControl(batteryHandle, IOCTL_BATTERY_QUERY_TAG, NULL, 0, &batteryQueryInfo.BatteryTag,
                    sizeof(batteryQueryInfo.BatteryTag), &bytesReturned, NULL);

    // getting the chemistry of the battery by tag
    BATTERY_INFORMATION batteryInfo = {0};
    batteryQueryInfo.InformationLevel = BatteryInformation;

    DeviceIoControl(batteryHandle, IOCTL_BATTERY_QUERY_INFORMATION, &batteryQueryInfo, sizeof(batteryQueryInfo),
                    &batteryInfo, sizeof(batteryInfo), &bytesReturned, NULL);

    for (int i = 0; i < 4; i++)
    {
        batteryType += batteryInfo.Chemistry[i];
    }

    LocalFree(deviceDetail);
    SetupDiDestroyDeviceInfoList(descriptorOfAllDeviceBatteries);

    return batteryType;
}

ostream &operator<<(ostream &os, const PowerSupply &powerSupply)
{
    os
        << std::left << std::setw(40) << "Power supply:"
        << powerSupply.powerSupply << "\n"
        << std::setw(40) << "Battery type:"
        << powerSupply.batteryType << "\n"
        << std::setw(40) << "Battery life (%):"
        << powerSupply.batteryLifePercent << "\n"
        << std::setw(40) << "Power saving mode:"
        << powerSupply.powerSavingMode << "\n"
        << std::setw(40) << "Full battery life time:"
        << toTime(powerSupply.fullBatteryLifeWithoutCharging) << "\n"
        << std::setw(40) << "Battery life without charging:"
        << toTime(powerSupply.batteryLifeWithoutCharging) << "\n";

    return os;
}