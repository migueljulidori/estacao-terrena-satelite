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
| Satélite → estação | Telemetria | `satellite/esp32-01/telemetry` |
| Estação → satélite | Comando / configuração | `satellite/esp32-01/command` |

## Protocolo

- Transporte: **MQTT**
- Formato livre, definido pela equipe (campos, IDs, IV/HMAC etc.)
- Dados úteis: energia, temperaturas, irradiação, etc. (a definir)
- **Nota:** a Fase 1 antiga citava payload ≤ 16 bytes; o PI 3T não fixa esse limite. Mantemos 16 bytes no código de teste até a equipe definir o protocolo final.

## Segurança (mínimo)

1. **Autenticação** das mensagens (ex.: HMAC-SHA256 / mbedTLS)
2. **Confidencialidade** (ex.: AES-CBC / mbedTLS)

Cliente Python deve verificar e rejeitar mensagens inválidas. Chaves: gerar, guardar e documentar distribuição.

## Pipeline do firmware ESP32

1. **Wi-Fi** ✓ — IP `192.168.30.52`
2. **MQTT** ✓ — connect + publish + subscribe comando
3. **Gerar dados** ✓ — telemetria simulada (`dados_mod.h`)
3b. **Comandos RX** ✓ — configuração de **nome** (`NOME=...`) + PING/STATUS
4. **Criptografia** ← próxima
5. **Montar pacote** — pendente
6. **Publicar** — já usado; versão final com segurança depois

## Link budget (GEO)

Usar fórmulas do PI 3T (slant range, FSPL, ganho, C/N₀, P_t). Banda/frequência a escolher pela equipe.

## Desafio hacking

Após validar comunicação: avaliar SSH, tráfego, mecanismos de segurança — só na rede isolada, ética e legislação (Lei 12.737/2012, LGPD).

## Entregáveis (resumo)

Código ESP (Arduino) + cliente Python, fluxogramas, especificação do protocolo, segurança/chaves, memória de cálculo, evidências, análise de testes e resultados do desafio.

## Decisões da equipe

| Item | Decisão |
|------|---------|
| Sentido do enlace | **Bidirecional** (PI 3T) |
| Tópico telemetria | `satellite/esp32-01/telemetry` |
| Tópico comando | `satellite/esp32-01/command` |
| Broker | Mosquitto (IP no ESP: `mqtt_broker`) |
| Autenticação / cripto | HMAC-SHA256 + AES-CBC (planejado) |
| Progresso firmware | Bidirecional: telemetria TX + comandos RX interpretados |
| Layout TX/RX | `docs/protocolo/README.md` (provisório) |

## Fontes

- `docs/referencia/PI_3T_Sistema_Comunicacao_Satelite.pdf`
- `docs/referencia/Estacao_Terrena_Satelite_Fase1.pdf`
- `docs/referencia/Planejamento_sistema_ESP32_HMAC_AES.pdf`
