#ifndef MQTT_MOD_H
#define MQTT_MOD_H

#include <WiFi.h>
#include <PubSubClient.h>
#include <string.h>

// IP da máquina que executa o Mosquitto (não é o IP do ESP32).
const char *mqtt_broker = "192.168.30.250";
const int mqtt_porta = 1883;
const char *mqtt_id = "SAT-01";

// Credenciais locais do usuário criado no Mosquitto.
const char *mqtt_user = "satellite";
const char *mqtt_senha = "emmj";

// Tópicos do enlace bidirecional.
const char *topico_telemetria = "satellite/esp32-01/telemetry";
const char *topico_comando = "satellite/esp32-01/command";

WiFiClient wifi;
PubSubClient mqtt(wifi);

// Recebe comandos enviados pela aplicação da estação terrena.
void mqttReceber(char *topico, byte *payload, unsigned int tamanho) {
  if (strcmp(topico, topico_comando) != 0) {
    return;
  }

  Serial.print("\nRX ");
  Serial.print(topico);
  Serial.print(" | ");

  if (tamanho == 0 || tamanho > 16) {
    Serial.print("comando rejeitado: tamanho invalido (1..16 bytes). Recebido: ");
    Serial.println(tamanho);
    return;
  }

  Serial.print("HEX: ");
  for (unsigned int i = 0; i < tamanho; i++) {
    if (payload[i] < 0x10) Serial.print("0");
    Serial.print(payload[i], HEX);
    Serial.print(" ");
  }

  Serial.print("| Texto: ");
  for (unsigned int i = 0; i < tamanho; i++) {
    if (payload[i] >= 32 && payload[i] <= 126) {
      Serial.print((char)payload[i]);
    } else {
      Serial.print(".");
    }
  }
  Serial.println();
}

void mqttConectar() {
  mqtt.setServer(mqtt_broker, mqtt_porta);
  mqtt.setCallback(mqttReceber);

  while (!mqtt.connected()) {
    Serial.print("MQTT ");
    Serial.print(mqtt_broker);
    Serial.print("...");

    if (mqtt.connect(mqtt_id, mqtt_user, mqtt_senha)) {
      Serial.println(" OK");

      if (mqtt.subscribe(topico_comando, 0)) {
        Serial.print("Inscrito (RX) em: ");
        Serial.println(topico_comando);
      } else {
        Serial.println("Falha ao assinar topico de comando.");
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

// Publica telemetria no tópico próprio (máx. 16 bytes).
bool mqttPublicar(const uint8_t *dados, unsigned int tamanho) {
  if (!mqtt.connected()) return false;

  if (tamanho == 0 || tamanho > 16) {
    Serial.println("Publish rejeitado: tamanho invalido (1..16)");
    return false;
  }

  return mqtt.publish(topico_telemetria, dados, tamanho);
}

#endif
