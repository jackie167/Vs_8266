#include "comms_mqtt.h"
#include <Arduino.h>

void mqttConfigure(PubSubClient& mqtt, const char* host, uint16_t port, MqttCallback cb) {
  mqtt.setServer(host, port);
  mqtt.setCallback(cb);
  mqtt.setKeepAlive(60);
  mqtt.setSocketTimeout(10);
}

void mqttEnsureConnected(
  PubSubClient& mqtt,
  const char* clientIdBase,
  const char* username,
  const char* password,
  const char* subscribeTopic
) {
  while (mqtt.connected() == false) {
    uint64_t chipId = ESP.getEfuseMac();
    String clientId = String(clientIdBase) + "-" + String((uint32_t)(chipId >> 32), HEX) + String((uint32_t)chipId, HEX);
    if (mqtt.connect(clientId.c_str(), username, password)) {
      Serial.print("MQTT connected=");
      Serial.println(mqtt.connected() ? "YES" : "NO");
      Serial.println("MQTT keepalive=60");
      if (subscribeTopic && subscribeTopic[0]) {
        mqtt.subscribe(subscribeTopic);
      }
    } else {
      delay(1000);
    }
  }
}
