#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <SD.h>
#include <SPI.h>
#include <WiFiUdp.h>
#include <TimeLib.h>
#include "webpages.h"
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266mDNS.h>

ESP8266WebServer server(80);

//init vars
const char* ssid = "test";
const char* passphrase = "test";

String st;
String content;
int statusCode;
String changes;
String sensorState;

String lastIP = "";                            //IP of the current active swtich
String nextActiveIP = "base";

bool motion_detectedDelay = false;
bool motionEnable = true;
bool lightOn = false;
bool initial_motion_detected = true;  //when true, enables process motion detected data
bool initial_light_off = false;
bool enable_commandLastIPoff = false;  // Used to enable communication to last sensor upon completion of hosting communication from new sensor

int gpio0_pin = 0;
int gpio2_pin = 2;
int PIR_INPUT = 4;

int counter=0;
int stateChanges=0;
int irState;
int setTimer=10;
int incTimerDelay = 10;
int off_counter = incTimerDelay;    //initialy off_counter cannot be lower than incTimerDelay

Sd2Card card;
SdVolume volume;
SdFile root;
File myFile;

int lastHour;   //NTP Debug
int lastMin;
int timeZone = -7;                     
IPAddress timeServer(132, 163, 4, 102); // time-a.timefreq.bldrdoc.gov
WiFiUDP Udp;
unsigned int localPort = 8888;  // local port to listen for UDP packets
String whatTime;

char hostString[16] = {0};

void setup() {
  // preparing GPIOs
  pinMode(gpio0_pin, OUTPUT);
  digitalWrite(gpio0_pin, LOW);
  pinMode(gpio2_pin, INPUT);
  digitalWrite(gpio2_pin, LOW);
  pinMode(PIR_INPUT, INPUT);
  
  Serial.begin(115200);
  EEPROM.begin(512);
  delay(10);
  Serial.println();
  Serial.println();

// initialize SD card
        Serial.println("Initializing SD card...");
        if (!SD.begin(15)) {
            Serial.println("ERROR - SD card initialization failed!");}
        Serial.println("SUCCESS - SD card initialized.");     

// initialize NTP Time
        Serial.println("Starting UDP");
        Udp.begin(localPort);
        Serial.print("Local port: ");
        Serial.println(Udp.localPort());

        Serial.println("WiFi Startup");
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
            startDNS();
            launchWeb(0);
            setSyncProvider(getNtpTime);
            timeDelay();
            return;
           } 
        }
      setupAP();
}
