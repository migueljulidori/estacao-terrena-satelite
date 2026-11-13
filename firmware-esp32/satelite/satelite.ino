/*
 * Satélite ESP32 — orquestra os módulos
 * PI 3T: enlace bidirecional (telemetria TX + comando RX)
 *
 *   wifi_mod.h  — Wi-Fi
 *   mqtt_mod.h  — MQTT (publish + subscribe)
 *   dados_mod.h — envio de telemetria (teste)
 *
 * Biblioteca: PubSubClient | Serial: 115200
 */

#include "wifi_mod.h"
#include "mqtt_mod.h"
#include "dados_mod.h"

unsigned long ultimoEnvio = 0;

void setup() {
  Serial.begin(115200);
  delay(500);

  Serial.println("Satelite ESP32 — bidirecional (TX telemetria / RX comando)");
  wifiConectar();
  mqttConectar();
  ultimoEnvio = millis();
}

void loop() {
  if (!wifiOk()) {
    Serial.println("Wi-Fi caiu. Reconectando...");
    wifiConectar();
    mqttConectar();
    return;
  }

  mqttManter();

  if (millis() - ultimoEnvio >= 5000) {
    ultimoEnvio = millis();
    dadosEnviarTeste();
  }
}
