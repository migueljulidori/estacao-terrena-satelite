# Protocolo MQTT — payload máximo de 16 bytes

O transporte definido pelo projeto é MQTT. Esta pasta contém a especificação da mensagem de aplicação, que deverá ser detalhada byte a byte.

## Tópicos

| Direção | Tópico | Operação |
|---|---|---|
| Satélite ESP32 → estação terrena | `satellite/esp32-01/telemetry` | ESP publica; PC assina |
| Estação terrena → satélite ESP32 | `satellite/esp32-01/command` | PC publica; ESP assina |

## Requisitos que não mudam

- Cada payload da aplicação deve ter no máximo **16 bytes**.
- Os bytes de identificação, tipo, contador, autenticação e/ou criptografia contam dentro dos 16 bytes.
- O protocolo deve definir ordem, tamanho, unidade, endianness, valores válidos e tratamento de mensagens inválidas.
- ESP32 e aplicação PC devem rejeitar payloads vazios ou maiores que 16 bytes.

## Estado desta implementação

A comunicação MQTT está organizada em duas direções. O ESP32 publica mensagens de teste `TESTE-N` e recebe comandos; a aplicação Python recebe as mensagens e permite publicar comandos.

Nesta etapa, os comandos são apenas exibidos no Monitor Serial do ESP32. Ainda não há execução de ações físicas. Os nomes `PING`, `STATUS` e `LED_ON` são exemplos de texto para validar o transporte, não um protocolo final.

O formato definitivo da telemetria e dos comandos ainda precisa ser especificado. Atenção: HMAC-SHA256 completo e AES-CBC com IV/cabeçalho têm custos de espaço que podem ser incompatíveis com 16 bytes. A equipe precisará decidir uma estratégia criptográfica e documentar qualquer truncamento/compactação antes de implementar segurança.

## Teste bruto

A aplicação Python deve mostrar as mensagens recebidas em HEX. Para testar o sentido de retorno, digite `PING` na aplicação e confira a mensagem recebida no Monitor Serial do ESP32.
