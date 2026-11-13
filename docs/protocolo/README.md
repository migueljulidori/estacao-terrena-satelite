# Protocolo ESP — bidirecional

## Resposta direta

**Sim:** o ESP **manda** (telemetria) e **recebe** (comandos), se estiver gravado com o firmware atual e inscrito em `command`.

| Direção | Tópico | O que faz |
|---|---|---|
| ESP → estação | `satellite/esp32-01/telemetry` | telemetria periódica (+ ACK) |
| Estação → ESP | `satellite/esp32-01/command` | configuração, principalmente **nome** |

## Configuração principal: NOME

Texto (teste):

```bash
mosquitto_pub -h IP -p 1883 -u USUARIO -P 'SENHA' \
  -t 'satellite/esp32-01/command' -m 'NOME=Alpha'
```

- ESP guarda o nome (até 12 caracteres)
- Responde ACK na telemetria
- Próximas telemetrias levam até 6 chars do nome nos bytes 10–15
- Serial: `Nome do satelite: Alpha`

Outros textos úteis: `PING`, `STATUS`.

## Telemetria (16 bytes)

| Offset | Campo |
|--------|--------|
| 0 | sat_id |
| 1 | tipo `0x01` telemetria / `0x02` ACK |
| 2 | seq |
| 3 | bateria % (ou cmd no ACK) |
| 4–9 | temps + solar |
| 10–15 | início do **nome** |
