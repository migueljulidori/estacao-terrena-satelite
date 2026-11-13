#ifndef MQTT_MOD_H
#define MQTT_MOD_H

#include <WiFi.h>
#include <PubSubClient.h>\n#include <string.h>

// IP da máquina com Mosquitto (não é o IP do ESP) — ajuste localmente.
const char *mqtt_broker = "192.168.30.250";
const int mqtt_porta = 1883;
const char *mqtt_id = "SAT-01";

// Credenciais locais do usuário criado no Mosquitto.
const char *mqtt_user = "satellite";
const char *mqtt_senha = "emmj";

// Enlace bidirecional (PI 3T).
const char *topico_telemetria = "satellite/esp32-01/telemetry";
const char *topico_comando = "satellite/esp32-01/command";

// Limite herdado do firmware de teste; confirmar com o enunciado/equipe.
const unsigned int MQTT_MAX_PAYLOAD_TESTE = 16;

WiFiClient wifi;
PubSubClient mqtt(wifi);

void aoReceberComando(char *topico, byte *payload, unsigned int tamanho) {
  if (strcmp(topico, topico_comando) != 0) return;

  Serial.print("RX comando | ");
  Serial.println(topico);

  if (tamanho == 0 || tamanho > MQTT_MAX_PAYLOAD_TESTE) {
    Serial.println("Rejeitado: tamanho fora do limite provisório (1..16 bytes)");
    return;
  }

  Serial.print("HEX: ");
  for (unsigned int i = 0; i < tamanho; i++) {
    if (payload[i] < 0x10) Serial.print("0");
    Serial.print(payload[i], HEX);
    Serial.print(" ");
  }
  Serial.println();

  Serial.print("TXT: ");
  for (unsigned int i = 0; i < tamanho; i++) {
    char c = (char)payload[i];
    Serial.print((c >= 32 && c <= 126) ? c : '.');
  }
  Serial.println();
}

void mqttConectar() {
  mqtt.setServer(mqtt_broker, mqtt_porta);
  mqtt.setCallback(aoReceberComando);

  while (!mqtt.connected()) {
    Serial.print("MQTT ");
    Serial.print(mqtt_broker);
    Serial.print("...");

    if (mqtt.connect(mqtt_id, mqtt_user, mqtt_senha)) {
      Serial.println(" OK (TX+RX)");
      if (mqtt.subscribe(topico_comando, 0)) {
        Serial.print("Inscrito: ");
        Serial.println(topico_comando);
      } else {
        Serial.println("Falha ao assinar tópico de comando.");
      }
    } else {
      Serial.print(" falhou rc=");
      Serial.println(mqtt.state());
      delay(2000);
    }
  }
}

void mqttManter() {
  if (!mqtt.connected()) {
    Serial.println("MQTT caiu. Reconectando...");
    mqttConectar();
  }
  mqtt.loop();
}

bool mqttOk() {
  return mqtt.connected();
}

bool mqttPublicar(const uint8_t *dados, unsigned int tam) {
  if (!mqtt.connected()) return false;
  if (tam == 0 || tam > MQTT_MAX_PAYLOAD_TESTE) {
    Serial.println("Publish rejeitado: tamanho fora do limite provisório (1..16)");
    return false;
  }
  return mqtt.publish(topico_telemetria, dados, tam);
}

#endif
