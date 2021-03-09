#include "Arduino.h"
#include "plc.h"

#include "Wire.h"
#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_SH1106.h"
#include "images.h"
#include <cmath>
#include "BluetoothSerial.h"
#include <WiFi.h>
#include <Preferences.h>
#include <ArduinoJson.h>
Preferences preferences;

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif



Adafruit_SH1106 display(21,22); // Inicializira display

BluetoothSerial SerialBT; // Inicializira bluetooth

//Deklaracije funkcij
//TODO: Prestavi v headerje
void drawLoadingBar(int x0,int y0,int width,int height, int percentage);
void bluetoothPairingMode();

char WiFiSSID[32];
char WiFiPassword[32];
StaticJsonDocument<128> docWiFi;

void setup() {
  Serial.begin(115200);
  Serial.print("ESP Board MAC Address:  ");
  Serial.println(WiFi.macAddress());

  preferences.begin("Settings", false);

  //strcpy(WiFiSSID, preferences.getString("SSID","NULL");
  preferences.getString("SSID","NULL").toCharArray(WiFiSSID, 32);
  preferences.getString("Password","NULL").toCharArray(WiFiPassword,32);

  Serial.println(String(WiFiSSID)+" "+String(WiFiPassword));
  //strcpy(WiFiPassword, preferences.getString("Password","NULL");

  pinMode(BTNL, INPUT_PULLUP);
  pinMode(BTNR, INPUT_PULLUP);

  display.begin(SH1106_SWITCHCAPVCC, 0x3C); 
  display.clearDisplay();
  // pinMode(DCOUT1, OUTPUT);
  // pinMode(DCOUT2, OUTPUT);
  // pinMode(DCOUT3, OUTPUT);
  // pinMode(DCOUT4, OUTPUT);
  
   pinMode(ACOUT1, OUTPUT);
  // pinMode(ACOUT2, OUTPUT);
  // pinMode(ACOUT3, OUTPUT);
  // pinMode(ACOUT4, OUTPUT);

  // ledcSetup(0, 5000,8);
  // ledcAttachPin(BUZZER, 0);

  bool flagBTpairMode = false;

  unsigned long int buttonPressTimeout = 0;

  bool buttonsPressed = false;

  for(int i = 1; i<=100; i++)
  {
    drawLoadingBar(14,32,100,10,i);

    if(!digitalRead(BTNL) && !digitalRead(BTNR))
    {
      if(buttonPressTimeout == 0)
      {
        buttonPressTimeout = millis();
        buttonsPressed = true;
        Serial.println("both buttons pressed");
      }
      else if (millis()-buttonPressTimeout>2000 && buttonsPressed)
      {
        flagBTpairMode = true;
        Serial.println("entering bt mode...");
      }
    }
    else
      buttonsPressed= false;
  }
  if(flagBTpairMode)
    bluetoothPairingMode();

  display.clearDisplay();
  display.drawBitmap(0, 0,  Klogo, 128, 64, 1);
  display.display();
  delay(1000);
  display.clearDisplay();
  display.display();
}

void loop() {

}




void drawLoadingBar(int x0,int y0,int width,int height, int percentage)
{
  display.drawRect(x0,y0,width,height,1);
  display.fillRect(x0,y0,percentage,height, 1);
  display.display();
}

void bluetoothPairingMode()
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

