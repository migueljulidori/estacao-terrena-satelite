# Firmware ESP32 (satélite) — Miguel

## Papel no enlace

- **ESP32:** só **publica** (TX) telemetria no broker  
- **PC:** só **assina** (RX) como client MQTT  
- **Sem** troca bidirecional / sem telecomando no ESP  

## Ordem

1. Wi-Fi ✓  
2. **MQTT** ← atual (só conectar / manter; ainda sem `publish`)  
3. Gerar dados  
4. Criptografia  
5. Montar pacote  
6. Publicar telemetria  

## Arquivos

| Arquivo | Função |
|---------|--------|
| `satelite/satelite.ino` | `setup` / `loop` |
| `satelite/wifi_mod.*` | Wi-Fi |
| `satelite/mqtt_mod.*` | MQTT (cliente publisher) |

## Como gravar

1. Instale **PubSubClient**.
2. `wifi_mod.cpp`: rede e IP do ESP.
3. `mqtt_mod.cpp`: `mqtt_broker` = IP do Mosquitto.
4. Abra a pasta `satelite/`, grave, Serial **115200**.

Tópico de TX (quando chegar a etapa 6): `satellite/esp32-01/telemetry`
