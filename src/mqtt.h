#pragma once
#include "PubSubClient.h"
#include <WiFi.h>
#include <Arduino.h>
#include "ArduinoJson.h"
//PubSubClient PSclient;
StaticJsonDocument<8192> doc;
StaticJsonDocument<1024> tdoc;
char mac[18];
void mqttreconnect(PubSubClient &mqtt){
    Serial.print("Attempting MQTT connection...");
    WiFi.macAddress().toCharArray(mac, 18);
    if(mqtt.connect(mac))
    {
        Serial.println("connected");
        //mqtt.publish("test", "hello world");
        mqtt.subscribe(mac);
        //mqtt.subscribe("test");
    }
    else {
    Serial.print("failed, rc=");
    Serial.print(mqtt.state());
    Serial.println(" try again in 5 seconds");
    delay(5000);
    }
}

void mqttcallback(char* topic, byte* payload, unsigned int length) 
{
  String message;
  if(doc["objType"]=="cts")
    return;
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0;i<length;i++) {
    message+= (char)payload[i];
  }
  Serial.println(message);

  //TODO: Recieve messages and do logic
  
  DeserializationError error = deserializeJson(doc, message);

  if(error){
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }

  
  
  //Serial.println(<String>doc["AC1"]);
  //if(doc["AC1"] != NULL)
    digitalWrite(ACOUT1, doc["AC1"]);
  //if(doc["AC2"] != NULL)
    digitalWrite(ACOUT2, doc["AC2"]);
 // if(doc["AC3"] != NULL)
    digitalWrite(ACOUT3, doc["AC3"]);
  //if(doc["AC4"] != NULL)
    digitalWrite(ACOUT4, doc["AC4"]);
  
  //if(doc["DC1"] != NULL)
    digitalWrite(DCOUT1, doc["DC1"]);
  //if(doc["DC2"] != NULL)
    digitalWrite(DCOUT2, doc["DC2"]);
  //if(doc["DC3"] != NULL)
    digitalWrite(DCOUT3, doc["DC3"]);
  //if(doc["DC4"] != NULL)
    digitalWrite(DCOUT4, doc["DC4"]);

  //if(doc["IO1"] != NULL)
    digitalWrite(IO1, doc["IO1"]);
  //if(doc["IO2"] != NULL)
    digitalWrite(IO2, doc["IO2"]);
  //if(doc["IO3"] != NULL)
    digitalWrite(IO3, doc["IO3"]);
 // if(doc["IO4"] != NULL)
    digitalWrite(IO4, doc["IO4"]);
  //if(doc["IO5"] != NULL)
    digitalWrite(IO4, doc["IO4"]);

  //if(doc["AO1"] != NULL)
    dacWrite(AO1, doc["AO1"]);
 // if(doc["AO2"] != NULL)
    dacWrite(AO2, doc["AO2"]);

  //if(doc["BUZZER"] != NULL)  
    digitalWrite(BUZZER, doc["BUZZER"]);
}

 void readNsend(PubSubClient &mqtt){
   tdoc["objType"] = "cts";
   tdoc["I1"] = analogRead(I1);
   tdoc["I2"] =  analogRead(I2);
   tdoc["VP"] = analogRead(VP);
   tdoc["VN"] = analogRead(VN);
   char msg[64];
   serializeJson(tdoc, msg);

   mqtt.publish(mac, msg);
 }