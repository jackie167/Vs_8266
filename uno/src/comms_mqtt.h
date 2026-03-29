#pragma once

#include <PubSubClient.h>

typedef void (*MqttCallback)(char*, uint8_t*, unsigned int);

void mqttConfigure(PubSubClient& mqtt, const char* host, uint16_t port, MqttCallback cb);
void mqttEnsureConnected(
  PubSubClient& mqtt,
  const char* clientIdBase,
  const char* username,
  const char* password,
  const char* subscribeTopic
);
