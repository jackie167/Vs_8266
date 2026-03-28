// ESP32 (XIAO ESP32C3): Publish soil moisture sensor readings via MQTT

#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

// WiFi
const char* WIFI_SSID = "Khang Khang";
const char* WIFI_PASS = "nguyenkhang2505";

// MQTT
const char* MQTT_HOST = "test.mosquitto.org";
const uint16_t MQTT_PORT = 1883;
const char* TOPIC_SOIL = "khang/esp32/soil"; // publishes raw + percent

// XIAO ESP32C3 A1 is GPIO1
const int SENSOR_PIN = 1;

WiFiClient wifiClient;
PubSubClient mqtt(wifiClient);

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

void connectMqtt() {
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

void setup() {
  Serial.begin(115200);
  delay(200);

  connectWiFi();

  mqtt.setServer(MQTT_HOST, MQTT_PORT);
  connectMqtt();
}

void loop() {
  if (mqtt.connected() == false) connectMqtt();
  mqtt.loop();

  int raw = analogRead(SENSOR_PIN); // 0..4095
  float percent = (raw / 4095.0f) * 100.0f;

  char payload[64];
  snprintf(payload, sizeof(payload), "{\"raw\":%d,\"pct\":%.1f}", raw, percent);
  mqtt.publish(TOPIC_SOIL, payload, true);

  Serial.print("RAW=");
  Serial.print(raw);
  Serial.print("  PCT=");
  Serial.print(percent, 1);
  Serial.println("%  published");

  delay(1000);
}
