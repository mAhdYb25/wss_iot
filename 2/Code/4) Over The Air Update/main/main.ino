#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
const char* ssid = "YOUR-SSID";
const char* password = "YOUR-PASSWORD";
void setup() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  ArduinoOTA.begin();
}
void loop() {
  ArduinoOTA.handle();
}

