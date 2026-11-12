# Firmware ESP32 (satélite) — Miguel

## Papel

ESP só **TX** (publish) · PC só **RX** (subscribe)

## Arquivos

| Arquivo | Função |
|---------|--------|
| `satelite.ino` | `setup` / `loop` |
| `wifi_mod.h` | Wi-Fi |
| `mqtt_mod.h` | MQTT + `mqttPublicar` |
| `dados_mod.h` | Monta/envia dados (agora: teste `TESTE-N`) |

## Agora

A cada 5 s publica em `satellite/esp32-01/telemetry` algo como `TESTE-0`, `TESTE-1`, … (≤ 16 bytes).

## Teste no PC

```bash
mosquitto_sub -h 127.0.0.1 -p 1883 -u groundstation -P 'SENHA' -t 'satellite/esp32-01/telemetry' -v
```

Ou o cliente Python em `cliente-pc/`.

## Configurar no ESP

Em `mqtt_mod.h`: `mqtt_broker`, `mqtt_user`, `mqtt_senha`.
