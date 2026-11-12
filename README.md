# Estação Terrena de Comunicação via Satélite

Projeto Integrado – Telecomunicações (3º Trimestre 2026).

Simulação de uma **estação terrena** que se comunica com **satélites** (nós ESP32) via MQTT, com dimensionamento de enlace (link budget) e mecanismos de segurança (autenticação HMAC e/ou criptografia AES).

## Para a equipe — comece por aqui

Este repositório é compartilhado. Cada pessoa deve **assumir uma parte** e enviar atualizações pelo GitHub.

1. **Passo a passo Git:** [`CONTRIBUTING.md`](CONTRIBUTING.md)
2. **Escolha sua parte:** [`EQUIPE.md`](EQUIPE.md)
3. **Registre mudanças:** [`ATUALIZACOES.md`](ATUALIZACOES.md) (obrigatório em todo PR)

## Status atual

- **Enlace:** ESP só **TX** (publish); PC só **RX** (subscribe) — sem telecomando
- **Miguel — ESP32:** Wi-Fi + MQTT connect em [`firmware-esp32/`](firmware-esp32/)
- **Broker / cliente PC:** `estacao-terrena/`, `cliente-pc/`
- **Pendente:** IP do broker, gerar dados, cripto, pacote, publish — veja [`EQUIPE.md`](EQUIPE.md)

## Estrutura

```
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
│       └── secrets.example.h
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

## Documentos principais

- [`CONTEXTO.md`](CONTEXTO.md) — objetivos, cenário, protocolo e fases.
- [`EQUIPE.md`](EQUIPE.md) — divisão de responsabilidades.
- [`docs/referencia/Estacao_Terrena_Satelite_Fase1.pdf`](docs/referencia/Estacao_Terrena_Satelite_Fase1.pdf)
- [`docs/referencia/Planejamento_sistema_ESP32_HMAC_AES.pdf`](docs/referencia/Planejamento_sistema_ESP32_HMAC_AES.pdf)

## Segurança

Não envie credenciais Wi-Fi, MQTT, SSH, chaves HMAC/AES ou arquivos locais de configuração. Use os modelos `*.example` e mantenha os valores reais em arquivos ignorados pelo Git.
