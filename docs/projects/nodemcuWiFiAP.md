# NodeMCU WiFi Access Point

คุณสามารถใช้ WiFi กับ NodeMCU ได้แม้จะไม่มีเราเตอร์ WiFi สิ่งที่คุณต้องทำคือสร้างจุดเชื่อมต่อ WiFi ของ NodeMCU จากนั้นอุปกรณ์จะทำหน้าที่เป็นเกตเวย์ WiFi

## วงจร

- ต่อวงจรจาก pin D7 ไปที่ ขา +LED
- ต่อสายไฟจาก -LED เข้า R10K และ ออกจาก R10K ลงขา GND pin

## Code Program

```cpp
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

// แทนที่ด้วยข้อมูลประจำตัวเครือข่ายของคุณ
const char* ssid = "<YOUR WIFI SSID>";
const char* password = "<YOUR WIFI PASSWORD>";

ESP8266WebServer server(80);   // สร้างอินสแตนซ์เซิร์ฟเวอร์ที่พอร์ต 80 (พอร์ต http) 

String page = "";
int LEDPin = 13;

void setup(void){
    // HTML ของหน้าเว็บ
    page = "<h1>Simple NodeMCU Web Server</h1><p><a href=\"LEDOn\"><button>ON</button></a>&nbsp;<a href=\"LEDOff\"><button>OFF</button></a></p>";
    // ทำให้พิน LED เป็น OUTPUT และ ปิด ไว้ก่อน
    pinMode(LEDPin, OUTPUT);
    digitalWrite(LEDPin, LOW);
    
    delay(1000);
    Serial.begin(115200);
    WiFi.softAP(ssid, password); // เริ่มต้น จุดเชื่อมต่อ WiFi AP
    Serial.println("");

    // Wait for connection
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.softAPIP()); 
    
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
    Serial.println("Web server started!");
}
 
void loop(void){
    server.handleClient();
}
```
คุณจะเห็นว่าเราใช้ `WiFi.softAP()` แทน `WiFi.begin()` (บรรทัดที่ 22) ซึ่งจะสร้างจุดเชื่อมต่อ WiFi ของ NodeMCU จากนั้นพิมพ์ IP ของ NodeMCU ซึ่งเราต้องการเพื่อดูหน้าเซิร์ฟเวอร์ เราใช้  `WiFi.softAPIP()` (บรรทัดที่ 34 )

อัปโหลดโค้ดด้านบนไปยัง NodeMCU ของคุณ หากสำเร็จ อุปกรณ์จะมองเห็นได้เป็นจุดเชื่อมต่อ WiFi พร้อม SSID และรหัสผ่านที่คุณระบุไว้ในโค้ดด้านบน

คุณสามารถใช้เมธอดนี้ในการส่งข้อมูลระหว่าง NodeMCU สองตัวได้ โดยพื้นฐานแล้ว อุปกรณ์หนึ่งทำหน้าที่เป็นไคลเอนต์ อีกอุปกรณ์ทำหน้าที่เป็นเซิร์ฟเวอร์ และทำหน้าที่เป็นจุดเชื่อมต่อ ข้อมูลจะอยู่ระหว่างทั้งสองเท่านั้น ดังนั้นจึงปลอดภัยกว่ามากเมื่อเทียบกับการมีเกตเวย์/เราเตอร์ WiFi

## คำอธิบาย Code

**ส่วนที่ 1 : การ include ไลบรารี**
```cpp
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
```

**ส่วนที่ 2 : ตั้งค่า WiFi และ Web Server**
```cpp
const char* ssid = "<YOUR WIFI SSID>";
const char* password = "<YOUR WIFI PASSWORD>";
```

ชื่อ WiFi ที่ ESP จะปล่อยออกมา (เป็น Access Point ไม่ใช่ไปเชื่อมต่อเราเตอร์)

ผู้ใช้ต้องเปลี่ยน `<YOUR WIFI SSID>` และ `<YOUR WIFI PASSWORD>` เป็นชื่อ WiFi และรหัสผ่านที่ต้องการให้ ESP สร้าง

**ส่วนที่ 3 : สร้าง Web Server**
```cpp
ESP8266WebServer server(80);
```

สร้างอินสแตนซ์ของ Web Server ที่พอร์ต 80 (HTTP ทั่วไป)

**ส่วนที่ 4 : HTML ที่จะแสดง**
```cpp
page = "<h1>Simple NodeMCU Web Server</h1><p><a href=\"LEDOn\"><button>ON</button></a>&nbsp;<a href=\"LEDOff\"><button>OFF</button></a></p>";
```

หน้า HTML ที่จะแสดงปุ่มควบคุม LED บนหน้าเว็บ
    
**ส่วนที่ 5 : `setup()`**

```cpp
pinMode(LEDPin, OUTPUT);
digitalWrite(LEDPin, LOW);
```

ตั้งค่า GPIO13 ให้เป็น OUTPUT และปิด LED ไว้ก่อน

```cpp
WiFi.softAP(ssid, password); // เริ่ม Access Point
```

เริ่มต้น WiFi โหมด Access Point (ESP ปล่อย WiFi เอง)

```cpp
Serial.println(WiFi.softAPIP());
```

แสดง IP Address ของ ESP ในโหมด Access Point (ปกติคือ 192.168.4.1)

**ส่วนที่ 6 : กำหนด Route ให้ Web Server**

```cpp
server.on("/", [](){
server.send(200, "text/html", page);
});
```

เมื่อผู้ใช้เปิดหน้าเว็บ / จะแสดงหน้า HTML ที่ประกาศไว้

```cpp
server.on("/LEDOn", [](){
server.send(200, "text/html", page);
digitalWrite(LEDPin, HIGH);
delay(1000);
});
```

เมื่อกดปุ่ม ON → เปิด LED

```cpp
server.on("/LEDOff", [](){
server.send(200, "text/html", page);
digitalWrite(LEDPin, LOW);
delay(1000); 
});
```

เมื่อกดปุ่ม OFF → ปิด LED

```cpp
server.begin();
```

เริ่มต้น Web Server ให้พร้อมรับคำขอ

**ส่วนที่ 7 : `loop()`**

```
server.handleClient();
```

คอยตรวจสอบว่ามี `client` (เช่น เบราว์เซอร์) เข้ามาใช้งานหรือไม่ และ ตอบกลับตามคำสั่งที่เรากำหนดไว้ใน `server.on(...)`
