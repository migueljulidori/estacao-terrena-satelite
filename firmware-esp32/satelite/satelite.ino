/*
 * Satélite simulado — ESP32
 * Projeto Integrado: Estação Terrena de Comunicação via Satélite
 *
 * Etapa atual: conexão Wi-Fi com a rede da turma/estação.
 * Próximas etapas (não implementadas ainda):
 *   1) Cliente MQTT → broker da estação terrena
 *   2) Payload de telemetria ≤ 16 bytes
 *   3) Autenticação HMAC-SHA256 (mbedTLS)
 *   4) Confidencialidade AES (mbedTLS)
 *
 * Setup:
 *   1. Copie secrets.example.h → secrets.h e preencha SSID/senha
 *   2. Abra esta pasta no Arduino IDE (placa: ESP32)
 *   3. Compile e grave; abra o Serial Monitor em 115200
 */

#include "config.h"
#include "wifi_link.h"

unsigned long ultimoStatusMs = 0;

void setup() {
  Serial.begin(SERIAL_BAUD);
  delay(500);

  Serial.println();
  Serial.println("=== Satelite ESP32 ===");
  Serial.print("ID: ");
  Serial.println(SATELITE_ID);
  Serial.println("Etapa: Wi-Fi");

  wifiConectar();
  ultimoStatusMs = millis();
}

void loop() {
  wifiManter();

  if (millis() - ultimoStatusMs >= WIFI_STATUS_INTERVAL_MS) {
    wifiLogStatus();
    ultimoStatusMs = millis();
  }

  // Próximo passo: quando o Wi-Fi estiver estável, conectar ao MQTT.
  // mqttManter();
  // publicarTelemetria();
}
