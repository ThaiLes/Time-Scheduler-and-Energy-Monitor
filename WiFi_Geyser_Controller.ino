//...........................................................................................................................
//https://github.com/PaulStoffregen/Time
//...........................................................................................................................
#include <FS.h>                   //this needs to be first, or it all crashes and burns...
//#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <ESP_WiFiManager.h>
//needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager
//...........................................................................................................................
//FRAM SETUP I2C address 0x50 SEE https://github.com/lnlp/FramI2C
//...........................................................................................................................
#include <Wire.h>
#include "FramI2C.h"

FramI2C fram;

//...........................................................................................................................
//OTA Include Files  // https://lastminuteengineers.com/esp8266-ota-updates-arduino-ide/
// https://arduino-esp8266.readthedocs.io/en/latest/ota_updates/readme.html
// https://1st.bitbumper.de/ota-firmware-update-tool-for-esp8266/#comment-264  
// I use the BitBumper Tool to upload the BIN file without the need of using the Arduino IDE (but you must have the OTA working properly first)
//...........................................................................................................................
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

//...........................................................................................................................
//TOKEN SETUP
//...........................................................................................................................
#include <ArduinoJson.h>          //https://github.com/bblanchon/ArduinoJson

//define your default values here, if there are different values in config.json, they are overwritten.

char blynk_token[34] = "YOUR_BLYNK_TOKEN";
//char PushsaferKey[34] = "YOUR_PUSH_TOKEN";

//flag for saving data
bool shouldSaveConfig = false;

//...........................................................................................................................
//NORMAL SETUP
//...........................................................................................................................
#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
//#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SimpleTimer.h>
#include <TimeLib.h>
#include <WidgetRTC.h>
#include <WidgetTimeInput.h>
#include <time.h>
#include <EEPROM.h>

SimpleTimer timer;

#include <WiFiClient.h>
#include <Pushsafer.h>

//...........................................................................................................................
//RMS Current Reading  (Would like Future upgrade to ADS1115 or the CS5490 chip the CS chip is used in Power Plugs
//...........................................................................................................................
#include "EmonLib.h"                   // Include Emon Library
EnergyMonitor emon1;                   // Create an instance
float pwr;
float Irms;

//...........................................................................................................................
//DS18B20 Temp Probe
//https://randomnerdtutorials.com/esp8266-ds18b20-temperature-sensor-web-server-with-arduino-ide/

//...........................................................................................................................
#include <OneWire.h>
#include <DallasTemperature.h>

// GPIO where the DS18B20 is connected to
const int oneWireBus = 13;          
// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);

//...........................................................................................................................
//OLED
//...........................................................................................................................
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

//...........................................................................................................................
//Set up Widget's
//...........................................................................................................................

WidgetLED led5(V5);   //Geyser ON
WidgetLED led8(V8);   //Holiday ON
WidgetLED led12(V12);   //Weekday AM STATUS LED
WidgetLED led22(V22);   //Weekday PM STATUS LED
WidgetRTC rtc;

// Attach virtual serial terminal to Virtual Pin V1
WidgetTerminal terminal(V43);

//...........................................................................................................................
//WiFi Credentials
//...........................................................................................................................

//char auth[] = "Blynk code here";
//char ssid[] = "SSID";
//char pass[] = "PASSWORD";

// Pushsafer private or alias key
#define PushsaferKey "PushsaferKey code here"

//...........................................................................................................................
//ESP8266 NODEMCU Pin Allocations
//...........................................................................................................................
#define TestLED         2     // WiFi board LED pin assignment
#define Relay           14    //D6 
#define Status_LED      12
#define analogInPin     A0
#define Flash_Button    16

#define WEEKDAY_AM V4
#define WEEKDAY_PM V3

#define BLYNK_GREEN     "#23C48E"
#define BLYNK_BLUE      "#04C0F8"
#define BLYNK_YELLOW    "#ED9D00"
#define BLYNK_RED       "#D3435C"
#define BLYNK_DARK_BLUE "#5F7CD8"
#define BLYNK_PINK      "#F34DF3"  //E434DF

//DEFINE MEMORY ADDRESS ALLOCATION
#define AM_Monday_Select        1 
#define AM_Tuesday_Select       2 
#define AM_Wednesday_Select     3 
#define AM_Tursday_Select       4 
#define AM_Friday_Select        5 
#define AM_Saturday_Select      6 
#define AM_Sunday_Select        7

#define PM_Monday_Select        8 
#define PM_Tuesday_Select       9 
#define PM_Wednesday_Select     10 
#define PM_Tursday_Select       11 
#define PM_Friday_Select        12 
#define PM_Saturday_Select      13 
#define PM_Sunday_Select        14

//all times below are in seconds (3 Bytes needed / AM-PM)
#define AM_Start_Time           15 //15 16 17
#define AM_Stop_Time            18 //18 19 20

#define PM_Start_Time           21 //21 22 23
#define PM_Stop_Time            24 //24 25 26

#define KWatt_24                31 

#define January     32  //32 33 34
#define February    35  //35 36 37
#define March       38  //38 39 40
#define April       41  //41 42 43
#define May         44  //44 45 46
#define June        47  //47 48 49
#define July        50  //50 51 52
#define August      53  //53 54 55
#define September   56  //56 57 58   
#define October     59  //59 60 61  
#define November    62  //62 63 64  
#define December    65  //65 66 67  

#define Power_Cost  70 
#define day_now     71 

#define KWatt_Hr                100 // 100 101 102
#define KWatt_MIN               106 // 106 107 108
#define Count_Sec               110 // 110 111 112
#define KWatt_Hr_Fraction       115 // 115 116 117

