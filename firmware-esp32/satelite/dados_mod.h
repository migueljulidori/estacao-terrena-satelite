#ifndef DADOS_MOD_H
#define DADOS_MOD_H

#include <string.h>
#include "mqtt_mod.h"
#include "cripto_mod.h"

/*
 * TX: monta dados (6 B) → HMAC + AES → publish 16 B cifrados.
 * Tipos: 0x01 telemetria | 0x02 ACK
 */

const uint8_t DADOS_SAT_ID = 1;
const uint8_t DADOS_TIPO_TELEMETRIA = 0x01;
const uint8_t DADOS_TIPO_ACK = 0x02;
const unsigned int DADOS_TAM = CRIPTO_TAM;
const unsigned int NOME_MAX = 6;

char g_satNome[NOME_MAX + 1] = "SAT-01";

void dadosDefinirNome(const char *nome) {
  if (nome == NULL || nome[0] == 0) return;
  strncpy(g_satNome, nome, NOME_MAX);
  g_satNome[NOME_MAX] = 0;
  Serial.print("Nome do satelite: ");
  Serial.println(g_satNome);
}

const char *dadosNome() {
  return g_satNome;
}

static void dadosEscreverI16(uint8_t *buf, int16_t v) {
  buf[0] = (uint8_t)(v & 0xFF);
  buf[1] = (uint8_t)((v >> 8) & 0xFF);
}

bool dadosPublicarCifrado(uint8_t tipo, uint8_t seq, const uint8_t dados[CRIPTO_DADOS_TAM]) {
  uint8_t pacote[CRIPTO_TAM];
  if (!criptoSelar(tipo, seq, dados, pacote)) {
    Serial.println("CRIPTO: falha ao assinar/cifrar");
    return false;
  }
  return mqttPublicar(pacote, CRIPTO_TAM);
}

void dadosEnviar() {
  static uint8_t sequencia = 0;
  uint8_t dados[CRIPTO_DADOS_TAM];
  memset(dados, 0, CRIPTO_DADOS_TAM);

  uint8_t bateria = (uint8_t)(70 + (sequencia % 20));
  int16_t tempInt = (int16_t)(250 + (sequencia % 10));
  int16_t tempExt = (int16_t)(-50 + (int)(sequencia % 15));
  uint16_t solar = (uint16_t)(800 + (sequencia % 50));

  dados[0] = bateria;
  dadosEscreverI16(&dados[1], tempInt);
  dadosEscreverI16(&dados[3], tempExt);
  dados[5] = (uint8_t)(solar / 4);

  if (dadosPublicarCifrado(DADOS_TIPO_TELEMETRIA, sequencia, dados)) {
    Serial.print("TX telemetria (HMAC+AES) nome=");
    Serial.print(g_satNome);
    Serial.print(" seq=");
    Serial.println(sequencia);
    sequencia++;
  } else {
    Serial.println("TX telemetria falhou");
  }
}

void dadosEnviarAck(uint8_t seq, uint8_t cmdOrigem) {
  uint8_t dados[CRIPTO_DADOS_TAM];
  memset(dados, 0, CRIPTO_DADOS_TAM);
  dados[0] = cmdOrigem;
  for (unsigned int i = 0; i < 5 && g_satNome[i]; i++) {
    dados[1 + i] = (uint8_t)g_satNome[i];
  }

  if (dadosPublicarCifrado(DADOS_TIPO_ACK, seq, dados)) {
    Serial.print("TX ACK (HMAC+AES) cmd=0x");
    if (cmdOrigem < 0x10) Serial.print("0");
    Serial.println(cmdOrigem, HEX);
  } else {
    Serial.println("TX ACK falhou");
  }
}

#endif
