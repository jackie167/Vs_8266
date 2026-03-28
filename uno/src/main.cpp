// ESP32 (XIAO ESP32C3): Publish soil moisture sensor readings via MQTT

#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

#include "config.h"
#include "pins.h"
#include "types.h"
#include "soil_sensor.h"
#include "comms_wifi.h"

WiFiClient wifiClient;
PubSubClient mqtt(wifiClient);

void setup() {
  Serial.begin(115200);
  delay(200);

  soilSensorBegin();
  connectWiFi();

  mqtt.setServer(MQTT_HOST, MQTT_PORT);
  connectMqtt(mqtt);
}

void loop() {
  if (mqtt.connected() == false) {
    connectMqtt(mqtt);
  }
  mqtt.loop();

  SoilReading r = soilRead();

  char payload[64];
  snprintf(payload, sizeof(payload), "{\"raw\":%d,\"pct\":%.1f}", r.raw, r.pct);
  mqtt.publish(TOPIC_SOIL, payload, true);

  Serial.print("RAW=");
  Serial.print(r.raw);
  Serial.print("  PCT=");
  Serial.print(r.pct, 1);
  Serial.println("%  published");

  delay(1000);
}
