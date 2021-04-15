//https://github.com/PaulStoffregen/Time

//...........................................................................................................................
//PM_Schedule time decode 
//...........................................................................................................................
BLYNK_WRITE(V3){ 
TimeInputParam t(param);
bool time_error_flag = false;
long startTimeInSecs;
long stopTimeInSecs;
long nowseconds;
byte bytes[3];
byte day_data[8];
byte x;
   
        startTimeInSecs = param[0].asLong();
        nowseconds = ((hour() * 3600) + (minute() * 60) + second());    //time right now
        stopTimeInSecs = param[1].asLong();
        
        if (startTimeInSecs <= 43199){
          time_error_flag = true;
          switch_off_system(2);
          led_status (2,2); //timer 2 = ERROR
        }
        else if (stopTimeInSecs <= 43199){
          time_error_flag = true;
          switch_off_system(2);
          led_status (2,2); //timer 2 = ERROR
        }
        else if (startTimeInSecs > stopTimeInSecs ){
          time_error_flag = true;
          switch_off_system(2);
          led_status (2,2); //timer 1 = ERROR
        }
        else if(pm_schedule_busy){ //operator suddenly changes the start time to more than the current time. Switch OFF the system and update the shedule
           if (startTimeInSecs > nowseconds){ 
              switch_off_system(2);
              led_status (2,1); //timer 1 = STBY
           }
           else if(stopTimeInSecs < nowseconds){
              switch_off_system(2);
              led_status (2,1); //timer 1 = STBY
           }
        }

        if (!time_error_flag & !Holiday_Flag){     
         if((nowseconds >= startTimeInSecs) & (nowseconds < stopTimeInSecs) & (!am_schedule_busy)){  //there has been a power failure and now we need to recover
           switch_on_system(2);
         }
        } 

        //if there were no time setting errors save the new start and stop times
       if(!time_error_flag){
          if(!pm_schedule_busy) led_status (2,1); //timer 2 = STBY
          //convert start time to 3 bytes
          bytes[0] = startTimeInSecs & 0xff; //contains Lsb
          bytes[1] = (startTimeInSecs >> 8) & 0xff;
          bytes[2] = (startTimeInSecs >> 16) & 0xff;
      
                
          //and save the start time in E2
          x = PM_Start_Time; //start time E2 address
          for (int i = 0; i <= 2; i++) {
             E2_write(x,bytes[i]);
             x++;
          }
      
          //convert the start time to 3 bytes
          bytes[0] = stopTimeInSecs & 0xff; //contains Lsb
          bytes[1] = (stopTimeInSecs >> 8) & 0xff;
          bytes[2] = (stopTimeInSecs >> 16) & 0xff;
      
          //and save the start time in E2
          x = PM_Stop_Time; //start time E2 address
          for (int i = 0; i <= 2; i++) {
             E2_write(x,bytes[i]);
             x++;
          }
       }

       //get the valid dyas
       for (int i = 1; i <= 7; i++) day_data[i] = (t.isWeekdaySelected(i)); 

       // and save the days to memory
       x = PM_Monday_Select;  //start on Monday 
       for (int i = 1; i <= 7; i++) {
        if (day_data[i] == 1)E2_write(x,i);
        else E2_write(x,0);
        x++;
       }
}
