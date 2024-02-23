#include <DHT22.h>
//In this code every 5 seconds data of temperature, humidity, light and button state is shown using Serial
//In parallel, led blinks every 3 seconds


#define DHT_PIN 33
#define LDR_PIN 32
#define RELAY_PIN 27
#define BUTTON_PIN 13
#define LED_PIN 2

String brightness;
uint16_t light = 0;
bool led = 0, button;
float temperature, humidity;
DHT22 dht22(DHT_PIN);

void ledBlink(void* parameter) {
  while (true) {
    led = 1;
    digitalWrite(LED_PIN, led);
    delay(1500);
    led = 0;
    digitalWrite(LED_PIN, led);
    delay(1500);
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  xTaskCreatePinnedToCore(ledBlink, "serialInput", 1000, NULL, 1, NULL, 0);
}


void loop() {
  temperature = dht22.getTemperature();
  humidity = dht22.getHumidity();
  light = analogRead(LDR_PIN);
  if (light < 1000)
    brightness = "dark";
  else if (light < 2000)
    brightness = "dim";
  else if (light < 3000)
    brightness = "normal";
  else
    brightness = "bright";
  button = digitalRead(BUTTON_PIN);

  String result = "led: " + String(led);
  result += "\tlight: " + String(light + brightness);
  result += "\ttemperature: " + String(temperature) + "°c";
  result += "\thumidity: " + String(humidity) + "%";
  result += "\tswitch: " + String(button);
  Serial.println(result);
  delay(5000);
}


