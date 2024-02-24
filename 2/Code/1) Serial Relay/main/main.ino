#define RELAY_PIN 27
bool relay = 0;
void setup() {
  Serial.begin(115200);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN,!relay);
}
void loop() {
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    processCommand(command);
  }
}
//relay=1 , relay=0&timer=45
void processCommand(String command) {
  int eqIdx, andIdx;
  eqIdx = command.indexOf("=");
  andIdx = command.indexOf("&");
  String key = command.substring(0, eqIdx);
  String value = command.substring(eqIdx + 1, andIdx);
  if(key == "relay"){
    relay = value.toInt();
    digitalWrite(RELAY_PIN, !relay);
  }
}
