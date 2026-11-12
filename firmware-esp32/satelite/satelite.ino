/*
 * Satélite ESP32 — etapa Wi-Fi
 * Ordem depois: MQTT → dados → criptografia → pacote → publicar
 * Serial: 115200
 */

#include <WiFi.h>

const char *ssid = "teleco2k25";
const char *senha = "teleco2k25";

IPAddress ip(192, 168, 30, 52);
IPAddress gateway(192, 168, 30, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress dns(192, 168, 30, 1);

void setup() {
  Serial.begin(115200);
  delay(500);

  Serial.println("Satelite ESP32 — Wi-Fi");
  Serial.print("Conectando: ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.config(ip, gateway, subnet, dns);
  WiFi.begin(ssid, senha);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("Conectado.");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Wi-Fi caiu. Reconectando...");
    WiFi.begin(ssid, senha);
    delay(5000);
    return;
  }

  Serial.print("OK | IP=");
  Serial.print(WiFi.localIP());
  Serial.print(" | RSSI=");
  Serial.println(WiFi.RSSI());
  delay(5000);
}
