#pragma once
#include "WiFi.h"

void initWiFi(const char ssid[32],const char password[32]) 
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  //delay(5000);
}