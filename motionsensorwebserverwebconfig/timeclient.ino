void checkOST(void){
  currentMillis = millis();
  if (currentMillis - previousMillis > 60000){
    previousMillis = currentMillis;
    printf("Time epoch: %d: ", timeClient.getEpochTime());
    Serial.println(timeClient.getFormattedTime());

    Serial.println(hour()); // The hour now (0-23)
    /*Serial.println(minute()); // The minute now (0-59)
    Serial.println(second()); // The second now (0-59)
    Serial.println(day()); // The day now (1-31)
    Serial.println(weekday()); // Day of the week, Sunday is day 1
    Serial.println(month()); // The month now (1-12)
    Serial.println(year()); // The full four digit year: (2009,*/
    
  }
}

