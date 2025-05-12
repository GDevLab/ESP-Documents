# Function Deep Sleep

ฟังก์ชัน Deep Sleep ใน ESP เป็นโหมดประหยัดพลังงานที่สำคัญมาก ช่วยให้ ESP กินกระแสน้อยที่สุดเมื่อไม่ได้ทำงานหลัก ทำให้เหมาะสำหรับแอปพลิเคชันที่ใช้พลังงานจากแบตเตอรี่เป็นหลัก เช่น เซ็นเซอร์ IoT ที่ทำงานด้วยแบตเตอรี่

## Deep Sleep Mode

Deep Sleep Mode (โหมดหลับลึก) เป็นโหมดประหยัดพลังงานของ ESP โดยเฉพาะ ESP8266 และ ESP32 ที่ทำให้ไมโครคอนโทรลเลอร์ใช้พลังงานต่ำมาก (ในระดับไมโครแอมป์) โดยปิดการทำงานของ CPU และส่วนประกอบส่วนใหญ่ ยกเว้นวงจร RTC (Real-Time Clock) ที่ใช้สำหรับปลุกระบบ

**หลักการทำงานของ Deep Sleep Mode**

เมื่อ ESP เข้าสู่โหมด Deep Sleep ระบบส่วนใหญ่จะถูกปิดการทำงาน เหลือไว้เพียงส่วนน้อยที่สุดที่ยังคงทำงานเพื่อรอการ "ตื่น" (Wake-up) ขึ้นมาทำงานอีกครั้ง ส่วนที่ยังทำงานอยู่มักจะเป็น :

- **RTC (Real-Time Clock)** : ทำงานเพื่อรักษาเวลาและสามารถใช้เป็นแหล่งปลุกให้ ESP กลับมาทำงานตามช่วงเวลาที่กำหนดได้

- **ULP Coprocessor (Ultra-Low Power Coprocessor)** : ในบางรุ่น (เช่น ESP32) มี ULP ที่สามารถทำงานด้วยพลังงานต่ำมาก สามารถตรวจสอบเซ็นเซอร์ภายนอกและปลุกชิปหลักได้เมื่อมีเหตุการณ์เกิดขึ้น

- **Wake-up Sources** : วงจรที่คอยตรวจสอบเงื่อนไขการปลุกต่างๆ เช่น การกดปุ่มภายนอก หรือสัญญาณจาก GPIO อื่นๆ

เมื่อ ESP อยู่ใน Deep Sleep จะใช้พลังงานน้อยมาก (ระดับไมโครแอมป์) ทำให้ยืดอายุการใช้งานแบตเตอรี่ได้อย่างมาก

**วิธีการเข้าสู่ Deep Sleep Mode**

ในการใช้งาน Deep Sleep ใน ESP โดยทั่วไปจะใช้ฟังก์ชัน `esp_deep_sleep_start()` ร่วมกับฟังก์ชันที่กำหนดเงื่อนไขการปลุก

**เงื่อนไขการปลุก (Wake-up Sources) ที่พบบ่อย**

- **Timer Wakeup** : ปลุก ESP หลังจากช่วงเวลาที่กำหนด
- **Touch Pad Wakeup (ESP32)** : ปลุกเมื่อมีการสัมผัส Touch Pad
- **External Wakeup (Ext0)** : ปลุกเมื่อมีการเปลี่ยนแปลงระดับลอจิกบน GPIO ที่กำหนด (เหมาะสำหรับการปลุกด้วยสัญญาณภายนอก เช่น การกดปุ่ม)
- **External Wakeup (Ext1) (ESP32)** : ปลุกเมื่อมีการเปลี่ยนแปลงระดับลอจิกบน GPIO กลุ่มหนึ่ง (สามารถตั้งค่าให้ปลุกเมื่อมีสัญญาณใดสัญญาณหนึ่งเปลี่ยนแปลง หรือทุกสัญญาณเปลี่ยนแปลงพร้อมกัน)
- **ULP Coprocessor Wakeup (ESP32)** : ปลุกเมื่อ ULP ทำงานเสร็จสิ้นหรือตรวจพบเงื่อนไขบางอย่าง

**ความแตกต่างระหว่าง ESP8266 และ ESP32**

- ESP8266 : รองรับ deep sleep ด้วยการใช้คำสั่ง `ESP.deepSleep()`
- ESP32 : มีการจัดการพลังงานที่ดีกว่า และรองรับหลายวิธีในการตื่นจาก deep sleep เช่น timer, GPIO, touchpad

## ตัวอย่าง Code สำหรับ ESP8266 Deep Sleep

