# Contrato criptográfico ESP32 ↔ estação terrena

Esta branch espelha o contrato implementado em `main` por `firmware-esp32/satelite/cripto_mod.h`. Os dois lados precisam usar exatamente os mesmos parâmetros e chaves.

## Transporte MQTT

| Direção | Tópico |
|---|---|
| ESP32 → PC (telemetria/ACK) | `satellite/esp32-01/telemetry` |
| PC → ESP32 (comando) | `satellite/esp32-01/command` |

## Formato protegido

O payload publicado no MQTT contém somente o ciphertext AES de 16 bytes.

Antes da cifra, o bloco claro tem exatamente 16 bytes:

| Offset | Tamanho | Campo |
|---|---:|---|
| 0 | 1 B | Tipo |
| 1 | 1 B | Sequência |
| 2–7 | 6 B | Dados |
| 8–15 | 8 B | HMAC truncado |

Cálculo:
1. Concatenar `tipo || sequência || dados` (8 bytes).
2. Calcular HMAC-SHA256 com a chave HMAC e guardar os 8 primeiros bytes.
3. Anexar o HMAC ao bloco claro.
4. Cifrar o bloco de 16 bytes com AES-128-CBC, sem padding adicional.
5. O IV não é transmitido: `SHA256("SAT-IV" || chave_AES)[0:16]`.

Na recepção, decifrar primeiro, recalcular o HMAC e comparar em tempo constante. Só interpretar os campos se a autenticação passar.

## Tipos de mensagem

| Tipo | Direção | Significado |
|---|---|---|
| `0x01` | ESP → PC | Telemetria |
| `0x02` | ESP → PC | ACK |
| `0x10` | PC → ESP | PING |
| `0x12` | PC → ESP | STATUS |
| `0x13` | PC → ESP | SET_NOME |

### Dados da telemetria

| Byte relativo aos dados | Campo | Codificação |
|---|---|---|
| 0 | Bateria | percentual |
| 1–2 | Temperatura interna | int16 little-endian, valor em décimos de °C |
| 3–4 | Temperatura externa | int16 little-endian, valor em décimos de °C |
| 5 | Irradiação solar | valor real dividido por 4; PC multiplica por 4 |

### ACK

- Dados[0]: tipo de comando confirmado (`0x10`, `0x12` ou `0x13`).
- Dados[1–5]: até cinco bytes iniciais do nome do satélite.

### Comandos

- PING: tipo `0x10`, seis bytes de dados zerados.
- STATUS: tipo `0x12`, seis bytes de dados zerados.
- SET_NOME: tipo `0x13`, nome ASCII de até seis bytes, completado com NUL.

A sequência é um contador de 8 bits no PC. A proteção contra replay ainda não está implementada.

## Implementação Python

- `cliente-pc/crypto_protocol.py`: selagem e abertura do pacote.
- `cliente-pc/mqtt_client.py`: decifra/verifica RX e cifra/assina TX.
- `cliente-pc/mqtt_config.py`: chaves locais; arquivo ignorado pelo Git.
- `cliente-pc/test_mqtt_client.py`: testes de cifra/decifra, HMAC, campos e comandos.

## Limitações importantes do contrato atual

Este formato foi reproduzido para interoperar com o firmware existente, mas não deve ser tratado como desenho criptográfico final:
- IV derivado da chave e reutilizado em todas as mensagens; CBC requer IV imprevisível e único por mensagem.
- As chaves do firmware em `main` estão em arquivo versionado e, portanto, são públicas para quem tem acesso ao repositório.
- HMAC truncado a 8 bytes e sequência de 1 byte são escolhas atuais do projeto; não há janela anti-replay.
- A troca desses parâmetros exige atualização simultânea do ESP32 e do cliente PC.

Usar apenas na rede isolada de testes até a equipe definir uma versão segura do protocolo e substituir as chaves expostas.
