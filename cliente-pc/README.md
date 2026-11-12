# Cliente PC (estação terrena)

Base inicial de cliente MQTT para testar a comunicação com o satélite ESP32.

## O que faz

1. Conecta ao broker Mosquitto.
2. Assina `satellite/esp32-01/telemetry`.
3. Exibe mensagens recebidas em hexadecimal.
4. Permite publicar telecomandos de teste em `satellite/esp32-01/command`.
5. Impede o envio de payloads maiores que 16 bytes.

A interpretação dos campos, verificação HMAC e descriptografia AES dependem das próximas etapas e da definição de `docs/protocolo/`.

## Instalação

Na pasta `cliente-pc/`:

```bash
python3 -m venv .venv
source .venv/bin/activate
pip install -r requirements.txt
cp mqtt_config.example.py mqtt_config.py
```

Edite `mqtt_config.py` com o IP do broker e as credenciais criadas no Mosquitto.

## Execução

```bash
python mqtt_client.py
```

Digite uma mensagem curta para enviá-la como telecomando. O ESP32 deve exibir os bytes no Monitor Serial.

Para encerrar, digite `/sair` ou pressione Ctrl+C.

## Limites desta versão

- O formato binário do payload ainda não foi definido.
- O cliente não interpreta telemetria nem valida HMAC/AES.
- O limite de 16 bytes é aplicado ao conteúdo da mensagem, sem contar cabeçalhos MQTT.
- Use apenas na rede isolada da turma; esta configuração não habilita TLS.
