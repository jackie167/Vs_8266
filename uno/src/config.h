#pragma once

// WiFi
static const char* WIFI_SSID = "Khang Khang";
static const char* WIFI_PASS = "nguyenkhang2505";

// MQTT
static const char* MQTT_HOST = "test.mosquitto.org";
static const uint16_t MQTT_PORT = 1883;

// Topics (new prefix)
static const char* TOPIC_SENSOR = "smart_irrigation/dinhthi/data/sensor";
static const char* TOPIC_CONFIG_TEST = "smart_irrigation/dinhthi/data/config_test";
static const char* TOPIC_PUMP_CMD = "smart_irrigation/dinhthi/cmd/pump";
static const char* TOPIC_PUMP_STATE = "smart_irrigation/dinhthi/data/pump_state";
