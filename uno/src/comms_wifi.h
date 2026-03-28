#pragma once

#include <PubSubClient.h>

void connectWiFi();
void connectMqtt(PubSubClient& mqtt);
