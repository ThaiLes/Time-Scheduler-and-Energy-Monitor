//...........................................................................................................................
//Blunk App Terminal
//...........................................................................................................................
void Terminal_Refresh(){
float rand_rate;
float month_cost;
int month_reading;
byte month_day;
    
  month_day = month();
  rand_rate = cent_cost_kwatt/100;

  terminal.clear();
 
  terminal.print("Tarrif Cost/KWH = R");  terminal.println(rand_rate,2);
  terminal.println();
  terminal.println(" MONTH   |  KWH/M |  COST");
  terminal.println("---------------------------");

  month_reading = Reconstruct_Data(January);        //Recover the existing Month reading
  month_cost = month_reading * rand_rate;
  terminal.print("January     ");  if(month_reading == 0)terminal.print("000"); else terminal.print(month_reading);  terminal.print("     R"); terminal.print(month_cost);
  if (month_day == 1) terminal.println(" <--");   // show current month
  else terminal.println();
  
  month_reading = Reconstruct_Data(February);        //Recover the existing Month reading
  month_cost = month_reading * rand_rate;
  terminal.print("February    ");  if(month_reading == 0)terminal.print("000"); else terminal.print(month_reading);  terminal.print("     R"); terminal.print(month_cost);
  if (month_day == 2) terminal.println(" <--");
  else terminal.println();

  month_reading = Reconstruct_Data(March);        //Recover the existing Month reading
  month_cost = month_reading * rand_rate;
  terminal.print("March       ");  if(month_reading == 0)terminal.print("000"); else terminal.print(month_reading);  terminal.print("     R"); terminal.print(month_cost);
  if (month_day == 3) terminal.println(" <--");
  else terminal.println();
   
  month_reading = Reconstruct_Data(April);        //Recover the existing Month reading
  month_cost = month_reading * rand_rate;
  terminal.print("April       ");  if(month_reading == 0)terminal.print("000"); else terminal.print(month_reading);  terminal.print("     R"); terminal.print(month_cost);
  if (month_day == 4) terminal.println(" <--");
  else terminal.println();

  month_reading = Reconstruct_Data(May);        //Recover the existing Month reading
  month_cost = month_reading * rand_rate;
  terminal.print("May         ");  if(month_reading == 0)terminal.print("000"); else terminal.print(month_reading);  terminal.print("     R"); terminal.print(month_cost);
  if (month_day == 5) terminal.println(" <--");
  else terminal.println();
  
  month_reading = Reconstruct_Data(June);        //Recover the existing Month reading
  month_cost = month_reading * rand_rate;
  terminal.print("June        ");  if(month_reading == 0)terminal.print("000"); else terminal.print(month_reading);  terminal.print("     R"); terminal.print(month_cost);
  if (month_day == 6) terminal.println(" <--");
  else terminal.println();

  month_reading = Reconstruct_Data(July);        //Recover the existing Month reading
  month_cost = month_reading * rand_rate;
  terminal.print("July        ");  if(month_reading == 0)terminal.print("000"); else terminal.print(month_reading);  terminal.print("     R"); terminal.print(month_cost);
  if (month_day == 7) terminal.println(" <--");
  else terminal.println();

  month_reading = Reconstruct_Data(August);        //Recover the existing Month reading
  month_cost = month_reading * rand_rate;
  terminal.print("August      ");  if(month_reading == 0)terminal.print("000"); else terminal.print(month_reading);  terminal.print("     R"); terminal.print(month_cost);
  if (month_day == 8) terminal.println(" <--");
  else terminal.println();
  
  month_reading = Reconstruct_Data(September);        //Recover the existing Month reading
  month_cost = month_reading * rand_rate;
  terminal.print("September   ");  if(month_reading == 0)terminal.print("000"); else terminal.print(month_reading);  terminal.print("     R"); terminal.print(month_cost);
  if (month_day == 9) terminal.println(" <--");
  else terminal.println();

  month_reading = Reconstruct_Data(October);        //Recover the existing Month reading
  month_cost = month_reading * rand_rate;
  terminal.print("October     ");  if(month_reading == 0)terminal.print("000"); else terminal.print(month_reading);  terminal.print("     R"); terminal.print(month_cost);
  if (month_day == 10) terminal.println(" <--");
  else terminal.println();

  month_reading = Reconstruct_Data(November);        //Recover the existing Month reading
  month_cost = month_reading * rand_rate;
  terminal.print("November    ");  if(month_reading == 0)terminal.print("000"); else terminal.print(month_reading);  terminal.print("     R"); terminal.print(month_cost);
  if (month_day == 11) terminal.println(" <--");
  else terminal.println();
  
  month_reading = Reconstruct_Data(December);        //Recover the existing Month reading
  month_cost = month_reading * rand_rate;
  terminal.print("December    ");  if(month_reading == 0)terminal.print("000"); else terminal.print(month_reading);  terminal.print("     R"); terminal.print(month_cost);
  if (month_day == 12) terminal.println(" <--");
  else terminal.println();

  terminal.println();
  terminal.println();
  terminal.print("IP: ");   terminal.println(WiFi.localIP());
  terminal.println("SOFTWARE V2.0  09/04/2021");
  terminal.println("DESIGNED BY mothball.les@gmail.com");

  
    
  terminal.flush();
  
}
