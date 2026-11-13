# Protocolo de comunicação — PI 3T

## Transporte e tópicos

O transporte definido pelo enunciado é MQTT. O enlace é bidirecional:

| Direção | Tópico |
|---|---|
| ESP32 → estação (telemetria) | `satellite/esp32-01/telemetry` |
| Estação → ESP32 (comando/configuração) | `satellite/esp32-01/command` |

## Estado atual

- ESP32 publica mensagens de teste `TESTE-N`.
- ESP32 assina o tópico de comandos.
- Cliente Python assina telemetria e publica comandos de teste.
- O firmware ainda apenas exibe comandos recebidos no Monitor Serial.
- Não há formato final de mensagem nem mecanismos criptográficos implementados.

## Definição que a equipe precisa concluir

Especificar para os dois sentidos:
1. ID do satélite de origem/destino.
2. Tipo de mensagem.
3. Dados úteis (telemetria, comando ou configuração).
4. Campos de segurança (por exemplo, IV e tag HMAC).
5. Tamanho e representação de cada campo.
6. Regras de codificação, decodificação, autenticação, descriptografia e rejeição.

O formato é livre, mas precisa ser documentado byte a byte.

## Tamanho do payload

O código atual mantém um limite de teste de 16 bytes. O PDF atualizado do PI 3T não declara esse limite explicitamente. Confirmar com o professor/equipe antes de fixá-lo como requisito do protocolo final; se mantido, os dados úteis e os campos de segurança precisam caber juntos nesse espaço.
