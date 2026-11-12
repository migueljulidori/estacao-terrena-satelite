#ifndef DADOS_MOD_H
#define DADOS_MOD_H

#include <stdio.h>
#include "mqtt_mod.h"

// Módulo de dados: por enquanto só um teste de comunicação (TX).
// Depois: telemetria real → criptografia → montar pacote → publicar.

void dadosEnviarTeste() {
  static uint8_t contador = 0;

  char msg[16];
  int n = snprintf(msg, sizeof(msg), "TESTE-%u", contador);

  if (n <= 0 || n > 16) {
    Serial.println("Dados: falha ao montar teste");
    return;
  }

  if (mqttPublicar((uint8_t *)msg, (unsigned int)n)) {
    Serial.print("TX ");
    Serial.print(topico_telemetria);
    Serial.print(" | ");
    Serial.println(msg);
    contador++;
  } else {
    Serial.println("TX falhou");
  }
}

#endif
