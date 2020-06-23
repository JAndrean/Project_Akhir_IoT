# Project_Akhir_IoT
Contains 3 code (2 ino code and 1 android studio) for my IoT class final project, Both uses ESP8266

Check the Demo Here: https://youtu.be/fHi7LZjhVQ0

Project 1 (Poor Man's Remote Light Switch / Servo_Lampu.ino & the Final Project rar):

The point of this code is to use a servo as a light switch, the ESP8266 is connected to firebase so the servo can be controlled via the internet using an Android App. This project uses MFRC522 RFID reader to allow the ESP to connect to the internet using WiFi once authorized RFID is scanned, and uses 2nd authorized RFID to disconnects the ESP from the WiFi. This project also uses LDR to later give signal to the application whether the rooms is currently bright or not (I didn't use ON or OFF as a precaution if an external light source aside from the room's light might be interferring with the LDR value).

Required Tools:

1 ESP8266

1 Servo (I recommend using metal gear, but maybe plastic gear will work too provided you planted the servo strong enough to the wall)

1 MFRC522 RFID Reader (Along with the 2 RFID tags)

1 LED (Used to indicate whether the device has been connected to the internet or not)

1 LDR sensor

and a few tools you can find to stick the servo to the wall, light switch are usually a bit heavy for a servo even the metal gear one so I recommend to find some double tapes, or anything that will hold the servo strong enough.

Project 2 (Simple Temperature checker displayed on I2C Screen / DHT_LCD.ino)

This one is rather simple, instead of RFID you can instead use button for this one too.

Required Tools:

1 ESP8266

1 I2C LCD Screen

1 DHT11 Temperature Sensor

1 MFRC522 RFID reader (Or optionally just use a button)

1 LED (Optional)
