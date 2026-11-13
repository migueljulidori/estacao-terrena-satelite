# Contexto do projeto

Documento vivo do Projeto Integrado. Atualize quando requisitos mudarem e registre em [`ATUALIZACOES.md`](ATUALIZACOES.md).

Fonte principal atual: [`docs/referencia/PI_3T_Sistema_Comunicacao_Satelite.pdf`](docs/referencia/PI_3T_Sistema_Comunicacao_Satelite.pdf).

## Objetivo geral

Simular um satélite (ESP32) que se comunica com uma estação terrestre (Raspberry Pi / PC com broker MQTT), dimensionando potência de enlace GEO e analisando segurança da comunicação e do acesso de gerência (SSH).

## Cenário (PI 3T)

| Papel | Hardware / software | Função |
|-------|---------------------|--------|
| Estação terrestre | Raspberry Pi / PC | Broker MQTT + gerência SSH |
| Satélite | ESP32 | Telemetria (TX) + comandos/config (RX) |
| App Python | PC | Enviar/receber mensagens e validar segurança |
| Gerência | SSH na LAN/VPN | Manutenção (não expor à internet) |

Rede isolada da turma.

## Enlace bidirecional (obrigatório no PI 3T)

| Direção | Conteúdo | Tópico |
|---------|----------|--------|
| Satélite → estação | Telemetria e ACK | `satellite/esp32-01/telemetry` |
| Estação → satélite | Comando / configuração | `satellite/esp32-01/command` |

## Protocolo de teste

- Transporte: **MQTT**
- Telemetria e ACK: pacotes binários de 16 bytes, conforme `docs/protocolo/README.md`.
- Comandos de texto para teste: `PING`, `STATUS` e `NOME=...` (o payload inteiro deve caber em 16 bytes).
- O limite de 16 bytes é o limite do firmware de teste atual; confirmar com a equipe antes de definir o protocolo final.

## Segurança (mínimo)

1. **Autenticação** das mensagens (ex.: HMAC-SHA256 / mbedTLS)
2. **Confidencialidade** (ex.: AES-CBC / mbedTLS)

Cliente Python deverá verificar e rejeitar mensagens inválidas quando a camada criptográfica for implementada. Chaves: gerar, guardar e documentar distribuição. **HMAC/AES ainda não estão implementados nesta etapa de teste.**

## Pipeline do firmware ESP32

1. **Wi-Fi** ✓ — IP fixo configurado
2. **MQTT** ✓ — connect + publish telemetria + subscribe comando
3. **Gerar dados** ✓ — telemetria simulada (`dados_mod.h`)
3b. **Comandos RX** ✓ — PING, STATUS e configuração de nome
4. **Criptografia** ← próxima
5. **Montar pacote final** — pendente
6. **Publicar** — telemetria e ACK já publicados no tópico de telemetria

## Link budget (GEO)

Usar fórmulas do PI 3T (slant range, FSPL, ganho, C/N₀, P_t). Banda/frequência a escolher pela equipe.

## Desafio hacking

Após validar comunicação: avaliar SSH, tráfego e mecanismos de segurança — somente na rede isolada da equipe, com autorização.

## Entregáveis (resumo)

Código ESP (Arduino) + cliente Python, fluxogramas, especificação do protocolo, segurança/chaves, memória de cálculo, evidências, análise de testes e resultados do desafio.

## Decisões da equipe

| Item | Decisão |
|------|---------|
| Sentido do enlace | **Bidirecional** |
| Tópico telemetria | `satellite/esp32-01/telemetry` |
| Tópico comando | `satellite/esp32-01/command` |
| Broker | Mosquitto (IP configurado no firmware e no cliente PC; confirmar na LAN) |
| Porta MQTT | TCP 1883 |
| Autenticação / cripto | HMAC-SHA256 + AES-CBC (planejado; não implementado) |
| Progresso firmware | Telemetria TX + comandos RX + ACK |
| Cliente PC | Subscriber de telemetria/ACK + publisher de comandos + painel local |
| Layout TX/RX | `docs/protocolo/README.md` |

## Fontes

- `docs/referencia/PI_3T_Sistema_Comunicacao_Satelite.pdf`
- `docs/referencia/Estacao_Terrena_Satelite_Fase1.pdf`
- `docs/referencia/Planejamento_sistema_ESP32_HMAC_AES.pdf`
