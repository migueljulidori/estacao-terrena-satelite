/*
 * Satélite simulado — ESP32
 * Etapa atual: 1/6 Wi-Fi
 *
 * Ordem: Wi-Fi → MQTT → gerar dados → criptografia → montar pacote → publicar
 *
 * Antes de gravar:
 *   cp secrets.example.h secrets.h
 *   (edite SSID e senha)
 * Serial Monitor: 115200
 *
 * Modularização:
 *   config.h      → constantes
 *   secrets.h     → SSID/senha (local, não vai pro Git)
 *   wifi_link.*   → funções do Wi-Fi
 *   satelite.ino  → setup/loop (orquestra as etapas)
 */

#include "config.h"
#include "wifi_link.h"

// Definição das constantes declaradas em config.h
const char *SATELITE_ID = "SAT-01";
const unsigned long SERIAL_BAUD = 115200;
const unsigned long WIFI_RETRY_MS = 500;
const unsigned long STATUS_MS = 5000;

unsigned long ultimoStatusMs = 0;

void setup() {
  Serial.begin(SERIAL_BAUD);
  delay(500);

  Serial.println("=== Satelite ESP32 ===");
  Serial.println("Etapa: Wi-Fi");

  wifiConectar();
  ultimoStatusMs = millis();
}

void loop() {
  wifiManter();

  if (millis() - ultimoStatusMs >= STATUS_MS) {
    wifiMostrarStatus();
    ultimoStatusMs = millis();
  }
}
