#pragma once
#include "PubSubClient.h"
#include <WiFi.h>
#include <Arduino.h>

//PubSubClient PSclient;

void mqttcallback(char* topic, byte* payload, unsigned int length) 
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}


void mqttreconnect(PubSubClient PSclient){
    Serial.print("Attempting MQTT connection...");
    char mac[18];
    WiFi.macAddress().toCharArray(mac, 18);
    if(PSclient.connect(mac))
    {
        Serial.println("connected");
        PSclient.publish("test", "hello world");
        PSclient.subscribe("test");
    }
        else {
    Serial.print("failed, rc=");
    Serial.print(PSclient.state());
    Serial.println(" try again in 5 seconds");
    delay(5000);
    }
}