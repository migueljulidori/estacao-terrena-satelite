# Copie este arquivo para mqtt_config.py e preencha os valores locais.
# mqtt_config.py esta no .gitignore. Nunca versione credenciais reais.

BROKER_HOST = "192.168.30.250"  # IP da máquina que executa o Mosquitto; confirme na rede
BROKER_PORT = 1883
MQTT_USERNAME = "groundstation"
MQTT_PASSWORD = "COLOQUE_A_SENHA_DO_USUARIO_GROUNDSTATION"

CLIENT_ID = "ground-station-pc"
TOPIC_TELEMETRY = "satellite/esp32-01/telemetry"  # ESP -> PC
TOPIC_COMMAND = "satellite/esp32-01/command"      # PC -> ESP
MAX_PAYLOAD_BYTES = 16

# O painel é servido somente na própria máquina.
WEB_HOST = "127.0.0.1"
WEB_PORT = 8765
