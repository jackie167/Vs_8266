// ESP32 (XIAO ESP32C3): Control LED via MQTT

#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

// Fill these in later
const char* WIFI_SSID = "Khang Khang";
const char* WIFI_PASS = "nguyenkhang2505";

// Public MQTT broker
const char* MQTT_HOST = "test.mosquitto.org";
const uint16_t MQTT_PORT = 1883;

// Topics (change prefix if you want)
const char* TOPIC_SET = "khang/esp32/led/set";     // payload: on/off/toggle
const char* TOPIC_STATE = "khang/esp32/led/state"; // publishes on/off

// XIAO ESP32C3 onboard LED is typically active LOW on GPIO2 (change if needed)
const int LED_PIN = 2;
bool ledOn = false;

WiFiClient wifiClient;
PubSubClient mqtt(wifiClient);

void setLed(bool on) {
  ledOn = on;
  digitalWrite(LED_PIN, on ? LOW : HIGH); // active LOW
}

void publishState() {
  mqtt.publish(TOPIC_STATE, ledOn ? "on" : "off", true);
}

void onMqttMessage(char* topic, byte* payload, unsigned int length) {
  String msg;
  for (unsigned int i = 0; i < length; i++) msg += (char)payload[i];
  msg.toLowerCase();

  if (msg == "on") setLed(true);
  else if (msg == "off") setLed(false);
  else if (msg == "toggle") setLed(!ledOn);

  publishState();
}

void connectMqtt() {
  while (!mqtt.connected()) {
    uint64_t chipId = ESP.getEfuseMac();
    String clientId = "esp32-" + String((uint32_t)(chipId >> 32), HEX) + String((uint32_t)chipId, HEX);
    if (mqtt.connect(clientId.c_str())) {
      mqtt.subscribe(TOPIC_SET);
      publishState();
    } else {
      delay(1000);
    }
  }
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  setLed(false);

  Serial.begin(115200);
  delay(200);

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  mqtt.setServer(MQTT_HOST, MQTT_PORT);
  mqtt.setCallback(onMqttMessage);
  connectMqtt();
  Serial.println("MQTT connected");
}

void loop() {
  if (!mqtt.connected()) connectMqtt();
  mqtt.loop();
}
