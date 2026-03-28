#include "comms_wifi.h"
#include <Arduino.h>
#include <WiFi.h>
#include "config.h"

void connectWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("Connecting to WiFi");
  while (true) {
    if (WiFi.status() == WL_CONNECTED) {
      break;
    }
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

void connectMqtt(PubSubClient& mqtt) {
  while (mqtt.connected() == false) {
    uint64_t chipId = ESP.getEfuseMac();
    String clientId = "esp32-" + String((uint32_t)(chipId >> 32), HEX) + String((uint32_t)chipId, HEX);
    if (mqtt.connect(clientId.c_str())) {
      Serial.println("MQTT connected");
    } else {
      delay(1000);
    }
  }
}
