# Firmware ESP32 (satélite) — Miguel

## Bidirecional (PI 3T)

- **TX:** `satellite/esp32-01/telemetry` (telemetria + ACK)
- **RX:** `satellite/esp32-01/command` (PING / STATUS / INT=n)

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

Serial: `CMD texto PING` e `TX ACK`.  
`STATUS` força telemetria; `INT=3` muda o período para 3 s.

## Configurar

`mqtt_mod.h`: broker, usuário, senha.

## Próximo

Criptografia (HMAC/AES) nos dois sentidos.
