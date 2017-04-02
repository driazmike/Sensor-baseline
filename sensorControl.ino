void commandLastIPoff(String mdIP)   //Function turns off last active sensor and assigns argument to
{
  HTTPClient http;

  Serial.println("in commandLastIPoff routine");
  Serial.println(lastIP);
  
  if (lastIP != mdIP)  //don't send command to turn off if motion is from same sensor that was previously on
  {            
    String URI = "http://" + lastIP + "/commandOff";

    if (lastIP != "base")
    { 
    Serial.println(URI);
    http.begin(URI);

  int httpCode = http.GET();

  // httpCode will be negative on error
        if(httpCode > 0) {
            // HTTP header has been send and Server response header has been handled
            //Serial.print("[HTTP] GET... code: ");
            //Serial.println(httpCode);

            // file found at server
            if(httpCode == HTTP_CODE_OK) {
                String payload = http.getString();
                Serial.println(payload);
            }
        } else {
            Serial.println("[HTTP] GET... failed, error: ");
        } 

        http.end();
    }
    lastIP = nextActiveIP;
   // nextActiveIP = "base";            //Set base as next active unless a different sensor activates first
  }
}



void send_motion_detected()
{
  HTTPClient http;
  
  IPAddress myIP = WiFi.localIP();
  String URI = "http://192.168.1.9/motionDetected?IP=" + String(myIP[0],DEC) + "." + String(myIP[1],DEC) + "." + String(myIP[2],DEC) + "." + String(myIP[3],DEC);
  Serial.println(URI);
  http.begin(URI);

  int httpCode = http.GET();

  // httpCode will be negative on error
        if(httpCode > 0) {
            // HTTP header has been send and Server response header has been handled
            //Serial.print("[HTTP] GET... code: ");
            //Serial.println(httpCode);

            // file found at server
            if(httpCode == HTTP_CODE_OK) {
                String payload = http.getString();
                Serial.println(payload);               //*****Add code here to process activate and ignore signals
            }
        } else {
            Serial.println("[HTTP] GET... failed, error: ");
        }

        http.end();
}

void pirDelay(int delayTime)
{
  Serial.println("in delay");
  delay(delayTime);              //delay to allow PIR delay timer to finish.
  motion_detectedDelay = false;
  motionEnable = true;
  initial_motion_detected = true;
}  
