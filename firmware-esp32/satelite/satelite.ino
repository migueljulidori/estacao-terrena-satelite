/*
 * Satélite simulado — ESP32
 * Projeto Integrado: Estação Terrena de Comunicação via Satélite
 *
 * Ordem oficial de implementação (uma etapa por vez):
 *   1) Wi-Fi          ← etapa atual
 *   2) MQTT           (conectar ao broker da estação)
 *   3) Gerar dados    (telemetria; payload bruto ≤ 16 B no total final)
 *   4) Criptografia   (AES-CBC e/ou HMAC-SHA256 via mbedTLS — PDFs)
 *   5) Montar pacote  (dados + IV/MAC conforme protocolo da equipe)
 *   6) Publicar       (MQTT publish no tópico do satélite)
 *
 * Setup desta etapa (Wi-Fi):
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
  Serial.println("Pipeline: Wi-Fi > MQTT > dados > cripto > pacote > publicar");
  Serial.println("Etapa atual: 1/6 Wi-Fi");

  // 1) Wi-Fi
  wifiConectar();

  // 2) MQTT — próximo (só depois do Wi-Fi estável)
  // mqttConectar();

  ultimoStatusMs = millis();
}

void loop() {
  // 1) Manter Wi-Fi
  wifiManter();

  if (millis() - ultimoStatusMs >= WIFI_STATUS_INTERVAL_MS) {
    wifiLogStatus();
    ultimoStatusMs = millis();
  }

  // Etapas futuras (não ativas ainda):
  // 2) mqttManter();
  // 3) gerarDados();
  // 4) aplicarCriptografia();
  // 5) montarPacote();
  // 6) publicarMqtt();
}
