#include "WiFiManager.h"

void setup()
{
    Serial.begin(115200);
    delay(1000); // Дать время для инициализации Serial
    Serial.println("Starting WiFi Manager Test on ESP32-S3-DevKitC-1...");

    // Инициализация Preferences
    preferences.begin("wifi-config", false);
    preferences.end();

    // Настройка Wi-Fi
    setupWiFi();
}

void loop()
{
    // Проверка подключения
    if (!isWiFiConnected())
    {
        Serial.println("WiFi disconnected. Reconnecting...");
        setupWiFi();
    }
    else
    {
        Serial.println(getWiFiStatus());
    }

    delay(5000); // Проверяем каждые 5 секунд
}