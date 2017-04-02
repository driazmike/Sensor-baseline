//Web Server if WiFi is configured
void createWebServer(int webtype)
{
// Server goes to AP mode to configure WiFi
  if ( webtype == 1 ) {
    server.on("/", []() {
        IPAddress ip = WiFi.softAPIP();
        String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
        content = "<!DOCTYPE HTML>\r\n<html>Hello from ESP8266 at ";
        content += ipStr;
        content += "<p>";
        content += st;
        content += "</p><form method='get' action='setting'><label>SSID: </label><input name='ssid' length=32><input name='pass' length=64><input type='submit'></form>";
        content += "</html>";
        server.send(200, "text/html", content);  
    });
    
    server.on("/setting", []() {
        String qsid = server.arg("ssid");
        String qpass = server.arg("pass");
        if (qsid.length() > 0 && qpass.length() > 0) {
          Serial.println("clearing eeprom");
          for (int i = 0; i < 96; ++i) { EEPROM.write(i, 0); }
          Serial.println(qsid);
          Serial.println("");
          Serial.println(qpass);
          Serial.println("");
            
          Serial.println("writing eeprom ssid:");
          for (int i = 0; i < qsid.length(); ++i)
            {
              EEPROM.write(i, qsid[i]);
              Serial.print("Wrote: ");
              Serial.println(qsid[i]); 
            }
          Serial.println("writing eeprom pass:"); 
          for (int i = 0; i < qpass.length(); ++i)
            {
              EEPROM.write(32+i, qpass[i]);
              Serial.print("Wrote: ");
              Serial.println(qpass[i]); 
            }    
          EEPROM.commit();
          content = "{\"Success\":\"saved to eeprom... reset to boot into new wifi\"}";
          statusCode = 200;
        } else {
          content = "{\"Error\":\"404 not found\"}";
          statusCode = 404;
          Serial.println("Sending 404");
        }
        server.send(statusCode, "application/json", content);
    });

// Server is in server mode = success!    
  } else if (webtype == 0) {
    server.on("/", []() {
      server.send(200, "text/html", file1);
    });

    //Light status web page
    server.on("/lightstatus", [](){
      String file2 = file2a + sensorState + file2b + changes + file2c + setTimer + file2d + whatTime + file2f;
      server.send(200, "text/html", file2);
      });
    
    //Settings web page; Writes the timeout delay to appropriate file for each day and hour
    server.on("/setting", [](){
      int cntr = 1;
      while(cntr < 25){
        char charBuf[10];
        String settTime = server.arg(cntr);
        String fileName = server.arg("wkDay") + cntr + ".txt";
        fileName.toCharArray(charBuf, 12);
        setTimer = settTime.toInt();
        SD.remove(charBuf);
        myFile = SD.open(charBuf, FILE_WRITE);
          if (myFile) {
          Serial.print("Writing to charBuf.txt...");
          myFile.println(setTimer);
          // close the file:
          myFile.close();
          Serial.println("done.");
        } else {
          // if the file didn't open, print an error:
          Serial.println("error opening Timeout.txt");
        }
        cntr++;
      }      
      statusCode = 200;
      server.send(statusCode, "text/html", file4);
    });

      // Shows day of week and the timeout for each hour
      server.on("/day", [](){
      String dayfile = "";
      int cntr = 1;
      while(cntr < 25){
        char charBuf[10];
        String fileName = server.arg("wkDay") + cntr + ".txt";
        fileName.toCharArray(charBuf, 12);
        myFile = SD.open(charBuf);
        if (myFile) {
          // read from the file until there's nothing else in it:
          while (myFile.available()) {
          char ltr = myFile.read();
          dayfile = dayfile + ltr;
          }
          myFile.close();
          Serial.println("file closed.");
          } else {
        // if the file didn't open, print an error:
        Serial.println("error opening the file");
        }
        cntr++;
      }
      server.send(200, "text/plain", dayfile);
    });

    //Day Time setting page. User sets delay timeout. Auto-directs to /setting
    server.on("/dayz", [](){
      server.send(200, "text/html", file6);
    });

    server.on("/motionDetected", [](){
      String mdIP = server.arg("IP");   //Motion Detected IP address
      Serial.println(mdIP);
      Serial.println("in motionDetected function");
      String reply = "";
      if (true) {                        //check rules table goes here
        reply += "activate\r\n";           //tell remote sensor switch to turn on light
        enable_commandLastIPoff = true;    //set toggle to allow wi-fi module to initiate new communication once this session closes
        if (lightOn)
        {
          motion_detectedDelay = true;  //used to toggle on a delay in main loop to allow PIR timer to finish
          motionEnable = false;     //prevent light from immediately turning back on in main loop
          counter = setTimer;          //Will cause light to turn off in main loop          
        }
      }
      else
        reply += "ignore\r\n";     // tell remote sensor to ignore motion detected
      statusCode = 200;
      server.send(statusCode, "text/html", reply);
      nextActiveIP = mdIP;
      delay(10);
      // lastIP = mdIP;
    });

    server.on("/commandOff", [](){
      if (lightOn)
        {
          Serial.println("in /command off handler");
          motion_detectedDelay = true;  //used to toggle on a delay in main loop to allow PIR timer to finish
          motionEnable = false;     //prevent light from immediately turning back on in main loop
          counter = setTimer;        //Will cause light to turn off in main loop  
        }   
      server.send(200, "text/html", "");           
    });
    
    }
  }
