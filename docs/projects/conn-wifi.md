# ESP WiFiManager (ESP8266/ESP32)

**ESP WiFiManager เชื่อมต่ออัตโนมัติ จัดการ SSID และ รหัสผ่านของคุณ**

ESP WiFiManager เพื่อสร้าง Web Server ควบคุม GPIO (ขา 4 และ 5) ผ่าน WiFi โดยไม่ต้องเขียน SSID/Password ในโค้ดเอง ใช้ระบบ Smart WiFi Configuration เพื่อให้ผู้ใช้ตั้งค่า WiFi ผ่านหน้าเว็บในตอนเริ่มต้น

**แนวคิดของโครงการนี้**

- ใช้ WiFiManager library เพื่อจัดการ WiFi แบบอัตโนมัติ (AutoConnect)
- ถ้าไม่เคยเชื่อมต่อ WiFi หรือเชื่อมไม่ติด : จะเปิด Access Point ให้ผู้ใช้เชื่อมต่อเข้าไป และตั้งค่า WiFi SSID + Password ผ่านหน้าเว็บ
- เมื่อตั้งค่าเสร็จแล้ว บอร์ดจะเชื่อม WiFi ให้อัตโนมัติ และรันเป็น Web Server ที่ให้ผู้ใช้ควบคุมขา GPIO 4 และ 5 ได้ผ่านหน้าเว็บ

โครงการ WiFiManager เป็นไลบรารีสำหรับ ESP8266 และ ESP32 ที่ช่วยให้การจัดการการเชื่อมต่อ WiFi บนอุปกรณ์ของคุณเป็นเรื่องง่ายและสะดวก โดยมีฟังก์ชันหลักดังนี้ :

- การเชื่อมต่ออัตโนมัติ : เมื่ออุปกรณ์เริ่มต้นทำงาน WiFiManager จะพยายามเชื่อมต่อกับเครือข่าย WiFi ที่เคยบันทึกไว้ หากไม่พบ หรือการเชื่อมต่อล้มเหลว จะเข้าสู่โหมด Access Point (AP) โดยอัตโนมัติ
- Captive Portal : เมื่ออยู่ในโหมด AP ผู้ใช้สามารถเชื่อมต่อกับเครือข่าย WiFi ของ ESP8266 ผ่านอุปกรณ์ใดก็ได้ (เช่น สมาร์ทโฟน แล็ปท็อป) และจะถูกนำไปยังหน้าเว็บ (Captive Portal) เพื่อเลือกเครือข่าย WiFi ที่ต้องการเชื่อมต่อและป้อนรหัสผ่าน
- การจัดการ SSID และรหัสผ่าน : ผู้ใช้สามารถดู แก้ไข หรือลบข้อมูลเครือข่าย WiFi ที่บันทึกไว้ผ่านหน้าเว็บ
- พารามิเตอร์ที่กำหนดเอง : นักพัฒนาสามารถเพิ่มฟิลด์เพิ่มเติมในหน้าเว็บการตั้งค่า WiFi เพื่อให้ผู้ใช้ป้อนค่าต่างๆ ที่จำเป็นสำหรับการทำงานของอุปกรณ์ (เช่น IP Address แบบ Static, API Key, MQTT Broker Address)
- การจัดเก็บข้อมูล : ข้อมูลการตั้งค่า WiFi (SSID, รหัสผ่าน, พารามิเตอร์ที่กำหนดเอง) จะถูกจัดเก็บไว้ใน EEPROM ของ ESP8266 ทำให้เมื่ออุปกรณ์รีบูตยังคงจำการตั้งค่าเดิมได้

ประโยชน์หลักของ WiFiManager :

- ลดความยุ่งยากในการตั้งค่า WiFi : ผู้ใช้ไม่จำเป็นต้องเขียนโค้ด Hardcode สำหรับการเชื่อมต่อ WiFi หรือทำการ Serial Configuration ที่ซับซ้อน
- ความยืดหยุ่นในการใช้งาน : เหมาะสำหรับอุปกรณ์ IoT ที่อาจถูกนำไปติดตั้งในสภาพแวดล้อม WiFi ที่แตกต่างกัน
- ประสบการณ์ผู้ใช้ที่ดี : การตั้งค่า WiFi ผ่านหน้าเว็บเป็นวิธีที่ใช้งานง่ายและคุ้นเคยสำหรับผู้ใช้ทั่วไป

## ข้อกำหนดเบื้องต้น

