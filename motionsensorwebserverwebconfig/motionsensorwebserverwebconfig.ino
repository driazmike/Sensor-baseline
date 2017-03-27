#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <SD.h>
#include <SPI.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <TimeLib.h>
#include "webpages.h"


ESP8266WebServer server(80);

//init vars
const char* ssid = "test";
const char* passphrase = "test";
String st;
String content;
int statusCode;
String changes;
String sensorState;
int gpio0_pin = 0;
int gpio2_pin = 2;
int gpio4_pin = 4;

int stateChanges=0;

int setTimer=10;
Sd2Card card;
SdVolume volume;
SdFile root;
File myFile;
WiFiUDP ntpUDP;
int16_t utc = 7; //UTC 7:00 Brazil
uint32_t currentMillis = 0;
uint32_t previousMillis = 0;
 
NTPClient timeClient(ntpUDP, "a.st1.ntp.br", utc*3600, 60000);

void setup() {
  // preparing GPIOs
  pinMode(gpio0_pin, OUTPUT);
  digitalWrite(gpio0_pin, LOW);
  pinMode(gpio2_pin, INPUT);
  digitalWrite(gpio2_pin, LOW);
  //attachInterrupt(0, manualinterrupt, CHANGE);
  pinMode(gpio4_pin, INPUT);
  
  Serial.begin(115200);
  EEPROM.begin(512);
  delay(10);
  Serial.println();
  Serial.println();

// initialize SD card
        Serial.println("Initializing SD card...");
        if (!SD.begin(15)) {
            Serial.println("ERROR - SD card initialization failed!");
            return;    // init failed
        }
        Serial.println("SUCCESS - SD card initialized.");
        // check for test.txt file
        if (!SD.exists("test.txt")) {
            Serial.println("ERROR - Can't find test.txt file!");
            return;  // can't find index file
        }
        Serial.println("SUCCESS - Found test.txt file."); 

  Serial.println("Startup");
  // read eeprom for ssid and pass
  Serial.println("Reading EEPROM ssid");
  String esid;
  for (int i = 0; i < 32; ++i)
    {
      esid += char(EEPROM.read(i));
    }
  Serial.print("SSID: ");
  Serial.println(esid);
  Serial.println("Reading EEPROM pass");
  String epass = "";
  for (int i = 32; i < 96; ++i)
    {
      epass += char(EEPROM.read(i));
    }
  Serial.print("PASS: ");
  Serial.println(epass);  
  if ( esid.length() > 1 ) {
      WiFi.begin(esid.c_str(), epass.c_str());
      if (testWifi()) {
        launchWeb(0);
        return;
      } 
  }

  setupAP();
}