//...........................................................................................................................
//Private Definitions
//...........................................................................................................................
bool am_schedule_busy = false;     //Weekday AM Timer
bool pm_schedule_busy = false;     //Weekday PM Timer

bool isFirstConnect = true;
int Override_Time;
int min_coundown_time;
float cent_cost_kwatt;

bool Override_Flag = 0;
bool Holiday_Flag = 0;
bool toggle_LED = 0;

int watt_sec_count;
int watt_min_count;
float watt_sec;
bool reboot = true;

bool toggle_OLED;   

//...........................................................................................................................
//Setup the device and defaults
//...........................................................................................................................
void setup(){
byte e2_data;

  Serial.begin(115200);
 // EEPROM.begin(512);  //Initialize onboard EEPROM
  rtc.begin();
  Wire.begin();

  fram.begin(16); // Specifying the correct FRAM density is essential for proper operation
 
  // Start the DS18B20 sensor
  sensors.begin();
  sensors.setResolution(12); //Set resolution of DS18B20 (can be 9, 10, 11, 12)
   
  pinMode(Flash_Button, INPUT);

  pinMode(TestLED, OUTPUT); 
  digitalWrite(TestLED, HIGH); // set LED OFF
  
  pinMode(Relay, OUTPUT); //set open collector
  digitalWrite(Relay, LOW); // set LED OFF
    
  pinMode(Status_LED, OUTPUT); //set open collector
  digitalWrite(Status_LED, HIGH); // set LED OFF

   
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  OLED_Refresh();

  // delay so you have a gap to press the WIFI RESET switch
  for (uint8_t t = 4; t > 0; t--) {
    Serial.println(t);
    digitalWrite(Status_LED, LOW); // set LED ON
    delay(500);
    digitalWrite(Status_LED, HIGH); // 
    delay(500);
   }
  digitalWrite(Status_LED, LOW); // set LED ON
  delay(2000);
   
  WiFi_Manager();
    
 // Blynk.begin(auth, ssid, pass);
  Blynk.config(blynk_token);  // in place of Blynk.begin(auth, ssid, pass);

  timer.setInterval(1000L, activetoday);  // check every 1 SECOND if schedule should run today 
  timer.setInterval(30000L, reconnectBlynk);  // check every 30s if still connected to server 
  timer.setInterval(500L, toggle_heartbeat);  // toggle Heart Beat every 100milli sec 
  timer.setInterval(2000L, sendWifi);
  timer.setInterval(60000L, CountdownTimer);
  timer.setInterval(3000L, StrobeTimer);
  timer.setInterval(10000L, OLED_Page_2_Timer);
 
   
  Blynk.virtualWrite(V2,0); 
  Blynk.virtualWrite(V25,0); 
  Blynk.virtualWrite(V7,0);

  led_status (1,1); //timer 1 = STBY
  led_status (2,1); //timer 2 = STBY
  
  Blynk.syncVirtual(V10);
  Blynk.syncVirtual(V4);
  Blynk.syncVirtual(V3);
 
  //emon1.current(A0, 7.5);  // Current: input pin, calibration 1:1000 CT
  emon1.current(A0, 26.37);   // Current: input pin, calibration factor for a 1:1800 CT 100Amp:50mAmp and 47ohm burden resistor (WAS 29.8 with 50ohm)

  Timestamp_Converter (0,0,1);
  Timestamp_Converter (0,0,2);
  
  setSyncInterval(10 * 60); // Sync interval in seconds (10 minutes)
  
  digitalWrite(Status_LED, HIGH); // set LED OFF


   //OTA Stuff a copy and pase extract from the ARDUINO OTA basic example
     ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_FS
      type = "filesystem";
    }

    // NOTE: if updating FS this would be the place to unmount FS using FS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if(reboot){  // there was a power failure or reboot so recover historical data 
    reboot = false;
    Recover_Reboot_Data();
  }
 
}
//...........................................................................................................................
//Setup the Timer to pulse the WiFi board LED as a Heart Beat 
//........................................................................................................................... 
void toggle_heartbeat(){       

   toggle_LED = !toggle_LED;
  /// digitalWrite(TestLED, toggle_LED);  //toggle LED heart beat
   if (Holiday_Flag){
   // digitalWrite(Status_LED, toggle_LED);  
   }
 }
//...........................................................................................................................
//
//........................................................................................................................... 
void activetoday(){ 

  if (digitalRead(Flash_Button) == LOW){
    toggle_OLED = 1; //Page 2 of the OLED will be displayed and stay active for 10 sec then revert
    OLED_Page_2();
  }
 
  Run_Schedule();
  Calculate_Irms();
  Get_Temperature();  
  if (!toggle_OLED) OLED_Refresh(); 
  else OLED_Page_2();

}

//...........................................................................................................................
//
//........................................................................................................................... 
void OLED_Page_2_Timer(){ 
  
   //The Flash Button was pressed this will now timeout after 10 sec
    toggle_OLED = 0; 

}
//...........................................................................................................................
//Holiday mode selected so we strobe the status LED to confirm the selection.
//........................................................................................................................... 
void StrobeTimer(){  

  if(!am_schedule_busy && !pm_schedule_busy && !Holiday_Flag && !Override_Flag){
    digitalWrite(Status_LED, LOW);
    delay(50);
    digitalWrite(Status_LED, HIGH);
  }

  else if(Holiday_Flag){
    digitalWrite(Status_LED, LOW);
    delay(50);
    digitalWrite(Status_LED, HIGH);
    delay(100);
    digitalWrite(Status_LED, LOW);
    delay(50);
    digitalWrite(Status_LED, HIGH);
  }
}

//...........................................................................................................................
// Do this forever....
//...........................................................................................................................
void loop(){
  Blynk.run();
  timer.run();
  ArduinoOTA.handle();
  
}
