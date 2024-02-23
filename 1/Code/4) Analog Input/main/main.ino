#define LDR_PIN 32
void setup() {
  Serial.begin(115200);
}
void loop() {
  uint16_t analogValue = analogRead(LDR_PIN);
  float voltage = (float)analogValue * 3.3 / 4096.0;
  String brightness;
  if (analogValue < 1000)
    brightness = "dark";
  else if (analogValue < 3000)
    brightness = "normal";
  else
    brightness = "bright";
  Serial.print("voltage is: ");
  Serial.print(voltage);
  Serial.print(" and brightness is: ");
  Serial.println(brightness);
  delay(500);
}
