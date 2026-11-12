# Estação Terrena de Comunicação via Satélite

Projeto Integrado – Telecomunicações (3º Trimestre 2026).

Simulação de uma **estação terrena** que se comunica com **satélites** (nós ESP32) via MQTT, com dimensionamento de enlace (link budget) e mecanismos de segurança (autenticação HMAC e/ou criptografia AES).

## Para a equipe

Este repositório é **privado e compartilhado**. Cada mudança no código ou na documentação deve ir para o GitHub para que **todo o grupo veja o mesmo estado**.

Fluxo recomendado (resumo):

1. Clonar o repositório e trabalhar em uma **branch** (não direto em `main`).
2. Fazer commit com mensagem clara.
3. Atualizar [`ATUALIZACOES.md`](ATUALIZACOES.md) descrevendo o que mudou.
4. Abrir um **Pull Request** e pedir revisão de pelo menos um colega.
5. Após aprovação, fazer merge em `main`.

Detalhes em [`CONTRIBUTING.md`](CONTRIBUTING.md).

## Estrutura

```
estacao-terrena-satelite/
├── README.md                 # Visão geral (este arquivo)
├── CONTEXTO.md               # Contexto e requisitos do projeto
├── ATUALIZACOES.md           # Histórico de mudanças (obrigatório atualizar)
├── CONTRIBUTING.md           # Como colaborar no GitHub
├── firmware-esp32/           # Firmware do satélite (ESP32)
├── cliente-pc/               # Cliente da estação terrena (PC)
├── estacao-terrena/          # Configuração do broker MQTT / scripts da estação
└── docs/
    ├── referencia/           # PDFs oficiais do projeto
    ├── protocolo/            # Especificação do protocolo (16 bytes)
    └── link-budget/          # Memória de cálculo do enlace
```

## Documentação de referência

- [`CONTEXTO.md`](CONTEXTO.md) — objetivos, cenário, protocolo e fases
- [`docs/referencia/Estacao_Terrena_Satelite_Fase1.pdf`](docs/referencia/Estacao_Terrena_Satelite_Fase1.pdf)
- [`docs/referencia/Planejamento_sistema_ESP32_HMAC_AES.pdf`](docs/referencia/Planejamento_sistema_ESP32_HMAC_AES.pdf)

## Status atual

Repositório criado com documentação inicial. Implementação (firmware, cliente e protocolo) começa a partir das próximas instruções da equipe.
