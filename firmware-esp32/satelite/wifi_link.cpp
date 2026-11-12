#include <Arduino.h>
#include <WiFi.h>

#include "config.h"
#include "secrets.h"
#include "wifi_link.h"

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
