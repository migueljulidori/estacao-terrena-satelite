# Firmware ESP32 (satélite) — Miguel

## Papel (PI 3T)

- **TX:** publica telemetria em `satellite/esp32-01/telemetry`
- **RX:** assina comandos em `satellite/esp32-01/command`

## Arquivos

| Arquivo | Função |
|---------|--------|
| `satelite.ino` | `setup` / `loop` |
| `wifi_mod.h` | Wi-Fi |
| `mqtt_mod.h` | MQTT (publish + subscribe) |
| `dados_mod.h` | Envio de teste `TESTE-N` |

## Configurar

Em `mqtt_mod.h`: `mqtt_broker`, `mqtt_user`, `mqtt_senha`.

## Teste (só ESP)

1. Grave o sketch (PubSubClient).
2. Serial 115200: `TX ... TESTE-N`.
3. Para testar RX, publique no broker no tópico `satellite/esp32-01/command` (ex.: `mosquitto_pub`) — o Serial mostra `RX comando`.
