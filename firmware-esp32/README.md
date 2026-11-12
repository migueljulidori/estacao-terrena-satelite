# Firmware ESP32 (satélite) — Miguel

Etapas implementadas: **Wi-Fi + conexão MQTT**.

Pipeline do projeto:
1. Wi-Fi
2. MQTT
3. Gerar dados
4. Criptografia
5. Montar pacote
6. Publicar

As etapas de geração de telemetria, criptografia, montagem do payload e publicação ainda não foram implementadas.

## Arquivos

- `satelite/satelite.ino`: conexão Wi-Fi/MQTT e recepção de telecomandos.
- `satelite/secrets.example.h`: modelo de configuração local.
- `satelite/secrets.h`: arquivo local com credenciais reais (não enviar ao Git).

## Preparação

1. Instale a biblioteca **PubSubClient**, de Nick O'Leary, pelo gerenciador de bibliotecas da Arduino IDE.
2. Copie `satelite/secrets.example.h` para `satelite/secrets.h`.
3. Preencha SSID, senha Wi-Fi, IP do broker, usuário e senha MQTT.
4. Abra `satelite/satelite.ino` na Arduino IDE.
5. Selecione sua placa ESP32 e a porta serial.
6. Grave o firmware e abra o Monitor Serial em **115200 baud**.

## Comportamento esperado

- O ESP32 conecta ao Wi-Fi usando o IP fixo `192.168.30.52`.
- Tenta conectar ao broker MQTT na porta configurada (padrão: 1883).
- Ao conectar, assina `satellite/esp32-01/command`.
- Mostra mensagens recebidas em hexadecimal.
- Rejeita mensagens com mais de 16 bytes.
- Tenta reconectar automaticamente se Wi-Fi ou MQTT caírem.

## Teste

Com o broker ativo e o cliente PC configurado, publique uma mensagem de teste no tópico de telecomando. O ESP32 deve exibir o tópico e os bytes recebidos no Monitor Serial.

**Importante:** o IP do broker ainda não foi definido nos documentos do projeto. Substitua o valor de exemplo em `secrets.h` pelo IP real do computador/Raspberry Pi que executa o Mosquitto. O IP do broker não é o IP fixo do ESP32.

A recepção é apenas bruta nesta fase. A interpretação do comando depende da especificação final em `docs/protocolo/`.
