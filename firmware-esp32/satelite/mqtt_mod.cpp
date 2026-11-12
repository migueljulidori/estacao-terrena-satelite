#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "mqtt_mod.h"

// IP da máquina com Mosquitto (não é o IP do ESP) — ajuste
const char *mqtt_broker = "192.168.30.1";
const int mqtt_porta = 1883;
const char *mqtt_id = "SAT-01";

// ESP só publica (TX). PC só assina este tópico (RX). Sem telecomando.
const char *topico_telemetria = "satellite/esp32-01/telemetry";

WiFiClient wifi;
PubSubClient mqtt(wifi);

void mqttConectar() {
  mqtt.setServer(mqtt_broker, mqtt_porta);

  while (!mqtt.connected()) {
    Serial.print("MQTT ");
    Serial.print(mqtt_broker);
    Serial.print("...");

    if (mqtt.connect(mqtt_id)) {
      Serial.println(" OK (publish only)");
    } else {
      Serial.print(" falhou rc=");
      Serial.println(mqtt.state());
      delay(2000);
    }
  }
}

void mqttManter() {
  if (!mqtt.connected()) {
    Serial.println("MQTT caiu. Reconectando...");
    mqttConectar();
  }
  mqtt.loop();
}

bool mqttOk() {
  return mqtt.connected();
}
