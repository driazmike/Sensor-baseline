//Web Server if WiFi is configured
void createWebServer(int webtype)
{
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
  } else if (webtype == 0) {
    server.on("/", []() {
      server.send(200, "text/html", file1);
    });
    server.on("/lightstatus", [](){
      String file2 = file2a + sensorState + file2b + changes + file2c + setTime;
      server.send(200, "text/html", file2);
      });
    server.on("/timeoutsetting", [](){
      server.send(200, "text/html", file3);
    });
    server.on("/setting", [](){
      String settTime = server.arg("Time");
      setTime = settTime.toInt();
      SD.remove("Timeout.txt");
      myFile = SD.open("Timeout.txt", FILE_WRITE);
      if (myFile) {
        Serial.print("Writing to Timeout.txt...");
        myFile.println(setTime);
        // close the file:
        myFile.close();
        Serial.println("done.");
        } else {
        // if the file didn't open, print an error:
        Serial.println("error opening Timeout.txt");
        }
      statusCode = 200;
      server.send(statusCode, "text/html", file4);
    });
    server.on("/readme", [](){
      String tested;
      myFile = SD.open("tested.txt");
      if (myFile) {
      Serial.println("tested.txt:");
      // read from the file until there's nothing else in it:
      while (myFile.available()) {
      char ltr = myFile.read();
      tested = tested + ltr;
      }
      myFile.close();
      Serial.println("file closed.");
      } else {
      // if the file didn't open, print an error:
      Serial.println("error opening test.txt");
      }
      String Timeout;
      myFile = SD.open("Timeout.txt");
      if (myFile) {
      Serial.println("Timeout.txt:");
      // read from the file until there's nothing else in it:
      while (myFile.available()) {
      char ltr = myFile.read();
      Timeout = Timeout + ltr;
      }
      myFile.close();
      Serial.println("file closed.");
      } else {
      // if the file didn't open, print an error:
      Serial.println("error opening Timeout.txt");
      }
      String file5 = file5a + tested + file5b + file5c + Timeout;
      server.send(200, "text/html", file5);
      });
    }
  }