```C++
#include <ESP8266WiFi.h>

#define SLEEP_TIME 30  // เวลาหลับ (วินาที)

void setup() {
    Serial.begin(115200);
    Serial.println("\nStarting...");

    // ตั้งค่าให้ตื่นโดยใช้ Timer
    ESP.deepSleep(SLEEP_TIME * 1000000); // แปลงวินาทีเป็นไมโครวินาที
    
    // โค้ดหลังบรรทัดนี้จะไม่ทำงาน
    Serial.println("This will not be printed");
}

void loop() {
    // ไม่มีโค้ดในนี้
}
```

## ตัวอย่างที่ 1 : Code สำหรับ ESP32 Basic Timer Wakeup

*เหมาะสำหรับผู้เริ่มต้นที่ต้องการใช้งาน Deep Sleep แบบพื้นฐาน*

**คุณสมบัติ** :

- ใช้ Timer เป็นตัวปลุกอุปกรณ์ (Wake Up)
- ไม่มีการเก็บสถานะหรือข้อมูลระหว่างการปลุก
- ไม่มีการตรวจสอบสาเหตุการปลุก (Wake Up Reason)

**ข้อดี** :

- โค้ดเรียบง่าย เหมาะสำหรับการเริ่มต้นใช้งาน Deep Sleep

**ข้อเสีย** :

- ไม่มีการจัดการหรือแสดงข้อมูลเกี่ยวกับการปลุก เช่น Boot Count หรือสาเหตุการปลุก

```C++
#include <esp_sleep.h>
#include <esp_log.h>
#include <soc/rtc_cntl_reg.h>
#include <soc/sens_reg.h>

static const char *TAG = "DEEP_SLEEP_EXAMPLE";

#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  5        /* Time ESP32 will go to sleep (in seconds) */

void setup() {
    Serial.begin(115200);
    Serial.println("Deep Sleep Example");
    Serial.printf("Going to sleep for %d seconds...\n", TIME_TO_SLEEP);

    // Configure timer as wakeup source
    esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
    Serial.println("Setup ESP32 to sleep for every " + String(TIME_TO_SLEEP) + " Seconds");

    // Go to sleep now
    Serial.println("Going to deep sleep now");
    esp_deep_sleep_start();
    Serial.println("This will never be printed");
}

void loop() {
  // This is not going to be called
}
```

**คำอธิบาย Code**

1. `#include` directives :

    - `esp_sleep.h` : Header file ที่มีฟังก์ชันที่เกี่ยวข้องกับการ Sleep modes
    - `esp_log.h` : Header file สำหรับการ Logging (ใช้สำหรับแสดงข้อความ Debug)
    - `soc/rtc_cntl_reg.h` และ `soc/sens_reg.h` : Header files ที่อาจจำเป็นสำหรับการเข้าถึง Registers ระดับล่าง (ในตัวอย่างนี้ไม่ได้ใช้โดยตรง แต่บางครั้งจำเป็นสำหรับการจัดการพลังงานขั้นสูง)

2. `static const char *TAG = "DEEP_SLEEP_EXAMPLE";` : 

    - กำหนด Tag สำหรับการ Logging เพื่อให้ง่ายต่อการติดตามข้อความที่มาจากส่วนนี้ของ Code

3. `#define uS_TO_S_FACTOR 1000000` : 

    - กำหนดค่าคงที่สำหรับการแปลง microseconds เป็น seconds

4. `#define TIME_TO_SLEEP 5` : 

    - กำหนดระยะเวลาที่จะให้ ESP เข้าสู่ Deep Sleep เป็น 5 วินาที

5. `void setup() { ... }` : 

    - ฟังก์ชัน `setup()` จะถูกเรียกใช้เพียงครั้งเดียวเมื่อ ESP เริ่มทำงานหรือรีเซ็ต

        - `Serial.begin(115200);` : เริ่มการสื่อสารผ่าน Serial Port ที่ความเร็ว 115200 bps เพื่อแสดงข้อความ
        - `Serial.println("Deep Sleep Example");` : แสดงข้อความเริ่มต้น
        - `Serial.printf("Going to sleep for %d seconds...\n", TIME_TO_SLEEP);` : แสดงระยะเวลาที่จะเข้าสู่ Deep Sleep
        - `esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);` : กำหนด Timer เป็นแหล่งปลุก ฟังก์ชันนี้จะตั้งค่าให้ ESP ตื่นขึ้นมาหลังจากผ่านไป `TIME_TO_SLEEP` วินาที (แปลงเป็น microseconds)
        - `Serial.println("Setup ESP32 to sleep for every " + String(TIME_TO_SLEEP) + " Seconds");` : แสดงข้อความยืนยันการตั้งค่า Timer
        - `Serial.println("Going to deep sleep now");` : แสดงข้อความก่อนเข้าสู่ Deep Sleep
        - `esp_deep_sleep_start();` : สั่งให้ ESP เข้าสู่โหมด Deep Sleep หลังจากฟังก์ชันนี้ถูกเรียกใช้ ESP จะเข้าสู่โหมด Deep Sleep และหยุดการทำงานของ Code ส่วนที่เหลือใน `setup()` และ `loop()`
        - `Serial.println("This will never be printed");` : บรรทัดนี้จะไม่ถูกเรียกใช้งาน เนื่องจาก ESP จะเข้าสู่ Deep Sleep ก่อน

