# Firmware ESP32 (satélite) — Miguel

## Papel

O ESP32 atua como cliente MQTT com duas funções:

- **Publish (TX):** envia telemetria para a estação terrena.
- **Subscribe (RX):** recebe comandos enviados pela aplicação da estação.

## Arquivos

| Arquivo | Função |
|---|---|
| `satelite.ino` | Orquestra `setup` e `loop` |
| `wifi_mod.h` | Conexão Wi-Fi e IP fixo |
| `mqtt_mod.h` | Conexão MQTT, assinatura de comandos e publicação |
| `dados_mod.h` | Monta/envia dados (atualmente: `TESTE-N`) |

## Funcionamento atual

- A cada 5 s publica `TESTE-0`, `TESTE-1`, ... em `satellite/esp32-01/telemetry`.
- Mantém assinatura em `satellite/esp32-01/command`.
- Ao receber um comando válido (1 a 16 bytes), exibe HEX e texto no Monitor Serial.
- Payload vazio ou acima de 16 bytes é rejeitado.

## Configuração

Em `mqtt_mod.h`, confira o IP do broker e as credenciais locais. O IP deve ser o da máquina que executa o Mosquitto, não o IP fixo do ESP32.

## Teste

1. Inicie o Mosquitto na estação terrena.
2. Grave o firmware no ESP32 e abra o Monitor Serial em 115200.
3. Execute a aplicação em `cliente-pc/`.
4. Confira se `TESTE-N` aparece na aplicação.
5. Digite `PING` na aplicação e confira `RX ... PING` no Monitor Serial.
