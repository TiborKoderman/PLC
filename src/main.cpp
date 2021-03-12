#include "Arduino.h"
#include "plc.h"

#include "Adafruit_GFX.h"
#include "Adafruit_SH1106.h"
#include "images.h"
#include <WiFi.h>
#include <Preferences.h>


#include "bluetooth.h"
#include "wifiHeader.h"
//#include "display.h"

Preferences preferences;

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

Adafruit_SH1106 display(21,22); // Inicializira display

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
    bluetoothPairingMode(display, preferences);

    preferences.getString("SSID",WiFiSSID,32);
    preferences.getString("Password", WiFiPassword,32);
      initWiFi(WiFiSSID, WiFiPassword);
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



