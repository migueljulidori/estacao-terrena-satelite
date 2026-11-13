# Firmware ESP32 (satélite) — Miguel

## Papel (PI 3T)

- **TX:** telemetria em `satellite/esp32-01/telemetry`
- **RX:** comandos em `satellite/esp32-01/command`

## Arquivos

| Arquivo | Função |
|---------|--------|
| `satelite.ino` | `setup` / `loop` |
| `wifi_mod.h` | Wi-Fi |
| `mqtt_mod.h` | MQTT |
| `dados_mod.h` | Gera e envia telemetria (16 bytes) |

## Telemetria (layout provisório)

| Bytes | Campo |
|-------|--------|
| 0 | id satélite |
| 1 | tipo (0x01 telemetria) |
| 2 | sequência |
| 3 | bateria % |
| 4–5 | temp. interna (°C×10, int16 LE) |
| 6–7 | temp. externa (°C×10, int16 LE) |
| 8–9 | irradiação (uint16 LE) |
| 10–15 | reservado (segurança depois) |

Valores **simulados** (sem sensores). A cada 5 s publica no broker.

## Configurar

`mqtt_mod.h`: `mqtt_broker`, `mqtt_user`, `mqtt_senha`

## Próximo no ESP

Criptografia → montar pacote (HMAC/AES nos bytes reservados / payload final).