6. `void loop() { ... }` : 

    - ฟังก์ชัน `loop()` จะไม่ถูกเรียกใช้งานในตัวอย่างนี้ เนื่องจาก ESP จะเข้าสู่ Deep Sleep และเมื่อตื่นขึ้นมาก็จะเริ่มทำงานใหม่ที่ฟังก์ชัน `setup()` อีกครั้ง (ถ้าไม่ได้ตั้งค่าให้ Resume จากจุดเดิม)

**การทำงานเมื่อ ESP ตื่นจาก Deep Sleep**

เมื่อ ESP ตื่นขึ้นจาก Deep Sleep จะมีการ Reset ระบบ (โดยทั่วไป) และจะเริ่มทำงานใหม่ที่ฟังก์ชัน `setup()` อีกครั้ง หากต้องการให้ ESP ทำงานต่อเนื่องจากจุดที่เข้าสู่ Sleep จะต้องมีการจัดการ State และ Memory ที่เหมาะสม (ซึ่งซับซ้อนกว่าและไม่ได้แสดงในตัวอย่างนี้)

## ตัวอย่างที่ 2 : Code สำหรับ ESP32 Timer Wakeup with Boot Counter**

*เพิ่มฟีเจอร์ RTC Memory เหมาะสำหรับการเก็บสถานะการบูต*

**คุณสมบัติ** :

- ใช้ Timer เป็นตัวปลุกอุปกรณ์
- ใช้ตัวแปร `RTC_DATA_ATTR` เพื่อเก็บค่า Boot Count ที่เก็บอยู่ใน RTC Memory
- ไม่มีการตรวจสอบสาเหตุของการปลุก

**ข้อดี** :

- เก็บค่าจำนวนครั้งที่อุปกรณ์บูต (Boot Count) ไว้เพื่อการ Debug หรือการวิเคราะห์

**ข้อเสีย** :

- ขาดฟีเจอร์การตรวจสอบสาเหตุของการปลุก

```C++
#include <esp_sleep.h>

#define uS_TO_S_FACTOR 1000000  // แปลงไมโครวินาทีเป็นวินาที
#define TIME_TO_SLEEP  10       // เวลา sleep (วินาที)

RTC_DATA_ATTR int bootCount = 0;

void setup() {
    Serial.begin(115200);
    delay(1000);  // รอ Serial พร้อม
    ++bootCount;
    Serial.println("Boot number: " + String(bootCount));

    // ตั้งเวลาสำหรับ deep sleep
    esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
    Serial.println("Going to sleep for " + String(TIME_TO_SLEEP) + " seconds");

    // เข้าสู่ deep sleep
    esp_deep_sleep_start();
}

void loop() {
    // จะไม่ถูกเรียกเลยหลังจาก deep sleep
}
```

**คำอธิบาย Code**

- `#include <esp_sleep.h>` : ใช้ library สำหรับควบคุมโหมด sleep ของ ESP32
- `RTC_DATA_ATTR` : ใช้เก็บตัวแปรไว้ใน RTC memory (ไม่หายตอน deep sleep)
- `esp_sleep_enable_timer_wakeup(...)` : ตั้งเวลาปลุกให้ ESP ตื่นจาก deep sleep
- `esp_deep_sleep_start()` : เข้าสู่โหมด deep sleep จริง
- `setup()` : ฟังก์ชันที่รันเมื่อบูตหรือหลังจากตื่นจาก deep sleep
- `loop()` : จะไม่ถูกรันหลังจาก deep sleep เพราะ ESP จะรีเซ็ตตัวเอง

## ตัวอย่างที่ 3 : Code สำหรับ ESP32 Timer Wakeup with Detailed Wakeup Reason

*เพิ่มฟีเจอร์ Wake Up Reason เหมาะสำหรับ Debug และการวิเคราะห์*

