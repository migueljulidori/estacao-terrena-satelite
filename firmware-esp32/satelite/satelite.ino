/*
 * Satélite ESP32 — orquestra os módulos
 * PI 3T: bidirecional (telemetria TX + comando RX)
 *
 *   wifi_mod.h    — Wi-Fi
 *   mqtt_mod.h    — MQTT
 *   dados_mod.h   — telemetria
 *   comando_mod.h — interpreta comandos da estação
 *
 * Próxima: criptografia
 * Biblioteca: PubSubClient | Serial: 115200
 */

#include "wifi_mod.h"
#include "mqtt_mod.h"
#include "dados_mod.h"
#include "comando_mod.h"

unsigned long ultimoEnvio = 0;

void setup() {
  Serial.begin(115200);
  delay(500);

  Serial.println("Satelite ESP32 — telemetria + comandos");
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

  bool forcar = comandoConsumirForcarTx();
  if (forcar || (millis() - ultimoEnvio >= comandoIntervaloMs())) {
    ultimoEnvio = millis();
    dadosEnviar();
  }
}