ขอแนะนำให้อ่านทรัพยากรต่อไปนี้ :

- ติดตั้ง WiFiManager และ ArduinoJSON : คุณต้องติดตั้ง ไลบรารี WiFiManager และ ไลบรารี ArduinoJSON ด้วย ทำตามคำแนะนำต่อไปนี้
    - การติดตั้งไลบรารี WiFiManager
        - เลือกดาวน์โหลดไลบรารี WiFiManager จาก [All_Library](https://github.com/GDevLab/All_Library)
        - แตกไฟล์ .zip แล้วคุณจะได้โฟลเดอร์ WiFiManager-master
        - เปลี่ยนชื่อโฟลเดอร์ของคุณจาก WiFiManager-master เป็น WiFiManager
        - ย้ายโฟลเดอร์ WiFiManager ไปยัง โฟลเดอร์ไลบรารีของคุณ
        - สุดท้ายให้เปิด Arduino IDE ของคุณอีกครั้ง
    - การติดตั้งไลบรารี ArduinoJSON
        - เลือกดาวน์โหลดไลบรารี ArduinoJSON จาก [All_Library](https://github.com/GDevLab/All_Library)
        - แตกไฟล์ .zip แล้วคุณจะได้โฟลเดอร์ ArduinoJSON-master 
        - เปลี่ยนชื่อโฟลเดอร์ของคุณจาก ArduinoJSON-5.13.5 เป็น ArduinoJSON
        - ย้ายโฟลเดอร์ ArduinoJSON ไปยัง โฟลเดอร์ไลบรารีของคุณ
        - สุดท้ายให้เปิด Arduino IDE ของคุณอีกครั้ง

## รายการฮาร์ดแวร์ : 

**นี่คือฮาร์ดแวร์ที่คุณต้องมีเพื่อทำให้โครงการนี้เสร็จสมบูรณ์ :**

- ESP8266/ESP32

## Code Program

```cpp
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String output5State = "off";
String output4State = "off";

// Assign output variables to GPIO pins
const int output5 = 5;
const int output4 = 4;

void setup() {
    Serial.begin(115200);
    
    // Initialize the output variables as outputs
    pinMode(output5, OUTPUT);
    pinMode(output4, OUTPUT);
    // Set outputs to LOW
    digitalWrite(output5, LOW);
    digitalWrite(output4, LOW);

    // WiFiManager
    // Local intialization. Once its business is done, there is no need to keep it around
    WiFiManager wifiManager;
    
    // Uncomment and run it once, if you want to erase all the stored information
    //wifiManager.resetSettings();
    
    // set custom ip for portal
    //wifiManager.setAPConfig(IPAddress(10,0,1,1), IPAddress(10,0,1,1), IPAddress(255,255,255,0));

    // fetches ssid and pass from eeprom and tries to connect
    // if it does not connect it starts an access point with the specified name
    // here  "AutoConnectAP"
    // and goes into a blocking loop awaiting configuration
    wifiManager.autoConnect("AutoConnectAP");
    // or use this for auto generated name ESP + ChipID
    //wifiManager.autoConnect();
    
    // if you get here you have connected to the WiFi
    Serial.println("Connected.");
    
    server.begin();
}

void loop(){
    WiFiClient client = server.available();   // Listen for incoming clients

    if (client) {                             // If a new client connects,
        Serial.println("New Client.");        // print a message out in the serial port
        String currentLine = "";              // make a String to hold incoming data from the client
        while (client.connected()) {          // loop while the client's connected
            if (client.available()) {         // if there's bytes to read from the client,
                char c = client.read();       // read a byte, then
                Serial.write(c);              // print it out the serial monitor
                header += c;
                if (c == '\n') {              // if the byte is a newline character
                    // if the current line is blank, you got two newline characters in a row.
                    // that's the end of the client HTTP request, so send a response:
                    if (currentLine.length() == 0) {
                        // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
                        // and a content-type so the client knows what's coming, then a blank line:
                        client.println("HTTP/1.1 200 OK");
                        client.println("Content-type:text/html");
                        client.println("Connection: close");
                        client.println();
                        
                        // turns the GPIOs on and off
                        if (header.indexOf("GET /5/on") >= 0) {
                        Serial.println("GPIO 5 on");
                        output5State = "on";
                        digitalWrite(output5, HIGH);
                        } else if (header.indexOf("GET /5/off") >= 0) {
                        Serial.println("GPIO 5 off");
                        output5State = "off";
                        digitalWrite(output5, LOW);
                        } else if (header.indexOf("GET /4/on") >= 0) {
                        Serial.println("GPIO 4 on");
                        output4State = "on";
                        digitalWrite(output4, HIGH);
                        } else if (header.indexOf("GET /4/off") >= 0) {
                        Serial.println("GPIO 4 off");
                        output4State = "off";
                        digitalWrite(output4, LOW);
                        }
                        
                        // Display the HTML web page
                        client.println("<!DOCTYPE html><html>");
                        client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
                        client.println("<link rel=\"icon\" href=\"data:,\">");
                        // CSS to style the on/off buttons 
                        // Feel free to change the background-color and font-size attributes to fit your preferences
                        client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
                        client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
                        client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
                        client.println(".button2 {background-color: #77878A;}</style></head>");
                        
                        // Web Page Heading
                        client.println("<body><h1>ESP8266 Web Server</h1>");
                        
                        // Display current state, and ON/OFF buttons for GPIO 5  
                        client.println("<p>GPIO 5 - State " + output5State + "</p>");
                        // If the output5State is off, it displays the ON button       
                        if (output5State=="off") {
                        client.println("<p><a href=\"/5/on\"><button class=\"button\">ON</button></a></p>");
                        } else {
                        client.println("<p><a href=\"/5/off\"><button class=\"button button2\">OFF</button></a></p>");
                        } 
                        
                        // Display current state, and ON/OFF buttons for GPIO 4  
                        client.println("<p>GPIO 4 - State " + output4State + "</p>");
                        // If the output4State is off, it displays the ON button       
                        if (output4State=="off") {
                        client.println("<p><a href=\"/4/on\"><button class=\"button\">ON</button></a></p>");
                        } else {
                        client.println("<p><a href=\"/4/off\"><button class=\"button button2\">OFF</button></a></p>");
                        }
                        client.println("</body></html>");
                        
                        // The HTTP response ends with another blank line
                        client.println();
                        // Break out of the while loop
                        break;
                    } else { // if you got a newline, then clear currentLine
                        currentLine = "";
                    }
                } else if (c != '\r') {  // if you got anything else but a carriage return character,
                currentLine += c;        // add it to the end of the currentLine
                }
            }
        }
        // Clear the header variable
        header = "";
        // Close the connection
        client.stop();
        Serial.println("Client disconnected.");
        Serial.println("");
    }
}
```

## คำอธิบาย Code

**ส่วนประกอบหลักของโค้ด :**

**Include Libraries :**

- `ESP8266WiFi.h` : สำหรับการจัดการการเชื่อมต่อ WiFi บน ESP8266
- `DNSServer.h` : สำหรับการทำงานของ DNS Server ในโหมด Access Point เพื่อ Redirect ทุกการเข้าชมเว็บไปยัง Captive Portal
- `ESP8266WebServer.h` : สำหรับสร้าง Web Server บน ESP8266 เพื่อแสดงหน้าเว็บและจัดการ HTTP Request
- `WiFiManager.h` : ไลบรารีหลักสำหรับจัดการการเชื่อมต่อ WiFi อัตโนมัติและ Captive Portal

**Web Server Setup :**

- `WiFiServer server(80);` : สร้างอ็อบเจ็กต์ server สำหรับ Web Server ที่ทำงานบนพอร์ต 80 (HTTP)

**Variables :**

- `String header;` : ใช้เก็บ HTTP Request ที่เข้ามาจาก Client
- `String output5State = "off";` : เก็บสถานะปัจจุบันของ GPIO5 (เริ่มต้นเป็น "off")
- `String output4State = "off";` : เก็บสถานะปัจจุบันของ GPIO4 (เริ่มต้นเป็น "off")
- `const int output5 = 5;` : กำหนดหมายเลขพินสำหรับ GPIO5
- `const int output4 = 4;` : กำหนดหมายเลขพินสำหรับ GPIO4

**void setup() :**

- `Serial.begin(115200);` : เริ่มต้นการสื่อสาร Serial ที่ความเร็ว 115200 bps สำหรับการ Debug
- `pinMode(output5, OUTPUT);` : กำหนดให้ GPIO5 เป็นขาออก
- `pinMode(output4, OUTPUT);` : กำหนดให้ GPIO4 เป็นขาออก
- `digitalWrite(output5, LOW);` : ตั้งค่าเริ่มต้นให้ GPIO5 เป็น LOW (ปิด)
- `digitalWrite(output4, LOW);` : ตั้งค่าเริ่มต้นให้ GPIO4 เป็น LOW (ปิด)
- `WiFiManager wifiManager;` : สร้างอ็อบเจ็กต์ wifiManager
- `// wifiManager.resetSettings();` : บรรทัดนี้ถูก Comment ไว้ หากต้องการล้างข้อมูล WiFi ที่บันทึกไว้ใน EEPROM ให้ Uncomment และรันโค้ดนี้เพียงครั้งเดียว
- `// wifiManager.setAPConfig(IPAddress(10,0,1,1), IPAddress(10,0,1,1), IPAddress(255,255,255,0));` : บรรทัดนี้ถูก Comment ไว้ หากต้องการกำหนดค่า IP Address, Gateway, และ Subnet Mask สำหรับ Access Point เอง ให้ Uncomment และแก้ไขค่า
- `wifiManager.autoConnect("AutoConnectAP");` : ฟังก์ชันหลักของ WiFiManager:
    - พยายามเชื่อมต่อกับเครือข่าย WiFi ที่เคยบันทึกไว้ใน EEPROM
    - หากไม่พบ หรือเชื่อมต่อล้มเหลว จะสร้าง Access Point ชื่อ "AutoConnectAP"
    - จะอยู่ใน Loop ที่ Blocking (รอ) จนกว่าจะมีการเชื่อมต่อ WiFi สำเร็จผ่าน Captive Portal
- `// wifiManager.autoConnect();` : อีกทางเลือกหนึ่ง autoConnect() จะสร้าง AP โดยใช้ชื่อเริ่มต้นเป็น "ESP" ตามด้วย Chip ID ของ ESP8266
- `Serial.println("Connected.");` : แสดงข้อความเมื่อเชื่อมต่อ WiFi สำเร็จ
- `server.begin();` : เริ่มต้น Web Server

**void loop():**

- `WiFiClient client = server.available();` : ตรวจสอบว่ามี Client ใหม่พยายามเชื่อมต่อมายัง Web Server หรือไม่ หากมี จะคืนอ็อบเจ็กต์ `WiFiClient`
- `if (client)` : หากมี Client เชื่อมต่อเข้ามา :
    - `Serial.println("New Client.");` : แสดงข้อความว่ามี Client ใหม่เชื่อมต่อ
    - `String currentLine = "";` : สร้าง String เพื่อเก็บข้อมูลที่รับมาจาก Client ทีละบรรทัด
    - `while (client.connected())` : Loop ตราบใดที่ Client ยังคงเชื่อมต่ออยู่
        - `if (client.available())` : ตรวจสอบว่ามีข้อมูลที่ Client ส่งมาหรือไม่
            - `char c = client.read();` : อ่านข้อมูลจาก Client ทีละ Byte
            - `Serial.write(c);` : แสดง Byte ที่อ่านได้ทาง Serial Monitor
            - `header += c;` : เพิ่ม Byte ที่อ่านได้ลงใน String `header` เพื่อเก็บ HTTP Request ทั้งหมด
            - `if (c == '\n')` : ตรวจสอบว่าเป็น Character ขึ้นบรรทัดใหม่ (`\n`) หรือไม่ ซึ่งเป็นสัญญาณสิ้นสุดของบรรทัดใน HTTP Header
                - `if (currentLine.length() == 0)` : ตรวจสอบว่าเป็นบรรทัดว่างหรือไม่ (สอง `\n` ติดกัน) ซึ่งเป็นสัญญาณสิ้นสุดของ HTTP Header
                    - `client.println("HTTP/1.1 200 OK");` : ส่ง HTTP Response Header กลับไปยัง Client (สถานะ OK)
                    - `client.println("Content-type:text/html");` : ระบุประเภท Content เป็น HTML
                    - `client.println("Connection: close");` : บอกให้ Client ปิด Connection หลังจากได้รับ Response
                    - `client.println();` : ส่งบรรทัดว่างเพื่อสิ้นสุด HTTP Header
                    - ควบคุม GPIO ตาม HTTP Request :
                        - `if (header.indexOf("GET /5/on") >= 0)` : ตรวจสอบว่าใน HTTP Request Header มี String "GET /5/on" หรือไม่ ถ้ามี แสดงว่า Client ต้องการเปิด GPIO5 :
                            - `Serial.println("GPIO 5 on");`
                            - `output5State = "on";`
                            - `digitalWrite(output5, HIGH);` : สั่งให้ GPIO5 เป็น HIGH (เปิด)
                        - `else if (header.indexOf("GET /5/off") >= 0)` : ตรวจสอบว่ามี "GET /5/off" หรือไม่ ถ้ามี แสดงว่าต้องการปิด GPIO5
                            - `Serial.println("GPIO 5 off");`
                            - `output5State = "off";`
                            - `digitalWrite(output5, LOW);` : สั่งให้ GPIO5 เป็น LOW (ปิด)
                        - `else if (header.indexOf("GET /4/on") >= 0)` : ทำเช่นเดียวกันสำหรับ GPIO4 ในการเปิด
                            - `Serial.println("GPIO 4 on");`
                            - `output4State = "on";`
                            - `digitalWrite(output4, HIGH);`
                        - `else if (header.indexOf("GET /4/off") >= 0)` : ทำเช่นเดียวกันสำหรับ GPIO4 ในการปิด
                            - `Serial.println("GPIO 4 off");`
                            - `output4State = "off";`
                            - `digitalWrite(output4, LOW);`
                    - สร้างและส่งหน้าเว็บ HTML :
                        - ส่วนนี้สร้างหน้าเว็บ HTML ที่แสดงสถานะปัจจุบันของ `GPIO5` และ `GPIO4` พร้อมปุ่ม `ON/OFF` ที่มี Link ไปยัง `/5/on`, `/5/off`, `/4/on`, `/4/off` เพื่อควบคุม `GPIO`
                    - `client.println();` : ส่งบรรทัดว่างเพื่อสิ้นสุด `HTTP Response`
                    - `break;` : ออกจาก `while (client.connected())` Loop เนื่องจากส่ง Response กลับไปแล้ว
                - `else` : ถ้ายังไม่เจอสอง `\n` ติดกัน แสดงว่ายังอ่าน HTTP Header ไม่หมด ให้เคลียร์ `currentLine` เพื่ออ่านบรรทัดใหม่
                    - `currentLine = "";`
        - `else if (c != '\r')` : หาก Character ที่อ่านไม่ใช่ Carriage Return (`\r`) ให้เพิ่มลงใน `currentLine`
            - `currentLine += c;`
- `header = "";` : เคลียร์ `header` เพื่อรอ HTTP Request ใหม่
- `client.stop();` : ปิด Connection กับ Client
- `Serial.println("Client disconnected.");`
- `Serial.println("");`

**สรุปการทำงาน :**

- เมื่อ ESP8266 เริ่มทำงาน `setup()` จะถูกเรียก
- WiFiManager จะพยายามเชื่อมต่อกับ WiFi ที่เคยบันทึกไว้ หากไม่สำเร็จ จะสร้าง Access Point ชื่อ "AutoConnectAP"
- ผู้ใช้สามารถเชื่อมต่อกับ AP นี้และจะถูกนำไปยัง Captive Portal เพื่อเลือก WiFi และป้อนรหัสผ่าน
- เมื่อเชื่อมต่อ WiFi สำเร็จ `loop()` จะเริ่มทำงาน
- ใน `loop()` ESP8266 จะรอ Client เชื่อมต่อมายัง Web Server (ที่ IP Address ของ ESP8266 บนเครือข่าย WiFi)
- เมื่อมี Client เชื่อมต่อเข้ามา ESP8266 จะอ่าน HTTP Request ที่ Client ส่งมา
- หากใน HTTP Request มี Path `/5/on`, `/5/off`, `/4/on`, หรือ `/4/off` ESP8266 จะทำการเปิดหรือปิด GPIO5 หรือ GPIO4 ตามคำสั่ง
- จากนั้น ESP8266 จะสร้างและส่งหน้าเว็บ HTML กลับไปยัง Client เพื่อแสดงสถานะปัจจุบันของ GPIO และมีปุ่มควบคุม
- Client สามารถคลิกที่ปุ่ม ON/OFF บนหน้าเว็บเพื่อส่ง HTTP Request ไปยัง ESP8266 และควบคุม GPIO ได้

โค้ดนี้แสดงให้เห็นถึงการทำงานร่วมกันของ WiFiManager ในการจัดการการเชื่อมต่อ WiFi และ Web Server ในการสร้าง Interface สำหรับควบคุมฮาร์ดแวร์ของ ESP8266 ผ่านเครือข่าย WiFi
