#define BUTTON_PIN 13
uint32_t lastTimeKeyPressed = 0;
volatile bool flag = false;
void IRAM_ATTR isr() {
  if (millis() - lastTimeKeyPressed > 500)
    flag = true;
}
void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT);
  attachInterrupt(BUTTON_PIN, isr, RISING);
}
void loop() {
  if (flag) {
    flag = false;
    lastTimeKeyPressed = millis();
    Serial.println("key pressed");
  }
}


