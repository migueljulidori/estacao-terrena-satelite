#ifndef DADOS_MOD_H
#define DADOS_MOD_H

#include <string.h>
#include "mqtt_mod.h"

/*
 * Telemetria (ESP → estação), 16 bytes:
 *  [0]     id numérico
 *  [1]     tipo (0x01 telemetria)
 *  [2]     sequência
 *  [3]     bateria %
 *  [4..5]  temp. interna °C*10 (int16 LE)
 *  [6..7]  temp. externa °C*10 (int16 LE)
 *  [8..9]  solar (uint16 LE)
 *  [10..15] primeiros 6 chars do nome (configurado pela estação)
 */

const uint8_t DADOS_SAT_ID = 1;
const uint8_t DADOS_TIPO_TELEMETRIA = 0x01;
const uint8_t DADOS_TIPO_ACK = 0x02;
const unsigned int DADOS_TAM = 16;
const unsigned int NOME_MAX = 12;  // cabe em comando 16 B

char g_satNome[NOME_MAX + 1] = "SAT-01";

void dadosDefinirNome(const char *nome) {
  if (nome == NULL || nome[0] == 0) {
    return;
  }
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

static void dadosEscreverU16(uint8_t *buf, uint16_t v) {
  buf[0] = (uint8_t)(v & 0xFF);
  buf[1] = (uint8_t)((v >> 8) & 0xFF);
}

unsigned int dadosGerar(uint8_t *buf, uint8_t sequencia) {
  memset(buf, 0, DADOS_TAM);

  uint8_t bateria = (uint8_t)(70 + (sequencia % 20));
  int16_t tempInt = (int16_t)(250 + (sequencia % 10));
  int16_t tempExt = (int16_t)(-50 + (int)(sequencia % 15));
  uint16_t solar = (uint16_t)(800 + (sequencia % 50));

  buf[0] = DADOS_SAT_ID;
  buf[1] = DADOS_TIPO_TELEMETRIA;
  buf[2] = sequencia;
  buf[3] = bateria;
  dadosEscreverI16(&buf[4], tempInt);
  dadosEscreverI16(&buf[6], tempExt);
  dadosEscreverU16(&buf[8], solar);

  // ecoa até 6 caracteres do nome configurado
  for (unsigned int i = 0; i < 6 && g_satNome[i] != 0; i++) {
    buf[10 + i] = (uint8_t)g_satNome[i];
  }

  return DADOS_TAM;
}

void dadosEnviar() {
  static uint8_t sequencia = 0;
  uint8_t buf[DADOS_TAM];
  unsigned int n = dadosGerar(buf, sequencia);

  if (mqttPublicar(buf, n)) {
    Serial.print("TX telemetria nome=");
    Serial.print(g_satNome);
    Serial.print(" seq=");
    Serial.print(sequencia);
    Serial.print(" HEX=");
    for (unsigned int i = 0; i < n; i++) {
      if (buf[i] < 0x10) Serial.print("0");
      Serial.print(buf[i], HEX);
      Serial.print(i + 1 < n ? " " : "");
    }
    Serial.println();
    sequencia++;
  } else {
    Serial.println("TX telemetria falhou");
  }
}

#endif
