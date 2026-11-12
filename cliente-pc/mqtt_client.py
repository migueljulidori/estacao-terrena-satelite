"""
Cliente MQTT da estação terrena.

Nesta etapa:
- conecta ao Mosquitto;
- assina o tópico de telemetria;
- exibe mensagens recebidas em HEX;
- permite enviar telecomandos de teste de até 16 bytes.

O formato/decodificação dos dados será implementado após a definição
do protocolo em docs/protocolo/.
"""

import sys
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
        print(f"Inscrito em: {config.TOPIC_TELEMETRY}")
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

    print("\nCliente pronto.")
    print("Digite um telecomando de teste (ate 16 bytes) e pressione Enter.")
    print("Digite /sair para encerrar.")
    try:
        while True:
            command = input("> ")
            if command.strip() == "/sair":
                break

            payload = command.encode("utf-8")
            if len(payload) > config.MAX_PAYLOAD_BYTES:
                print(f"Mensagem rejeitada: {len(payload)} bytes. Limite: 16.")
                continue

            info = client.publish(config.TOPIC_COMMAND, payload, qos=0, retain=False)
            info.wait_for_publish()
            if info.rc == mqtt.MQTT_ERR_SUCCESS:
                print(f"Telecomando enviado ({len(payload)} byte(s)).")
            else:
                print(f"Falha ao publicar. Codigo: {info.rc}")
    except (KeyboardInterrupt, EOFError):
        print("\nEncerrando...")
    finally:
        client.loop_stop()
        client.disconnect()


if __name__ == "__main__":
    main()
