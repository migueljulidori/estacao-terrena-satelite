# Copie este arquivo para mqtt_config.py e preencha os valores locais.
# mqtt_config.py está no .gitignore.

BROKER_HOST = "IP_DO_COMPUTADOR_LINUX"  # Computador que executa o Mosquitto
BROKER_PORT = 1883
MQTT_USERNAME = "groundstation"
MQTT_PASSWORD = "COLOQUE_A_SENHA_MQTT"

CLIENT_ID = "ground-station-pc"
TOPIC_TELEMETRY = "satellite/esp32-01/telemetry"  # ESP32 publica; Python assina
TOPIC_COMMAND = "satellite/esp32-01/command"      # Python publica; ESP32 assina
MAX_PAYLOAD_BYTES = 16
