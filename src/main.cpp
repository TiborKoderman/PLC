#include <Arduino.h>
#include <WiFi.h>
#include <Preferences.h>

//
#include "plc.h"

//
#include "Adafruit_GFX.h"
#include "Adafruit_SH1106.h"
#include "images.h"

//wireless conectivity functions
#include "bluetooth.h"
#include "wifiHeader.h"
#include "mqtt.h"

PubSubClient PSclient;

Preferences preferences;

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

Adafruit_SH1106 display(-1,-1); // Inicializira display

void drawLoadingBar(int x0,int y0,int width,int height, int percentage);

char WiFiSSID[32];
char WiFiPassword[32];

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

  PSclient.setServer("koderman.net",1883);
  PSclient.setCallback(mqttcallback);

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
  unsigned long int wifiTryTimeout = 0;
  bool buttonsPressed = false;

  for(int i = 1; i<=100; i++)
  {
    drawLoadingBar(14,32,100,10,i);

    if((millis()-wifiTryTimeout > 3000 || wifiTryTimeout==0) && WiFi.status() != WL_CONNECTED)
    {
      initWiFi(WiFiSSID, WiFiPassword);
      display.drawBitmap(128-31, 5, wifiLogo, 26,21,WHITE);
      display.display();
      wifiTryTimeout = millis();
    }

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
    bluetoothPairingMode(display, preferences);

    
  if(!PSclient.connected())
  {
      mqttreconnect(PSclient);
  }

    preferences.getString("SSID",WiFiSSID,32);
    preferences.getString("Password", WiFiPassword,32);
    display.clearDisplay();
    display.display();
  display.drawBitmap(128-31, 5, wifiLogo, 26,21,WHITE);
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



