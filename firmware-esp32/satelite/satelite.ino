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
 */

#include <WiFi.h>
#include "secrets.h"

// --- Variáveis ---
const char *SATELITE_ID = "SAT-01";
const unsigned long SERIAL_BAUD = 115200;
const unsigned long WIFI_RETRY_MS = 500;
const unsigned long STATUS_MS = 5000;

unsigned long ultimoStatusMs = 0;

// --- Funções da etapa Wi-Fi ---

void wifiConectar() {
  Serial.println();
  Serial.print("Conectando ao Wi-Fi: ");
  Serial.println(WIFI_SSID);

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(WIFI_RETRY_MS);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("Wi-Fi conectado.");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
  Serial.print("RSSI: ");
  Serial.print(WiFi.RSSI());
  Serial.println(" dBm");
}

void wifiManter() {
  if (WiFi.status() == WL_CONNECTED) {
    return;
  }

  Serial.println("Wi-Fi caiu. Reconectando...");
  WiFi.disconnect();
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  unsigned long inicio = millis();
  while (WiFi.status() != WL_CONNECTED && (millis() - inicio) < 15000) {
    delay(WIFI_RETRY_MS);
    Serial.print(".");
  }
  Serial.println();

  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("Reconectado. IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("Ainda sem Wi-Fi. Tentamos de novo no loop.");
  }
}

void wifiMostrarStatus() {
  Serial.print(SATELITE_ID);
  Serial.print(" | WiFi=");
  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("OK");
    Serial.print(" | IP=");
    Serial.print(WiFi.localIP());
    Serial.print(" | RSSI=");
    Serial.print(WiFi.RSSI());
    Serial.println(" dBm");
  } else {
    Serial.println("DOWN");
  }
}

// --- Programa ---

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
