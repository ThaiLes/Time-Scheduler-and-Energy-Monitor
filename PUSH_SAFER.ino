// Take a look at the Pushsafer.com API at
// https://www.pushsafer.com/en/pushapi
//...........................................................................................................................
//
//...........................................................................................................................
void PushSafer(){

/*WiFiClientSecure client;*/
WiFiClient client;
Pushsafer pushsafer(PushsaferKey, client);

  pushsafer.debug = true;
  struct PushSaferInput input;
  input.message = "Geyser ON";
  input.title = "Geyser Home";
  input.sound = "1";
  input.vibration = "1";
  input.icon = "1";
  input.iconcolor = "#FFCCCC";
  input.priority = "1";
  input.device = "a";
  
  //input.url = "https://www.pushsafer.com";
  //input.urlTitle = "Open Pushsafer.com";
  //input.picture = "";
  //input.picture2 = "";
  //input.picture3 = "";
  //input.time2live = "";
  //input.retry = "";
  //input.expire = "";
  //input.answer = "";

  Serial.println(pushsafer.sendEvent(input));
  Serial.println("Sent");
}
