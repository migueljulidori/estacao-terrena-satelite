/*
 * Satélite ESP32 — orquestra os módulos
 * Ordem: Wi-Fi → MQTT → dados → criptografia → pacote → publicar
 *
 * Módulos:
 *   wifi_mod.*  — Wi-Fi
 *   mqtt_mod.*  — MQTT
 *
 * Biblioteca: PubSubClient | Serial: 115200
 */

#include <WiFi.h>
#include "wifi_mod.h"
#include "mqtt_mod.h"

unsigned long ultimoStatus = 0;

void setup() {
  Serial.begin(115200);
  delay(500);

  Serial.println("Satelite ESP32 — Wi-Fi + MQTT");
  wifiConectar();
  mqttConectar();
  ultimoStatus = millis();
}

void loop() {
  if (!wifiOk()) {
    Serial.println("Wi-Fi caiu. Reconectando...");
    wifiConectar();
    mqttConectar();
    return;
  }

  mqttManter();

  if (millis() - ultimoStatus >= 5000) {
    ultimoStatus = millis();
    Serial.print("OK | IP=");
    Serial.print(WiFi.localIP());
    Serial.print(" | MQTT=");
    Serial.println(mqttOk() ? "sim" : "nao");
  }
}
