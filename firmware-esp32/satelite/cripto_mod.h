#ifndef CRIPTO_MOD_H
#define CRIPTO_MOD_H

#include <Arduino.h>
#include <string.h>
#include "mbedtls/aes.h"
#include "mbedtls/md.h"

/*
 * Contrato ESP ↔ estação (Manuela / PC) — payload MQTT = 16 bytes
 *
 * 1) Montar bloco claro (16 B):
 *      [0] tipo
 *      [1] seq
 *      [2..7] dados (6 B)
 *      [8..15] HMAC-SHA256(chave_hmac, tipo||seq||dados)[0..7]  // truncado
 * 2) AES-128-CBC cifrar o bloco → 16 B no MQTT
 * 3) IV NÃO vai no pacote: SHA256("SAT-IV" || chave_aes)[0..15]
 *
 * Mesmas chaves nos dois lados. RX inválido (HMAC) → rejeitar.
 *
 * Transição: CRIPTO_ACEITAR_CLARO permite PING/NOME= sem cifra (aviso).
 */

const unsigned int CRIPTO_TAM = 16;
const unsigned int CRIPTO_DADOS_TAM = 6;
const unsigned int CRIPTO_MAC_TAM = 8;

const uint8_t CRIPTO_CHAVE_AES[16] = {
  'T','E','L','E','C','O','2','K','2','5','-','A','E','S','1','2'
};

const uint8_t CRIPTO_CHAVE_HMAC[16] = {
  'T','E','L','E','C','O','2','K','2','5','-','M','A','C','1','6'
};

#ifndef CRIPTO_ACEITAR_CLARO
#define CRIPTO_ACEITAR_CLARO 1
#endif

static bool criptoIvDerivado(uint8_t iv[16]) {
  const mbedtls_md_info_t *info = mbedtls_md_info_from_type(MBEDTLS_MD_SHA256);
  if (!info) return false;

  uint8_t hash[32];
  mbedtls_md_context_t ctx;
  mbedtls_md_init(&ctx);
  if (mbedtls_md_setup(&ctx, info, 0) != 0) {
    mbedtls_md_free(&ctx);
    return false;
  }
  mbedtls_md_starts(&ctx);
  mbedtls_md_update(&ctx, (const uint8_t *)"SAT-IV", 6);
  mbedtls_md_update(&ctx, CRIPTO_CHAVE_AES, 16);
  mbedtls_md_finish(&ctx, hash);
  mbedtls_md_free(&ctx);
  memcpy(iv, hash, 16);
  return true;
}

static bool criptoHmacTrunc(const uint8_t *msg, size_t len, uint8_t out[CRIPTO_MAC_TAM]) {
  const mbedtls_md_info_t *info = mbedtls_md_info_from_type(MBEDTLS_MD_SHA256);
  if (!info) return false;
  uint8_t full[32];
  if (mbedtls_md_hmac(info, CRIPTO_CHAVE_HMAC, 16, msg, len, full) != 0) return false;
  memcpy(out, full, CRIPTO_MAC_TAM);
  return true;
}

static bool criptoMacIgual(const uint8_t *a, const uint8_t *b, size_t n) {
  uint8_t d = 0;
  for (size_t i = 0; i < n; i++) d |= (uint8_t)(a[i] ^ b[i]);
  return d == 0;
}

bool criptoSelar(uint8_t tipo, uint8_t seq, const uint8_t dados[CRIPTO_DADOS_TAM],
                 uint8_t saida[CRIPTO_TAM]) {
  uint8_t claro[CRIPTO_TAM];
  claro[0] = tipo;
  claro[1] = seq;
  memcpy(&claro[2], dados, CRIPTO_DADOS_TAM);

  uint8_t paraMac[2 + CRIPTO_DADOS_TAM];
  paraMac[0] = tipo;
  paraMac[1] = seq;
  memcpy(&paraMac[2], dados, CRIPTO_DADOS_TAM);
  if (!criptoHmacTrunc(paraMac, sizeof(paraMac), &claro[8])) return false;

  uint8_t iv[16];
  if (!criptoIvDerivado(iv)) return false;

  mbedtls_aes_context aes;
  mbedtls_aes_init(&aes);
  if (mbedtls_aes_setkey_enc(&aes, CRIPTO_CHAVE_AES, 128) != 0) {
    mbedtls_aes_free(&aes);
    return false;
  }
  int rc = mbedtls_aes_crypt_cbc(&aes, MBEDTLS_AES_ENCRYPT, CRIPTO_TAM, iv, claro, saida);
  mbedtls_aes_free(&aes);
  return rc == 0;
}

bool criptoAbrir(const uint8_t entrada[CRIPTO_TAM], uint8_t *tipo, uint8_t *seq,
                 uint8_t dados[CRIPTO_DADOS_TAM]) {
  uint8_t iv[16];
  if (!criptoIvDerivado(iv)) return false;

  uint8_t claro[CRIPTO_TAM];
  mbedtls_aes_context aes;
  mbedtls_aes_init(&aes);
  if (mbedtls_aes_setkey_dec(&aes, CRIPTO_CHAVE_AES, 128) != 0) {
    mbedtls_aes_free(&aes);
    return false;
  }
  int rc = mbedtls_aes_crypt_cbc(&aes, MBEDTLS_AES_DECRYPT, CRIPTO_TAM, iv, entrada, claro);
  mbedtls_aes_free(&aes);
  if (rc != 0) return false;

  uint8_t paraMac[2 + CRIPTO_DADOS_TAM];
  paraMac[0] = claro[0];
  paraMac[1] = claro[1];
  memcpy(&paraMac[2], &claro[2], CRIPTO_DADOS_TAM);

  uint8_t macCalc[CRIPTO_MAC_TAM];
  if (!criptoHmacTrunc(paraMac, sizeof(paraMac), macCalc)) return false;
  if (!criptoMacIgual(macCalc, &claro[8], CRIPTO_MAC_TAM)) {
    Serial.println("CRIPTO: HMAC invalido — rejeitado");
    return false;
  }

  *tipo = claro[0];
  *seq = claro[1];
  memcpy(dados, &claro[2], CRIPTO_DADOS_TAM);
  return true;
}

#endif
