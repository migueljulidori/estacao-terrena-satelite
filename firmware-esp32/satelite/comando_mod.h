#ifndef COMANDO_MOD_H
#define COMANDO_MOD_H

#include <string.h>
#include <stdlib.h>
#include "dados_mod.h"
#include "cripto_mod.h"

/*
 * RX: ciphertext 16 B → AES decrypt → verifica HMAC → executa.
 * Tipos de comando (campo tipo do bloco claro): 0x10 PING | 0x12 STATUS | 0x13 SET_NOME
 * SET_NOME: dados[0..5] = nome
 *
 * Se CRIPTO_ACEITAR_CLARO: ainda aceita texto PING/STATUS/NOME= (transição).
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
  if (!g_forcarTelemetria) return false;
  g_forcarTelemetria = false;
  return true;
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
    if (tmp[i] < 32 || tmp[i] > 126) tmp[i] = '_';
  }
  dadosDefinirNome(tmp);
}

void comandoExecutar(uint8_t tipo, uint8_t seq, const uint8_t dados[CRIPTO_DADOS_TAM]) {
  switch (tipo) {
    case CMD_PING:
      Serial.println("CMD PING");
      dadosEnviarAck(seq, CMD_PING);
      break;
    case CMD_STATUS:
      Serial.println("CMD STATUS");
      g_forcarTelemetria = true;
      dadosEnviarAck(seq, CMD_STATUS);
      break;
    case CMD_SET_NOME:
      Serial.println("CMD SET_NOME");
      comandoSetNomeBytes(dados, CRIPTO_DADOS_TAM);
      dadosEnviarAck(seq, CMD_SET_NOME);
      g_forcarTelemetria = true;
      break;
    default:
      Serial.print("CMD desconhecido: 0x");
      Serial.println(tipo, HEX);
      break;
  }
}

bool comandoPareceTexto(const byte *payload, unsigned int tamanho) {
  if (tamanho == 0) return false;
  for (unsigned int i = 0; i < tamanho; i++) {
    if (payload[i] == 0) break;
    if (payload[i] < 32 || payload[i] > 126) return false;
  }
  return true;
}

void comandoExecutarTextoClaro(const byte *payload, unsigned int tamanho) {
  char txt[17];
  unsigned int n = tamanho > 16 ? 16 : tamanho;
  memcpy(txt, payload, n);
  txt[n] = 0;
  for (int i = (int)n - 1; i >= 0; i--) {
    if (txt[i] == '\r' || txt[i] == '\n' || txt[i] == ' ') txt[i] = 0;
    else break;
  }

  Serial.println("AVISO: comando em claro (Manuela deve passar a cifrar+HMAC)");

  if (strcasecmp(txt, "PING") == 0) {
    dadosEnviarAck(0, CMD_PING);
    return;
  }
  if (strcasecmp(txt, "STATUS") == 0) {
    g_forcarTelemetria = true;
    dadosEnviarAck(0, CMD_STATUS);
    return;
  }
  if (strncasecmp(txt, "NOME=", 5) == 0) {
    comandoSetNomeBytes((const byte *)(txt + 5), strlen(txt + 5));
    dadosEnviarAck(0, CMD_SET_NOME);
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

  Serial.print("RX HEX=");
  for (unsigned int i = 0; i < tamanho; i++) {
    if (payload[i] < 0x10) Serial.print("0");
    Serial.print(payload[i], HEX);
    Serial.print(i + 1 < tamanho ? " " : "");
  }
  Serial.println();

  if (tamanho == CRIPTO_TAM) {
    uint8_t tipo, seq;
    uint8_t dados[CRIPTO_DADOS_TAM];
    if (criptoAbrir(payload, &tipo, &seq, dados)) {
      Serial.println("RX OK (AES+HMAC)");
      comandoExecutar(tipo, seq, dados);
      return;
    }
  }

#if CRIPTO_ACEITAR_CLARO
  if (comandoPareceTexto(payload, tamanho)) {
    comandoExecutarTextoClaro(payload, tamanho);
    return;
  }
#endif

  Serial.println("Comando rejeitado");
}

#endif
