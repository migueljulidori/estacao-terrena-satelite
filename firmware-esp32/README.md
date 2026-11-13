# Firmware ESP32 (satélite) — Miguel

## Bidirecional (PI 3T)

- **TX:** `satellite/esp32-01/telemetry` (telemetria + ACK)
- **RX:** `satellite/esp32-01/command` — configuração principal: **nome** (`NOME=...`)

## Arquivos

| Arquivo | Função |
|---------|--------|
| `satelite.ino` | orquestra |
| `wifi_mod.h` | Wi-Fi |
| `mqtt_mod.h` | MQTT |
| `dados_mod.h` | gera/envia telemetria |
| `comando_mod.h` | interpreta comandos |

## Testar comando

```bash
mosquitto_pub -h IP_DO_BROKER -p 1883 -u USUARIO -P 'SENHA' \
  -t 'satellite/esp32-01/command' -m 'PING'
```

Para configurar o nome:
```bash
mosquitto_pub ... -t 'satellite/esp32-01/command' -m 'NOME=Alpha'
```
Serial: `Nome do satelite: Alpha` + ACK; telemetria passa a ecoar o nome.

## Configurar

`mqtt_mod.h`: broker, usuário, senha.

## Próximo

Criptografia (HMAC/AES) nos dois sentidos.
