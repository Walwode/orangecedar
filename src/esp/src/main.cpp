#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include "config.h"

ESP8266WiFiMulti WiFiMulti;
WiFiClient client;
HTTPClient http;
String data;

bool connectWiFi();
void sendRequest();
void createData();
void goSleep();

void setup() {
  Serial.begin(115200);

  if (!connectWiFi()) return;

  createData();
  sendRequest();
}

void loop() {
  goSleep();
}

bool connectWiFi() {
  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(ssid, password);

  int retry = 0;
  Serial.print("[WiFi] Connecting...");
  while ((WiFi.status() != WL_CONNECTED) && (retry++ < 10)) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println();

  if (retry >= 11) return false;

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

  return true;
}

void sendRequest() {
  if (http.begin(client, "http://allaroundthewalt.free.beeceptor.com/esp")) {
    Serial.print("[HTTP] request...");

    http.addHeader("Content-Type", "text/json");
    int httpCode = http.POST(data);

    if (httpCode > 0) {
      Serial.println(httpCode);
      Serial.println(http.getString());
    } else {
      Serial.println("failed");
    }
    http.end();
  }
}

void createData() {
  const size_t capacity = JSON_OBJECT_SIZE(4);
  DynamicJsonDocument doc(capacity);
  doc["mac"] = WiFi.macAddress();
  doc["rssi"] = WiFi.RSSI();
  doc["esp"] = ESP.getChipId();
  doc["time"] = millis();
  doc["window"] = 1;
  serializeJson(doc, data);
}

void goSleep() {
  // ESP.deepSleep(10e6, WAKE_RF_DEFAULT);
  ESP.deepSleep(10e6);
  delay(100);
}