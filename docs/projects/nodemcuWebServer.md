# NodeMCU Web Server

นี่จะแสดงวิธีสร้าง Web Server NodeMCU ง่ายๆ ที่จะควบคุม LED ที่ติดอยู่กับพินใดพินหนึ่งของ NodeMCU ฟังดูน่าสนใจไหม?

## วงจร

- ต่อวงจรจาก pin D7 ไปที่ ขา +LED
- ต่อสายไฟจาก -LED เข้า R10K และ ออกจาก R10K ลงขา GND pin

## Code Program

นี่คือโค้ดสำหรับเว็บเซิร์ฟเวอร์ nodemcu ที่เรียบง่ายของเรา
```cpp
#include <ESP8266WiFi.h> 
#include <WiFiClient.h> 
#include <ESP8266WebServer.h> 

// แทนที่ด้วยข้อมูลประจำตัวเครือข่ายของคุณ
const char* ssid = "<YOUR WIFI SSID>"; 
const char* password = "<YOUR WIFI PASSWORD>"; 

ESP8266WebServer server(80); // สร้างอินสแตนซ์เซิร์ฟเวอร์ที่พอร์ต 80 (พอร์ต http) 

String page = ""; 
int LEDPin = 13; 
void setup(void){ 
    //HTML ของหน้าเว็บ
    page = "<h1>Simple NodeMCU Web Server</h1><p><a href=\"LEDOn\"><button>ON</button></a> <a href=\"LEDOff\"><button>OFF</button></a></p>"; 
    // ทำให้พิน LED ส่งออกและปิดในเบื้องต้น
    pinMode(LEDPin, OUTPUT); 
    digitalWrite(LEDPin, LOW); 
    
    delay(1000); 
    Serial.begin(115200); 
    WiFi.begin(ssid, password); // เริ่มการเชื่อมต่อ WiFi 
    Serial.println(""); 

    // รอการเชื่อมต่อ
    while (WiFi.status() != WL_CONNECTED) { 
        delay(500); 
        Serial.print("."); 
    } 
    Serial.println("") 
    ; Serial.print("เชื่อมต่อกับ ") 
    ; Serial.println(ssid) 
    ; Serial.print("ที่อยู่ IP: "); 
    Serial.println(WiFi.localIP()); 
    
    server.on("/", [](){ 
        server.send(200, "text/html", page); 
    }); 
    server.on("/LEDOn", [](){ 
        server.send(200, "text/html", page); 
        digitalWrite(LEDPin, HIGH); 
        delay(1000); 
    }); 
    server.on("/LEDOff", [](){ 
        server.send(200, "text/html", page); 
        digitalWrite(LEDPin, LOW); 
        delay(1000); 
    }); 
    server.begin(); 
    Serial.println("เว็บเซิร์ฟเวอร์เริ่มทำงานแล้ว!"); 
} 
 
void loop(void){ 
    server.handleClient(); 
}
```
## คำอธิบาย Code

**ส่วนที่ 1 : การ include ไลบรารี**
```cpp
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
```
ไลบรารีสำหรับ :

- การเชื่อมต่อ WiFi (ESP8266WiFi.h)
- การใช้งาน Web Server (ESP8266WebServer.h)

**ส่วนที่ 2 : ตั้งค่า WiFi และ Web Server**
```cpp
const char* ssid = "<YOUR WIFI SSID>";
const char* password = "<YOUR WIFI PASSWORD>";
ESP8266WebServer server(80); // ใช้พอร์ต 80 สำหรับ HTTP
```
เปลี่ยน "`<YOUR WIFI SSID>`" และ "`<YOUR WIFI PASSWORD>`" ให้เป็น WiFi ที่คุณต้องการเชื่อมต่อ

สร้าง Web Server ที่พอร์ต 80 (พอร์ตปกติของ HTTP)

**ส่วนที่ 3 : สร้างหน้า HTML**
```cpp
String page = "<h1>Simple NodeMCU Web Server</h1><p><a href=\"LEDOn\"><button>ON</button></a> <a href=\"LEDOff\"><button>OFF</button></a></p>";
```
HTML ที่แสดงบนเว็บเมื่อเข้าหน้าเว็บเซิร์ฟเวอร์

ปุ่ม `<button>` มีลิงก์ไปที่ `/LEDOn` และ `/LEDOff` เพื่อควบคุม LED

- **ส่วน `setup()`**
```cpp
pinMode(LEDPin, OUTPUT);
digitalWrite(LEDPin, LOW);
```
ตั้ง `GPIO13 (LEDPin)` เป็น `OUTPUT` และปิดไว้ก่อน (`LOW`)

- **เชื่อมต่อ WiFi**
```cpp
WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
}
Serial.println("เชื่อมต่อกับ WiFi แล้ว!");
```
เชื่อมต่อ WiFi และรอจนเชื่อมต่อสำเร็จ

แสดง IP Address ที่ได้รับผ่าน Serial Monitor

- **กำหนด route ให้ Web Server**
```cpp
server.on("/", [](){ 
server.send(200, "text/html", page); 
});
```

- **เมื่อผู้ใช้เปิดหน้า / (หน้าแรก) → แสดงหน้า HTML ที่เราสร้างไว้**
```cpp
server.on("/LEDOn", [](){ 
server.send(200, "text/html", page); 
digitalWrite(LEDPin, HIGH); 
});
```

- **เมื่อเปิดลิงก์ `/LEDOn` → เปิด LED**
```cpp
server.on("/LEDOff", [](){ 
server.send(200, "text/html", page); 
digitalWrite(LEDPin, LOW); 
});
```
- **เมื่อเปิดลิงก์ `/LEDOff` → ปิด LED**

- **ส่วน `loop()`**
```cpp
server.handleClient();
```
บอร์ดจะคอยรอ client (เช่น browser) เข้ามาเรียกใช้งาน และตอบกลับตามที่เรากำหนดไว้

**สรุปภาพรวมของการทำงาน**

- เปิดบอร์ด	
- เชื่อมต่อ WiFi
- เปิดเว็บผ่าน IP ที่ Serial แจ้ง	
- แสดงหน้าเว็บที่มีปุ่ม ON/OFF
- กดปุ่ม ON หรือ OFF	
- ESP ส่งคำสั่งไปยัง GPIO13 เพื่อเปิด/ปิด LED

**ข้อควรระวัง**

- GPIO13 = D7 บน NodeMCU (ตรวจให้แน่ใจว่าต่อ LED กับขานี้ หรือแก้ให้ตรงกับขาที่คุณใช้)
- โค้ดยังไม่มีระบบรักษาความปลอดภัย (เช่นรหัสผ่านเว็บ), เหมาะสำหรับทดลอง/เรียนรู้
- หากใช้ WiFi 5GHz จะเชื่อมต่อไม่ติด เพราะ ESP8266 รองรับแค่ 2.4GHz
