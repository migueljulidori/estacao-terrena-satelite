/*
 * Satélite ESP32 — orquestra os módulos
 * Ordem: Wi-Fi → MQTT → dados → criptografia → pacote → publicar
 *
 *   wifi_mod.h  — Wi-Fi
 *   mqtt_mod.h  — MQTT
 *   dados_mod.h — envio de dados (teste agora)
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

  Serial.println("Satelite ESP32 — Wi-Fi + MQTT + dados teste");
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

  // A cada 5 s: envia mensagem de teste (ver comunicação)
  if (millis() - ultimoEnvio >= 5000) {
    ultimoEnvio = millis();
    dadosEnviarTeste();
  }
}
