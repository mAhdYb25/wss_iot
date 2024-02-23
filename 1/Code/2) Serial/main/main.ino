uint8_t counter = 0;
uint32_t lastSavedTime = 0;
void setup() {
  Serial.begin(115200);
}
void loop() {
  if(millis() - lastSavedTime > 1000){
    counter ++;
    Serial.print("One second past and counter is ");
    Serial.println(counter);
    lastSavedTime = millis();
  }
}





