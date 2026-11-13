#ifndef COMANDO_MOD_H
#define COMANDO_MOD_H

#include <string.h>
#include <stdlib.h>
#include "dados_mod.h"

/*
 * Estação → satélite: configuração principal = NOME.
 *
 * Texto (fácil no mosquitto_pub):
 *   NOME=Alpha
 *   PING          (teste de enlace)
 *   STATUS        (força telemetria)
 *
 * Binário:
 *   [0] sat_id (1 ou 0=todos)
 *   [1] cmd  0x10 PING | 0x12 STATUS | 0x13 SET_NOME
 *   [2] seq
 *   [3..] nome (só no SET_NOME), até 12 chars
 */

const uint8_t CMD_PING = 0x10;
const uint8_t CMD_STATUS = 0x12;
const uint8_t CMD_SET_NOME = 0x13;

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

  // devolve pedaço do nome atual no ACK
  for (unsigned int i = 0; i < 6 && g_satNome[i] != 0; i++) {
    buf[10 + i] = (uint8_t)g_satNome[i];
  }

  if (mqttPublicar(buf, DADOS_TAM)) {
    Serial.print("TX ACK cmd=0x");
    if (cmdOrigem < 0x10) Serial.print("0");
    Serial.print(cmdOrigem, HEX);
    Serial.print(" nome=");
    Serial.println(g_satNome);
  } else {
    Serial.println("TX ACK falhou");
  }
}

void comandoSetNomeBytes(const byte *nome, unsigned int tam) {
  char tmp[NOME_MAX + 1];
  unsigned int n = tam > NOME_MAX ? NOME_MAX : tam;
  if (n == 0) {
    Serial.println("NOME vazio — ignorado");
    return;
  }
  memcpy(tmp, nome, n);
  tmp[n] = 0;
  for (unsigned int i = 0; i < n; i++) {
    if (tmp[i] < 32 || tmp[i] > 126) {
      tmp[i] = '_';
    }
  }
  dadosDefinirNome(tmp);
}

void comandoExecutarBinario(const byte *payload, unsigned int tamanho) {
  uint8_t satId = payload[0];
  uint8_t cmd = payload[1];
  uint8_t seq = (tamanho > 2) ? payload[2] : 0;

  if (satId != 0 && satId != DADOS_SAT_ID) {
    Serial.println("Comando ignorado: sat_id diferente");
    return;
  }

  switch (cmd) {
    case CMD_PING:
      Serial.println("CMD PING");
      comandoEnviarAck(seq, CMD_PING);
      break;

    case CMD_STATUS:
      Serial.println("CMD STATUS");
      g_forcarTelemetria = true;
      comandoEnviarAck(seq, CMD_STATUS);
      break;

    case CMD_SET_NOME:
      Serial.println("CMD SET_NOME");
      if (tamanho > 3) {
        comandoSetNomeBytes(&payload[3], tamanho - 3);
      }
      comandoEnviarAck(seq, CMD_SET_NOME);
      g_forcarTelemetria = true;
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

  if (strncasecmp(txt, "NOME=", 5) == 0) {
    Serial.println("CMD texto NOME=");
    comandoSetNomeBytes((const byte *)(txt + 5), strlen(txt + 5));
    comandoEnviarAck(0, CMD_SET_NOME);
    g_forcarTelemetria = true;
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

  if (tamanho >= 2 &&
      (payload[1] == CMD_PING || payload[1] == CMD_STATUS || payload[1] == CMD_SET_NOME)) {
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
