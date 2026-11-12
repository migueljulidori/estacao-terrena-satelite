# Firmware ESP32 (satélite) — Miguel

## Ordem

1. Wi-Fi ✓  
2. **MQTT** ← atual (conectar + assinar telecomando)  
3. Gerar dados  
4. Criptografia  
5. Montar pacote  
6. Publicar telemetria  

## Arquivo

Só `satelite/satelite.ino`

1. Instale **PubSubClient** (Nick O'Leary) no Arduino IDE.
2. Confira `ssid` / `senha` (rede `teleco2k25`).
3. Ajuste `mqtt_broker` para o IP da máquina com Mosquitto (não use o IP do ESP).
4. Grave e abra o Serial em **115200**.

Esperado: `Wi-Fi OK`, `MQTT ... OK`, inscrição em `satellite/esp32-01/command`.

Tópicos (combinados com a equipe):
- telemetria: `satellite/esp32-01/telemetry` (publicar depois)
- comando: `satellite/esp32-01/command` (já assina)