**คุณสมบัติ** :

- ใช้ Timer เป็นตัวปลุกอุปกรณ์
- เก็บค่า Boot Count ใน RTC Memory
- ตรวจสอบและแสดง Wake Up Reason (สาเหตุการปลุก)

**ข้อดี** :

- มีการตรวจสอบและแสดงผลสาเหตุการปลุก ทำให้วิเคราะห์การทำงานได้ง่ายสามารถ Debug ได้ดีขึ้นด้วย Boot Count และ Wake Up Reason

**ข้อเสีย** :

- ไม่มีการตั้งค่าการปลุกด้วย GPIO

```C++
#include <esp_sleep.h>

#define uS_TO_S_FACTOR 1000000  // ตัวคูณแปลงไมโครวินาทีเป็นวินาที
#define TIME_TO_SLEEP  5        // เวลาที่จะหลับ (วินาที)

RTC_DATA_ATTR int bootCount = 0; // เก็บค่าใน RTC memory

void print_wakeup_reason(){
    esp_sleep_wakeup_cause_t wakeup_reason;

    wakeup_reason = esp_sleep_get_wakeup_cause();

    switch(wakeup_reason)
    {
        case ESP_SLEEP_WAKEUP_EXT0 : Serial.println("Wakeup caused by external signal using RTC_IO"); break;
        case ESP_SLEEP_WAKEUP_EXT1 : Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
        case ESP_SLEEP_WAKEUP_TIMER : Serial.println("Wakeup caused by timer"); break;
        case ESP_SLEEP_WAKEUP_TOUCHPAD : Serial.println("Wakeup caused by touchpad"); break;
        case ESP_SLEEP_WAKEUP_ULP : Serial.println("Wakeup caused by ULP program"); break;
        default : Serial.printf("Wakeup was not caused by deep sleep: %d\n",wakeup_reason); break;
    } 
}

void setup(){
    Serial.begin(115200);
    delay(1000); // รอให้ Serial ทำงาน

    // เพิ่ม boot count และแสดงค่า
    ++bootCount;
    Serial.println("Boot number: " + String(bootCount));

    // แสดงสาเหตุที่ตื่น
    print_wakeup_reason();

    // ตั้งค่า Timer เพื่อปลุก ESP
    esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
    Serial.println("Setup ESP32 to sleep for every " + String(TIME_TO_SLEEP) + " Seconds");

    // เข้าโหมด Deep Sleep
    Serial.println("Going to sleep now");
    Serial.flush(); 
    esp_deep_sleep_start();
}

void loop(){
    // โค้ดนี้จะไม่ทำงาน เพราะ ESP จะรีเซตหลังจากตื่นจาก Deep Sleep
}
```

**คำอธิบาย Code**

- `#include <esp_sleep.h>` : รวมไลบรารี่สำหรับจัดการโหมด sleep
- `RTC_DATA_ATTR` : กำหนดให้ตัวแปรเก็บใน RTC memory ซึ่งจะยังคงค่าไว้ระหว่าง Deep Sleep

**ฟังก์ชัน** `print_wakeup_reason()` :

- ตรวจสอบสาเหตุที่ ESP ตื่นจากโหมด sleep โดยใช้ `esp_sleep_get_wakeup_cause()`

แสดงผลสาเหตุต่างๆ เช่น ตื่นจาก Timer, การสัมผัส, สัญญาณภายนอก เป็นต้น

**ฟังก์ชัน** `setup()` :

- `++bootCount` : นับจำนวนครั้งที่ ESP ถูกบูต
- `esp_sleep_enable_timer_wakeup()` : ตั้งเวลาให้ ESP ตื่นหลังจากเวลาที่กำหนด (5 วินาทีในตัวอย่าง)
- `esp_deep_sleep_start()` : เข้าสู่โหมด Deep Sleep

## ตัวอย่างที่ 4 : Code สำหรับ ESP32 Multiple Wakeup Sources

*รองรับการปลุกด้วย GPIO และ Timer เหมาะสำหรับงานที่ต้องการความหลากหลาย*

**คุณสมบัติ** :

- ใช้ Timer และ GPIO (Pin 33) เป็นแหล่งปลุก
- ตรวจสอบ Wake Up Reason ก่อนการทำงาน

**ข้อดี** :

- รองรับการปลุกได้ทั้ง GPIO และ Timer
- เหมาะสำหรับการใช้งานที่ต้องการการปลุกแบบหลากหลาย

**ข้อเสีย** :

- ไม่มีการเก็บ Boot Count หรือการบันทึกสถานะใน RTC Memory

