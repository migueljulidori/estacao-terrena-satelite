"""
Aplicação da estação terrena — cliente MQTT bidirecional.

- recebe telemetria do ESP32 (subscribe / RX);
- envia comandos ao ESP32 (publish / TX);
- valida o tamanho provisório de 1 a 16 bytes, conforme o firmware de teste.
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
        result, _mid = client.subscribe(config.TOPIC_TELEMETRY, qos=0)
        if result == mqtt.MQTT_ERR_SUCCESS:
            print(f"SUBSCRIBE ativo: {config.TOPIC_TELEMETRY}")
        else:
            print(f"Falha ao assinar telemetria. Codigo: {result}")
    else:
        print(f"Falha na conexao MQTT. Codigo: {reason_code}")


def on_message(client, userdata, message):
    payload = message.payload
    print("\n" + "=" * 52)
    print(f"TELEMETRIA recebida | tópico: {message.topic}")
    print(f"Tamanho: {len(payload)} byte(s)")
    print(f"HEX: {payload.hex(' ')}")

    if len(payload) == 0 or len(payload) > config.MAX_PAYLOAD_BYTES:
        print("MENSAGEM REJEITADA: tamanho fora do intervalo provisório.")
        print("=" * 52)
        return

    try:
        print(f"Texto: {payload.decode('utf-8')}")
    except UnicodeDecodeError:
        print("Texto: conteúdo binário (não UTF-8).")

    print("=" * 52)


def publicar_comando(client, texto):
    """Publica comando UTF-8 respeitando o limite provisório do firmware."""
    payload = texto.encode("utf-8")

    if not payload:
        print("Comando vazio: nada foi enviado.")
        return
    if len(payload) > config.MAX_PAYLOAD_BYTES:
        print(f"Comando rejeitado: {len(payload)} bytes; limite provisório: "
              f"{config.MAX_PAYLOAD_BYTES}.")
        return
    if not client.is_connected():
        print("Não conectado ao broker. Comando não enviado.")
        return

    info = client.publish(config.TOPIC_COMMAND, payload, qos=0, retain=False)
    if info.rc == mqtt.MQTT_ERR_SUCCESS:
        info.wait_for_publish(timeout=5)
        if info.is_published():
            print(f"Comando publicado ({len(payload)} byte(s)) em "
                  f"{config.TOPIC_COMMAND}: {texto}")
        else:
            print("Publicação ainda não confirmada pelo cliente MQTT.")
    else:
        print(f"Falha ao publicar. Código: {info.rc}")


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
        print(f"Não foi possível conectar ao broker: {error}")
        sys.exit(1)

    client.loop_start()
    print("\nEstação terrena pronta.")
    print("Recebendo telemetria continuamente.")
    print("Digite um comando para enviar ao ESP32 (ex.: PING ou STATUS).")
    print("O limite atual de 16 bytes é provisório e precisa ser confirmado.")
    print("Digite /sair para encerrar.\n")

    try:
        while True:
            try:
                comando = input("Comando > ").strip()
            except EOFError:
                break
            if comando.lower() in ("/sair", "/exit", "sair"):
                break
            if comando:
                publicar_comando(client, comando)
    except KeyboardInterrupt:
        print("\nEncerrando aplicação...")
    finally:
        client.loop_stop()
        client.disconnect()
        print("Desconectado do broker.")


if __name__ == "__main__":
    main()
