# Protocolo ESP — bidirecional (provisório)

## Tópicos

| Direção | Tópico |
|---|---|
| ESP → estação | `satellite/esp32-01/telemetry` |
| Estação → ESP | `satellite/esp32-01/command` |

## Telemetria / ACK (ESP → estação) — 16 bytes

| Offset | Campo | Notas |
|--------|--------|------|
| 0 | `sat_id` | `1` = SAT-01 |
| 1 | `tipo` | `0x01` telemetria · `0x02` ACK |
| 2 | `seq` | |
| 3 | bateria % **ou** cmd de origem (se ACK) | |
| 4–5 | temp. interna (°C×10, int16 LE) | telemetria |
| 6–7 | temp. externa (°C×10, int16 LE) | telemetria |
| 8–9 | solar (uint16 LE) | telemetria |
| 10–15 | reservado | segurança depois |

## Comando (estação → ESP) — até 16 bytes

### Binário

| Offset | Campo |
|--------|--------|
| 0 | `sat_id` (`1` ou `0` = todos) |
| 1 | `cmd` |
| 2 | `seq` |
| 3 | `param0` |

| `cmd` | Nome | Efeito |
|-------|------|--------|
| `0x10` | PING | publica ACK |
| `0x11` | SET_INTERVAL | `param0` = segundos (1..60) do TX |
| `0x12` | STATUS | força telemetria agora + ACK |

### Texto (teste rápido)

```bash
mosquitto_pub -h IP -t 'satellite/esp32-01/command' -m 'PING'
mosquitto_pub -h IP -t 'satellite/esp32-01/command' -m 'STATUS'
mosquitto_pub -h IP -t 'satellite/esp32-01/command' -m 'INT=3'
```

(Use `-u` / `-P` se o broker exigir senha.)
