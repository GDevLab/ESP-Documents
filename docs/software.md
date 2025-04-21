# Software Arduino IDE

**Arduino IDE คืออะไร?**

Arduino IDE (Integrated Development Environment) คือโปรแกรมที่ใช้เขียนโค้ด (C/C++) สำหรับบอร์ดไมโครคอนโทรลเลอร์ตระกูล Arduino และสามารถใช้งานกับ ESP8266/ESP32 ได้ด้วย มันมาพร้อมฟีเจอร์ :

- แก้ไขโค้ดง่ายๆ แบบ Text Editor
- ปุ่ม “Upload” เพื่ออัปโหลดโค้ดไปยังบอร์ด
- Monitor Serial (ดูข้อมูลจากบอร์ด)
- ระบบจัดการ Library และ Board Manager
- มีตัวอย่างโค้ดให้ดูเพียบ

## ติดตั้ง Arduino IDE

1. เข้าเว็บทางการ : [https://www.arduino.cc/en/software](https://www.arduino.cc/en/software)
2. เลือกรุ่นที่เหมาะกับระบบคุณ : Windows / macOS / Linux
3. ติดตั้งตามขั้นตอนปกติ

## การใช้ Arduino IDE กับ ESP32 หรือ ESP8266

1. เปิด Preferences แล้วใส่ URL นี้ใน Additional Board URLs :

    - สำหรับ ESP32 : 

    ```
    https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
    ```

    - สำหรับ ESP8266 :

    ```
    http://arduino.esp8266.com/stable/package_esp8266com_index.json
    ```

2. ไปที่ Tools > Board > Board Manager
3. ค้นหา “ESP32” หรือ “ESP8266” แล้วกด Install
4. เลือกบอร์ดให้ตรงกับที่ใช้ เช่น NodeMCU-32S, ESP32 Dev Module, Wemos D1 Mini

**ตัวอย่างโปรแกรมง่ายๆ : Blink**
```cpp
void setup() {
  pinMode(2, OUTPUT); // LED บนบอร์ดบางรุ่นจะอยู่ที่ GPIO 2
}

void loop() {
  digitalWrite(2, HIGH);
  delay(500);
  digitalWrite(2, LOW);
  delay(500);
}
```
กด Upload แล้วดู LED กระพริบบนบอร์ดเลย!
