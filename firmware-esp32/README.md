# Firmware ESP32 (satélite) — Miguel

## Ordem

1. Wi-Fi ✓  
2. **MQTT** ← atual (conectar + assinar telecomando)  
3. Gerar dados  
4. Criptografia  
5. Montar pacote  
6. Publicar telemetria  

## Arquivos

| Arquivo | Função |
|---------|--------|
| `satelite/satelite.ino` | `setup` / `loop` (só orquestra) |
| `satelite/wifi_mod.h` / `wifi_mod.cpp` | Wi-Fi (SSID, IP fixo, conectar) |
| `satelite/mqtt_mod.h` / `mqtt_mod.cpp` | MQTT (broker, tópicos, callback) |

## Como gravar

1. Instale **PubSubClient** (Nick O'Leary).
2. Em `wifi_mod.cpp`: rede e IP do ESP.
3. Em `mqtt_mod.cpp`: `mqtt_broker` = IP do Mosquitto.
4. Abra a pasta `satelite/` no Arduino IDE, grave, Serial **115200**.
