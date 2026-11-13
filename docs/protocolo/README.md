# Protocolo — telemetria ESP (provisório)

## Tópicos

| Direção | Tópico |
|---|---|
| ESP → estação | `satellite/esp32-01/telemetry` |
| Estação → ESP | `satellite/esp32-01/command` |

## Payload de telemetria (16 bytes) — firmware atual

| Offset | Tamanho | Campo | Notas |
|--------|---------|--------|------|
| 0 | 1 | `sat_id` | `1` = SAT-01 |
| 1 | 1 | `tipo` | `0x01` telemetria |
| 2 | 1 | `seq` | 0..255 |
| 3 | 1 | `bateria` | % |
| 4 | 2 | `temp_int` | °C × 10, int16 little-endian |
| 6 | 2 | `temp_ext` | °C × 10, int16 little-endian |
| 8 | 2 | `solar` | uint16 LE (unidade a definir) |
| 10 | 6 | reservado | zeros; futuro HMAC/IV |

Ainda **sem** criptografia. Equipe pode alterar este layout antes da etapa de segurança.
