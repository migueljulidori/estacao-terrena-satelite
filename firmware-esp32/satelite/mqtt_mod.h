#ifndef MQTT_MOD_H
#define MQTT_MOD_H

#include <WiFi.h>
#include <PubSubClient.h>

// IP da máquina com Mosquitto (não é o IP do ESP) — ajuste
const char *mqtt_broker = "192.168.30.250";
const int mqtt_porta = 1883;
const char *mqtt_id = "SAT-01";

const char *mqtt_user = "satellite";
const char *mqtt_senha = "emmj";

// Enlace bidirecional (PI 3T)
const char *topico_telemetria = "satellite/esp32-01/telemetry";  // ESP → estação
const char *topico_comando = "satellite/esp32-01/command";       // estação → ESP

WiFiClient wifi;
PubSubClient mqtt(wifi);

// Implementado em comando_mod.h
void comandoProcessar(const byte *payload, unsigned int tamanho);

void aoReceberComando(char *topico, byte *payload, unsigned int tamanho) {
  Serial.print("RX em ");
  Serial.println(topico);

  if (tamanho > 16) {
    Serial.println("Rejeitado: mais de 16 bytes");
    return;
  }

  comandoProcessar(payload, tamanho);
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
  if (!mqtt.connected()) {
    return false;
  }
  if (tam == 0 || tam > 16) {
    Serial.println("Publish rejeitado: tamanho invalido (1..16)");
    return false;
  }
  return mqtt.publish(topico_telemetria, dados, tam);
}

#endif
