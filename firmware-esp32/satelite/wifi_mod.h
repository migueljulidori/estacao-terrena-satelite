#ifndef WIFI_MOD_H
#define WIFI_MOD_H

#include <WiFi.h>

const char *ssid = "teleco2k25";
const char *senha = "teleco2k25";

IPAddress ip(192, 168, 30, 52);
IPAddress gateway(192, 168, 30, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress dns(192, 168, 30, 1);

void wifiConectar() {
  Serial.print("Wi-Fi: ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.config(ip, gateway, subnet, dns);
  WiFi.begin(ssid, senha);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("Wi-Fi OK | IP=");
  Serial.println(WiFi.localIP());
}

bool wifiOk() {
  return WiFi.status() == WL_CONNECTED;
}

#endif
