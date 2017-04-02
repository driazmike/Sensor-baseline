void timeDelay(void){

  if (lastMin != minute()){
    Serial.println("updating timeout delay now");
    String dayfile;
    String fileName = "";
    char charBuf[10];
    Serial.println(weekday());
    Serial.println(hour());
    String dDay = String(weekday(),DEC);
    String hHour = String(hour(),DEC);
    int hourAdj = hHour.toInt();
    hourAdj = hourAdj + 1;
    String hourAdjusted = String(hourAdj);
    fileName = dDay + hourAdjusted + ".txt";
    Serial.println(fileName);
    fileName.toCharArray(charBuf, 12);
    Serial.println(charBuf);
    myFile = SD.open(charBuf);
    lastMin = minute();
    if (myFile) {
          // read from the file until there's nothing else in it:
          while (myFile.available()) { 
          char ltr = myFile.read();
          dayfile = dayfile + ltr;
          }
          myFile.close();
          setTimer = dayfile.toInt();
          Serial.println("File closed, timeout delay updated.");
          } else {
        // if the file didn't open, print an error:
        Serial.println("Shit: error opening the file");
        }
  }
}

