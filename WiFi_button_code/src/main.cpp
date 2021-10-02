
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <stdio.h>

const int bled = 12;
const int rled = 4;
const float vol_div = 0.175;
const char serverName[] = "http://192.168.1.102:80";
const char WIFI_ssid[] = "Y'Ghatan";
const char WIFI_pass[] = "06094829";

int sendPOST_toWebCounter(const char exercise[32], int number);
int checkVoltage();
void warnLowVoltage();
int connectToWiFi();

void setup() {
  Serial.begin(9600);
  Serial.println('\n');
  pinMode(rled, OUTPUT);
  pinMode(bled, OUTPUT);
  checkVoltage();
  digitalWrite(bled, HIGH);
  connectToWiFi();
  sendPOST_toWebCounter("Nadchwyt", 6);
  digitalWrite(bled, LOW);
  Serial.println("Going into deep sleep mode");
  ESP.deepSleep(0); 
}



void loop() {
  // put your main code here, to run repeatedly:
}


int sendPOST_toWebCounter(const char exercise[32], int number) 
{
  char requestData[128];
  WiFiClient client;
  HTTPClient http;
  sprintf(requestData, "%s_number=%u&%s=%%2B", exercise, number, exercise);
  Serial.println("requestForm:");
  Serial.println(requestData);
  http.begin(client, serverName);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  Serial.println("HTTP Response:");
  Serial.println(http.POST(requestData));
  http.end();
  return 0;
}

/*
@brief checks voltage level of the battery and warns user if the battery is low
@return 0 if battery is fine, 1 if battery is low
*/
int checkVoltage() 
{
  const float lowBatThreshold = 3.5;
  int rawRead = analogRead(A0);
  float readVoltage = (((float)rawRead/1023.00)*(1/vol_div));
  Serial.println(rawRead);
  Serial.println(readVoltage);
  if(readVoltage < lowBatThreshold){
    warnLowVoltage();
    return 1;
  }
  return 0;
}

void warnLowVoltage() 
{
  const int onTime = 300;
  const int offTime = 300;
  for(int i = 0; i<3; i++){
    digitalWrite(rled, HIGH);
    delay(onTime);
    digitalWrite(rled, LOW);
    delay(offTime);
  }
}

int connectToWiFi() 
{
  WiFi.begin(WIFI_ssid, WIFI_pass);
  Serial.print("Connecting to ");
  Serial.print(WIFI_ssid);
  Serial.println("...");
  while (WiFi.status() != WL_CONNECTED) { // Wait for the Wi-Fi to connect
    delay(250);
    Serial.print('.');
  }
  Serial.println('\n');
  Serial.println("Connection established!");
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());
  return 0;
}