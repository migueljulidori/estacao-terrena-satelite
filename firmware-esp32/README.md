# Firmware ESP32 (satélite) — parte do Miguel

Responsável: **Miguel Julidori**.

## Ordem oficial

1. **Wi-Fi** ← atual  
2. MQTT  
3. Gerar dados  
4. Criptografia  
5. Montar pacote (≤ 16 bytes)  
6. Publicar  

## Arquivos (etapa Wi-Fi)

| Arquivo | Função |
|---------|--------|
| `satelite/satelite.ino` | Código do satélite (Wi-Fi) |
| `satelite/secrets.example.h` | Modelo de SSID/senha |
| `satelite/secrets.h` | Suas credenciais (**não vai para o Git**) |

## Como gravar

```bash
cd firmware-esp32/satelite
cp secrets.example.h secrets.h
# edite WIFI_SSID e WIFI_PASSWORD
```

Abra `satelite.ino` no Arduino IDE (placa ESP32), grave e use o Serial em **115200**.

Esperado: `Wi-Fi conectado`, IP e RSSI. Só depois avançamos para MQTT.
