//...........................................................................................................................
//https://randomnerdtutorials.com/esp8266-ds18b20-temperature-sensor-web-server-with-arduino-ide/
//http://www.fileformat.info/info/unicode/char/b0/index.htm 
//........................................................................................................................... 
 float Get_Temperature(){
 float temperatureC; 
 
  sensors.requestTemperatures(); 
  temperatureC = sensors.getTempCByIndex(0);
  String TempDisplay = (String(temperatureC,2) + "\u00B0C");  // Display value to two decimal point and in degrees Celsius 
  Blynk.virtualWrite(V23,TempDisplay);

  return temperatureC;
 }
