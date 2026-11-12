# Cliente PC (estação terrena) — somente RX

Assina a telemetria publicada pelo ESP32. **Não** envia telecomandos.

## O que faz

1. Conecta ao broker Mosquitto
2. Assina `satellite/esp32-01/telemetry`
3. Exibe o payload em hexadecimal (limite do projeto: 16 bytes)

## Instalação

```bash
python3 -m venv .venv
source .venv/bin/activate
pip install -r requirements.txt
cp mqtt_config.example.py mqtt_config.py
```

Edite `mqtt_config.py` com o IP do broker.

## Execução

```bash
python mqtt_client.py
```

Aguarda mensagens do ESP. Ctrl+C para sair.
