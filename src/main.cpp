#include "Arduino.h"
#include "plc.h"

#include "Wire.h"
#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_SH1106.h"
#include "images.h"
#include <cmath>


Adafruit_SH1106 display(21,22);

bool toggle = false;

void drawLoadingBar(int x0,int y0,int width,int height, int percentage);
void bluetoothPairingMode();


void setup() {
  Serial.begin(115200);

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
  // //ledcWriteTone(0, 440);


  // //WORKS
  // digitalWrite(DCOUT1, HIGH);
  // digitalWrite(DCOUT2, HIGH);
  // digitalWrite(DCOUT3, HIGH);
  // digitalWrite(DCOUT4, HIGH);
  

  // digitalWrite(ACOUT1, HIGH);
  // digitalWrite(ACOUT2, HIGH);
  // digitalWrite(ACOUT3, HIGH);
  // digitalWrite(ACOUT4, HIGH);

  // //ledcWriteTone(0, 440);
  
  // delay(1000);
  // digitalWrite(DCOUT1, LOW);
  // digitalWrite(DCOUT2, LOW);
  // digitalWrite(DCOUT3, LOW);
  // digitalWrite(DCOUT4, LOW);

  // digitalWrite(ACOUT1, LOW);
  // digitalWrite(ACOUT2, LOW);
  // digitalWrite(ACOUT3, LOW);
  // digitalWrite(ACOUT4, LOW);
  // //ledcWriteTone(0, 0);
  //display.drawBitmap(0, 0, Klogo, 128, 64, 1);
  //display.drawBitmap(0, 0,  Klogo, 128, 64, 1);
 //display.display();



  //display.drawCircle(display.width()/2, display.height()/2, 30, 1);

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
      }
      else if (millis()-buttonPressTimeout>2000 && buttonsPressed)
        flagBTpairMode = true;
    }
    else
      buttonsPressed= false;
  }
  if(flagBTpairMode)
    bluetoothPairingMode();

  
  delay(1000);
  display.clearDisplay();
  //display.display();
  //delay(10000);
  
}
 

 int counter =0;
 String  hw = "Test 2";
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
   display.clearDisplay();
  for(int i = 0; i<20; i++)
  {
    if(i%2==0)
    display.drawBitmap(display.width()/2-21/2,display.height()/2-32/2,BTLogo,21,32,1);
    else
    display.clearDisplay();
    display.display();
    delay(500);
  }
}