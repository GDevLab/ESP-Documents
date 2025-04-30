# ESP Pinout

## ESP8266 Pinout

![ESP8266 Pinout][def]

## ESP8266 NodeMCU V1.0

![ESP8266 NodeMCU V1.0 Pinout][def2]

NodeMCU V1.0 ใช้ชิพ ESP-12E (มีพินเพิ่มเติมอีกสองสามพิน) เวอร์ชันนี้ใช้ Silabs CP2102 เป็นตัวควบคุม USB คุณสามารถดาวน์โหลดไดรเวอร์ USB สำหรับบอร์ดนี้ได้ที่[ลิงก์นี้](https://www.pololu.com/file/0J14/pololu-cp2102-windows-220616.zip)

## ESP8266 NodeMCU V3.0

![ESP8266 NodeMCU V3.0 Pinout][def3]

NodeMCU V3.0 นี่เป็นเวอร์ชั่นที่ Lolin คิดค้นขึ้นโดยใช้ชิป CH340G USB-TTL แทน Silabs CP2102 จาก V1.0 ดังที่แสดงไว้ มีโครงสร้างพินเอาต์แบบเดียวกับ V1.0 แต่บอร์ดนี้จะใหญ่กว่า V1.0 เล็กน้อย คุณสามารถดาวน์โหลดไดรเวอร์ CH340G ได้ที่[ลิงก์นี้](https://sparks.gogo.co.nz/assets/_site_/downloads/CH34x_Install_Windows_v3_4.zip)

**สำคัญ!** แม้ว่าจะระบุค่าเป็น 3.3V และในแผ่นข้อมูลก็ระบุไว้ แต่พิน 3V นั้นเป็น 3V เท่านั้น! การเชื่อมต่ออุปกรณ์ 3.3V จะไม่ทำงาน ดังนั้นโปรดระวัง

บอร์ดเหล่านี้มีชื่อเรียกว่า “NodeMCU” แต่ไม่ใช่บอร์ดเดียวที่มี ESP8266

## ESP32 Pinout

![ESP32 Pinout][def4]

## ESP32 NodeMCU-32S

![ESP32 NodeMCU-32S Pinout][def5]

NodeMCU-32S ใช้ Silabs CP2102 เป็นตัวควบคุม USB คุณสามารถดาวน์โหลดไดรเวอร์ USB สำหรับบอร์ดนี้ได้ที่[ลิงก์นี้](https://www.pololu.com/file/0J14/pololu-cp2102-windows-220616.zip)

[def]: ../img/ESP8266Pinout.png
[def2]: ../img/NodeMCUv1.0-pinout.jpg
[def3]: ../img/NodeMCUv3.0-pinout.jpg
[def4]: ../img/ESP32Pinout.png
[def5]: ../img/esp32-nodemcu-32s.jpeg