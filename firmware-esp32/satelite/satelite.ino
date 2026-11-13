/*
 * Satélite ESP32 — PI 3T
 * Bidirecional + HMAC-SHA256 (trunc 8) + AES-128-CBC (payload 16 B)
 *
 * ESP e estação (Manuela) fazem os dois lados:
 *   cifrar / descriptografar / assinar (HMAC) / verificar
 *
 *   wifi_mod.h | mqtt_mod.h | cripto_mod.h | dados_mod.h | comando_mod.h
 * mbedTLS (core ESP32) + PubSubClient | Serial 115200
 */

#include "wifi_mod.h"
#include "mqtt_mod.h"
#include "cripto_mod.h"
#include "dados_mod.h"
#include "comando_mod.h"

unsigned long ultimoEnvio = 0;

void setup() {
  Serial.begin(115200);
  delay(500);

  Serial.println("Satelite ESP32 — HMAC + AES-CBC (16 B)");
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
