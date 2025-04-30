# สถานีตรวจอากาศพลังงานแสงอาทิตย์

สถานีตรวจอากาศแบบ DIY ที่ใช้ ESP32 รองรับ Home Assistant เต็มรูปแบบผ่านการผสานรวม MQTT Discovery 

โครงการนี้ใช้คุณสมบัติโหมดหลับลึกของ ESP32 เพื่อลดการใช้แบตเตอรี่ และเครื่องจะตื่นขึ้นและดึงข้อมูลเซ็นเซอร์ทุกๆ 5 นาที

## คุณสมบัติ

- พลังงานต่ำ : สามารถทำงานได้โดยไม่ต้องใช้แสงแดดเป็นเวลาอย่างน้อย 1 สัปดาห์ในกรณีฤดูฝน
- กันน้ำได้อย่างสมบูรณ์ : ช่องแยกระหว่างส่วนเซ็นเซอร์และส่วน MCU
- ทำงานร่วมกับ Home Assistant : สถานีตรวจอากาศจะถูกเพิ่มเข้าไปใน Home Assistant โดยอัตโนมัติผ่านการค้นพบ MQTT
- Adaptive sleeping : sleep 5 นาทีในสภาพปกติ 30 นาทีเมื่อแบตเตอรี่เหลือต่ำกว่า 30% และ หยุดทำงานโดยสมบูรณ์หากแบตเตอรี่ถึงระดับวิกฤต

## ส่วนประกอบ

- บอร์ดพัฒนา ESP32
- โมดูลชาร์จ MPPT CN3791
- แผงโซล่าเซลล์ 6V 6W
- เซ็นเซอร์อุณหภูมิและความชื้น SHT3X
- เซ็นเซอร์ตรวจจับอนุภาค PMS3003
- วงจรสวิตชิ่งเซนเซอร์ PMS
- ทรานซิสเตอร์ BC337
- ตัวต้านทาน 330Ω
- แบตเตอรี่ลิเธียมไออน 1x 18650
- ที่ใส่แบตเตอรี่ 18650
- กล่องต่อสาย 2x NANO-201CW 2x4x2"
- PCB, ขั้วต่อ, สายไฟ ฯลฯ

## แผนผังการเชื่อมต่อ

## แพลตฟอร์มและสิ่งที่ต้องพึ่งพา

โค้ดต้นฉบับเขียนบน Arduino IDE

**Library ที่ใช้ :**

- PubSubClient
- ArduinoHA
- PMS
- Arduino SHT

## บันทึกการเปลี่ยนแปลง

- 2023.4 : เปลี่ยนเซนเซอร์อุณหภูมิและความชื้นเป็นเซนเซอร์ SHT
- ใช้โมดูลเซ็นเซอร์ SHT3x แทน DHT22 ซึ่งจะช่วยเพิ่มเสถียรภาพและความแม่นยำได้อย่างมาก
- เดินสายกล่องส่วนประกอบเซ็นเซอร์ใหม่โดยใช้สาย I2C (แทนที่จะใช้โปรโตคอลสายเดียวของ DHT22)
- ใช้สายดินทั่วไปสำหรับเซ็นเซอร์ PMS และเซ็นเซอร์ SHT
- ใช้แบตเตอรี่ 18650 เพียงก้อนเดียวแทนที่จะใช้สองก้อน (แบตเตอรี่หนึ่งก้อนสามารถใช้งานได้นานกว่าหนึ่งสัปดาห์โดยไม่ต้องชาร์จ)

ภาพหน้าจอ

หน้าบูรณาการ Home Assistant

Grafana

จากข้อมูล SoC ของแบตเตอรี่ขั้นต่ำอยู่ที่ประมาณ 95% แม้ว่าจะมีเมฆครึ้มเป็นเวลาหลายวัน เมื่อมองย้อนกลับไป การใช้แบตเตอรี่ 18650 เพียงก้อนเดียวก็น่าจะเพียงพอ

## code

