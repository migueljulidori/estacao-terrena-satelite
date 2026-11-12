"""
Cliente MQTT da estação terrena (somente RX).

- conecta ao Mosquitto;
- assina satellite/esp32-01/telemetry;
- exibe mensagens em HEX.

O ESP32 só publica (TX). Este PC só recebe (RX). Sem telecomando.
"""

import sys
import time
import paho.mqtt.client as mqtt

try:
    import mqtt_config as config
except ImportError:
    print("Arquivo mqtt_config.py nao encontrado.")
    print("Copie mqtt_config.example.py para mqtt_config.py e configure o broker.")
    sys.exit(1)


def on_connect(client, userdata, flags, reason_code, properties):
    if reason_code == 0:
        print(f"Conectado ao broker {config.BROKER_HOST}:{config.BROKER_PORT}")
        client.subscribe(config.TOPIC_TELEMETRY, qos=0)
        print(f"Inscrito (RX) em: {config.TOPIC_TELEMETRY}")
    else:
        print(f"Falha na conexao MQTT. Codigo: {reason_code}")


def on_message(client, userdata, message):
    payload = message.payload

    print(f"\nMensagem recebida em: {message.topic}")
    print(f"Tamanho: {len(payload)} byte(s)")
    print(f"HEX: {payload.hex(' ')}")

    if len(payload) > config.MAX_PAYLOAD_BYTES:
        print("AVISO: payload acima do limite de 16 bytes.")
        return

    try:
        print(f"Texto: {payload.decode('utf-8')}")
    except UnicodeDecodeError:
        print("Texto: conteudo binario (nao UTF-8)")


def main():
    client = mqtt.Client(
        mqtt.CallbackAPIVersion.VERSION2,
        client_id=config.CLIENT_ID,
        protocol=mqtt.MQTTv311,
    )
    client.username_pw_set(config.MQTT_USERNAME, config.MQTT_PASSWORD)
    client.on_connect = on_connect
    client.on_message = on_message

    print(f"Conectando ao broker {config.BROKER_HOST}:{config.BROKER_PORT}...")
    try:
        client.connect(config.BROKER_HOST, config.BROKER_PORT, keepalive=30)
    except OSError as error:
        print(f"Nao foi possivel conectar ao broker: {error}")
        sys.exit(1)

    client.loop_start()
    print("Cliente RX pronto. Aguardando telemetria do ESP (Ctrl+C para sair).")

    try:
        while True:
            time.sleep(0.5)
    except KeyboardInterrupt:
        print("\nEncerrando...")
    finally:
        client.loop_stop()
        client.disconnect()


if __name__ == "__main__":
    main()
