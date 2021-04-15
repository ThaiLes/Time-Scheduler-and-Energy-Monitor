//...........................................................................................................................
//
//...........................................................................................................................
void Kwatt_Calculation(float watt_pwr){

  watt_sec = watt_sec + (watt_pwr / 3600);    //convert Watt/Sec to Watt/Hr    
  Blynk.virtualWrite(V15, watt_sec);          // to display engineering data
  watt_sec_count++;
  Blynk.virtualWrite(V16, watt_sec_count);    // to display engineering data
  watt_min_count++;
 
  if (watt_min_count == 299){     //save data every 5 min (299 sec) in case there is a power failure or reset. in this case you will only lose 5 min data
    watt_min_count = 0;
    Update_Kwatt_MIN(watt_sec);
    Update_Sec_Count(watt_sec_count);
  }
  
  if(watt_sec_count >= 3599){    //we now have 3600 seconds of readings = 1 Watt/hr
    Update_Kwatt_HR(watt_sec);   // save accumalated power consuption of the last hour
    watt_sec_count = 0;          // reset count every hr to start again
    watt_sec = 0;                // We now start new for the next hours count
    //watt_sec_count = 3580; //debug
    //watt_sec = 800; //debug
 
  }
}
//...........................................................................................................................
//
//...........................................................................................................................
void Update_Sec_Count(int watt_sec_count_vlaue){
byte bytes[3];

  //convert the KWatt to 3 bytes
  bytes[0] = watt_sec_count_vlaue & 0xff; //contains Lsb
  bytes[1] = (watt_sec_count_vlaue >> 8) & 0xff;
  bytes[2] = (watt_sec_count_vlaue >> 16) & 0xff;

  //and save Count_Sec in E2
  int x = Count_Sec; //Count_Sec E2 address
  for (int i = 0; i <= 2; i++) {
    E2_write(x,bytes[i]);
    x++;
  }
}
//...........................................................................................................................
//
//...........................................................................................................................
void Update_Kwatt_MIN(float watt_sec_value){
byte bytes[3];
int kwatt_record;

  kwatt_record = watt_sec_value;
 
  //convert the KWatt to 3 bytes
  bytes[0] = kwatt_record & 0xff; //contains Lsb
  bytes[1] = (kwatt_record >> 8) & 0xff;
  bytes[2] = (kwatt_record >> 16) & 0xff;

  //and save KWatt Hour in E2
  int x = KWatt_MIN; //KWatt_Hr E2 address
  for (int i = 0; i <= 2; i++) {
    E2_write(x,bytes[i]);
    x++;
  }
}
//...........................................................................................................................
//any reading below 1 KWatt will be saved and added to the next round
//...........................................................................................................................
void Update_KWatt_Hr_Fraction(float watt_hr_value){
byte bytes[3];
int kwatt_record;

  kwatt_record = watt_hr_value;
  Blynk.virtualWrite(V17, kwatt_record);             // to display engineering data

  //convert the KWatt fraction remaining to 3 bytes
  bytes[0] = kwatt_record & 0xff; //contains Lsb
  bytes[1] = (kwatt_record >> 8) & 0xff;
  bytes[2] = (kwatt_record >> 16) & 0xff;

  //and save KWatt Hour in E2
  int x = KWatt_Hr_Fraction; //KWatt_Hr E2 address
  for (int i = 0; i <= 2; i++) {
    E2_write(x,bytes[i]);
    x++;
  }
}

