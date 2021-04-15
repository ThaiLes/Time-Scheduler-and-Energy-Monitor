//https://randomnerdtutorials.com/guide-for-oled-display-with-arduino/
//https://lastminuteengineers.com/oled-display-arduino-tutorial/
//https://stackoverflow.com/questions/5437674/what-unicode-characters-represent-time
//...........................................................................................................................
//
//...........................................................................................................................
void OLED_Refresh(){
byte e2data_1;
byte e2data_2;
byte e2data_3;
char time_format[] = "00:00:00";
long recover_data;
long nowseconds;

float rand_rate;
float month_cost;
int   month_reading;
byte  month_day;
    
  month_day = month();
  rand_rate = cent_cost_kwatt/100;

 //OLED DISPLAY
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  //SET UP HEADING
  display.setCursor(0, 0);
  display.print("START"); 

  display.setCursor(85, 0);
  display.println("STOP");

  //display AM START TIME
  recover_data = Reconstruct_Data(AM_Start_Time);
  e2data_1  = Timestamp_Converter (0,recover_data,4);
  e2data_2  = Timestamp_Converter (0,recover_data,5);
  display.setCursor(0, 10);
  sprintf(time_format,"%02d:%02d",e2data_1,e2data_2); //Convert to human-readable date
  display.print(time_format);display.println("AM");

   //display AM END TIME
  recover_data = Reconstruct_Data(AM_Stop_Time);
  e2data_1  = Timestamp_Converter (0,recover_data,4);
  e2data_2  = Timestamp_Converter (0,recover_data,5);
  display.setCursor(85, 10);
  sprintf(time_format,"%02d:%02d",e2data_1,e2data_2); //Convert to human-readable date
  display.print(time_format);display.println("AM");

  //display PM START TIME
  recover_data = Reconstruct_Data(PM_Start_Time);
  e2data_1  = Timestamp_Converter (0,recover_data,4);
  e2data_2  = Timestamp_Converter (0,recover_data,5);
  display.setCursor(0, 20);
  sprintf(time_format,"%02d:%02d",e2data_1,e2data_2); //Convert to human-readable date
  display.print(time_format);display.println("PM");

  //display PM STOP TIME
  recover_data = Reconstruct_Data(PM_Stop_Time);
  e2data_1  = Timestamp_Converter (0,recover_data,4);
  e2data_2  = Timestamp_Converter (0,recover_data,5);
  display.setCursor(85, 20);
  sprintf(time_format,"%02d:%02d",e2data_1,e2data_2); //Convert to human-readable date
  display.print(time_format);display.println("PM");

  //display GEYSER STATUS
  display.setCursor(0, 30);
  display.println("STATUS: "); 
  display.setCursor(45, 30);
  if(Holiday_Flag)display.println("HOLIDAY LOCK");
  else if(Override_Flag){
    display.print("OVRD "); display.print(min_coundown_time); display.print(" MIN");
  }
  else if (am_schedule_busy || pm_schedule_busy)display.println("GEYSER ON");
  else display.println("GEYSER OFF");

 //display WATT MONTH

  month_day = 29 + (month_day * 3);             // offset calculation to read correct eeprom location
  month_reading = Reconstruct_Data(month_day);  //Recover the current Month reading
  month_cost = month_reading * rand_rate;       // calculate the current Rand (Dollar) cost
  
  display.setCursor(0, 40);
  display.print(month_reading);display.println(" KWH/M");
  display.setCursor(70, 40);
  display.print("R"); display.println(month_cost);
 

  //display WATT NOW
  display.setCursor(0, 50);
  long pwr_display = pwr; 
  display.print(pwr_display);display.println(" WATT");

  //display time
  nowseconds = ((hour() * 3600) + (minute() * 60) + second());    //get the time right now from the Blynk cloud
  e2data_1  = Timestamp_Converter (0,nowseconds,4);  //HR
  e2data_2  = Timestamp_Converter (0,nowseconds,5);  //MIN
  e2data_3  = Timestamp_Converter (0,nowseconds,6);  //SEC
  display.setCursor(70, 50);
  sprintf(time_format,"%02d:%02d:%02d",e2data_1,e2data_2,e2data_3); //Convert to human-readable date
  display.print(time_format);

  //display temp
 //float temperatureC = Get_Temperature(); 
 //display.setCursor(70, 50);   //was 80
// display.print(temperatureC); 
// display.print((char)247); // degree symbol 
//display.println("C");

  
 display.display(); 
}
//...........................................................................................................................
//
//...........................................................................................................................
void OLED_Page_2(){
byte  month_day;  
int   month_reading;
byte  month_day_E2_offset;  
byte  month_day_offset;

 //display IP on OLED
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  
  //SET UP HEADING
  display.setCursor(0, 0);
  display.print("IP: "); 
  display.print(WiFi.localIP());
  

  month_day = month();    //get current month

  //month_day = 1;    //debug

  if(month_day == 1) month_day_offset = 11;
  else if (month_day == 2) month_day_offset = 12;
  else month_day_offset = month_day - 2;
  display.setCursor(0, 20);
  display.print("MTH "); display.print(month_day_offset); 
  month_day_E2_offset = 29 + (month_day_offset * 3);            // offset calculation to read correct E2 location
  month_reading = Reconstruct_Data(month_day_E2_offset);        //Recover the existing Month reading from E2
  display.setCursor(35, 20);
  display.print(" = "); display.print(month_reading); display.print(" KWH"); 

  if(month_day == 1) month_day_offset = 12;
  else month_day_offset = month_day - 1;
  display.setCursor(0, 30);
  display.print("MTH "); display.print(month_day_offset); 
  month_day_E2_offset = 29 + (month_day_offset * 3);            // offset calculation to read correct E2 location
  month_reading = Reconstruct_Data(month_day_E2_offset);        //Recover the existing Month reading from E2
  display.setCursor(35, 30);
  display.print(" = "); display.print(month_reading); display.print(" KWH"); 
  
  display.setCursor(0, 40);
  display.print("MTH "); display.print(month_day); 
  month_day_E2_offset = 29 + (month_day * 3);                   // offset calculation to read correct E2 location
  month_reading = Reconstruct_Data(month_day_E2_offset);        //Recover the existing Month reading from E2
  display.setCursor(35, 40);
  display.print(" = "); display.print(month_reading); display.print(" KWH"); 

   display.display(); 
}
