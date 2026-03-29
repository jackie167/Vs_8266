// ESP32: MQTT smart irrigation (sensor publish + pump command subscribe)

#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

#include "config.h"
#include "pins.h"
#include "types.h"
#include "soil_sensor.h"
#include "comms_wifi.h"
#include "comms_mqtt.h"

WiFiClient wifiClient;
PubSubClient mqtt(wifiClient);

static const char* DEVICE_ID = "esp32_01";

// Topic chuẩn hệ ROS2 hiện tại
// Topics are defined in config.h

void onMqttMessage(char* topic, byte* payload, unsigned int length) {
  String msg;
  for (unsigned int i = 0; i < length; i++) msg += (char)payload[i];
  msg.trim();
  msg.toUpperCase();

  if (String(topic) == TOPIC_PUMP_CMD) {
    if (msg == "ON") {
      digitalWrite(PUMP_RELAY_PIN, LOW);   // đổi LOW/HIGH theo relay của bạn
      mqtt.publish(TOPIC_PUMP_STATE, "ON", false);
    } else if (msg == "OFF") {
      digitalWrite(PUMP_RELAY_PIN, HIGH);
      mqtt.publish(TOPIC_PUMP_STATE, "OFF", false);
    }
  }
}

void setup() {
  Serial.begin(115200);
  delay(200);

  pinMode(PUMP_RELAY_PIN, OUTPUT);
  digitalWrite(PUMP_RELAY_PIN, HIGH); // mặc định OFF (active LOW relay)

  soilSensorBegin();
  connectWiFi();

  mqttConfigure(mqtt, MQTT_HOST, MQTT_PORT, onMqttMessage);
  mqttEnsureConnected(mqtt, DEVICE_ID, TOPIC_PUMP_CMD);

  // gửi config_test 1 lần lúc start
  JsonDocument configDoc;
  configDoc["device_id"] = DEVICE_ID;
  configDoc["type"] = "config_test";
  configDoc["value"] = 123;
  char configPayload[128];
  serializeJson(configDoc, configPayload, sizeof(configPayload));
  mqtt.publish(TOPIC_CONFIG_TEST, configPayload, false);
}

void loop() {
  if (mqtt.connected() == false) {
    mqttEnsureConnected(mqtt, DEVICE_ID, TOPIC_PUMP_CMD);
  }
  mqtt.loop();

  static unsigned long lastPub = 0;
  unsigned long now = millis();
  if (now - lastPub >= 3000) {
    lastPub = now;

    SoilReading r = soilRead();

    JsonDocument doc;
    doc["device_id"] = DEVICE_ID;
    doc["type"] = "sensor_data";
    doc["soil_moisture"] = r.pct;
    doc["temperature"] = 28.5;
    doc["humidity"] = 72.0;

    char payload[192];
    serializeJson(doc, payload, sizeof(payload));
    mqtt.publish(TOPIC_SENSOR, payload, false);

    Serial.print("RAW=");
    Serial.print(r.raw);
    Serial.print("  PCT=");
    Serial.print(r.pct, 1);
    Serial.println("% published");
  }
}
