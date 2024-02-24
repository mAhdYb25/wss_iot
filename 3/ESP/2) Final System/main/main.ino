#include <DHT22.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include "html.h"

#define STA_SSID "galax"
#define STA_PASSWORD "123456789"
#define MQTT_BROKER "broker.emqx.io"
#define MQTT_PORT 1883
#define PUBLISH_TOPIC "wss/iot/data"
#define SUBSCRIBE_TOPIC "wss/iot/command"
#define DATA_TIME_INTERVAL 10 * 1000

#define DHT_PIN 33
#define LDR_PIN 32
#define RELAY_PIN 27
#define BUTTON_PIN 13

String ssid = STA_SSID;
String password = STA_PASSWORD;
bool relay = 0;
float temperature, humidity;
uint16_t light;
volatile bool buttonPressedFlag = false;
uint32_t lastTimeButtonPressed = 0;
uint32_t lastTimeDataSent = 0;

DHT22 dht22(DHT_PIN);
WiFiClient espClient;
WebServer server(80);
PubSubClient mqttClient(espClient);

//This function executes every time interval and whenever new event occurs on device to sync data with other clients
void publishDataToBroker() {
  if(!mqttClient.connected())
    mqttClientSetup();
  JsonDocument doc;
  String jsonMessage;
  temperature = dht22.getTemperature();
  humidity = dht22.getHumidity();
  light = analogRead(LDR_PIN);
  doc["temperature"] = temperature;
  doc["humidity"] = humidity;
  doc["light"] = light;
  doc["relay"] = relay;
  doc["wifi"] = ssid;
  //json format is used for data
  serializeJson(doc, jsonMessage);
  mqttClient.publish(PUBLISH_TOPIC, jsonMessage.c_str());
  lastTimeDataSent = millis();
}

//this functions executes whenever a new mqtt message arrives
void mqttCallback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message:");
  String jsonMessage;
  for (int i = 0; i < length; i++) {
    jsonMessage += (char)payload[i];
  }
  Serial.println(jsonMessage);
  JsonDocument doc;
  deserializeJson(doc, jsonMessage);
  //{"relay":true}
  if (doc.containsKey("relay")) {
    relay = doc["relay"];
    digitalWrite(RELAY_PIN, !relay);
    publishDataToBroker();
  }
}

void handleRoot() {
  String wifiNetworks;
  //Check whether any parameter is available in HTTP request
  if (server.args() > 0) {
    //switching relay
    if (server.argName(0) == "relay") {
      relay = server.arg(0).toInt();
      digitalWrite(RELAY_PIN, !relay);
      publishDataToBroker();
    }
    //scanning available wifi networks
    else if (server.argName(0) == "scan") {
      int n = WiFi.scanNetworks();
      wifiNetworks = "<b>List of available networks</b>";
      for (int i = 0; i < n; ++i)
        wifiNetworks += "<li class='list-group-item'>" + WiFi.SSID(i) + "</li>";
      WiFi.scanDelete();
    }
    //connecting to a wifi network
    else if (server.argName(0) == "ssid") {
      ssid = server.arg(0);
      password = server.arg(1);
      server.send(200, "text/html", "<h1>connecting to " + ssid + " ... </h1>");
      connectWifi();
      return;
    }
  }

  //Perparing data required by app
  temperature = dht22.getTemperature();
  humidity = dht22.getHumidity();
  light = analogRead(LDR_PIN);
  String lightStat;
  if (light < 1000)
    lightStat = "dark";
  else if (light < 3000)
    lightStat = "normal";
  else
    lightStat = "bright";
  lightStat += " | " + String(light);
  String relayStat = relay ? "on" : "off";
  String wifiStat = (WiFi.status() != WL_CONNECTED) ? "disconnected" : "connected to " + ssid;

  //Placing appropriate data in HTML template in html.h
  String html = HTML_CONTENT;
  html.replace("{{temperature}}", String(temperature));
  html.replace("{{humidity}}", String(humidity));
  html.replace("{{light}}", lightStat);
  html.replace("{{relay}}", relayStat);
  html.replace("{{wifiStat}}", wifiStat);
  html.replace("{{wifiNetworks}}", wifiNetworks);

  //Sending rendred html file to user
  server.send(200, "text/html", html);
}

//Connecting to Wifi network
void connectWifi() {
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  Serial.println("Connecting to: " + ssid + " with password:" + password);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

//Setting up local web server
void localWebServerSetup() {
  server.on("/", handleRoot);
  server.begin();
  Serial.println("HTTP server started");
}

//Connecing mqtt client to broker
void mqttClientSetup() {
  mqttClient.setServer(MQTT_BROKER, MQTT_PORT);
  mqttClient.setCallback(mqttCallback);
  while (!mqttClient.connected()) {
    String client_id = "esp32-client-";
    client_id += String(WiFi.macAddress());
    Serial.printf("The client %s is connecting to the MQTT broker\n", client_id.c_str());
    if (mqttClient.connect(client_id.c_str())) {
      Serial.println("MQTT broker connected");
    } else {
      Serial.print("failed with state ");
      Serial.print(mqttClient.state());
      delay(2000);
    }
  }
  mqttClient.publish(PUBLISH_TOPIC, "Hello from ESP32");
  mqttClient.subscribe(SUBSCRIBE_TOPIC);
}

//Interrupt Service Routine function when key is pressed
void IRAM_ATTR buttonIsr() {
  //software debouncing using minimum 500ms time interval between each key press
  if (millis() - lastTimeButtonPressed > 500) {
    buttonPressedFlag = true;
    lastTimeButtonPressed = millis();
  }
}

void setup(void) {
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, !relay);
  pinMode(BUTTON_PIN, INPUT);
  attachInterrupt(BUTTON_PIN, buttonIsr, RISING);
  Serial.begin(115200);
  connectWifi();
  localWebServerSetup();
  mqttClientSetup();
}

void loop(void) {
  //toggle relay when button is pressed
  if (buttonPressedFlag) {
    if (digitalRead(BUTTON_PIN)) {
      relay = !relay;
      digitalWrite(RELAY_PIN, !relay);
      buttonPressedFlag = false;
      Serial.println("key was pressed");
      publishDataToBroker();
    }
  }
  //Send data to broker every time interval
  if(millis() - lastTimeDataSent > DATA_TIME_INTERVAL)
    publishDataToBroker();
  mqttClient.loop();
  server.handleClient();
  delay(2);  //allow the cpu to switch to other tasks
}
