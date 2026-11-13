# Protocolo MQTT — PI 3T (bidirecional)

## Tópicos

| Direção | Tópico |
|---|---|
| Satélite ESP32 → estação | `satellite/esp32-01/telemetry` |
| Estação → satélite ESP32 | `satellite/esp32-01/command` |

## Estado

- Comunicação de teste: ESP envia `TESTE-N`; PC pode enviar comando curto.
- Formato byte a byte **ainda não definido** (dados úteis + ID + segurança).
- Limite de 16 bytes mantido no código de teste; confirmar com o PI 3T / professor o tamanho final.

Próximo: especificar campos de telemetria e de comando em conjunto com firmware e cliente Python.
