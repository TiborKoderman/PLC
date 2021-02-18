

#include "Arduino.h"
#include "plc.h"
//#include tone

void setup() {
  Serial.begin(115200);
  pinMode(DCOUT1, OUTPUT);
  pinMode(DCOUT2, OUTPUT);
  pinMode(DCOUT3, OUTPUT);
  pinMode(DCOUT4, OUTPUT);

  pinMode(ACOUT1, OUTPUT);
  pinMode(ACOUT2, OUTPUT);
  pinMode(ACOUT3, OUTPUT);
  pinMode(ACOUT4, OUTPUT);

  ledcSetup(0, 5000,8);
  ledcAttachPin(BUZZER, 0);
  //ledcWriteTone(0, 440);


  //WORKS
  digitalWrite(DCOUT1, HIGH);
  digitalWrite(DCOUT2, HIGH);
  digitalWrite(DCOUT3, HIGH);
  digitalWrite(DCOUT4, HIGH);
  

  digitalWrite(ACOUT1, HIGH);
  digitalWrite(ACOUT2, HIGH);
  digitalWrite(ACOUT3, HIGH);
  digitalWrite(ACOUT4, HIGH);

  //ledcWriteTone(0, 440);
  
  delay(1000);
  digitalWrite(DCOUT1, LOW);
  digitalWrite(DCOUT2, LOW);
  digitalWrite(DCOUT3, LOW);
  digitalWrite(DCOUT4, LOW);

  digitalWrite(ACOUT1, LOW);
  digitalWrite(ACOUT2, LOW);
  digitalWrite(ACOUT3, LOW);
  digitalWrite(ACOUT4, LOW);
  //ledcWriteTone(0, 0);
  delay(500);
  
}
 
void loop() {
  Serial.println("wait, wnat");

  
}