//...........................................................................................................................
// This routine will update every 3600sec (1hr)
//...........................................................................................................................
void Update_Kwatt_HR(float watt_hr_value){
byte bytes[3];
byte e2_data;
int kwatt_record;
int month_record;
int kwatt_record_fraction;
byte kwatt_count_value = 0;
byte month_day;
int x;

   //recover the saved KWatt_Hr_Fraction from the previous hour and add it to the current Watts/Hr, so we do not lose any data
   kwatt_record_fraction = Reconstruct_Data(KWatt_Hr_Fraction);   
   watt_hr_value = watt_hr_value + kwatt_record_fraction;
   Serial.print("Recovered + recoded watts = "); Serial.println(watt_hr_value);

   //clear the fraction EEPROM, if you dont clear, it will just add the value on top in the calculations below
   E2_write(115,0);
   E2_write(116,0);
   E2_write(117,0);
   
   // if the reading is below 1KWatt/Hr for the past hour we save the fraction value
   if (watt_hr_value < 1000){
      Update_KWatt_Hr_Fraction(watt_hr_value);     //the stored value will be inclusive of the recovered fraction value      
      Serial.println("less than 1000");
   }

   // if the Kwatt/Hr value is greater than 1k/watt then extract how many k/watts were used in the last hour and save
   else{
     for (int i = 0; i <= 15; i++) {    //check how many KWatt HR we have (1000 = 1KWatt) it cant be more than 15k/watt for a domestic house 63Amp/CB x 230VAC = 14,490 KWatt/HR
      if (watt_hr_value > 1000){
        kwatt_count_value++;            // we count up how many KWatts were used in the last HR
        watt_hr_value = watt_hr_value - 1000; // subtract 1000 watt and check again
      }
      else if(watt_hr_value < 1000){                   //if the remainder is less than 1 K/Watt
        Update_KWatt_Hr_Fraction(watt_hr_value);       //then save the remainder of the KWatt Hour so that it will be included in the next round
        i = 16;                                        // force the exit
         Serial.print("Update_KWatt_Hr_Fraction = "); Serial.println(watt_hr_value);
         Serial.print("kwatt_count_value = "); Serial.println(kwatt_count_value);
      }
    }

    e2_data = E2_read(KWatt_24);      //read the current saved value
    e2_data = e2_data + kwatt_count_value;    //add the latest 1hr reading to the recovered value
    E2_write(KWatt_24,e2_data);       //and save the new watt reading
    Blynk.virtualWrite(V27, e2_data); //display raw data

    kwatt_record = Reconstruct_Data(KWatt_Hr);        //Recover the existing KWatt/HR reading
    kwatt_record = kwatt_record + kwatt_count_value;   //add the current KWatt/Hr reading calculated from the for loop above
    month_record = kwatt_record;
    Blynk.virtualWrite(V13, kwatt_record);             // update new reading
      
    //convert the KWatt to 3 bytes.....> 
    bytes[0] = kwatt_record & 0xff; //contains Lsb
    bytes[1] = (kwatt_record >> 8) & 0xff;
    bytes[2] = (kwatt_record >> 16) & 0xff;
  
    //and save the updated KWatt Hour reading in E2.....>
    x = KWatt_Hr; //KWatt_Hr E2 address
    for (int i = 0; i <= 2; i++) {
      E2_write(x,bytes[i]);
      x++;
    }
    
    //and then save the update KWatt Hour reading to the current month E2 address location
    month_day = month();        //get the current month from the cloud
    x = 29 + (month_day * 3);   // offset calculation to read correct eeprom address and save the Watt Hour reading to the Month reading in E2
    for (int i = 0; i <= 2; i++) {
      E2_write(x,bytes[i]);
      x++;
    }
  }
  
   Manage_Month_Data();   //update the app Terminal readings every hour with the latest recorded power readings
 }
//...........................................................................................................................
//
//...........................................................................................................................
long Reconstruct_Data(byte e2_address){
byte bytes[3];

  //reconstruct data
   int x = e2_address;
   for (int i = 0; i <= 2; i++) {
       bytes[i] = E2_read(x);
       x++;
   }
   long Decoded_Data = (bytes[2] << 16) | (bytes[1] << 8) | bytes[0];  //reconstruct the start time to seconds
   return Decoded_Data;
 }

 //...........................................................................................................................
// 
//...........................................................................................................................
void Manage_Month_Data(){
int x;
int m;
byte md;
byte month_day;

 // calculations to see if we are at the end of a calander year then we need to do a roll over and clear the data ahead and leave only the previous month history
  
  month_day = month();
  md = month_day + 1;     // add one month extra
  if (md == 13) md = 1;   // if over 12 months then make the month Jan 
  
  if(month_day == 1) m = 11;  //if month is Jan then clear to Nov only we want to keep Dec as history on the year roll over
  else m = 12;                 // else if month greater than Jan clear all locations ahead of the current month using the next calculation 
  
  month_day = 29 + (month_day * 3); // offset calculation to read correct eeprom location
  x = month_day + 3; // add 3 so we do not clear the current month.
  
   for (int y = md; y <= m; y++) {
    for (int i = 0; i <= 2; i++) {
      E2_write(x,0);
      x++;
    }
 }
 Terminal_Refresh(); //update the app terminal
}
