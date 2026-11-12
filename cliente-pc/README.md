# Aplicação da estação terrena — cliente MQTT

A aplicação Python conecta-se ao Mosquitto e trabalha nos dois sentidos:

- **Subscribe (RX):** recebe a telemetria publicada pelo ESP32.
- **Publish (TX):** envia comandos para o ESP32.

O Mosquitto é o broker. Este programa é um cliente MQTT; não inicia um segundo broker.

## Fluxo

```text
ESP32 --publish telemetry--> Mosquitto --entrega--> Aplicação Python
ESP32 <--recebe command----- Mosquitto <--publish-- Aplicação Python
```

## Instalação

Entre nesta pasta e execute:

```bash
python3 -m venv .venv
source .venv/bin/activate
pip install -r requirements.txt
cp mqtt_config.example.py mqtt_config.py
```

Edite `mqtt_config.py` e informe o IP da máquina que executa o Mosquitto, além da senha do usuário `groundstation`.

## Execução

```bash
python mqtt_client.py
```

A aplicação começa a receber telemetria automaticamente. Para transmitir, digite um comando no terminal, por exemplo:

- `PING`
- `STATUS`
- `LED_ON`

O texto enviado é convertido para UTF-8 e precisa ocupar entre 1 e 16 bytes. Digite `/sair` para encerrar.

## Tópicos e permissões

| Cliente | Ação | Tópico |
|---|---|---|
| ESP32 | Publish | `satellite/esp32-01/telemetry` |
| Aplicação PC | Subscribe | `satellite/esp32-01/telemetry` |
| Aplicação PC | Publish | `satellite/esp32-01/command` |
| ESP32 | Subscribe | `satellite/esp32-01/command` |

## Observação

Nesta etapa, o firmware apenas exibe o comando recebido no Monitor Serial. Os comandos ainda não acionam hardware nem alteram parâmetros do satélite. A interpretação/execução será definida depois, junto ao protocolo.