```C++
#include <esp_sleep.h>

// Pin ที่ใช้ในการปลุกอุปกรณ์ (Wake Up)
#define WAKE_UP_PIN GPIO_NUM_33 

void setup() {
    Serial.begin(115200);
    delay(1000); // ให้เวลาสำหรับ Serial Monitor
    
    // ตรวจสอบสาเหตุของการปลุก (Wake Up)
    esp_sleep_wakeup_cause_t wakeUpReason = esp_sleep_get_wakeup_cause();
    
    switch (wakeUpReason) {
        case ESP_SLEEP_WAKEUP_EXT0:
        Serial.println("ตื่นขึ้นโดย External Wakeup (Pin)");
        break;
        case ESP_SLEEP_WAKEUP_TIMER:
        Serial.println("ตื่นขึ้นโดย Timer");
        break;
        default:
        Serial.println("ตื่นขึ้นด้วยเหตุผลอื่น");
        break;
    }

    // กำหนด Wake Up ด้วย Timer (5 วินาที)
    esp_sleep_enable_timer_wakeup(5000000); // 5,000,000 ไมโครวินาที = 5 วินาที
    Serial.println("ตั้งค่าให้ตื่นด้วย Timer หลังจาก 5 วินาที");

    // กำหนด Wake Up ด้วย GPIO
    esp_sleep_enable_ext0_wakeup(WAKE_UP_PIN, 0); // ตื่นเมื่อ Pin 33 ถูกดึง Low
    Serial.println("ตั้งค่าให้ตื่นด้วย GPIO (Pin 33)");
    
    Serial.println("เข้าสู่โหมด Deep Sleep...");
    delay(1000); // ให้ Serial Log ถูกส่งออกก่อนนอน
    
    // เข้าสู่โหมด Deep Sleep
    esp_deep_sleep_start();
}

void loop() {
    // ไม่มีอะไรต้องทำใน loop เพราะอุปกรณ์จะเข้าสู่ Deep Sleep
}
```

**คำอธิบาย Code**

1. กำหนด Wake Up Pin

    - กำหนดให้ Pin ที่ใช้ปลุกอุปกรณ์เป็น GPIO33 ผ่าน `esp_sleep_enable_ext0_wakeup(WAKE_UP_PIN, 0)` ซึ่งหมายถึงจะตื่นเมื่อสัญญาณถูกดึง Low (0)

2. Wake Up ด้วย Timer

    - ใช้ `esp_sleep_enable_timer_wakeup(5000000)` เพื่อตั้งค่าให้ตื่นหลังจาก 5 วินาที (จำนวนในฟังก์ชันอยู่ในหน่วยไมโครวินาที)

3. ตรวจสอบสาเหตุการตื่น

    - `esp_sleep_get_wakeup_cause()` ใช้ตรวจสอบว่าการปลุก (Wake Up) เกิดจากสาเหตุใด เช่น จาก Timer หรือ GPIO

4. เข้าสู่ Deep Sleep

    - ใช้คำสั่ง `esp_deep_sleep_start()` เพื่อเข้าสู่โหมด Deep Sleep

5. Serial Log

    - ใช้ `Serial.begin(115200)` และ `Serial.println()` เพื่อพิมพ์ข้อความสำหรับ Debugging (อุปกรณ์จะหยุดการทำงานของ Serial เมื่อเข้าสู่ Deep Sleep)

**ข้อควรระวังในการใช้งาน Deep Sleep**

- **การเชื่อมต่อ GPIO** : เมื่อ ESP เข้าสู่ Deep Sleep GPIO บางพินอาจมีสถานะเปลี่ยนแปลงได้ หากเชื่อมต่อกับอุปกรณ์ภายนอกที่ sensitive อาจต้องพิจารณาการจัดการสถานะ GPIO ก่อนเข้าสู่ Sleep
- **การใช้พลังงานของอุปกรณ์ภายนอก** : อุปกรณ์ภายนอกที่เชื่อมต่อกับ ESP ยังคงใช้พลังงานอยู่ ควรพิจารณาการปิดหรือควบคุมการจ่ายไฟให้กับอุปกรณ์เหล่านั้นเพื่อประหยัดพลังงานโดยรวม
- **เวลาในการตื่น** : การตื่นจาก Deep Sleep จะมี Latency เล็กน้อย ESP จะต้องทำการ Initialize ระบบใหม่ ซึ่งอาจใช้เวลาไม่กี่มิลลิวินาทีถึงหลักสิบมิลลิวินาที ขึ้นอยู่กับการตั้งค่า
