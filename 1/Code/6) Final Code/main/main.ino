#include <DHT22.h>

#define LED_BUILTIN 2
#define LED_EXTERNAL 18
#define LDR 32
#define DHT 33
#define SWITCH 13

String brightness;
uint16_t light = 0;
bool ledInt = 0, ledExt = 0, btn;

float temperature, humidity;
DHT22 dht22(DHT);
SemaphoreHandle_t xMutex = xSemaphoreCreateMutex();  // Create a mutex object

void setup() {
  Serial.begin(115200);
  xTaskCreatePinnedToCore(toggleIntLED, "toggleIntLED", 1000, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(toggleExtLED, "toggleEx", 1000, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(lightDetect, "light", 1000, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(dhtProcess, "dht", 1000, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(dataMonitor, "serialMonitor", 1000, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(switchHandler, "PushBtn", 1000, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(serialInput, "serialInput", 1000, NULL, 1, NULL, 1);
}

void loop() {
}


void toggleIntLED(void* parameter) {
  vTaskDelay(10 / portTICK_PERIOD_MS);
  String res = "toggleIntLED running on core " + String(xPortGetCoreID());
  Serial.println(res);

  pinMode(LED_BUILTIN, OUTPUT);
  while (true) {
    ledInt = 1;
    digitalWrite(BUILTIN_LED, ledInt);
    vTaskDelay(500 / portTICK_PERIOD_MS);
    ledInt = 0;
    digitalWrite(BUILTIN_LED, ledInt);  
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

void toggleExtLED(void* parameter) {
  vTaskDelay(20 / portTICK_PERIOD_MS);
  String res = "toggleExtLED running on core " + String(xPortGetCoreID());
  Serial.println(res);

  pinMode(LED_EXTERNAL, OUTPUT);
  while (true) {
    ledExt = 1;
    digitalWrite(LED_EXTERNAL, ledExt);
    vTaskDelay(750 / portTICK_PERIOD_MS);
    ledExt = 0;
    digitalWrite(LED_EXTERNAL, ledExt);
    vTaskDelay(750 / portTICK_PERIOD_MS);
  }
}


void lightDetect(void* parameter) {
  vTaskDelay(30 / portTICK_PERIOD_MS);
  String res = "lightDetect running on core " + String(xPortGetCoreID());
  Serial.println(res);

  while (true) {
    light = analogRead(LDR);
    if (light < 1000)
      brightness = "dark";
    else if (light < 2000)
      brightness = "dim";
    else if (light < 3000)
      brightness = "semi-bright";
    else
      brightness = "bright";
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void dataMonitor(void* parameter) {
  vTaskDelay(40 / portTICK_PERIOD_MS);
  String res = "dataMonitor running on core " + String(xPortGetCoreID());
  Serial.println(res);

  while (true) {
    if (xSemaphoreTake(xMutex, portMAX_DELAY)) {
      String result = "LedInternal: " + String(ledInt);
      result += "\tLedExternal: " + String(ledExt);
      result += "\tlight: " + String(light + brightness);
      result += "\ttemperature: " + String(temperature) + "°c";
      result += "\thumidity: " + String(humidity) + "%";
      result += "\tswitch: " + String(btn);
      Serial.println(result);
      xSemaphoreGive(xMutex);  // release the mutex
      vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
  }
}


void dhtProcess(void* parameter) {
  vTaskDelay(50 / portTICK_PERIOD_MS);
  String res = "dhtProcess running on core " + String(xPortGetCoreID());
  Serial.println(res);

  while (true) {
    temperature = dht22.getTemperature();
    humidity = dht22.getHumidity();
    vTaskDelay(5000 / portTICK_PERIOD_MS);
  }
}

void switchHandler(void* parameter) {
  vTaskDelay(60 / portTICK_PERIOD_MS);
  String res = "switchHandler running on core " + String(xPortGetCoreID());
  Serial.println(res);

  pinMode(SWITCH, INPUT);
  while (true) {
    btn = digitalRead(SWITCH);
    vTaskDelay(20 / portTICK_PERIOD_MS);
  }
}


void serialInput(void* parameter) {
  vTaskDelay(70 / portTICK_PERIOD_MS);
  String res = "serialInput running on core " + String(xPortGetCoreID());
  Serial.println(res);

  while (true) {
    if (xSemaphoreTake(xMutex, portMAX_DELAY)) {
      if (Serial.available()) {
        String command = Serial.readStringUntil('\n');
        processCommand(command);
      }
      xSemaphoreGive(xMutex);  // release the mutex
    }
  }
}


void processCommand(String command) {
  int eqIdx, andIdx;
  String params[4][2];
  uint8_t i = 0;
  do {
    eqIdx = command.indexOf("=");
    andIdx = command.indexOf("&");
    String key = command.substring(0, eqIdx);
    String value = command.substring(eqIdx + 1, andIdx);
    Serial.println("key:" + key + "\tvalue: " + value);
    params[i][0] = key;
    params[i][1] = value;
    i++;
    command = command.substring(andIdx + 1);
  } while (andIdx != -1);
}
