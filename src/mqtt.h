#pragma once
#include "PubSubClient.h"
#include <WiFi.h>
#include <Arduino.h>
#include "ArduinoJson.h"
//PubSubClient PSclient;
StaticJsonDocument<1024> docWiFi;

void mqttreconnect(PubSubClient &mqtt){
    Serial.print("Attempting MQTT connection...");
    char mac[18];
    WiFi.macAddress().toCharArray(mac, 18);
    if(mqtt.connect(mac))
    {
        Serial.println("connected");
        mqtt.publish("test", "hello world");
        mqtt.subscribe(mac);
        mqtt.subscribe("test");
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
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0;i<length;i++) {
    message+= (char)payload[i];
  }
  Serial.println(message);

  JSON.

}