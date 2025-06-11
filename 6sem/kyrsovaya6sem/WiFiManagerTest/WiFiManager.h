#pragma once
#include <WiFi.h>
#include <Preferences.h>
#include <WebServer.h>

// Параметры точки доступа
#define AP_SSID "WiFiManager-AP"
#define AP_PASSWORD "12345678"

Preferences preferences;
WebServer server(80);
bool newCredentialsReceived = false;
String newSSID = "";
String newPassword = "";

void handleRoot()
{
    Serial.println("Web server: Serving root page");
    String html = "<!DOCTYPE html><html><head>";
    html += "<meta charset='UTF-8'><title>WiFi Manager Setup</title>";
    html += "<style>body { font-family: Arial, sans-serif; text-align: center; margin-top: 50px; }";
    html += "input, button { padding: 10px; margin: 5px; font-size: 16px; }";
    html += "</style></head><body>";
    html += "<h2>WiFi Manager Setup</h2>";
    html += "<form action='/setwifi' method='POST'>";
    html += "SSID: <input type='text' name='ssid'><br>";
    html += "Password: <input type='password' name='password'><br>";
    html += "<button type='submit'>Connect</button></form>";
    html += "</body></html>";
    server.send(200, "text/html", html);
}

void handleSetWiFi()
{
    Serial.println("Web server: Handling setwifi request");
    if (server.hasArg("ssid") && server.hasArg("password"))
    {
        newSSID = server.arg("ssid");
        newPassword = server.arg("password");
        newCredentialsReceived = true;
        Serial.println("Received credentials: SSID=" + newSSID + ", Password=" + newPassword);
        server.send(200, "text/html", "<h2>Credentials received. Attempting to connect...</h2><p>Please wait.</p>");
    }
    else
    {
        Serial.println("Web server: Missing SSID or password");
        server.send(400, "text/html", "<h2>Error: SSID and Password required</h2>");
    }
}

void printWiFiStatus()
{
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());
    IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);
    long rssi = WiFi.RSSI();
    Serial.print("Signal strength (RSSI): ");
    Serial.print(rssi);
    Serial.println(" dBm");
}

bool initWiFi(const char *ssid, const char *password)
{
    Serial.println("initWiFi: Attempting to connect to " + String(ssid));
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    unsigned long startTime = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startTime < 10000) // Таймаут 10 секунд
    {
        Serial.print(".");
        delay(500);
    }

    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.println("\ninitWiFi: Successful connection to WiFi!");
        printWiFiStatus();
        return true;
    }
    else
    {
        Serial.println("\ninitWiFi: Failed to connect to " + String(ssid));
        WiFi.disconnect();
        return false;
    }
}

void startAccessPoint()
{
    Serial.println("startAccessPoint: Starting AP");
    WiFi.mode(WIFI_AP);
    bool apStarted = WiFi.softAP(AP_SSID, AP_PASSWORD);
    if (apStarted)
    {
        Serial.println("startAccessPoint: Access Point started successfully");
        Serial.print("AP SSID: ");
        Serial.println(AP_SSID);
        Serial.print("AP Password: ");
        Serial.println(AP_PASSWORD);
        Serial.print("AP IP Address: ");
        Serial.println(WiFi.softAPIP());

        // Настройка веб-сервера
        server.on("/", handleRoot);
        server.on("/setwifi", HTTP_POST, handleSetWiFi);
        server.begin();
        Serial.println("startAccessPoint: Web server started at http://" + WiFi.softAPIP().toString());
    }
    else
    {
        Serial.println("startAccessPoint: Failed to start Access Point");
    }
}

bool loadWiFiCredentials(String &ssid, String &password)
{
    Serial.println("loadWiFiCredentials: Reading credentials");
    preferences.begin("wifi-config", true);
    ssid = preferences.getString("ssid", "");
    password = preferences.getString("password", "");
    preferences.end();
    Serial.println("loadWiFiCredentials: SSID=" + ssid + ", Password=" + (password == "" ? "empty" : "set"));
    return ssid != "" && password != "";
}

void saveWiFiCredentials(const String &ssid, const String &password)
{
    Serial.println("saveWiFiCredentials: Saving SSID=" + ssid);
    preferences.begin("wifi-config", false);
    preferences.putString("ssid", ssid);
    preferences.putString("password", password);
    preferences.end();
    Serial.println("saveWiFiCredentials: Credentials saved");
}

void setupWiFi()
{
    Serial.println("setupWiFi: Starting WiFi setup");
    delay(100); // Небольшая задержка для стабилизации

    // Очистка Preferences для теста (уберите после первого теста)
    preferences.begin("wifi-config", false);
    preferences.clear();
    preferences.end();
    Serial.println("setupWiFi: Preferences cleared");

    String ssid, password;
    bool credentialsExist = loadWiFiCredentials(ssid, password);

    if (credentialsExist)
    {
        Serial.println("setupWiFi: Found existing credentials, attempting to connect");
        if (initWiFi(ssid.c_str(), password.c_str()))
        {
            Serial.println("setupWiFi: Connected successfully");
            return;
        }
        Serial.println("setupWiFi: Failed to connect with existing credentials");
    }
    else
    {
        Serial.println("setupWiFi: No credentials found");
    }

    // Запускаем AP и веб-сервер
    startAccessPoint();

    // Ожидаем ввода учетных данных через веб-интерфейс
    newCredentialsReceived = false;
    while (!newCredentialsReceived)
    {
        server.handleClient(); // Обрабатываем запросы к веб-серверу
        delay(10);
    }

    // Пытаемся подключиться с новыми учетными данными
    saveWiFiCredentials(newSSID, newPassword);
    if (initWiFi(newSSID.c_str(), newPassword.c_str()))
    {
        Serial.println("setupWiFi: Connected with new credentials");
        server.close();              // Закрываем веб-сервер
        WiFi.softAPdisconnect(true); // Отключаем AP
    }
    else
    {
        Serial.println("setupWiFi: Failed to connect with new credentials. Restarting AP...");
        startAccessPoint();
    }
}

bool isWiFiConnected()
{
    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("isWiFiConnected: WiFi not connected");
        return false;
    }
    Serial.println("isWiFiConnected: WiFi connected");
    return true;
}

String getWiFiStatus()
{
    if (WiFi.status() == WL_CONNECTED)
    {
        return "Connected to: " + WiFi.SSID() + "\nRSSI: " + String(WiFi.RSSI()) + " dBm";
    }
    else
    {
        return "Not connected to WiFi";
    }
}