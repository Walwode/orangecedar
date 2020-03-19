#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include "config.h"

// #define _Serial_
#define _Humidity_
// #define _Window_

#define VoltagePin D1
#define HumidityVccPin D2
#define WindowVccPin D2
#define WindowReadPin D5 // D3 has pull down resistor

ESP8266WiFiMulti WiFiMulti;
WiFiClient client;
HTTPClient http;
String data;

bool connectWiFi();
void sendRequest();
void createData();
void goSleep(uint64_t time);
int readAnalog(int vccPin);
int readDigital(int vccPin, int readPin);
void print(uint64_t value);

void setup() {
  #if defined(_Serial_)
  Serial.begin(115200);
  Serial.println();
  #endif

  createData();
  if (!connectWiFi()) { goSleep(60e6 * 30); }
  sendRequest();
  #if defined(_Humidity_)
  goSleep(60e6 * 60);
  #endif
  #if defined(_Window_)
  goSleep(60e6 * 5);
  #endif
}

void loop() {}

bool connectWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  pinMode(2, OUTPUT);
  // WiFiMulti.addAP(ssid, password);

  int retry = 0;
  #if defined(_Serial_)
  Serial.print("[WiFi] Connecting...");
  #endif
  // while ((WiFiMulti.run() != WL_CONNECTED) && (retry++ < 10)) {
  while ((WiFi.status() != WL_CONNECTED) && (retry++ < 10)) {
    digitalWrite(2, LOW);
    delay(500);
    digitalWrite(2, HIGH);
    delay(500);
    #if defined(_Serial_)
    Serial.print(".");
    #endif
  }
  #if defined(_Serial_)
  Serial.println();
  #endif

  if (retry >= 11) {
    #if defined(_Serial_)
    Serial.println("[WiFi] Could not connect to wifi.");
    #endif
    return false;
  }

  #if defined(_Serial_)
  Serial.print("[WiFi] SSID: ");
  Serial.println(WiFi.SSID());
  Serial.print("[WiFi] IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("[WiFi] RSSI: ");
  Serial.println(WiFi.RSSI());
  Serial.print("[WiFi] MAC: ");
  Serial.println(WiFi.macAddress());
  Serial.print("[WiFi] ESP Chip ID: ");
  Serial.println(ESP.getChipId());
  #endif

  return true;
}

void sendRequest() {
  if (http.begin(client, "http://api.walterheger.de/orangecedar/arduino.php")) {
    #if defined(_Serial_)
    Serial.print("[HTTP] request...");
    #endif

    http.addHeader("Content-Type", "text/json");
    int httpCode = http.POST(data);

    #if defined(_Serial_)
    if (httpCode > 0) {
      Serial.println(httpCode);
      Serial.println(http.getString());
    } else {
      Serial.println("failed");
    }
    #endif
    http.end();
  }
}

void createData() {
  DynamicJsonDocument doc(128);
  doc["mac"] = WiFi.macAddress();
  doc["rssi"] = WiFi.RSSI();
  doc["esp"] = ESP.getChipId();
  doc["time"] = millis();
  #if defined(_Humidity_)
  // doc["voltage"] = readAnalog(VoltagePin);
  doc["humidity"] = readAnalog(HumidityVccPin);
  #endif
  #if defined(_Window_)
  // doc["voltage"] = readAnalog(VoltagePin);
  doc["window"] = readDigital(WindowVccPin, WindowReadPin);
  #endif
  serializeJson(doc, data);
}

void goSleep(uint64_t time) {
  #if defined(_Serial_)
  Serial.print("[ESP] Going to sleep for ");
  // print(time);
  Serial.print(time/60e6);
  Serial.println("min...");
  #endif
  ESP.deepSleep(time);
  delay(100);
}

int readAnalog(int vccPin) {
  pinMode(vccPin, OUTPUT);
  digitalWrite(vccPin, HIGH);
  delay(10);
  for (int i = 0; i < 10; i++) { analogRead(A0); delay(5); }
  delay(20);
  int value = analogRead(A0);
  #if defined(_Serial_)
  Serial.print("Pin ");
  Serial.print(vccPin);
  Serial.print(": ");
  Serial.println(value);
  #endif
  digitalWrite(vccPin, LOW);
  return value;
}

int readDigital(int vccPin, int readPin) {
  pinMode(readPin, INPUT);
  digitalWrite(readPin, LOW);
  pinMode(vccPin, OUTPUT);
  digitalWrite(vccPin, HIGH);
  int value = digitalRead(readPin);
  digitalWrite(vccPin, LOW);
  #if defined(_Serial_)
  Serial.print("Pin ");
  Serial.print(vccPin);
  Serial.print(": ");
  Serial.println(value);
  #endif
  return value;
}

void print(uint64_t value)
{
    const int NUM_DIGITS    = log10(value) + 1;

    char sz[NUM_DIGITS + 1];
   
    sz[NUM_DIGITS] =  0;
    for ( size_t i = NUM_DIGITS; i--; value /= 10)
    {
        sz[i] = '0' + (value % 10);
    }
   
    Serial.print(sz);
}