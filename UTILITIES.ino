//...........................................................................................................................
// 
//...........................................................................................................................
byte Check_AM_Valid_Day(byte today){
byte e2_data;
byte e2_address = AM_Monday_Select; // start checking from Monday
 
  //Now check if the geyser day schedule matches the BLYNK day of the week.
  //If there is a match the RETURN data (day) start checking from Monday

  for(int i = 1; i < 8; i++){
    e2_data = E2_read(e2_address);    
    if (e2_data >= today) {
      i = 8; // force exit out of the loop
      return e2_data;
    }
    e2_address++; 
  }
}

//...........................................................................................................................
// 
//...........................................................................................................................
byte Check_PM_Valid_Day(byte today){
byte e2_data;
byte e2_address = PM_Monday_Select; // start checking from Monday

 
  //Now check if the geyser day schedule matches the BLYNK day of the week.
  //If there is a match the RETURN data (day) start checking from Monday

  for(int i = 1; i < 8; i++){
    e2_data = E2_read(e2_address);    
    if (e2_data >= today) {
      i = 8; // force exit out of the loop
      return e2_data;
    }
    e2_address++; 
  }
}

//...........................................................................................................................
//if overide was selected and a scheduled timer starts reset the overide
//...........................................................................................................................
void Schedule_Overide() {
  
  Override_Flag = 0;
  Blynk.virtualWrite(V2, 0); //Disable Select
  Blynk.virtualWrite(V7, 0); //Disable Select
}

//...........................................................................................................................
// Switch OFF overide after delay timeout
//...........................................................................................................................
void Delay_Switch_OFF() {
  
  Override_Flag = 0;
  digitalWrite(Relay, LOW); // set Relay OFF
  digitalWrite(Status_LED, HIGH); // set LED OFF
  Blynk.virtualWrite(V2, 0); //Disable Select
  Blynk.virtualWrite(V7, 0); //Disable Select
}

//...........................................................................................................................
// 60 sec Countdown Timer
//...........................................................................................................................
void CountdownTimer() {
byte day_read;
byte today_E2_read;

    //read the saved day from E2 then check if the calander day has changed, if so update 
    day_read = day();             // the day now (1-31)
    today_E2_read = E2_read(day_now);
    if(day_read != today_E2_read) {
      E2_write(day_now, day_read);
      Serial.print("update day & 24hr readings");
      byte e2_data = E2_read(KWatt_24);        //read the current saved value
      Blynk.virtualWrite(V26, e2_data);   //displays the last 24hr reading 
      Blynk.virtualWrite(V27, 0);         //clear the last 24hr raw data - bug fix 23/01/21
      E2_write(KWatt_24,0);               //clear E2 for the next 24hr data
    }
    
   //Check if the WiFi connection is still OK
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi Connection lost reset and try reconnect");
    delay(3000);
    ESP.reset();
    delay(5000);
  }

 if(Override_Flag){
    min_coundown_time--;
    //Serial.print("Time Remaining: ");
    //Serial.println(min_coundown_time);
    Blynk.virtualWrite(V7, min_coundown_time); 
       if (min_coundown_time == 0){
          Override_Flag = 0;
          Serial.println("Geyser Timeout");
          Blynk.virtualWrite(V2,0); //disable override if previously selected
          Blynk.virtualWrite(V5,0); //disable override if previously selected
          Blynk.virtualWrite(V7,0); //disable override if previously selected
          digitalWrite(Relay, LOW); // set Relay OFF
          digitalWrite(Status_LED, HIGH); // set LED OFF
       }
   }
}

//...........................................................................................................................
// Convert epoch to human-readable data
//...........................................................................................................................
byte Timestamp_Converter (long stopsecondswd, long nowseconds, int time_widgit){ 
  int runHours;
  int runMinutes;
  int runSeconds;
  int secsRemaining;
  char time_remaining[8];
  unsigned long allSeconds;
  long timestamp;

      allSeconds = stopsecondswd - nowseconds;
      
      if (time_widgit >= 3){
        allSeconds = nowseconds;
      }
      
      secsRemaining = allSeconds % 86400;
      runHours = secsRemaining / 3600;
            
      secsRemaining = allSeconds % 3600;
      runMinutes = secsRemaining / 60;
      
      runSeconds = secsRemaining % 60;

      sprintf(time_remaining, "%02d:%02d:%02d", runHours, runMinutes, runSeconds);

      
      if (time_widgit == 1){
        Blynk.virtualWrite(V9, time_remaining); // Display AM Widget
        
      }
      else if (time_widgit == 2){
        Blynk.virtualWrite(V11, time_remaining); // Display PM Widget
        
      }
      else if (time_widgit == 3){
         Blynk.virtualWrite(V1, time_remaining); // Display Local Time
        
      }
      else if (time_widgit == 4){
         return runHours;
      }
      
       else if (time_widgit == 5){
         return runMinutes;
      }
      else if (time_widgit == 6){
         return runSeconds;
      }
          
}

