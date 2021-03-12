#pragma once
#include <Arduino.h>
#include "WiFi.h"
#include "BluetoothSerial.h"
#include <ArduinoJson.h>
#include "Preferences.h"
//#include "Adafruit_SH1106.h"
#include "images.h"

//BluetoothSerial SerialBT;
BluetoothSerial SerialBT;
StaticJsonDocument<128> docWiFi;

void bluetoothPairingMode(Adafruit_SH1106 display, Preferences preferences)
{
  Serial.println("Entering pairing mode");
  long int timeout = millis();
  bool on = true;
   SerialBT.begin("ESP "+WiFi.macAddress());
   display.clearDisplay();

    while(!SerialBT.connected())
    {
      if(millis()-timeout > 500)
      {
        if(on)
          display.drawBitmap(display.width()/2-21/2,display.height()/2-32/2,BTLogo,21,32,1);
        else
          display.clearDisplay();

        display.display();
        on = !on;
        timeout = millis();
      }
    }

    display.drawBitmap(display.width()/2-21/2,display.height()/2-32/2,BTLogo,21,32,1);
        display.display();

    while(SerialBT.connected())
    {
      Serial.println("Entered loop");

          


      if(SerialBT.available()){
        String json = SerialBT.readString();
        Serial.println(json);

      DeserializationError err = deserializeJson(docWiFi,json);
      if (err) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(err.f_str());
        return;
        }
      //TODO: Check input logic, parse input

      char ssid[32];
      char pass[32];
      Serial.println("Updating... ssid: "+String(ssid)+" password: "+String(pass));
      strcpy(ssid,docWiFi["ssid"]);
      strcpy(pass, docWiFi["pass"]);

      
      if(strlen(ssid)>0 && strlen(pass)>0)
      {
      preferences.putString("SSID", ssid);
      preferences.putString("Password", pass);
      Serial.println("Succesfully updated ssid: "+String(ssid)+" password: "+String(pass));
      SerialBT.disconnect();
      }
      }
    }
    
  }