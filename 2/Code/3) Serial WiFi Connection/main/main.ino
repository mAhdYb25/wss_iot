#include "WiFi.h"

String staSsid, staPassword;
void setup() {
  WiFi.mode(WIFI_STA);
  Serial.begin(115200);
}
void loop() {
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    processCommand(command);
  }
}
void processCommand(String command) {
  int eqIdx, andIdx;
  eqIdx = command.indexOf("=");
  andIdx = command.indexOf("&");
  String key = command.substring(0, eqIdx);
  String value = command.substring(eqIdx + 1, andIdx);
  //scan=true
  if (key == "scan") {
    scanNetworks();
  }
  //ssid=galax&password=123456789
  if (key == "ssid") {
    String ssid = value;
    command= command.substring(andIdx + 1);
    eqIdx = command.indexOf("=");
    andIdx = command.indexOf("&");
    key = command.substring(0, eqIdx);
    value = command.substring(eqIdx + 1, andIdx);
    String password = value;
    connectWifi(ssid, password);
  }
}

void scanNetworks() {
  int n = WiFi.scanNetworks();
  if (n == 0) {
    Serial.println("no networks found");
  } else {
    String result = String(n) + " networks found\n";
    for (int i = 0; i < n; ++i)
      result += String(i + 1) + " | " + WiFi.SSID(i) + " | " + String(WiFi.RSSI(i)) + "\n";
    Serial.println(result);
  }
  WiFi.scanDelete();
}


void connectWifi(String ssid, String password) {
  staSsid = ssid;
  staPassword = password;

  WiFi.disconnect();
  WiFi.begin(staSsid, staPassword);
  String resultText = "ssid: " + ssid + " | pass=" + password;
  Serial.println(resultText);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  resultText = "\nconnected to " + ssid + "\n IP address is: " + WiFi.localIP().toString();
  Serial.println(resultText);
}

