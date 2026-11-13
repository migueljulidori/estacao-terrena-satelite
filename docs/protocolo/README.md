# Protocolo ESP — bidirecional (versão de teste)

## Tópicos MQTT

| Direção | Tópico | Operação do ESP32 | Operação do PC |
|---|---|---|---|
| ESP → estação | `satellite/esp32-01/telemetry` | Publisher | Subscriber |
| Estação → ESP | `satellite/esp32-01/command` | Subscriber | Publisher |

## Comandos de texto

O firmware atual reconhece os seguintes comandos no tópico `satellite/esp32-01/command`:

| Payload | Efeito esperado |
|---|---|
| `PING` | Responde ACK na telemetria. |
| `STATUS` | Solicita telemetria imediata e responde ACK. |
| `NOME=Alpha` | Atualiza o nome do satélite, responde ACK e força telemetria. |

Para o teste textual, o comando completo deve caber no limite atual de 16 bytes. Por isso, `NOME=` permite até 11 caracteres no payload de texto.

Exemplo com Mosquitto:

```bash
mosquitto_pub -h IP_DO_BROKER -p 1883 -u groundstation -P 'SENHA' \
  -t 'satellite/esp32-01/command' -m 'PING'
```

## Pacote de telemetria — 16 bytes

O firmware publica no tópico `satellite/esp32-01/telemetry`.

| Offset | Tamanho | Campo | Decodificação |
|---|---:|---|---|
| 0 | 1 B | sat_id | Identificador do satélite (atualmente 1) |
| 1 | 1 B | tipo | `0x01` telemetria; `0x02` ACK |
| 2 | 1 B | sequência | Contador de mensagens |
| 3 | 1 B | bateria / comando | Bateria em % na telemetria; código do comando no ACK |
| 4–5 | 2 B | temperatura interna | Inteiro com sinal, little-endian, dividido por 10 (°C) |
| 6–7 | 2 B | temperatura externa | Inteiro com sinal, little-endian, dividido por 10 (°C) |
| 8–9 | 2 B | irradiação solar | Inteiro sem sinal, little-endian |
| 10–15 | 6 B | nome | Até 6 caracteres ASCII iniciais do nome |

Os campos de temperatura e irradiação são usados no pacote tipo `0x01`. No ACK tipo `0x02`, o byte 3 contém o código do comando confirmado e os bytes 10–15 carregam o início do nome atual.

## Códigos de ACK

| Código | Comando |
|---|---|
| `0x10` | PING |
| `0x12` | STATUS |
| `0x13` | SET_NOME |

## Teste ponta a ponta

1. Gravar no ESP32 o firmware bidirecional atual.
2. Confirmar que o ESP32 e o PC usam o mesmo broker Mosquitto.
3. Confirmar no broker a ACL: usuário `satellite` publica telemetria e lê comandos; usuário `groundstation` lê telemetria e publica comandos.
4. Executar o cliente PC e abrir o painel local.
5. Enviar `PING`: conferir recepção no Serial do ESP32 e ACK no painel.
6. Enviar `STATUS`: conferir ACK e telemetria imediata.
7. Enviar `NOME=Alpha`: conferir nome no Serial, ACK e nome nas próximas telemetrias.

## Segurança e limites

- O formato acima é provisório para os testes de comunicação.
- HMAC/AES ainda não estão implementados.
- Não considerar uma mensagem autenticada apenas porque chegou pelo broker.
- O limite de 16 bytes é o limite do firmware de teste atual; confirmar o protocolo final com a equipe.
