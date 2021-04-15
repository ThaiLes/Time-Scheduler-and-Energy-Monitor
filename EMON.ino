//https://github.com/openenergymonitor/EmonLib
//https://community.createlabz.com/knowledgebase/wireless-energy-monitoring-system-using-esp32-with-blynk-mobile-app/
//https://gist.github.com/whatnick/13e08d40ce0f9a4c7373   //USE THIS FOR ADS1115 CHIP
//https://william.robb.scot/2020/01/25/current-monitoring-with-esp8266.html
//...........................................................................................................................
//
//...........................................................................................................................
void Calculate_Irms(){

    Irms = emon1.calcIrms(1480);  // Calculate Irms only. Try 5588 current samples per second
    //Irms = RollingmAvg(Irms);   //smooth the reading (but we are not using it)
    pwr = Irms*230.0;             //typical supplied voltage is 230VAC 

    //debug
    //if (!toggle_watt){
    //  pwr = 300;  //debug only
    //}
    //else 
    //  pwr = 1180;  //debug only

    
    Blynk.virtualWrite(V31, pwr);   
    Blynk.virtualWrite(V14,Irms);
    long pwr_display = pwr; 
    Blynk.virtualWrite(V19,pwr_display);
    Kwatt_Calculation(pwr);
}

    
