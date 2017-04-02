void loop() {

  //automatic on/off with timer based on sensor input
    irState = digitalRead(PIR_INPUT);
  if (irState == HIGH && motionEnable) {
    if (initial_motion_detected) {
      initial_motion_detected = false;
      if (lastIP != "base")
         commandLastIPoff("base");    // if base, need to turn off lastIP unless last IP was the base
    }
    digitalWrite(gpio0_pin, HIGH);
    sensorState = "Lights are on!!!!";
//    Serial.println("LED is on");
    counter = 0;
    lightOn = true;
    if (off_counter < 10)
    {                                   // need to add code to check for max delay 
       setTimer += incTimerDelay;      // used to increase delay if motion immediately detected once light turns off
       off_counter = 10;
       Serial.println(setTimer);
    }
   initial_light_off = true;          //enabled to allow off routine to determine initial off condition
  }
  else {  
      if (counter == setTimer){
       if (initial_light_off)
       {
          initial_light_off = false;
          off_counter = 0; 
       }
       off_counter++; 
       lightOn = false; 
       digitalWrite(gpio0_pin, LOW);
       sensorState = "Lights are off!!!!";
//       Serial.println("LED is off");
       Serial.println(changes);
       if (motion_detectedDelay)
          pirDelay(4000);                     //delay to allow PIR delay timer to finish - in sensorControl
    }
    else{
      initial_motion_detected = true;         //enable to send wifi message next activation
       counter++;
      Serial.println(counter);
      if(counter == setTimer){
        stateChanges++;
        changes =(String(stateChanges, DEC));
      }
   }
   if (enable_commandLastIPoff)
   {
    enable_commandLastIPoff = false;
    commandLastIPoff(nextActiveIP);
   }
  }  
delay(500);

server.handleClient();
timeDelay();
timeCheck();


}

