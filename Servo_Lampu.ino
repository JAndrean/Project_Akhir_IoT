#include <Servo.h>
#include <ESP8266WiFi.h>
#include "FirebaseESP8266.h"
#include <MFRC522.h>
#include <SPI.h>

#define MFRC_RST D1
#define MFRC_SS D2

#define FIREBASE_HOST "project-akhir-71160036.firebaseio.com"
#define FIREBASE_AUTH "zPPwprzDA9sTpe5PFKQZBR11nrqFzukpULt1rTjp"
#define WIFI_SSID "71160036"
#define WIFI_PASS "vanguard999"

#define ldr A0
#define led D8

MFRC522 mfrc522(MFRC_SS, MFRC_RST);
FirebaseData firebaseData;
Servo servo;


String path = "/Node1";
int oldLdr;
int newLdr;
String uid = "";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  servo.attach(D3);

  pinMode(led,OUTPUT);
  pinMode(ldr,INPUT);

  digitalWrite(led,LOW);
  oldLdr = analogRead(ldr);
  servo.write(22);
  
  Firebase.begin(FIREBASE_HOST,FIREBASE_AUTH);
  
  WiFi.disconnect();
}

void loop() {
  delay(500);
  // put your main code here, to run repeatedly:
  if(WiFi.status() != WL_CONNECTED){
    digitalWrite(led,LOW);
  }else{
    digitalWrite(led,HIGH);
  }
  newLdr = analogRead(ldr);
  Serial.println(newLdr);

  if(newLdr != oldLdr){
    Firebase.setInt(firebaseData, path + "/ldr", newLdr);
    oldLdr = newLdr;
  }

  int pos = Firebase.getInt(firebaseData, path + "/servoPos"); //this code here is necessary i don't know why, but this is what makes the code bellow to works
  Serial.println(pos);
  //update pos value
  if(Firebase.getInt(firebaseData, path + "/servoPos")){
      if(firebaseData.intData() == 1){
        servo.write(6);
      }else if(firebaseData.intData() == 2){
        servo.write(22); 
      }else{
        servo.write(34);
   }
  }
  //^^^This Code
  
    if(mfrc522.PICC_IsNewCardPresent()) { // Look for new cards
      if(mfrc522.PICC_ReadCardSerial()) { // Select one of the cards
        /*Serial.println("UID Scanned");
        Serial.print("Card UID : ");*/ //for checking purpose
        String content = "";

        for (byte i = 0; i < mfrc522.uid.size; i++) {
          // Dump a byte array as hex values to String
          content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
          content.concat(String(mfrc522.uid.uidByte[i], HEX)); //
      }

      content.toUpperCase(); // Make string become UPPERCASE
      mfrc522.PICC_HaltA();
      if(content.substring(1) == "B6 1D 61 AC") { // Change UID of the card that you want to give access, for example A0 B1 C2 D3
        // Do something with access granted
        WiFi.begin(WIFI_SSID,WIFI_PASS);
      }

      if(content.substring(1) == "74 42 07 85"){
        // Do something with access denied
        WiFi.disconnect();
      }
      uid = content.substring(1);
      Serial.print("Scanned card: ");
      Serial.println(uid); //check scanned UID and print to serial
    }
  }
}
