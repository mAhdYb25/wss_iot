#define BUTTON_PIN 13
uint32_t lastTimeKeyPressed = 0;
void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT);
}
void loop() {
  if (digitalRead(BUTTON_PIN)) {
    if (millis() - lastTimeKeyPressed > 500)
      Serial.println("key pressed");
    lastTimeKeyPressed = millis();
  }
}
