//...........................................................................................................................
//Run_Schedule
//...........................................................................................................................
void Run_Schedule(){
byte am_day_of_the_week;
byte pm_day_of_the_week;
byte today;
long nowseconds;
long Schedule_Times [4];
long result;
byte e2_data;

  
     nowseconds = ((hour() * 3600) + (minute() * 60) + second());    //get the time right now from the Blynk cloud
     Timestamp_Converter (0,nowseconds,3); //update local time on the phone app
     //Serial.println(nowseconds);

     today = Update_Day(); //get the current day of the week from the Blynk Cloud BLYNK days are 1(MON)----7(SUN)
     am_day_of_the_week = Check_AM_Valid_Day(today); //check if a valid day according to the selected days on the phone app
     pm_day_of_the_week = Check_PM_Valid_Day(today); //check if a valid day according to the selected days on the phone app
     
     if ((am_day_of_the_week != today)& am_schedule_busy){
      switch_off_system(1);         //the system was on but the day is no loger valid edge case
      led_status (1,1); //timer 1 = STBY
     }
     else if ((pm_day_of_the_week != today)& pm_schedule_busy){
      switch_off_system(2);
      led_status (2,1); //timer 2 = STBY
     }
     
     if (!Holiday_Flag){
      if(isAM()){ 
       if (am_day_of_the_week == today){
           //reconstruct all Start and Stop times in sec
           Schedule_Times[1] = Reconstruct_Data(AM_Start_Time);
           Schedule_Times[2] = Reconstruct_Data(AM_Stop_Time);
          
           if(am_schedule_busy){
             Timestamp_Converter (Schedule_Times[2],nowseconds,1);  //time remaining calculation to display on phone app 
           }
               
           result = nowseconds ^ Schedule_Times[1];      //AM Start_Time
           if(result == 0){  // AM start time VALID
             switch_on_system(1);
           }
          
           if ((nowseconds > Schedule_Times[2])& am_schedule_busy){
             switch_off_system(1);
             led_status (1,1); //timer 1 = STBY
           }

           
        }
     }

     else if(isPM()){
       if (pm_day_of_the_week == today){
           //reconstruct all Start and Stop times in sec
           Schedule_Times[3] = Reconstruct_Data(PM_Start_Time);
           Schedule_Times[4] = Reconstruct_Data(PM_Stop_Time);

           if(pm_schedule_busy)Timestamp_Converter (Schedule_Times[4],nowseconds,2);  //time remaining calculation  
           
           result = nowseconds ^ Schedule_Times[3];         //PM_Start_Time
           if(result == 0){  // Station 1 start time VALID
             switch_on_system(2);
           }
                
           if ((nowseconds > Schedule_Times[4])& pm_schedule_busy){ //PM_Stop_Time
             switch_off_system(2);
             led_status (2,1); //timer 2 = STBY
           }
       }
    }
  }
}

//...........................................................................................................................
//
//...........................................................................................................................
void switch_on_system(int timer_no){

    if (timer_no == 1){
        digitalWrite(Relay, HIGH); // set Relay ON 
        digitalWrite(Status_LED, LOW); // set LED ON
        led_status (1,3); //timer 2 = ON
        am_schedule_busy = true;
        Blynk.notify("Yaaay... AM Schedule Started");
        Schedule_Overide();
    }
     if (timer_no == 2){
        digitalWrite(Relay, HIGH); // set Relay ON 
        digitalWrite(Status_LED, LOW); // set LED ON
        led_status (2,3); //timer 2 = ON
        pm_schedule_busy = true;
        Blynk.notify("Yaaay... PM Schedule Started");
        Schedule_Overide();
    }
}
//...........................................................................................................................
//
//...........................................................................................................................
void switch_off_system(int timer_no){
           
   if (timer_no == 1){
     am_schedule_busy = false;
     digitalWrite(Relay, LOW); 
     digitalWrite(Status_LED, HIGH); // set LED OFF
     Timestamp_Converter (0,0,timer_no);  //reset time remaining calculation
   }
            
   else if (timer_no == 2){
     pm_schedule_busy = false;
     digitalWrite(Relay,LOW); 
     digitalWrite(Status_LED, HIGH); // set LED OFF
     Timestamp_Converter (0,0,timer_no);  //reset time remaining calculation
   }
}
