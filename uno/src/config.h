#pragma once

// WiFi
static const char* WIFI_SSID = "Khang Khang";
static const char* WIFI_PASS = "nguyenkhang2505";

// MQTT (HiveMQ Cloud)
static const char* MQTT_HOST = "b005c9ecb8674930857a11ff36fcd93c.s1.eu.hivemq.cloud";
static const uint16_t MQTT_PORT = 8883;
static const char* MQTT_USERNAME = "dinhthi";
static const char* MQTT_PASSWORD = "Jackie167";

// Topics (new prefix)
static const char* TOPIC_SENSOR = "smart_irrigation/dinhthi/data/sensor";
static const char* TOPIC_CONFIG_TEST = "smart_irrigation/dinhthi/data/config_test";
static const char* TOPIC_PUMP_CMD = "smart_irrigation/dinhthi/cmd/pump";
static const char* TOPIC_PUMP_STATE = "smart_irrigation/dinhthi/data/pump_state";
