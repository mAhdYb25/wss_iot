#include <WiFi.h>
#include <WiFiClient.h>
#include <PubSubClient.h>

#define SSID "galax"
#define PASSWORD "123456789"
#define MQTT_BROKER "broker.emqx.io"
#define MQTT_PORT 1883

WiFiClient espClient;
PubSubClient mqttClient(espClient);

void mqttCallback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message:");
  String mqttMessage;
  for (int i = 0; i < length; i++)
    mqttMessage += (char)payload[i];
  Serial.println(mqttMessage);
}

void setup(void) {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASSWORD);
  mqttClient.setServer(MQTT_BROKER, MQTT_PORT);
  mqttClient.setCallback(mqttCallback);
  while (!mqttClient.connected()) {
    String client_id = "esp32-client-";
    client_id += String(WiFi.macAddress());
    Serial.printf("The client %s connects to the public MQTT broker\n", client_id.c_str());
    if (mqttClient.connect(client_id.c_str())) {
      Serial.println("MQTT broker connected");
    } else {
      Serial.print("failed with state ");
      Serial.print(mqttClient.state());
      delay(2000);
    }
  }
}

void loop(void) {
  mqttClient.loop();
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    processCommand(command);
  }
  delay(2);  //allow the cpu to switch to other tasks
}

//This function parses keys and values in this format:
//argName1=argValue1&argname2=argValue2&argName3=argValue3
void processCommand(String command) {
  String params[4][2];
  uint8_t i = 0;
  int eqIdx, andIdx;
  do {
    eqIdx = command.indexOf("=");
    andIdx = command.indexOf("&");
    String key = command.substring(0, eqIdx);
    String value = command.substring(eqIdx + 1, andIdx);
    //Serial.println("key:" + key + "\tvalue: " + value);
    params[i][0] = key;
    params[i][1] = value;
    i++;
    command = command.substring(andIdx + 1);
  } while (andIdx != -1);
  //subscribing for a topic using this command in serial:
  //command=sub&topic=anything
  if (params[0][1] == "sub") {
    String topic = params[1][1];
    Serial.println("Subsrcibing in topic:" + topic);
    mqttClient.subscribe(topic.c_str());
  }
  //Sending a message in a topic using this command in serial:
  //command=pub&topic=anything&message=anythingElse
  else if (params[0][1] == "pub") {
    String topic = params[1][1];
    String message = params[2][1];
    Serial.println("Publishing message in topic:" + topic + " with payload: " + message);
    mqttClient.publish(topic.c_str(), message.c_str());
  }
}
