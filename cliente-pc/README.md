# Aplicação da estação terrena — cliente MQTT

A aplicação Python conecta-se ao Mosquitto e atua nos dois sentidos:

- **Subscribe (RX):** recebe a telemetria publicada pelo ESP32.
- **Publish (TX):** envia comandos ao ESP32.

O Mosquitto é o broker. Este programa é um cliente MQTT, não um segundo broker.

## Fluxo

```text
ESP32 --publish telemetry--> Mosquitto --entrega--> Aplicação Python
ESP32 <--recebe command----- Mosquitto <--publish-- Aplicação Python
```

## Instalação

Dentro desta pasta:

```bash
python3 -m venv .venv
source .venv/bin/activate
pip install -r requirements.txt
cp mqtt_config.example.py mqtt_config.py
```

Edite `mqtt_config.py` com o IP da máquina que executa o Mosquitto e as credenciais locais do usuário `groundstation`.

## Execução

```bash
python mqtt_client.py
```

A aplicação assina o tópico de telemetria e permite digitar comandos no terminal. Exemplos: `PING`, `STATUS`, `LED_ON`. Use `/sair` para encerrar.

## Tópicos e permissões

| Cliente | Ação | Tópico |
|---|---|---|
| ESP32 | Publish | `satellite/esp32-01/telemetry` |
| Aplicação PC | Subscribe | `satellite/esp32-01/telemetry` |
| Aplicação PC | Publish | `satellite/esp32-01/command` |
| ESP32 | Subscribe | `satellite/esp32-01/command` |

## Limite de payload

O firmware e o cliente atualmente limitam mensagens de teste a 16 bytes. **O PDF atualizado do PI 3T não estabelece explicitamente esse limite**; portanto, ele deve ser tratado como uma restrição provisória herdada do código anterior, a confirmar com o professor/equipe antes da definição do protocolo final.

## Estado e limitações

- O cliente recebe e exibe bytes em HEX e tenta apresentar texto UTF-8.
- O cliente publica comandos de teste.
- O firmware, nesta etapa, apenas exibe comandos recebidos no Monitor Serial; ainda não executa ações no hardware.
- A validação criptográfica HMAC e a descriptografia ainda não estão implementadas.
- A execução real deve ser testada na LAN isolada com o broker e o ESP32 ligados.