```c++
// #include <Arduino.h>     // ไม่ได้ใช้งาน (ปกติ Arduino IDE จะรวมอัตโนมัติ)
#include <WiFi.h>           // สำหรับการเชื่อมต่อ WiFi
#include <PubSubClient.h>   // ไลบรารี MQTT
#include <ArduinoHA.h>      // สำหรับการทำงานกับ Home Assistant
#include <PMS.h>            // ไลบรารีเซ็นเซอร์วัดฝุ่น
#include <HardwareSerial.h> // สำหรับการสื่อสารแบบ Serial
#include <esp_task_wdt.h>   // ระบบ Watchdog Timer ของ ESP32
#include <Wire.h>           // สำหรับการสื่อสาร I2C
#include "SHTSensor.h"      // เซ็นเซอร์อุณหภูมิ/ความชื้น

// การกำหนดค่าคงที่ (Constants)
#define WIFI_STA_NAME "YOUR-WIFI-NAME"          // WiFi SSID
#define WIFI_STA_PASS "YOUR-WIFI-PASSWORD"      // WiFi password
// ข้อมูลสำหรับเชื่อมต่อกับ MQTT Broker
#define MQTT_SERVER   "MQTT-SERVER-IP"          // MQTT broker IP
#define MQTT_PORT     1883                      // MQTT port
#define MQTT_USERNAME "MQTT-SERVER-USERNAME"    // MQTT credentials
#define MQTT_PASSWORD "MQTT-SERVER-PASSWORD"    // MQTT credentials
#define MQTT_NAME     "Solar_weather_station"   // Device name
// ตัวกำหนดเวลาสำหรับ deep sleep และ watchdog
#define DEEP_SLEEP_NORMAL 5                     // Deep sleep for 5 mins if battery sufficient
#define DEEP_SLEEP_LOW 30                       // Deep sleep for 30 mins if battery is running low
#define DEEP_SLEEP_EMPTY 120                    // Deep sleep for 2 hr if battery ran out.
#define WIFI_TIMEOUT  30                        // Do not connect Wi-Fi more than 30 seconds
#define WDT_TIMEOUT 120                         // All process should be finished within 2 minutes.
#define STA_ID "WeatherStation_1"               // Station ID
// กำหนดค่าคงที่สำหรับ Pins GPIO ที่เชื่อมต่อกับอุปกรณ์ต่างๆ
#define LED_BUILTIN 5                           // ขาที่เชื่อมต่อกับ LED Built-in บน ESP32
#define DHT22_PIN 33                            // ขาที่เชื่อมต่อกับเซ็นเซอร์ DHT22
#define BATT_PIN 35                             // ขา Analog Input ที่ใช้สำหรับอ่านค่าแรงดันแบตเตอรี่
#define SENSOR_EN_PIN 13                        // ขา Digital Output ที่ใช้สำหรับเปิด/ปิดการทำงานของเซ็นเซอร์อื่นๆ (PMS และ SHT)
#define TX2_PIN 25                              // ขา TX (Transmit) ใช้สำหรับการสื่อสารกับเซ็นเซอร์ PMS
#define RX2_PIN 26                              // ขา RX (Receive) ใช้สำหรับการสื่อสารกับเซ็นเซอร์ PMS
#define CHARGE_EN 27                            // ขา Digital Output ที่อาจใช้สำหรับควบคุมการชาร์จแบตเตอรี่

// ประกาศตัวแปร WiFi และ MQTT
/*  
*   บรรทัดแรกสร้าง Object ชื่อ client จาก Class WiFiClient ซึ่งใช้สำหรับการจัดการการเชื่อมต่อ TCP/IP ผ่าน Wi-Fi 
*   บรรทัดที่สองเป็นการ Comment Out การสร้าง Object ชื่อ mqtt จาก Class PubSubClient ซึ่งหมายความว่าไม่ได้ใช้งานโดยตรง
*   แต่มีการใช้งานไลบรารี ArduinoHA ซึ่งอาจจะมีการสร้าง PubSubClient Object ภายใน
*/
WiFiClient client; // 
// PubSubClient mqtt(client);

// ตัวแปรสำหรับจัดการ MQTT และ Home Assistant objects
HADevice *haDevice;         // Pointer ไปยัง Object ของอุปกรณ์ Home Assistant
HAMqtt *haMQTT;             // Pointer ไปยัง Object สำหรับจัดการการเชื่อมต่อ MQTT และการส่งข้อมูลไปยัง Home Assistant
HASensor *haSensor_vBatt;   // Object Battery voltage sensor
HASensor *haSensor_SoC;     // Object State of Charge sensor
HASensor *haSensor_PM1_0;   // Object PM1.0 sensor
HASensor *haSensor_PM2_5;   // Object PM2.5 sensor
HASensor *haSensor_PM10_0;  // Object PM10 sensor
HASensor *haSensor_temp;    // Object Temperature sensor
HASensor *haSensor_humid;   // Object Humidity sensor

//Sensors
PMS pms(Serial2);                   // Particle sensor connected to Serial2 PMS sensor ใช้ UART2
PMS::DATA data;                     // Structure for particle data
SHTSensor sht(SHTSensor::SHT3X);    // Temp/Humidity sensor

// Sensor value variables ประกาศตัวแปรสำหรับเก็บค่าที่อ่านได้จากเซ็นเซอร์ต่างๆ
/*
*   vBatt : ตัวแปร Float สำหรับเก็บค่าแรงดันแบตเตอรี่
*   SoC : ตัวแปร Integer สำหรับเก็บค่าสถานะแบตเตอรี่ (State of Charge) เป็นเปอร์เซ็นต์
*   pmsOK : ตัวแปร Boolean สำหรับเก็บสถานะการทำงานของเซ็นเซอร์ PMS (true = ทำงานปกติ, false = มีปัญหา)
*   PM1_0, PM2_5, PM10_0 : ตัวแปร Integer สำหรับเก็บค่าฝุ่นละออง PM1.0, PM2.5 และ PM10.0 ที่อ่านได้จากเซ็นเซอร์ PMS
*   shtOK : ตัวแปร Boolean สำหรับเก็บสถานะการทำงานของเซ็นเซอร์ SHT (true = ทำงานปกติ, false = มีปัญหา)
*   temp : ตัวแปร Float สำหรับเก็บค่าอุณหภูมิที่อ่านได้จากเซ็นเซอร์ SHT
*   humid : ตัวแปร Float สำหรับเก็บค่าความชื้นที่อ่านได้จากเซ็นเซอร์ SHT
*/
float vBatt = 0;
int SoC = 100;

bool pmsOK = false;
int PM1_0 = 0;
int PM2_5 = 0;
int PM10_0 = 0;

bool shtOK = false;
float temp = 0;
float humid = 0;

// สร้าง Enumeration (enum) ชื่อ BatteryCondition ซึ่งกำหนดค่าที่เป็นไปได้สำหรับสถานะแบตเตอรี่ : 
// BATT_NORMAL, BATT_LOW, และ BATT_EMPTY
enum BatteryCondition {BATT_NORMAL, BATT_LOW, BATT_EMPTY};

// ฟังก์ชัน deepSleep
// มีหน้าที่นำ ESP32 เข้าสู่ Deep Sleep Mode เพื่อประหยัดพลังงาน 
// โดยจะรับค่า BatteryCondition เป็น Argument เพื่อกำหนดระยะเวลาในการ Sleep
void deepSleep(BatteryCondition BatteryCondition) {
    digitalWrite(LED_BUILTIN,LOW);

    // โครงสร้าง switch จะตรวจสอบค่า BatteryCondition และกำหนดระยะเวลา Wake-up Timer 
    // โดยใช้ esp_sleep_enable_timer_wakeup() ซึ่งรับค่าระยะเวลาเป็น Microseconds (จึงมีการคูณด้วย 60 เพื่อแปลงเป็นนาที และ 1000000 เพื่อแปลงเป็น Microseconds) 
    // ตามค่าคงที่ที่กำหนดไว้ (DEEP_SLEEP_NORMAL, DEEP_SLEEP_LOW, DEEP_SLEEP_EMPTY)
    switch (BatteryCondition){
        case BATT_NORMAL:
            Serial.println("Start deep sleep (normal)");
            esp_sleep_enable_timer_wakeup(DEEP_SLEEP_NORMAL * 60  * 1000000);
            break;
        case BATT_LOW:
            Serial.println("Start deep sleep (batt low)");
            esp_sleep_enable_timer_wakeup(DEEP_SLEEP_LOW * 60  * 1000000);
            break;
        case BATT_EMPTY:
            Serial.println("Start deep sleep (batt empty)");
            esp_sleep_enable_timer_wakeup(DEEP_SLEEP_EMPTY * 60  * 1000000);
            break;
        // หาก BatteryCondition ไม่ตรงกับ Case ใดๆ จะเข้าสู่ default Case และใช้ระยะเวลา Sleep ปกติ
        default:
            esp_sleep_enable_timer_wakeup(DEEP_SLEEP_NORMAL * 60  * 1000000);
            break;
    }
    // esp_deep_sleep_start() ฟังก์ชันที่สั่งให้ ESP32 เข้าสู่ Deep Sleep Mode ทันที
    esp_deep_sleep_start();
}

// ฟังก์ชัน calculateSoC
// มีหน้าที่คำนวณ State of Charge (SoC) ของแบตเตอรี่เป็นเปอร์เซ็นต์ 
// โดยรับค่าแรงดันแบตเตอรี่ (voltage) เป็น Argument
int calculateSoC(float voltage) {
    float BATT_MIN = 3.15;  // กำหนดค่าแรงดันต่ำสุด (BATT_MIN) ของแบตเตอรี่
    float BATT_MAX = 4.2;   // กำหนดค่าแรงดันสูงสุด (BATT_MAX) ของแบตเตอรี่
    // คำนวณ SoC โดยใช้สูตรเชิงเส้น : ((แรงดันปัจจุบัน - แรงดันต่ำสุด) / (แรงดันสูงสุด - แรงดันต่ำสุด)) * 100
    int soc = ((voltage - BATT_MIN) / (BATT_MAX - BATT_MIN)) * 100;
    if (soc > 100)
      soc = 100;
    if (soc < 0)
      soc = 0;

    return soc;
}

// ฟังก์ชัน getBattVoltage
// มีหน้าที่อ่านค่าแรงดันแบตเตอรี่จากขา Analog Input 
// โดยรับค่าจาก Pin ที่กำหนด (BATT_PIN)
float getBattVoltage() {
    //read battery voltage per %
    long sum = 0;                  // sum of samples taken
    float voltage = 0.0;           // calculated voltage
    int soc ;            //SoC
    const float battery_max = 4.2; //maximum voltage of battery
    const float battery_min = 3.1; //minimum voltage of battery before shutdown

    float R1 = 100000.0; // resistance of R1 (100K)
    float R2 = 100000.0;  // resistance of R2 (100K)

    for (int i = 0; i < 500; i++) {
        sum += analogRead(BATT_PIN);
        delayMicroseconds(1000);
    }
    // calculate the voltage
    voltage = sum / (float)500;
    voltage = (voltage * 3.6) / 4096.0; //for internal 1.35v reference
   
    // Compensate Divider Circuit
    voltage = voltage / (R2/(R1+R2));

    if (voltage > 10 || voltage < 0)
      voltage = 0;

    return voltage;
}

// ฟังก์ชัน fetchTempHumid นี้มีหน้าที่อ่านค่าอุณหภูมิและความชื้นจากเซ็นเซอร์ SHT
bool fetchTempHumid(){

    if (sht.init()) {
        Serial.print("SHT success\n");
    } else {
        Serial.print("SHT failed\n");
        shtOK = false;
    }

    sht.setAccuracy(SHTSensor::SHT_ACCURACY_MEDIUM);

    if(sht.readSample()) {
        temp = sht.getTemperature();
        humid = sht.getHumidity();
        shtOK = true;
    } else {
        shtOK = false;
    }

    return shtOK;
}

bool fetchPMS() {

    Serial.print("Warming up PMS Sensor");
    pms.passiveMode();    // Switch to passive mode
    for (int i = 0 ; i < 30; i ++)
    {
        Serial.print(".");
        if (pms.readUntil(data))
        {
        // Serial.print("PM 1.0 (ug/m3): ");
        // Serial.println(data.PM_AE_UG_1_0);

        // Serial.print("PM 2.5 (ug/m3): ");
        // Serial.println(data.PM_AE_UG_2_5);

        // Serial.print("PM 10.0 (ug/m3): ");
        // Serial.println(data.PM_AE_UG_10_0);

        PM1_0 = data.PM_AE_UG_1_0;
        PM2_5 = data.PM_AE_UG_2_5;
        PM10_0 = data.PM_AE_UG_10_0;
        pmsOK = true;
        }
        else
        {
        // Serial.println("No data.");
        }
        delay(1000);
    }

    Serial.println();
    return pmsOK;
}

void fetchSensorValues() {

    Wire.begin();

    //Turn on PMS sensor and SHT sensor.
    digitalWrite(SENSOR_EN_PIN,1);

    if(!fetchPMS())
        Serial.println("PMS Sensor error");

    if(!fetchTempHumid())
        Serial.println("SHT Sensor error");

    //Turn off PMS sensor and SHT sensor.
    digitalWrite(SENSOR_EN_PIN,LOW);
    
    Serial.println();
    Serial.printf("vBatt\t= %.2f\n", vBatt);
    Serial.printf("SoC\t= %-3d%\n", SoC);
    Serial.printf("PM1\t= %-3d\n", data.PM_AE_UG_1_0);
    Serial.printf("PM2.5\t= %-3d\n", data.PM_AE_UG_2_5);
    Serial.printf("PM10\t= %-3d\n", data.PM_AE_UG_10_0);
    Serial.printf("Temp\t= %.2fc\n", temp);
    Serial.printf("Humid\t= %2.f%\n", humid);
}

bool connectAndSend() {

    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(WIFI_STA_NAME);

    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_STA_NAME, WIFI_STA_PASS);

    int connectCount = 0;
    while (WiFi.status() != WL_CONNECTED) {
        delay(100);
        Serial.print(".");
        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
        if(connectCount / 10 > WIFI_TIMEOUT){
        digitalWrite(LED_BUILTIN, connectCount%2);
        return false;
        }
        connectCount++;
    }

    //initilise Wi-Fi
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    delay(100);

    bool mqttRes = haMQTT->begin(MQTT_SERVER,MQTT_PORT, MQTT_USERNAME,MQTT_PASSWORD);
    if(!mqttRes) {
        Serial.println("MQTT Connection Failed");
        return false;
    }

    delay(100);

    //Try to publish at least 10 times (in order to make haMQTT loop works)
    for(int i = 0; i < 10; i++) {
        haSensor_vBatt->setValue(vBatt);
        haSensor_SoC->setValue(SoC);

        if (pmsOK){ //Sensor OK
        haSensor_PM1_0->setValue(PM1_0);
        haSensor_PM2_5->setValue(PM2_5);
        haSensor_PM10_0->setValue(PM10_0);
        }else{  //Sensor Failure
        haSensor_PM1_0->setAvailability(false);
        haSensor_PM2_5->setAvailability(false);
        haSensor_PM10_0->setAvailability(false);
        }

        if(shtOK){  //Sensor OK
        haSensor_temp->setValue(temp);
        haSensor_humid->setValue(humid);
        }else{  //Sensor Failure
        haSensor_temp->setAvailability(false);
        haSensor_humid->setAvailability(false);
        }

        haMQTT->loop();
        delay(10);
    }

    return true;
}

void setup() {

    //Config WDT
    esp_task_wdt_init(WDT_TIMEOUT, true); 
    esp_task_wdt_add(NULL); 

    Serial.begin(115200);
    Serial2.begin(9600,SERIAL_8N1, RX2_PIN, TX2_PIN);  // PMS Serial

    Serial.println();
    Serial.println("Starting Solar Weather Station");

    //Setup IO   
    analogSetAttenuation(ADC_11db);
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(BATT_PIN, INPUT);
    pinMode(SENSOR_EN_PIN,OUTPUT);
    // pinMode(CHARGE_EN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
    // digitalWrite(CHARGE_EN,LOW);

    //Check minimum battery level before turning on wi-fi
    delay(2000);
    vBatt = getBattVoltage();
    SoC = calculateSoC(vBatt);
    digitalWrite(CHARGE_EN,HIGH);

    if (SoC == 0){
        //Battery too low, can't do anything further.
        deepSleep(BATT_EMPTY);
    }

    //initialise HA MQTT Library
    char id[17] = STA_ID;

    haDevice = new HADevice(id);
    haMQTT = new HAMqtt(client, *haDevice);
    haSensor_vBatt = new HASensor("vBatt");
    haSensor_SoC = new HASensor("SoC");
    haSensor_PM1_0 = new HASensor("PM1_0");
    haSensor_PM2_5 = new HASensor("PM2_5");
    haSensor_PM10_0 = new HASensor("PM10_0");
    haSensor_temp = new HASensor("TEMP");
    haSensor_humid = new HASensor("HUMID");

    haDevice->setName("SolarWeatherStation");
    haDevice->setManufacturer("Magi");
    haDevice->setModel("ESP32 WROVER B");

    haSensor_vBatt->setUnitOfMeasurement("V");
    haSensor_vBatt->setDeviceClass("voltage");
    haSensor_vBatt->setIcon("mdi:alpha-v-circle-outline");
    haSensor_vBatt->setName("Battery voltage");

    haSensor_SoC->setUnitOfMeasurement("%");
    haSensor_SoC->setDeviceClass("battery");
    haSensor_SoC->setIcon("mdi:battery");
    haSensor_SoC->setName("Battery SoC");

    haSensor_PM1_0->setUnitOfMeasurement("μg/m³");
    haSensor_PM1_0->setDeviceClass("pm1");
    haSensor_PM1_0->setIcon("mdi:weather-fog");
    haSensor_PM1_0->setName("PM1");

    haSensor_PM2_5->setUnitOfMeasurement("μg/m³");
    haSensor_PM2_5->setDeviceClass("pm25");
    haSensor_PM2_5->setIcon("mdi:weather-fog");
    haSensor_PM2_5->setName("PM2.5");

    haSensor_PM10_0->setUnitOfMeasurement("μg/m³");
    haSensor_PM10_0->setDeviceClass("pm10");
    haSensor_PM10_0->setIcon("mdi:weather-fog");
    haSensor_PM10_0->setName("PM10");

    haSensor_temp->setUnitOfMeasurement("°C");
    haSensor_temp->setDeviceClass("temperature");
    haSensor_temp->setIcon("mdi:thermometer");
    haSensor_temp->setName("Temperature");

    haSensor_humid->setUnitOfMeasurement("%");
    haSensor_humid->setDeviceClass("humidity");
    haSensor_humid->setIcon("mdi:water-percent");
    haSensor_humid->setName("Humidity");
    
    //Get values from sensors.
    fetchSensorValues();

    //Connect to wi-fi and send data.
    connectAndSend();

    //Deep Sleep
    if (SoC > 30) {
        deepSleep(BATT_NORMAL);
    }
    else {
        deepSleep(BATT_LOW);
    }
}

void loop() {

}
```