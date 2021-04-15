//...........................................................................................................................
//Override time received from Slider widget
//...........................................................................................................................
BLYNK_WRITE(V10){
  
  min_coundown_time = param.asInt(); // assigning incoming minute value from pin V10 to a variable
  Override_Time = min_coundown_time;
  if (Override_Flag) {
    Blynk.virtualWrite(V7, min_coundown_time);
  }
  else  {    
    Blynk.virtualWrite(V7, 0);
  }
  
}
//...........................................................................................................................
//Cent cost/kwatt from Slider widget
//...........................................................................................................................
BLYNK_WRITE(V40){
  
  cent_cost_kwatt = param.asInt(); // assigning cent cost to a variable
  Terminal_Refresh();
 
}
//...........................................................................................................................
// Override Button: Force the Geyser ON for the pre selected time if no holiday mode selected.
//...........................................................................................................................
BLYNK_WRITE(V2){ 

    if(!Holiday_Flag & !am_schedule_busy & !pm_schedule_busy){
       if (param.asInt()== 1) {  // Geyser Override Selected   
        Override_Flag = 1;
        digitalWrite(Relay, HIGH); // set Relay ON 
        digitalWrite(Status_LED, LOW); // set LED ON
        min_coundown_time = Override_Time;
        Blynk.virtualWrite(V7, min_coundown_time); //debug 24/04
     }
      else{
         Delay_Switch_OFF();
         Override_Flag = 0;
         Blynk.virtualWrite(V2, 0); //Disable Select
      }
    }
    else {
      Blynk.virtualWrite(V2, 0); //Disable Select
      Override_Flag = 0;
      
    }
}

//...........................................................................................................................
// Holiday Mode Seclected: I'm on holiday, disable the timer schedule and override button
//...........................................................................................................................
BLYNK_WRITE(V25)  
{
    if (param.asInt()==1) {  // Is Holidayn mode Selected?   
     
      Delay_Switch_OFF();
      Override_Flag = 0;
           
      Blynk.virtualWrite(V7, 0);
      Blynk.virtualWrite(V2, 0); // yes disable override if selected :)

      switch_off_system(1);
      switch_off_system(2);
     
      led_status (1,4); //timer 1 = LOCK
      led_status (2,4); //timer 2 = LOCK
      Holiday_Flag = 1;
      
    }
    else if (param.asInt()==0) {  // No! Holiday Mode not selected  :(  
      Holiday_Flag = 0;
      led_status (1,1); //timer 1 = STBY
      led_status (2,1); //timer 2 = STBY
      digitalWrite(Status_LED, HIGH); // set LED OFF
      Blynk.syncVirtual(V4);          // sync am timer
      Blynk.syncVirtual(V3);          // sync pm timer
    }
}
//...........................................................................................................................
// 
//...........................................................................................................................
BLYNK_WRITE(V20){
  
if (param.asInt()==1)Clear_Historical_Data();
    
}   
