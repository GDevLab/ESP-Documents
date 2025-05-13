#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>

// ใส่ข้อมูล Wi-Fi ของคุณ
const char* ssid = "True_2.4G";
const char* password = "0655419166";

// URL ของไฟล์เฟิร์มแวร์บนเว็บเซิร์ฟเวอร์
const char* firmwareURL = "http://your-web-server.com/esp8266_firmware.bin";

void setup() {
    Serial.begin(115200);
    Serial.println("ESP8266 OTA Example");

    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi ..");
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print('.');
        delay(1000);
    }
    Serial.println(" Connected!");

    // ตรวจสอบและอัปเดตเฟิร์มแวร์
    updateFirmware();
}

void loop() {
    // โปรแกรมหลักของคุณจะทำงานที่นี่
    Serial.println("Hello from ESP8266!");
    delay(5000);
}

void updateFirmware() {
    Serial.println("Checking for new firmware ...");

    t_httpUpdate_return ret = HTTP_UPDATE_FAILED; // ประกาศและกำหนดค่าเริ่มต้นให้ ret

    WiFiClient client;
    if (WiFi.status() == WL_CONNECTED) {
        ret = ESPhttpUpdate.update(client, firmwareURL);
    } else {
        Serial.println("WiFi not connected. Cannot perform OTA.");
        return;
    }

    switch (ret) {
        case HTTP_UPDATE_OK:
        Serial.println("Firmware update successful! Rebooting ...");
        ESP.restart();
        break;
        case HTTP_UPDATE_FAILED:
        Serial.printf("HTTP update failed (Error %d): %s\n", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
        break;
        case HTTP_UPDATE_NO_UPDATES:
        Serial.println("No new firmware available.");
        break;
    }
}

// ฟังก์ชัน callback สำหรับแสดง progress (optional)
void updateProgress(int current, int total) {
    Serial.printf("Progress: %d%%\r", (current * 100) / total);
}

// ฟังก์ชัน updateFirmware ที่มีการใช้งาน progress bar (optional)
void updateFirmwareWithProgress() {
    Serial.println("Checking for new firmware ...");

    ESPhttpUpdate.onProgress(updateProgress);

    t_httpUpdate_return ret = HTTP_UPDATE_FAILED; // ประกาศและกำหนดค่าเริ่มต้นให้ ret

    WiFiClient client;
    if (WiFi.status() == WL_CONNECTED) {
        ret = ESPhttpUpdate.update(client, firmwareURL);
    } else {
        Serial.println("WiFi not connected. Cannot perform OTA.");
        ESPhttpUpdate.onProgress(nullptr);
        return;
    }

    ESPhttpUpdate.onProgress(nullptr);

    switch (ret) {
        case HTTP_UPDATE_OK:
        Serial.println("\nFirmware update successful! Rebooting ...");
        ESP.restart();
        break;
        case HTTP_UPDATE_FAILED:
        Serial.printf("\nHTTP update failed (Error %d): %s\n", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
        break;
        case HTTP_UPDATE_NO_UPDATES:
        Serial.println("\nNo new firmware available.");
        break;
    }
}