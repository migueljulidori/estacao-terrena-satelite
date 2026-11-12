# Firmware ESP32 (satélite) — Miguel

## Papel

ESP só **TX** (publish) · PC só **RX** (subscribe)

## Arquivos (3 no total)

| Arquivo | Função |
|---------|--------|
| `satelite.ino` | `setup` / `loop` |
| `wifi_mod.h` | todo o Wi-Fi |
| `mqtt_mod.h` | todo o MQTT |

## Ordem

1. Wi-Fi ✓  
2. MQTT ← conectar (ainda sem publish)  
3. Gerar dados → 4. Criptografia → 5. Pacote → 6. Publicar  

## Como gravar

1. PubSubClient instalado  
2. Ajuste `mqtt_broker` em `mqtt_mod.h` (IP do PC com Mosquitto)  
3. Abra a pasta `satelite/`, grave, Serial **115200**
