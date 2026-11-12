# Firmware ESP32 (satélite) — parte do Miguel

Responsável: **Miguel Julidori**.

## Ordem oficial

1. **Wi-Fi** ← atual  
2. MQTT  
3. Gerar dados  
4. Criptografia  
5. Montar pacote (≤ 16 bytes)  
6. Publicar  

## Modularização (avise-se: o que é cada arquivo)

| Arquivo | Papel |
|---------|--------|
| `satelite/satelite.ino` | Programa principal: só `setup` e `loop`. Chama os módulos. |
| `satelite/config.h` | Declara constantes (ID, tempos). Valores definidos no `.ino`. |
| `satelite/wifi_link.h` | Declara as funções do Wi-Fi (`wifiConectar`, etc.). |
| `satelite/wifi_link.cpp` | Implementa o Wi-Fi de verdade. |
| `satelite/secrets.example.h` | Modelo de SSID/senha. |
| `satelite/secrets.h` | Suas credenciais. **Crie localmente; não vai para o Git.** |

Nas próximas etapas vamos acrescentar módulos no mesmo estilo, por exemplo:
`mqtt_link.*` → `dados.*` → `cripto.*` → `pacote.*` → e o `.ino` só orquestra.

## Como gravar

```bash
cd firmware-esp32/satelite
cp secrets.example.h secrets.h
# edite WIFI_SSID e WIFI_PASSWORD
```

Abra a pasta `satelite/` no Arduino IDE (placa ESP32), grave e use o Serial em **115200**.
