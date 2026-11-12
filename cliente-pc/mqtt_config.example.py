# Copie este arquivo para mqtt_config.py e preencha os valores locais.
# mqtt_config.py esta no .gitignore.

BROKER_HOST = "192.168.30.X"  # IP do PC/Raspberry Pi que executa o Mosquitto
BROKER_PORT = 1883
MQTT_USERNAME = "groundstation"
MQTT_PASSWORD = "COLOQUE_A_SENHA_MQTT"

CLIENT_ID = "ground-station-pc"
TOPIC_TELEMETRY = "satellite/esp32-01/telemetry"
TOPIC_COMMAND = "satellite/esp32-01/command"
MAX_PAYLOAD_BYTES = 16
