/*
 * Satélite ESP32 — Wi-Fi + MQTT
 * Ordem: Wi-Fi → MQTT → dados → criptografia → pacote → publicar
 *
 * Agora: conectar Wi-Fi, conectar MQTT, assinar telecomando.
 * Ainda não: gerar dados, cripto, montar pacote, publicar telemetria.
 *
 * Biblioteca: PubSubClient | Serial: 115200
 */

#include <WiFi.h>
#include <PubSubClient.h>

const char *ssid = "teleco2k25";
const char *senha = "teleco2k25";

// IP da máquina com Mosquitto (não é o IP do ESP) — ajuste
const char *mqtt_broker = "192.168.30.1";
const int mqtt_porta = 1883;
const char *mqtt_id = "SAT-01";

const char *topico_telemetria = "satellite/esp32-01/telemetry";
const char *topico_comando = "satellite/esp32-01/command";

IPAddress ip(192, 168, 30, 52);
IPAddress gateway(192, 168, 30, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress dns(192, 168, 30, 1);

WiFiClient wifi;
PubSubClient mqtt(wifi);

unsigned long ultimoStatus = 0;

void aoReceber(char *topico, byte *payload, unsigned int tamanho) {
  Serial.print("Comando em ");
  Serial.println(topico);

  if (tamanho > 16) {
    Serial.println("Rejeitado: mais de 16 bytes");
    return;
  }

  Serial.print("HEX: ");
  for (unsigned int i = 0; i < tamanho; i++) {
    if (payload[i] < 0x10) Serial.print("0");
    Serial.print(payload[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
}

void conectarWifi() {
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

void conectarMqtt() {
  mqtt.setServer(mqtt_broker, mqtt_porta);
  mqtt.setCallback(aoReceber);

  while (!mqtt.connected()) {
    Serial.print("MQTT ");
    Serial.print(mqtt_broker);
    Serial.print("...");

    if (mqtt.connect(mqtt_id)) {
      Serial.println(" OK");
      mqtt.subscribe(topico_comando);
      Serial.print("Inscrito: ");
      Serial.println(topico_comando);
    } else {
      Serial.print(" falhou rc=");
      Serial.println(mqtt.state());
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  delay(500);

  Serial.println("Satelite ESP32 — Wi-Fi + MQTT");
  conectarWifi();
  conectarMqtt();
  ultimoStatus = millis();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Wi-Fi caiu. Reconectando...");
    conectarWifi();
    conectarMqtt();
    return;
  }

  if (!mqtt.connected()) {
    Serial.println("MQTT caiu. Reconectando...");
    conectarMqtt();
  }

  mqtt.loop();

  if (millis() - ultimoStatus >= 5000) {
    ultimoStatus = millis();
    Serial.print("OK | IP=");
    Serial.print(WiFi.localIP());
    Serial.print(" | MQTT=");
    Serial.println(mqtt.connected() ? "sim" : "nao");
  }
}
