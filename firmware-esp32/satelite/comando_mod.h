#ifndef COMANDO_MOD_H
#define COMANDO_MOD_H

#include <string.h>
#include <stdlib.h>
#include "dados_mod.h"

/*
 * Comandos da estação → satélite (PI 3T — bidirecional).
 *
 * Layout binário (até 16 bytes):
 *  [0] sat_id   (1 = este satélite; 0 = broadcast)
 *  [1] cmd      (ver constantes abaixo)
 *  [2] seq
 *  [3] param0   (ex.: segundos em SET_INTERVAL)
 *  [4..15] reservado
 *
 * Também aceita texto simples (teste fácil com mosquitto_pub):
 *  "PING"      → responde ACK na telemetria
 *  "STATUS"    → envia telemetria na hora
 *  "INT=5"     → intervalo de TX em segundos (1..60)
 */

const uint8_t CMD_PING = 0x10;
const uint8_t CMD_SET_INTERVAL = 0x11;
const uint8_t CMD_STATUS = 0x12;

const uint8_t DADOS_TIPO_ACK = 0x02;

unsigned long g_intervaloTxMs = 5000;
bool g_forcarTelemetria = false;

unsigned long comandoIntervaloMs() {
  return g_intervaloTxMs;
}

bool comandoConsumirForcarTx() {
  if (!g_forcarTelemetria) {
    return false;
  }
  g_forcarTelemetria = false;
  return true;
}

void comandoEnviarAck(uint8_t seq, uint8_t cmdOrigem) {
  uint8_t buf[DADOS_TAM];
  memset(buf, 0, DADOS_TAM);
  buf[0] = DADOS_SAT_ID;
  buf[1] = DADOS_TIPO_ACK;
  buf[2] = seq;
  buf[3] = cmdOrigem;

  if (mqttPublicar(buf, DADOS_TAM)) {
    Serial.print("TX ACK cmd=0x");
    if (cmdOrigem < 0x10) Serial.print("0");
    Serial.println(cmdOrigem, HEX);
  } else {
    Serial.println("TX ACK falhou");
  }
}

void comandoExecutarBinario(const byte *payload, unsigned int tamanho) {
  uint8_t satId = payload[0];
  uint8_t cmd = payload[1];
  uint8_t seq = (tamanho > 2) ? payload[2] : 0;
  uint8_t param0 = (tamanho > 3) ? payload[3] : 0;

  if (satId != 0 && satId != DADOS_SAT_ID) {
    Serial.println("Comando ignorado: sat_id diferente");
    return;
  }

  switch (cmd) {
    case CMD_PING:
      Serial.println("CMD PING");
      comandoEnviarAck(seq, CMD_PING);
      break;

    case CMD_SET_INTERVAL: {
      uint8_t seg = param0;
      if (seg < 1) seg = 1;
      if (seg > 60) seg = 60;
      g_intervaloTxMs = (unsigned long)seg * 1000UL;
      Serial.print("CMD SET_INTERVAL -> ");
      Serial.print(seg);
      Serial.println(" s");
      comandoEnviarAck(seq, CMD_SET_INTERVAL);
      break;
    }

    case CMD_STATUS:
      Serial.println("CMD STATUS");
      g_forcarTelemetria = true;
      comandoEnviarAck(seq, CMD_STATUS);
      break;

    default:
      Serial.print("CMD desconhecido: 0x");
      Serial.println(cmd, HEX);
      break;
  }
}

bool comandoPareceTexto(const byte *payload, unsigned int tamanho) {
  if (tamanho == 0) return false;
  for (unsigned int i = 0; i < tamanho; i++) {
    byte c = payload[i];
    if (c == 0) break;
    if (c < 32 || c > 126) return false;
  }
  return true;
}

void comandoExecutarTexto(const byte *payload, unsigned int tamanho) {
  char txt[17];
  unsigned int n = tamanho > 16 ? 16 : tamanho;
  memcpy(txt, payload, n);
  txt[n] = 0;

  // trim simples de \r\n
  for (int i = (int)n - 1; i >= 0; i--) {
    if (txt[i] == '\r' || txt[i] == '\n' || txt[i] == ' ') {
      txt[i] = 0;
    } else {
      break;
    }
  }

  if (strcasecmp(txt, "PING") == 0) {
    Serial.println("CMD texto PING");
    comandoEnviarAck(0, CMD_PING);
    return;
  }

  if (strcasecmp(txt, "STATUS") == 0) {
    Serial.println("CMD texto STATUS");
    g_forcarTelemetria = true;
    comandoEnviarAck(0, CMD_STATUS);
    return;
  }

  if (strncasecmp(txt, "INT=", 4) == 0) {
    int seg = atoi(txt + 4);
    if (seg < 1) seg = 1;
    if (seg > 60) seg = 60;
    g_intervaloTxMs = (unsigned long)seg * 1000UL;
    Serial.print("CMD texto INT -> ");
    Serial.print(seg);
    Serial.println(" s");
    comandoEnviarAck(0, CMD_SET_INTERVAL);
    return;
  }

  Serial.print("CMD texto desconhecido: ");
  Serial.println(txt);
}

void comandoProcessar(const byte *payload, unsigned int tamanho) {
  if (tamanho == 0) {
    Serial.println("Comando vazio");
    return;
  }

  Serial.print("RX comando HEX=");
  for (unsigned int i = 0; i < tamanho; i++) {
    if (payload[i] < 0x10) Serial.print("0");
    Serial.print(payload[i], HEX);
    Serial.print(i + 1 < tamanho ? " " : "");
  }
  Serial.println();

  // Binário se tamanho >= 2 e byte[1] é um cmd conhecido
  if (tamanho >= 2 &&
      (payload[1] == CMD_PING || payload[1] == CMD_SET_INTERVAL || payload[1] == CMD_STATUS)) {
    comandoExecutarBinario(payload, tamanho);
    return;
  }

  if (comandoPareceTexto(payload, tamanho)) {
    comandoExecutarTexto(payload, tamanho);
    return;
  }

  Serial.println("Comando em formato nao reconhecido");
}

#endif
