# Framework

ขอแนะนำโครงการ ESP framework บน GitHub ที่น่าสนใจ เป็นโอเพนซอร์ส และ มีการพัฒนาอย่างต่อเนื่องดังนี้ :

1. ESP-IDF (Espressif IoT Development Framework)

    - GitHub : https://github.com/espressif/esp-idf
    - คำอธิบาย : นี่คือ framework หลักอย่างเป็นทางการ จาก Espressif Systems ผู้ผลิตชิป ESP8266 และ ESP32 เป็นเฟรมเวิร์กที่ทรงพลังและมีความยืดหยุ่นสูง เหมาะสำหรับการพัฒนาแอปพลิเคชัน IoT ที่ซับซ้อน
    - คุณสมบัติเด่น :
        - รองรับทั้ง ESP8266 และ ESP32
        - API ที่ครอบคลุมสำหรับการจัดการฮาร์ดแวร์ (GPIO, ADC, DAC, Timers, etc.)
        - รองรับโปรโตคอลเครือข่ายหลากหลาย (Wi-Fi, Bluetooth, Ethernet, TCP/IP, MQTT, HTTP, etc.)
        - ระบบปฏิบัติการแบบเรียลไทม์ (FreeRTOS) ในตัว
        - เครื่องมือสร้างและจัดการโปรเจกต์ที่แข็งแกร่ง (CMake)
        - เอกสารและการสนับสนุนจากชุมชนที่กว้างขวาง
    - เหมาะสำหรับ : นักพัฒนาที่ต้องการควบคุมฮาร์ดแวร์ในระดับลึกและสร้างแอปพลิเคชันที่มีความต้องการเฉพาะเจาะจง

2. Arduino-ESP32

    - GitHub : https://github.com/espressif/arduino-esp32
    - คำอธิบาย : นี่คือ core สำหรับการใช้งาน ESP32 บน Arduino IDE ทำให้สามารถใช้ภาษา Arduino และไลบรารี Arduino ที่คุ้นเคยในการพัฒนาโปรเจกต์บน ESP32 ได้
    - คุณสมบัติเด่น :
        - ใช้งานง่ายสำหรับผู้ที่คุ้นเคยกับ Arduino
        - รองรับไลบรารี Arduino จำนวนมาก
        - เข้าถึงฟังก์ชันพื้นฐานของ ESP32 ได้ง่าย (Wi-Fi, Bluetooth, GPIO, etc.)
        - มีตัวอย่างโค้ดและเอกสารมากมาย
    เหมาะสำหรับ : ผู้เริ่มต้นและนักพัฒนาที่ต้องการความสะดวกและรวดเร็วในการพัฒนาโปรเจกต์บน ESP32 โดยใช้สภาพแวดล้อม Arduino

3. Arduino-ESP8266 Community ESP8266 Core

    - GitHub : https://github.com/esp8266/Arduino
    - คำอธิบาย : นี่คือ core สำหรับการใช้งาน ESP8266 บน Arduino IDE พัฒนาโดยชุมชนผู้ใช้งาน ESP8266 ทำให้สามารถเขียนโค้ด Arduino บน ESP8266  ได้เช่นเดียวกับการใช้งาน ESP32 core
    - คุณสมบัติเด่น :
        - ใช้งานง่ายและมีไลบรารี Arduino สนับสนุนมากมาย
        - รองรับ Wi-Fi และฟังก์ชันพื้นฐานของ ESP8266 ได้อย่างดี
        - มีตัวอย่างโค้ดและชุมชนผู้ใช้งานที่แข็งแกร่ง
    - เหมาะสำหรับ : ผู้ที่ต้องการใช้ความคุ้นเคยกับ Arduino ในการพัฒนาโปรเจกต์บน ESP8266

4. MicroPython

    - GitHub : https://github.com/micropython/micropython
    - คำอธิบาย : MicroPython เป็น การนำภาษา Python 3 มาใช้งานบนไมโครคอนโทรลเลอร์ รวมถึง ESP8266 และ ESP32 ทำให้สามารถเขียนโค้ดด้วยภาษา Python ที่ง่ายและมีความยืดหยุ่นสูง
    - คุณสมบัติเด่น :
        - ภาษา Python ที่ใช้งานง่าย
        - REPL (Read-Eval-Print Loop) สำหรับการทดลองและดีบักแบบโต้ตอบ
        - รองรับ Wi-Fi, Bluetooth, และฟังก์ชันพื้นฐานของ ESP
        - มีไลบรารี MicroPython เฉพาะสำหรับ ESP
    - เหมาะสำหรับ : ผู้ที่คุ้นเคยกับภาษา Python หรือต้องการความรวดเร็วในการพัฒนาและทดลองไอเดียบน ESP

5. PlatformIO

    - GitHub : https://github.com/platformio/platformio-core (core) และ https://github.com/platformio/platformio-vscode-ide (IDE Extension)
    - คำอธิบาย : แม้จะไม่ใช่เฟรมเวิร์กโดยตรง แต่ PlatformIO เป็นแพลตฟอร์มการพัฒนา IoT แบบโอเพนซอร์ส ที่รองรับ ESP8266 และ ESP32 อย่างเต็มรูปแบบ โดยสามารถใช้ร่วมกับเฟรมเวิร์กอื่นๆ ที่กล่าวมาข้างต้น (ESP-IDF, Arduino, MicroPython)
    - คุณสมบัติเด่น :
        - รองรับบอร์ดและเฟรมเวิร์กหลากหลาย
        - ระบบจัดการ Library ที่ยอดเยี่ยม
        - สภาพแวดล้อมการพัฒนาที่สอดคล้องกันบนหลายแพลตฟอร์ม
        - การจัดการ Build และ Upload ที่ง่าย
        - ทำงานร่วมกับ IDE ยอดนิยม เช่น VS Code, Atom
    - เหมาะสำหรับ : นักพัฒนาที่ต้องการเครื่องมือที่ครอบคลุมและยืดหยุ่นในการจัดการโปรเจกต์ ESP

6. ESPHome

    - GitHub : https://github.com/esphome/esphome
    - คำอธิบาย : Framework สำหรับสร้างเฟิร์มแวร์บน ESP32/ESP8266 โดยเขียนเป็น YAML ใช้ใน Home Automation เช่น Home Assistant
    - เหมาะสำหรับ : สาย Smart Home, ไม่ต้องเขียนโค้ดเองเยอะ

7. ESP RainMaker

    - GitHub : https://github.com/espressif/esp-rainmaker
    - คำอธิบาย : โซลูชัน IoT พร้อม cloud support จาก Espressif เอง มีระบบ OTA, cloud dashboard ฯลฯ
    - เหมาะสำหรับ : สร้างผลิตภัณฑ์ IoT จริงจัง มี cloud backend

**การเลือก Framework**

การเลือกเฟรมเวิร์กขึ้นอยู่กับความต้องการและประสบการณ์ของคุณ :
