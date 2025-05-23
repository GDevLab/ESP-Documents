# ESP8266 Remote Sockets GPIO

ในโครงการนี้ คุณจะสร้างเว็บเซิร์ฟเวอร์ด้วย ESP8266 ที่สามารถควบคุมซ็อกเก็ตต่างๆ จากระยะไกลได้ (อย่างปลอดภัย)

## ข้อกำหนดเบื้องต้น 

ขอแนะนำให้อ่านทรัพยากรต่อไปนี้ :

- ติดตั้งไลบรารี RC Switch ทำตามคำแนะนำต่อไปนี้
    - การติดตั้งไลบรารี RC Switch
        - เลือกดาวน์โหลดไลบรารี RC Switch จาก [All_Library](https://github.com/GDevLab/All_Library) 
        - แตกไฟล์ .zip แล้วคุณจะได้โฟลเดอร์ rc-switch-master
        - เปลี่ยนชื่อโฟลเดอร์ของคุณจาก rc-switch-master เป็น rc-switch
        - ย้ายโฟลเดอร์ rc-switch ไปยัง โฟลเดอร์ไลบรารีของคุณ
        - สุดท้ายให้เปิด Arduino IDE ของคุณอีกครั้ง

## รายการฮาร์ดแวร์ : 

**นี่คือฮาร์ดแวร์ที่คุณต้องมีเพื่อทำให้โครงการนี้เสร็จสมบูรณ์ :**

- ESP8266/ESP32
- FT232RL FTDI Programmer
- Arduino UNO
- RF 433MHz Transmitter/Receiver
- RF Remote Controlled Sockets 433MHz
- สายจั๊มเปอร์

## Code Program

Arduino UNO

```cpp
#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();

void setup() {
  Serial.begin(9600);
  mySwitch.enableReceive(0);  // Receiver on inerrupt 0 => that is pin #2
}

void loop() {
  if (mySwitch.available()) {
    output(mySwitch.getReceivedValue(), mySwitch.getReceivedBitlength(), mySwitch.getReceivedDelay(), mySwitch.getReceivedRawdata(),mySwitch.getReceivedProtocol());
    mySwitch.resetAvailable();
  }
}
```
ESP8266/ESP32
```cpp
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();
MDNSResponder mdns;

// Replace with your network credentials
const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

ESP8266WebServer server(80);

// Replace with your remote TriState values
char* socket1TriStateOn = "0FFF0FFFFFFF";
char* socket1TriStateOff = "0FFF0FFFFFF0";
char* socket2TriStateOn = "0FFFFF0FFFFF";
char* socket2TriStateOff = "0FFFFF0FFFF0";

String webPage = "";
 
void setup(void){
  webPage += "<h1>ESP8266 Web Server</h1><p>Socket #1 <a href=\"socket1On\"><button>ON</button></a>&nbsp;<a href=\"socket1Off\"><button>OFF</button></a></p>";
  webPage += "<p>Socket #2 <a href=\"socket2On\"><button>ON</button></a>&nbsp;<a href=\"socket2Off\"><button>OFF</button></a></p>";
  mySwitch.enableTransmit(2);
  delay(1000);
  Serial.begin(115200);
  WiFi.begin(ssid, password);
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
  Serial.println(WiFi.localIP());
  
  if (mdns.begin("esp8266", WiFi.localIP())) {
    Serial.println("MDNS responder started");
  }
  
  server.on("/", [](){
    server.send(200, "text/html", webPage);
  });
  server.on("/socket1On", [](){
    server.send(200, "text/html", webPage);
    mySwitch.sendTriState(socket1TriStateOn);
    delay(1000);
  });
  server.on("/socket1Off", [](){
    server.send(200, "text/html", webPage);
    mySwitch.sendTriState(socket1TriStateOff);
    delay(1000); 
  });
  server.on("/socket2On", [](){
    server.send(200, "text/html", webPage);
    mySwitch.sendTriState(socket2TriStateOn);
    delay(1000);
  });
  server.on("/socket2Off", [](){
    server.send(200, "text/html", webPage);
    mySwitch.sendTriState(socket2TriStateOff);
    delay(1000); 
  });
  server.begin();
  Serial.println("HTTP server started");
}
 
void loop(void){
  server.handleClient();
} 
```

## คำอธิบาย Code

