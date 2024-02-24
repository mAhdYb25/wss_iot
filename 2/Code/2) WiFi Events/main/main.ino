#include <WiFi.h>
 
const char* ssid = "YOUR-SSID";
const char* password = "YOUR-PASSWORD";

void wifiConnectedHandler(WiFiEvent_t event, WiFiEventInfo_t info){
  Serial.println("connected to WiFi access point!");
}

void wifiIpHandler(WiFiEvent_t event, WiFiEventInfo_t info){
  Serial.println("Wifi got Ip. IP address: ");
  Serial.println(WiFi.localIP());
}

void wifiDisconnectedHandler(WiFiEvent_t event, WiFiEventInfo_t info){
  Serial.println("Disconnected from WiFi access point");
  // WiFi.begin(ssid, password);
}

void setup(){
  Serial.begin(115200);
  WiFi.disconnect(true);
  delay(1000);

  WiFi.onEvent(wifiConnectedHandler, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_CONNECTED);
  WiFi.onEvent(wifiIpHandler, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_GOT_IP);
  WiFi.onEvent(wifiDisconnectedHandler, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);
  
  WiFi.begin(ssid, password);
  Serial.println("Wait for WiFi... ");
}

void loop(){
  delay(1000);
}