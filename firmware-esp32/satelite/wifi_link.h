#ifndef WIFI_LINK_H
#define WIFI_LINK_H

#include <WiFi.h>
#include "config.h"
#include "secrets.h"

// Estado simples do enlace Wi-Fi (estação ↔ satélite na LAN da turma)
struct WifiLinkState {
  bool conectado;
  int rssi_dbm;
  IPAddress ip_local;
};

static WifiLinkState wifiEstado = {false, 0, IPAddress()};

// Conecta à rede da estação/turma e bloqueia até obter IP (ou usuário reiniciar).
inline void wifiConectar() {
  Serial.println();
  Serial.println("[WiFi] Iniciando conexao...");
  Serial.print("[WiFi] SSID: ");
  Serial.println(WIFI_SSID);

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(WIFI_RETRY_MS);
    Serial.print(".");
  }

  wifiEstado.conectado = true;
  wifiEstado.ip_local = WiFi.localIP();
  wifiEstado.rssi_dbm = WiFi.RSSI();

  Serial.println();
  Serial.println("[WiFi] Conectado.");
  Serial.print("[WiFi] IP local: ");
  Serial.println(wifiEstado.ip_local);
  Serial.print("[WiFi] RSSI: ");
  Serial.print(wifiEstado.rssi_dbm);
  Serial.println(" dBm");
}

// Mantém o enlace: se cair, tenta reconectar.
inline void wifiManter() {
  if (WiFi.status() == WL_CONNECTED) {
    wifiEstado.conectado = true;
    wifiEstado.ip_local = WiFi.localIP();
    wifiEstado.rssi_dbm = WiFi.RSSI();
    return;
  }

  wifiEstado.conectado = false;
  Serial.println("[WiFi] Enlace perdido. Reconectando...");
  WiFi.disconnect();
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  unsigned long inicio = millis();
  while (WiFi.status() != WL_CONNECTED && (millis() - inicio) < 15000) {
    delay(WIFI_RETRY_MS);
    Serial.print(".");
  }
  Serial.println();

  if (WiFi.status() == WL_CONNECTED) {
    wifiEstado.conectado = true;
    wifiEstado.ip_local = WiFi.localIP();
    wifiEstado.rssi_dbm = WiFi.RSSI();
    Serial.println("[WiFi] Reconectado.");
  } else {
    Serial.println("[WiFi] Falha ao reconectar. Nova tentativa no proximo ciclo.");
  }
}

inline void wifiLogStatus() {
  Serial.print("[");
  Serial.print(SATELITE_ID);
  Serial.print("] WiFi=");
  Serial.print(wifiEstado.conectado ? "OK" : "DOWN");
  if (wifiEstado.conectado) {
    Serial.print(" IP=");
    Serial.print(wifiEstado.ip_local);
    Serial.print(" RSSI=");
    Serial.print(wifiEstado.rssi_dbm);
    Serial.print(" dBm");
  }
  Serial.println();
}

#endif
