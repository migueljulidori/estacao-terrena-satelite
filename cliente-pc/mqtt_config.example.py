# Copie para mqtt_config.py (arquivo ignorado pelo Git).
# Preencha com os mesmos valores configurados no firmware ESP32.
BROKER_HOST = "192.168.30.250"
BROKER_PORT = 1883
MQTT_USERNAME = "groundstation"
MQTT_PASSWORD = "COLOQUE_A_SENHA_DO_USUARIO_GROUNDSTATION"
CLIENT_ID = "ground-station-pc"
TOPIC_TELEMETRY = "satellite/esp32-01/telemetry"
TOPIC_COMMAND = "satellite/esp32-01/command"
MAX_PAYLOAD_BYTES = 16
WEB_HOST = "127.0.0.1"
WEB_PORT = 8765

# Copie os 16 bytes ASCII de cada chave do cripto_mod.h do firmware.
# Não coloque as chaves reais neste arquivo de exemplo nem as versione.
AES_KEY = "SUBSTITUA_PELA_CHAVE_AES_DE_16_BYTES"
HMAC_KEY = "SUBSTITUA_PELA_CHAVE_HMAC_DE_16_BYTES"
