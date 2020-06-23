// DHT Temperature & Humidity Sensor
// Unified Sensor Library Example
// Written by Tony DiCola for Adafruit Industries
// Released under an MIT license.

// REQUIRES the following Arduino libraries:
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
// - Adafruit Unified Sensor Lib: https://github.com/adafruit/Adafruit_Sensor

#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <MFRC522.h>
#include <DHT.h>
#include <DHT_U.h>
#include <LiquidCrystal_I2C.h>

#define DHTPIN D3      // Digital pin connected to the DHT sensor 
// Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
// Pin 15 can work but DHT must be disconnected during program upload.

#define MFRC_RST D0
#define MFRC_SS  D4

#define led D8

// Uncomment the type of sensor in use:
#define DHTTYPE    DHT11     // DHT 11
//#define DHTTYPE    DHT22     // DHT 22 (AM2302)
//#define DHTTYPE    DHT21     // DHT 21 (AM2301)

// See guide for details on sensor wiring and usage:
//   https://learn.adafruit.com/dht/overview



DHT_Unified dht(DHTPIN, DHTTYPE);
sensors_event_t event;

MFRC522 mfrc522(MFRC_SS, MFRC_RST);   // Create MFRC522 instance
LiquidCrystal_I2C lcd(0x27, 16, 1);

uint32_t delayMS;
float temperature;
bool scanning = false;
String uid = "";
int auth = 0;


void setup() {
  Serial.begin(9600);
  pinMode(led,OUTPUT);
  digitalWrite(led,LOW);
  // Initialize device.
  dht.begin();
  lcd.begin(16,1);
  lcd.init();
  Serial.println(F("DHTxx Unified Sensor Example"));
  // Print temperature sensor details.
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  Serial.println(F("------------------------------------"));
  Serial.println(F("Temperature Sensor"));
  Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("°C"));
  Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("°C"));
  Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("°C"));
  Serial.println(F("------------------------------------"));
  // Print humidity sensor details.
  dht.humidity().getSensor(&sensor);
  Serial.println(F("Humidity Sensor"));
  Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("%"));
  Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("%"));
  Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("%"));
  Serial.println(F("------------------------------------"));
  // Set delay between sensor readings based on sensor details.
  SPI.begin();
  mfrc522.PCD_Init();
}

void loop() {
  // Delay between measurements.
  delay(500);
  
  dht.temperature().getEvent(&event);
  // Get temperature event and print its value.
    lcd.setCursor(0,0);
     if (isnan(event.temperature)) { //Fail
        lcd.clear();
        lcd.print("Fail to get Data");
      }
      else { //Success
        lcd.clear();
        lcd.print("Temp: ");
        lcd.print(event.temperature);
        lcd.print(" C");
    }
    
  if (mfrc522.PICC_IsNewCardPresent()) { // Look for new cards
    if (mfrc522.PICC_ReadCardSerial()) { // Select one of the cards
      String content = "";

      for (byte i = 0; i < mfrc522.uid.size; i++) {
        // Dump a byte array as hex values to String
        content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
        content.concat(String(mfrc522.uid.uidByte[i], HEX)); //
      }

      content.toUpperCase(); // Make string become UPPERCASE
      mfrc522.PICC_HaltA();
      if (content.substring(1) == "B6 1D 61 AC") { // Change UID of the card that you want to give access, for example A0 B1 C2 D3
        // Do something with access granted
        digitalWrite(led,HIGH);
          lcd.backlight();
      }

      if(content.substring(1) == "74 42 07 85"){
        // Do something with access denied
        delay(500);
        digitalWrite(led,LOW);
        lcd.noBacklight();
      }
      uid = content.substring(1);
      Serial.println(uid);
    }
  }
}
