#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "mqtt_mod.h"

// IP da máquina com Mosquitto (não é o IP do ESP) — ajuste
const char *mqtt_broker = "192.168.30.1";
const int mqtt_porta = 1883;
const char *mqtt_id = "SAT-01";

const char *topico_telemetria = "satellite/esp32-01/telemetry";
const char *topico_comando = "satellite/esp32-01/command";

WiFiClient wifi;
PubSubClient mqtt(wifi);

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

void mqttConectar() {
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
