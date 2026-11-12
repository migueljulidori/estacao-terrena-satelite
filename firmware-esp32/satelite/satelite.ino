/*
 * Satélite ESP32 — etapas Wi-Fi + MQTT
 *
 * Pipeline do projeto:
 * Wi-Fi -> MQTT -> gerar dados -> criptografia -> montar pacote -> publicar
 *
 * Nesta etapa:
 * - conecta ao Wi-Fi com IP fixo;
 * - conecta ao broker MQTT;
 * - assina o tópico de telecomandos;
 * - valida o limite de 16 bytes e exibe os bytes recebidos.
 *
 * A geração de telemetria, criptografia e publicação de dados ficam
 * para as próximas etapas, conforme CONTEXTO.md.
 *
 * Serial: 115200
 * Biblioteca: PubSubClient (Nick O'Leary)
 */

#include <WiFi.h>
#include <PubSubClient.h>
#include "secrets.h"

// Rede local do projeto
IPAddress ip(192, 168, 30, 52);
IPAddress gateway(192, 168, 30, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress dns(192, 168, 30, 1);

// Convenção inicial de tópicos MQTT (documentada em CONTEXTO.md)
const char *TOPICO_TELEMETRIA = "satellite/esp32-01/telemetry";
const char *TOPICO_TELECOMANDO = "satellite/esp32-01/command";

const unsigned long INTERVALO_RECONEXAO_WIFI = 5000;
const unsigned long INTERVALO_RECONEXAO_MQTT = 5000;
const unsigned long INTERVALO_STATUS = 5000;
const unsigned int PAYLOAD_MAXIMO = 16;

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

unsigned long ultimaTentativaWiFi = 0;
unsigned long ultimaTentativaMQTT = 0;
unsigned long ultimoStatus = 0;

void iniciarWiFi();
void manterWiFiConectado();
void manterMQTTConectado();
void receberTelecomando(char *topico, byte *payload, unsigned int tamanho);

void setup() {
  Serial.begin(115200);
  delay(500);

  Serial.println();
  Serial.println("=== SATELITE ESP32 ===");
  Serial.println("Etapas ativas: Wi-Fi + MQTT");

  WiFi.mode(WIFI_STA);
  WiFi.config(ip, gateway, subnet, dns);

  mqttClient.setServer(MQTT_BROKER_HOST, MQTT_BROKER_PORT);
  mqttClient.setCallback(receberTelecomando);
  mqttClient.setKeepAlive(30);
  mqttClient.setSocketTimeout(5);

  iniciarWiFi();
}

void loop() {
  manterWiFiConectado();

  if (WiFi.status() == WL_CONNECTED) {
    manterMQTTConectado();

    if (mqttClient.connected()) {
      mqttClient.loop();
    }

    if (millis() - ultimoStatus >= INTERVALO_STATUS) {
      ultimoStatus = millis();

      Serial.print("Wi-Fi OK | IP=");
      Serial.print(WiFi.localIP());
      Serial.print(" | RSSI=");
      Serial.print(WiFi.RSSI());
      Serial.print(" dBm | MQTT=");
      Serial.println(mqttClient.connected() ? "conectado" : "desconectado");
    }
  }
}

void iniciarWiFi() {
  Serial.print("Conectando ao Wi-Fi: ");
  Serial.println(WIFI_SSID);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  ultimaTentativaWiFi = millis();
}

void manterWiFiConectado() {
  if (WiFi.status() == WL_CONNECTED) {
    return;
  }

  // Se o Wi-Fi cair, o estado MQTT deixa de ser válido.
  if (mqttClient.connected()) {
    mqttClient.disconnect();
  }

  if (millis() - ultimaTentativaWiFi >= INTERVALO_RECONEXAO_WIFI) {
    ultimaTentativaWiFi = millis();

    Serial.println("Wi-Fi desconectado. Tentando reconectar...");
    WiFi.disconnect();
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  }

  static bool informouConexao = false;
  if (!informouConexao && WiFi.status() == WL_CONNECTED) {
    informouConexao = true;
    Serial.println("Wi-Fi conectado.");
    Serial.print("IP do satelite: ");
    Serial.println(WiFi.localIP());
  }
}

void manterMQTTConectado() {
  if (mqttClient.connected()) {
    return;
  }

  if (millis() - ultimaTentativaMQTT < INTERVALO_RECONEXAO_MQTT) {
    return;
  }

  ultimaTentativaMQTT = millis();

  Serial.print("Conectando ao broker MQTT em ");
  Serial.print(MQTT_BROKER_HOST);
  Serial.print(":");
  Serial.println(MQTT_BROKER_PORT);

  if (mqttClient.connect(MQTT_CLIENT_ID, MQTT_USERNAME, MQTT_PASSWORD)) {
    Serial.println("MQTT conectado.");

    if (mqttClient.subscribe(TOPICO_TELECOMANDO, 0)) {
      Serial.print("Inscrito no topico: ");
      Serial.println(TOPICO_TELECOMANDO);
    } else {
      Serial.println("Falha ao assinar o topico de telecomando.");
    }
  } else {
    Serial.print("Falha MQTT. Estado PubSubClient: ");
    Serial.println(mqttClient.state());
  }
}

void receberTelecomando(char *topico, byte *payload, unsigned int tamanho) {
  Serial.println();
  Serial.print("Mensagem recebida em: ");
  Serial.println(topico);

  // O requisito do projeto limita cada payload MQTT a 16 bytes.
  if (tamanho > PAYLOAD_MAXIMO) {
    Serial.print("Mensagem rejeitada: ");
    Serial.print(tamanho);
    Serial.println(" bytes (limite: 16).");
    return;
  }

  Serial.print("Payload recebido (");
  Serial.print(tamanho);
  Serial.println(" bytes):");

  Serial.print("HEX: ");
  for (unsigned int i = 0; i < tamanho; i++) {
    if (payload[i] < 0x10) {
      Serial.print("0");
    }
    Serial.print(payload[i], HEX);
    Serial.print(" ");
  }
  Serial.println();

  Serial.println("Telecomando recebido. Interpretacao sera implementada junto ao protocolo.");
}
