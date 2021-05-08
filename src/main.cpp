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


WiFiClient wifiClient;

PubSubClient mqtt(wifiClient);

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

  mqtt.setServer("koderman.net",1883);
  mqtt.setCallback(mqttcallback);

  //strcpy(WiFiSSID, preferences.getString("SSID","NULL");
  preferences.getString("SSID","NULL").toCharArray(WiFiSSID, 32);
  preferences.getString("Password","NULL").toCharArray(WiFiPassword,32);

  Serial.println(String(WiFiSSID)+" "+String(WiFiPassword));
  //strcpy(WiFiPassword, preferences.getString("Password","NULL");



  pinMode(BTNL, INPUT_PULLUP);
  pinMode(BTNR, INPUT_PULLUP);

  display.begin(SH1106_SWITCHCAPVCC, 0x3C); 
  display.clearDisplay();
   pinMode(DCOUT1, OUTPUT);
   pinMode(DCOUT2, OUTPUT);
   pinMode(DCOUT3, OUTPUT);
   pinMode(DCOUT4, OUTPUT);
  
   pinMode(ACOUT1, OUTPUT);
   pinMode(ACOUT2, OUTPUT);
   pinMode(ACOUT3, OUTPUT);
   pinMode(ACOUT4, OUTPUT);

  // ledcSetup(0, 5000,8);
  // ledcAttachPin(BUZZER, 0);

  bool flagBTpairMode = false;

  unsigned long int buttonPressTimeout = 0;
  unsigned long int wifiTryTimeout = 0;
  bool buttonsPressed = false;
  bool stattimeout = false;

  for(int i = 1; i<=100; i++)
  {
    drawLoadingBar(14,32,100,10,i);

    if((millis()-wifiTryTimeout > 5000 || wifiTryTimeout==0) && WiFi.status() != WL_CONNECTED)
    {
      initWiFi(WiFiSSID, WiFiPassword);
      wifiTryTimeout = millis();
      stattimeout = true;
    }
    if(WiFi.status() == WL_CONNECTED && stattimeout)
      {
        display.drawBitmap(128-31, 5, wifiLogo, 26,21,WHITE);
        display.display();
        Serial.println(WiFi.localIP());
        stattimeout = false;

        if(!mqtt.connected())
          mqttreconnect(mqtt);
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

    preferences.getString("SSID",WiFiSSID,32);
    preferences.getString("Password", WiFiPassword,32);
    display.clearDisplay();
    display.display();



  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println(WiFi.macAddress());
  
  display.drawBitmap(128-31, 5, wifiLogo, 26,21,WHITE);
  if(WiFi.status() != WL_CONNECTED)
  {
    display.drawLine(128-31, 27, 128-3, 3, 1);
    display.drawLine(128-32, 26, 128-4, 4, 1);
    display.drawLine(128-31, 25, 128-3, 2, 1);
  }
  else
  {
    display.setCursor(0,10);
    display.println(WiFi.localIP());
  }
  display.display();
}

unsigned long sec = millis();

void loop() {
  mqtt.loop();
  if(!mqtt.connected())
      mqttreconnect(mqtt);

  if(millis()-sec>1000)
  {
    readNsend(mqtt);
    sec = millis();
  }
}




void drawLoadingBar(int x0,int y0,int width,int height, int percentage)
{
  display.drawRect(x0,y0,width,height,1);
  display.fillRect(x0,y0,percentage,height, 1);
  display.display();
}

