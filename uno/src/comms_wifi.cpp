#include "comms_wifi.h"
#include <Arduino.h>
#include <WiFi.h>
#include "config.h"

void connectWiFi() {
  WiFi.setSleep(false);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect(true);
  delay(200);

  int n = WiFi.scanNetworks();
  Serial.print("WiFi scan found ");
  Serial.print(n);
  Serial.println(" networks");
  for (int i = 0; i < n; i++) {
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.print(WiFi.SSID(i));
    Serial.print(" (");
    Serial.print(WiFi.RSSI(i));
    Serial.println(" dBm)");
  }

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("Connecting to WiFi");
  unsigned long lastStatus = 0;
  while (true) {
    wl_status_t st = WiFi.status();
    if (st == WL_CONNECTED) {
      break;
    }
    delay(500);
    Serial.print(".");
    if (millis() - lastStatus >= 2000) {
      lastStatus = millis();
      Serial.print(" status=");
      Serial.print((int)st);
    }
  }
  Serial.println();
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

void connectMqtt(PubSubClient& mqtt) {
  mqtt.setKeepAlive(60);
  while (mqtt.connected() == false) {
    uint64_t chipId = ESP.getEfuseMac();
    String clientId = "esp32-" + String((uint32_t)(chipId >> 32), HEX) + String((uint32_t)chipId, HEX);
    if (mqtt.connect(clientId.c_str())) {
      Serial.print("MQTT connected=");
      Serial.println(mqtt.connected() ? "YES" : "NO");
      Serial.println("MQTT keepalive=60");
    } else {
      delay(1000);
    }
  }
}
