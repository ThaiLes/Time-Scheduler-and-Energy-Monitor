// https://github.com/tzapu/WiFiManager/blob/master/examples/AutoConnectWithFSParameters/AutoConnectWithFSParameters.ino
// https://github.com/tzapu/WiFiManager/issues/63
// the bottom two links still to be tested - migration from version 5 to 6 (I am running on V5)
// https://github.com/tzapu/WiFiManager,
// https://github.com/bblanchon/ArduinoJson/issues/1276
//...........................................................................................................................
// 
//...........................................................................................................................
void saveConfigCallback () {
  Serial.println("Should save config");
  shouldSaveConfig = true;
}
//...........................................................................................................................
// 
//...........................................................................................................................
void WiFi_Manager(){

  //clean FS, for testing
  //SPIFFS.format();

  //read configuration from FS json
  Serial.println("mounting FS...");

  if (SPIFFS.begin()) {
    Serial.println("mounted file system");
    if (SPIFFS.exists("/config.json")) {
      //file exists, reading and loading
      Serial.println("reading config file");
      File configFile = SPIFFS.open("/config.json", "r");
      if (configFile) {
        Serial.println("opened config file");
        size_t size = configFile.size();
        // Allocate a buffer to store contents of the file.
        std::unique_ptr<char[]> buf(new char[size]);

        configFile.readBytes(buf.get(), size);
        DynamicJsonBuffer jsonBuffer;
        JsonObject& json = jsonBuffer.parseObject(buf.get());
        json.printTo(Serial);
        if (json.success()) {
          Serial.println("\nparsed json");

          //strcpy(mqtt_server, json["mqtt_server"]);
          //strcpy(mqtt_port, json["mqtt_port"]);
          strcpy(blynk_token, json["blynk_token"]);
          //strcpy(PushsaferKey, json["PushsaferKey"]);

        } else {
          Serial.println("failed to load json config");
        }
        configFile.close();
      }
    }
  } else {
    Serial.println("failed to mount FS");
  }
  //end read

  // The extra parameters to be configured (can be either global or just in the setup)
  // After connecting, parameter.getValue() will get you the configured value
  // id/name placeholder/prompt default length
  
  WiFiManagerParameter custom_blynk_token("blynk", "blynk token", blynk_token, 32);
 // WiFiManagerParameter custom_blynk_token("blynk", "blynk token", blynk_token, 32);
  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;

  //set config save notify callback
  wifiManager.setSaveConfigCallback(saveConfigCallback);

  //set static ip
  //wifiManager.setSTAStaticIPConfig(IPAddress(10,0,1,99), IPAddress(10,0,1,1), IPAddress(255,255,255,0));
  
  //add all your parameters here
   wifiManager.addParameter(&custom_blynk_token);
  
  //reset settings - for testing
  // Press and hold the button to erase all the credentials
  if (digitalRead(Flash_Button) == LOW)  {
     digitalWrite(Status_LED, HIGH); // set LED OFF
     Serial.println("reset wifi and blynk token settings");
     for(int i = 0; i < 35; i++)blynk_token[i] = 0;
     wifiManager.resetSettings();
     delay(500);
     digitalWrite(Status_LED, LOW); // set LED ON
  }

  //set minimu quality of signal so it ignores AP's under that quality
  //defaults to 8%
  //wifiManager.setMinimumSignalQuality();
  
  //Configuration Portal Timeout
  //If you need to set a timeout so the ESP doesn't hang waiting to be configured, for instance after a power failure, you can add
  wifiManager.setConfigPortalTimeout(120);

  //fetches ssid and pass and tries to connect
  //if it does not connect it starts an access point with the specified name
  //here  "AutoConnectAP"
  //and goes into a blocking loop awaiting configuration

  if (!wifiManager.autoConnect("Geyser Connect", "admin")) {
    Serial.println("failed to connect after a power failure and timeout, reset and try again"); //on the second attampt your WiFi may now be connected to the internet
    delay(3000);
    ESP.reset();
    delay(5000);
  }

  //if you get here you have connected to the WiFi
  Serial.println("connected...yeey :)");
  //E2_write(Login_Set,1); //Valid Login

  //read updated parameters
   strcpy(blynk_token, custom_blynk_token.getValue());

  //save the custom parameters to FS
  if (shouldSaveConfig) {
    Serial.println("saving config");
    DynamicJsonBuffer jsonBuffer;
    JsonObject& json = jsonBuffer.createObject();
    json["blynk_token"] = blynk_token;

    File configFile = SPIFFS.open("/config.json", "w");
    if (!configFile) {
      Serial.println("failed to open config file for writing");
    }

    json.printTo(Serial);
    json.printTo(configFile);
    configFile.close();
    //end save
  }

  Serial.print("local ip = "); Serial.println(WiFi.localIP());
  Serial.print("blynk_token = "); Serial.println(blynk_token);
  

}
