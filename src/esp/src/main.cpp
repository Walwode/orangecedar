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
int readAnalog(int vccPin);
int readDigital(int vccPin, int readPin);

void setup() {
  Serial.begin(115200);
  Serial.println();

  if (!connectWiFi()) { delay(5000); ESP.restart(); }

  createData();
  sendRequest();
}

void loop() {
  goSleep();
}

bool connectWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  pinMode(2, OUTPUT);
  // WiFiMulti.addAP(ssid, password);

  int retry = 0;
  Serial.print("[WiFi] Connecting...");
  // while ((WiFiMulti.run() != WL_CONNECTED) && (retry++ < 10)) {
  while ((WiFi.status() != WL_CONNECTED) && (retry++ < 10)) {
    digitalWrite(2, LOW);
    delay(500);
    digitalWrite(2, HIGH);
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  if (retry >= 11) {
    Serial.println("[WiFi] Could not connect to wifi.");
    return false;
  }

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
  if (http.begin(client, "http://api.walterheger.de/orangecedar/arduino.php")) {
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
  DynamicJsonDocument doc(128);
  doc["mac"] = WiFi.macAddress();
  doc["rssi"] = WiFi.RSSI();
  doc["esp"] = ESP.getChipId();
  doc["time"] = millis();
  // doc["voltage"] = readAnalog(D1); // hibiscus south
  // doc["humidity"] = readAnalog(D2); // hibiscus south
  doc["voltage"] = readAnalog(D1); // bathroom window
  doc["window"] = readDigital(D2, D5); // bathroom window (D3 has pull down resistor)
  serializeJson(doc, data);
}

void goSleep() {
  Serial.println("[ESP] Going to sleep...");
  // ESP.deepSleep(60e6 * 1); // hibiscus south
  ESP.deepSleep(60e6 * 5); // bathroom window
  delay(100);
}

int readAnalog(int vccPin) {
  pinMode(vccPin, OUTPUT);
  digitalWrite(vccPin, HIGH);
  delay(100);
  for (int i = 0; i < 10; i++) { analogRead(A0); delay(5); }
  delay(50);
  int value = analogRead(A0);
  Serial.print("Pin ");
  Serial.print(vccPin);
  Serial.print(": ");
  Serial.println(value);
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
  Serial.print("Pin ");
  Serial.print(vccPin);
  Serial.print(": ");
  Serial.println(value);
  return value;
}