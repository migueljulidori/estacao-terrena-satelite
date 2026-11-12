# Estação Terrena de Comunicação via Satélite

Projeto Integrado – Telecomunicações (3º Trimestre 2026).

Simulação de uma **estação terrena** que se comunica com **satélites** (nós ESP32) via MQTT, com dimensionamento de enlace (link budget) e mecanismos de segurança (autenticação HMAC e/ou criptografia AES).

## Para a equipe — comece por aqui

Este repositório é **privado e compartilhado**. Cada pessoa deve **assumir uma parte** e enviar atualizações pelo GitHub.

1. **Passo a passo Git:** [`CONTRIBUTING.md`](CONTRIBUTING.md) (convite → clone → branch → PR)
2. **Escolha sua parte:** [`EQUIPE.md`](EQUIPE.md) — coloque seu nome na tabela e comece na pasta indicada
3. **Registre mudanças:** [`ATUALIZACOES.md`](ATUALIZACOES.md) (obrigatório em todo PR)

### Em andamento agora

- **Miguel — ESP32:** pipeline Wi-Fi → MQTT → dados → cripto → pacote → publicar — **agora só Wi-Fi** em [`firmware-esp32/`](firmware-esp32/)
- **Ainda sem responsável:** `cliente-pc/`, `estacao-terrena/`, `docs/protocolo/`, `docs/link-budget/` — veja [`EQUIPE.md`](EQUIPE.md)

## Estrutura

```
estacao-terrena-satelite/
├── README.md                 # Visão geral (este arquivo)
├── EQUIPE.md                 # Quem faz cada parte (coloque seu nome)
├── CONTEXTO.md               # Contexto e requisitos do projeto
├── ATUALIZACOES.md           # Histórico de mudanças (obrigatório atualizar)
├── CONTRIBUTING.md           # Passo a passo no GitHub
├── firmware-esp32/           # Satélite ESP32 — Miguel (Wi-Fi em andamento)
├── cliente-pc/               # Cliente PC — aguardando responsável
├── estacao-terrena/          # Broker MQTT / SSH — aguardando responsável
└── docs/
    ├── referencia/           # PDFs oficiais do projeto
    ├── protocolo/            # Payload ≤ 16 bytes — aguardando responsável
    └── link-budget/          # Cálculo de enlace — aguardando responsável
```

## Documentação de referência

- [`CONTEXTO.md`](CONTEXTO.md) — objetivos, cenário, protocolo e fases
- [`docs/referencia/Estacao_Terrena_Satelite_Fase1.pdf`](docs/referencia/Estacao_Terrena_Satelite_Fase1.pdf)
- [`docs/referencia/Planejamento_sistema_ESP32_HMAC_AES.pdf`](docs/referencia/Planejamento_sistema_ESP32_HMAC_AES.pdf)

## Status atual

- Repositório e documentação colaborativa prontos
- Firmware ESP32: **conexão Wi-Fi** iniciada (`firmware-esp32/satelite/`)
- Demais partes: aguardando cada integrante assumir em `EQUIPE.md`
