# Protocolo MQTT — payload máximo de 16 bytes

O transporte definido pelo projeto é MQTT. Esta pasta deverá conter a especificação byte a byte da mensagem de aplicação.

## Tópicos iniciais

| Direção | Tópico |
|---|---|
| Satélite ESP32 → estação terrena | `satellite/esp32-01/telemetry` |
| ~~Estação → ESP32 (telecomando)~~ | **não usado** — enlace só ESP TX → PC RX |

O identificador `esp32-01` é o identificador inicial do nó. Se a equipe definir outro ID, atualize firmware, cliente PC e esta documentação em conjunto.

## Requisitos que não mudam

- Cada payload da aplicação deve ter no máximo **16 bytes**.
- Os bytes de identificação, tipo, contador, autenticação e/ou criptografia contam dentro dos 16 bytes.
- O protocolo deve definir ordem, tamanho, unidade, endianness, valores válidos e tratamento de mensagens inválidas.
- O cliente PC deve rejeitar mensagens maiores que 16 bytes.

## Estado desta implementação

Os tópicos MQTT e o limite de tamanho já são usados pelo firmware e pelo cliente de teste. **O formato dos 16 bytes ainda não foi definido**: não atribua significado aos bytes recebidos nesta fase.

A próxima definição deverá ser feita em conjunto com firmware e cliente PC. Atenção: HMAC-SHA256 completo e AES-CBC com IV/cabeçalho têm custos de espaço que podem ser incompatíveis com 16 bytes. A equipe precisará decidir uma estratégia criptográfica e documentar qualquer truncamento/compactação antes de implementar segurança.

## Teste bruto

Quando o ESP publicar telemetria, o PC (ou `mosquitto_sub`) deve mostrar até 16 bytes em HEX no tópico `satellite/esp32-01/telemetry`.