//...........................................................................................................................
//TIMER LED Satus decode
//...........................................................................................................................
void led_status(int timer_no, int led_status){                                                   

  //TIMER 1 STATUS
  if (timer_no == 1){
    if(led_status == 1){
      led12.on();
      led12.setColor(BLYNK_GREEN);
      led12.setLabel(" STBY"); 
    }
    else if(led_status == 2){
      led12.on();
      led12.setColor(BLYNK_PINK);
      led12.setLabel("ERROR");
    }
    else if(led_status == 3){
      led12.on();
      led12.setColor(BLYNK_RED);
      led12.setLabel("  ON");
    }
    else if(led_status == 4){
      led12.on();
      led12.setColor(BLYNK_YELLOW);
      led12.setLabel(" LOCK");
    }
  }

  //TIMER 2 STATUS
  if (timer_no == 2){
    if(led_status == 1){
      led22.on();
      led22.setColor(BLYNK_GREEN);
      led22.setLabel(" STBY"); 
    }
    else if(led_status == 2){
      led22.on();
      led22.setColor(BLYNK_PINK);
      led22.setLabel("ERROR");
     }
    else if(led_status == 3){
      led22.on();
      led22.setColor(BLYNK_RED);
      led22.setLabel("  ON");
    }
    else if(led_status == 4){
      led22.on();
      led22.setColor(BLYNK_YELLOW);
      led22.setLabel(" LOCK");
    }
  }
}

//...........................................................................................................................
//get the current day of the week from the Blynk Cloud BLYNK days are 1(MON)----7(SUN) 
//...........................................................................................................................
byte Update_Day(){
byte today;
      if (weekday()== 2)     today = 1;
      else if (weekday()== 3)today = 2;
      else if (weekday()== 4)today = 3;
      else if (weekday()== 5)today = 4;
      else if (weekday()== 6)today = 5;
      else if (weekday()== 7)today = 6;
      else if (weekday()== 1)today = 7;

      return today;
 }

//...........................................................................................................................
//Clear Historical Data 
//...........................................................................................................................
void Clear_Historical_Data(){

     digitalWrite(Status_LED, LOW); // set LED ON
     Serial.println("RESET HISTORICAL DATA");
    
     //clear EEPROM locations of historical data
     E2_write(100,0);
     E2_write(101,0);
     E2_write(102,0);

     E2_write(106,0);
     E2_write(107,0);
     E2_write(108,0);

     E2_write(110,0);
     E2_write(111,0);
     E2_write(112,0);
     
     E2_write(115,0);
     E2_write(116,0);
     E2_write(117,0);
     
     E2_write(KWatt_24,0);               //clear E2 of the 24hr data

     watt_sec_count = 0;          // reset count every hr to start again
     watt_sec = 0;                // We now start new for the next hours count
     Blynk.virtualWrite(V15, 0);          
     Blynk.virtualWrite(V16, 0);    
     Blynk.virtualWrite(V17, 0);     
     Blynk.virtualWrite(V13,0);
     Blynk.virtualWrite(V26,0);
     delay(500);
     digitalWrite(Status_LED, HIGH); // set LED OFF

    Clear_Historical_Month_Data();
     
 
}
//...........................................................................................................................
// Clear_Historical_Month_Data
//...........................................................................................................................
void Clear_Historical_Month_Data(){
byte bytes[3];
int save_information_data = 0;
int x = January;
int month_reading;

   for (int y = 1; y <= 12; y++) {
    bytes[0] = save_information_data & 0xff; //contains Lsb
    bytes[1] = (save_information_data >> 8) & 0xff;
    bytes[2] = (save_information_data >> 16) & 0xff;
    
    //Serial.print("x = "); Serial.println(x);
    for (int i = 0; i <= 2; i++) {
      E2_write(x,bytes[i]);
      x++;
    }
    
    //save_information_data = (230 + (5 * x));  //debug 
    save_information_data = 0;
 }

 Terminal_Refresh();

  // read back data for debug perposes
  x = January;
  for (int y = 1; y <= 12; y++) {
      month_reading = Reconstruct_Data(x);        //Recover the existing Month reading
     Serial.print("month_reading "); Serial.print(x); Serial.print(" = "); Serial.println(month_reading);
     for (int i = 0; i <= 2; i++) {
      x++;
    }
  }
}
//...........................................................................................................................
// Recover Reboot Data
//...........................................................................................................................
void Recover_Reboot_Data(){
int e2_data;

    Serial.println("Reboot data Recovered"); 

    watt_sec = Reconstruct_Data(KWatt_MIN);
    Serial.print("Recovered watt_sec = "); Serial.println(watt_sec);

    watt_sec_count = Reconstruct_Data(Count_Sec);
    Serial.print("Recovered watt_sec_count = "); Serial.println(watt_sec_count);
    
    e2_data = Reconstruct_Data(KWatt_Hr_Fraction);
    Serial.print("Recovered KWatt_Hr_Fraction = "); Serial.println(e2_data); 
    Blynk.virtualWrite(V17, e2_data);             // debug

    e2_data = Reconstruct_Data(KWatt_Hr);
    Serial.print("Recovered KWatt_Hr = "); Serial.println(e2_data); 

    //all debug below
    //E2_write(KWatt_24,28);       //debug
    //Blynk.virtualWrite(V27, 28);             // debug
    //Blynk.virtualWrite(V26, 0);             // debug
    //watt_sec_count = 3590; //debug
    //watt_sec = 1340; //debug
    //E2_write(KWatt_24,18); 
    //Blynk.virtualWrite(V27,18);  
    //E2_write(day_now, 8);
    //Blynk.virtualWrite(V80, 28);             // debug
    
   

}
