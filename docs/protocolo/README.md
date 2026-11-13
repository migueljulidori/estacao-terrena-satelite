# Contrato de segurança ESP ↔ estação (Manuela)

Os **dois lados** fazem a mesma coisa:

| Operação | ESP | Estação (PC) |
|----------|-----|----------------|
| Assinar (HMAC) | sim (TX) | sim (TX comando) |
| Verificar HMAC | sim (RX) | sim (RX telemetria/ACK) |
| Cifrar (AES-CBC) | sim (TX) | sim (TX comando) |
| Decifrar (AES-CBC) | sim (RX) | sim (RX telemetria/ACK) |

## Limite

Payload MQTT da aplicação = **16 bytes** (conta dados + segurança).

## Algoritmos

- **AES-128-CBC**
- **HMAC-SHA256** truncado aos **8 primeiros bytes**
- Ordem: **HMAC depois AES** no bloco claro (MAC-then-Encrypt)
- **IV não enviado**: `SHA256("SAT-IV" || chave_aes)[0:16]` (igual nos dois lados)

## Chaves (iguais nos dois códigos)

```
AES:  TELECO2K25-AES12   (16 bytes ASCII)
HMAC: TELECO2K25-MAC16   (16 bytes ASCII)
```

Arquivo no ESP: `firmware-esp32/satelite/cripto_mod.h`

## Bloco claro (16 B) antes do AES

| Offset | Campo |
|--------|--------|
| 0 | tipo |
| 1 | seq |
| 2–7 | dados (6 B) |
| 8–15 | HMAC truncado (8 B) de `tipo \|\| seq \|\| dados` |

MQTT publica só o **ciphertext** (16 B).

## Tipos

| tipo | Quem manda | dados |
|------|------------|--------|
| `0x01` | ESP → PC | bat, tempInt×10, tempExt×10, solar/4 |
| `0x02` | ESP → PC (ACK) | cmd + início do nome |
| `0x10` | PC → ESP | PING |
| `0x12` | PC → ESP | STATUS |
| `0x13` | PC → ESP | SET_NOME (6 chars) |

## O que a Manuela implementa no PC

1. **Receber** telemetria/ACK: AES decrypt → checar HMAC → mostrar dados  
2. **Enviar** comando: montar claro → HMAC → AES → publish em `command`  
3. Rejeitar se HMAC falhar  

Enquanto o PC não cifrar, o ESP ainda aceita `PING` / `NOME=` em claro (aviso no Serial). Quando o PC estiver pronto: `#define CRIPTO_ACEITAR_CLARO 0` no ESP.
