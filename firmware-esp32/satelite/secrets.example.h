#pragma once

// Copie este arquivo para secrets.h e preencha os valores reais.
// secrets.h esta no .gitignore e nao deve ser enviado ao GitHub.

#define WIFI_SSID "COLOQUE_O_SSID_DA_REDE"
#define WIFI_PASSWORD "COLOQUE_A_SENHA_DA_REDE"

// IP ou nome de host da maquina que executa o Mosquitto.
// O IP do broker ainda precisa ser definido pela equipe.
#define MQTT_BROKER_HOST "192.168.30.X"
#define MQTT_BROKER_PORT 1883

// Credenciais criadas no broker Mosquitto.
#define MQTT_USERNAME "satellite"
#define MQTT_PASSWORD "COLOQUE_A_SENHA_MQTT"
#define MQTT_CLIENT_ID "satellite-esp32-01"
