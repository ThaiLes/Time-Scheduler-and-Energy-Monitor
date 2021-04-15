//...........................................................................................................................
//First Contact Notification
//...........................................................................................................................
BLYNK_CONNECTED() {
  if (isFirstConnect) {
    Blynk.syncAll();
    Blynk.notify("GEYSER BACK ONLINE");
    isFirstConnect = false;
    
  }
   rtc.begin();
}

//...........................................................................................................................
//If Blynk cloud server connection drops out reconnect
//...........................................................................................................................
void reconnectBlynk() {
  if (!Blynk.connected()) {
    if(Blynk.connect()) {
     BLYNK_LOG("Reconnected");
    } else {
      BLYNK_LOG("Not reconnected");
    }
  }
}

//...........................................................................................................................
//WiFi Status
//...........................................................................................................................
void sendWifi() {
  
  Blynk.virtualWrite(V0,WiFi.RSSI());
}