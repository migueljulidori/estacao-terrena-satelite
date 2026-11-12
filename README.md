# Estação Terrena de Comunicação via Satélite

Projeto Integrado – Telecomunicações (3º Trimestre 2026).

Simulação de uma **estação terrena** que se comunica com um satélite ESP32 via MQTT, com dimensionamento de enlace (link budget) e mecanismos de segurança (HMAC e/ou AES).

## Para a equipe

1. [Passo a passo Git](CONTRIBUTING.md)
2. [Divisão de responsabilidades](EQUIPE.md)
3. [Histórico de mudanças](ATUALIZACOES.md)

## Status atual

- **Broker:** Mosquitto na estação terrena.
- **Enlace MQTT:** bidirecional.
- **ESP32:** publica telemetria e assina comandos.
- **Aplicação Python:** assina telemetria e publica comandos.
- **Payload:** máximo de 16 bytes por mensagem.
- **Pendente:** definir protocolo final, telemetria real e segurança HMAC/AES.

## Fluxo

```text
ESP32 --publish telemetry--> Mosquitto --entrega--> Aplicação Python
ESP32 <--recebe command----- Mosquitto <--publish-- Aplicação Python
```

## Estrutura

```text
estacao-terrena-satelite/
├── README.md
├── EQUIPE.md
├── CONTEXTO.md
├── ATUALIZACOES.md
├── CONTRIBUTING.md
├── firmware-esp32/
│   ├── README.md
│   └── satelite/
│       ├── satelite.ino
│       ├── wifi_mod.h
│       ├── mqtt_mod.h
│       └── dados_mod.h
├── cliente-pc/
│   ├── README.md
│   ├── mqtt_client.py
│   ├── mqtt_config.example.py
│   └── requirements.txt
├── estacao-terrena/
│   ├── README.md
│   ├── mosquitto.conf.example
│   └── acl.example
└── docs/
    ├── referencia/
    ├── protocolo/
    └── link-budget/
```

## Segurança

Não envie credenciais reais, chaves Wi-Fi, HMAC/AES ou arquivos locais de configuração. Use os modelos `*.example` e mantenha valores reais em arquivos ignorados pelo Git.
