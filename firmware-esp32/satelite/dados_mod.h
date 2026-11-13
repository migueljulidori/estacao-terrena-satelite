#ifndef DADOS_MOD_H
#define DADOS_MOD_H

#include <string.h>
#include "mqtt_mod.h"

/*
 * Etapa: gerar dados (telemetria).
 * Ainda sem criptografia / HMAC — entram depois.
 *
 * Layout provisório do payload (16 bytes):
 *  [0]     id do satélite (1 = SAT-01)
 *  [1]     tipo (0x01 = telemetria)
 *  [2]     sequência (0..255)
 *  [3]     bateria % (0..100)
 *  [4..5]  temp. interna °C * 10  (int16 little-endian)
 *  [6..7]  temp. externa °C * 10  (int16 little-endian)
 *  [8..9]  irradiação solar (uint16 little-endian, unidade a definir)
 *  [10..15] reservado (0) — espaço futuro p/ segurança
 *
 * Valores agora são simulados (sem sensores físicos).
 */

const uint8_t DADOS_SAT_ID = 1;
const uint8_t DADOS_TIPO_TELEMETRIA = 0x01;
const unsigned int DADOS_TAM = 16;

static void dadosEscreverI16(uint8_t *buf, int16_t v) {
  buf[0] = (uint8_t)(v & 0xFF);
  buf[1] = (uint8_t)((v >> 8) & 0xFF);
}

static void dadosEscreverU16(uint8_t *buf, uint16_t v) {
  buf[0] = (uint8_t)(v & 0xFF);
  buf[1] = (uint8_t)((v >> 8) & 0xFF);
}

// Preenche o buffer com telemetria simulada. Retorna tamanho (16).
unsigned int dadosGerar(uint8_t *buf, uint8_t sequencia) {
  memset(buf, 0, DADOS_TAM);

  // Simulação simples (varia com a sequência)
  uint8_t bateria = (uint8_t)(70 + (sequencia % 20));          // 70..89 %
  int16_t tempInt = (int16_t)(250 + (sequencia % 10));         // 25.0..25.9 °C
  int16_t tempExt = (int16_t)(-50 + (int)(sequencia % 15));    // -5.0..-3.6 °C
  uint16_t solar = (uint16_t)(800 + (sequencia % 50));         // 800..849

  buf[0] = DADOS_SAT_ID;
  buf[1] = DADOS_TIPO_TELEMETRIA;
  buf[2] = sequencia;
  buf[3] = bateria;
  dadosEscreverI16(&buf[4], tempInt);
  dadosEscreverI16(&buf[6], tempExt);
  dadosEscreverU16(&buf[8], solar);

  return DADOS_TAM;
}

void dadosEnviar() {
  static uint8_t sequencia = 0;
  uint8_t buf[DADOS_TAM];
  unsigned int n = dadosGerar(buf, sequencia);

  if (mqttPublicar(buf, n)) {
    Serial.print("TX telemetria seq=");
    Serial.print(sequencia);
    Serial.print(" bat=");
    Serial.print(buf[3]);
    Serial.print("% HEX=");
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
