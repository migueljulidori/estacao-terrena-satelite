# Contexto do projeto

Documento vivo com o entendimento do Projeto Integrado. Atualize este arquivo quando requisitos, decisões de protocolo ou escopo da equipe mudarem. Registre a mudança também em [`ATUALIZACOES.md`](ATUALIZACOES.md).

## Objetivo geral

Simular uma estação terrena que se comunica com satélites (nós ESP32) por enlace sem fio, dimensionando a potência necessária para um enlace geoestacionário/orbital real e analisando a segurança da comunicação e do acesso de gerência.

## Objetivos específicos

- Comunicação estação–satélite por Wi-Fi, com a estação terrena atuando como **broker MQTT**.
- Dimensionar a potência de transmissão (link budget) para fechar o enlace.
- Compreender acesso de gerência via **SSH** (usuário/senha, rede local).
- Implementar e avaliar segurança da comunicação em ambiente controlado.
- Relacionar com legislação brasileira (Lei 12.737/2012 e LGPD).

## Cenário

| Papel | Hardware / software | Função |
|-------|---------------------|--------|
| Estação terrena | PC Linux ou Raspberry Pi | Broker MQTT Mosquitto, cliente de operação |
| Satélite | ESP32 | Só **publica** telemetria (TX) |
| Cliente PC | Computador | Só **assina** telemetria (RX) |
| Gerência | SSH na LAN | Manutenção local (sem exposição à internet) |

Rede isolada da turma — sem internet e sem dispositivos externos.

## Fases

| Fase | Foco |
|------|------|
| **Fase 1** | Comunicação satélite ↔ estação remota |
| **Fase 2** | Segurança da implementação |

## Protocolo de comunicação (requisitos)

- Transporte: **MQTT**
- **Payload máximo: 16 bytes** por mensagem MQTT (conteúdo da aplicação; cabeçalhos MQTT não fazem parte dos 16 bytes)
- Formato livre (binário / numérico), definido pela equipe
- Documentar: identificação do satélite, tipo de mensagem, campos, tamanhos e regras de decodificação
- Elementos de segurança **contam** no limite de 16 bytes

Conteúdo típico de telemetria (exemplos do enunciado): energia, temperatura interna/externa, irradiação solar, etc.

## MQTT — enlace unidirecional (decisão da equipe)

Broker: **Mosquitto** em PC Linux ou Raspberry Pi, porta TCP `1883`. IP da estação ainda a definir (`mqtt_broker` no ESP / config do cliente PC).

**Fluxo:** ESP32 só **TX** (publish) → broker → PC só **RX** (subscribe). Sem telecomando / sem troca bidirecional.

| Direção | Papel | Tópico |
|---|---|---|
| ESP32 → PC | Publish (TX) | `satellite/esp32-01/telemetry` |

Etapa atual do firmware: conectar e manter MQTT. Publicação da telemetria vem na etapa 6 do pipeline.

TLS no broker não é obrigatório nesta etapa (LAN isolada). HMAC/AES entram depois no payload da aplicação.

## Segurança (requisitos)

### a) Autenticação de mensagens

Permitir que o cliente PC verifique origem autorizada e integridade. Planejamento de apoio: HMAC-SHA256 com mbedTLS no ESP32 (ver PDF de planejamento).

Especificar: mecanismo, geração/distribuição de chaves, campos protegidos, verificação no PC, comportamento ante mensagens inválidas.

### b) Confidencialidade

Impedir leitura por terceiros no tráfego. Planejamento de apoio: AES-CBC com mbedTLS.

Especificar: algoritmo/modo, gestão de chaves, descriptografia no PC. Se houver só criptografia sem autenticação, documentar a limitação.

Observações: proteção contra replay quando aplicável; não basta embaralhar dados.

## Cliente PC

- Conectar ao broker MQTT da estação
- Assinar tópico do satélite da equipe
- Interpretar protocolo, verificar autenticidade e/ou descriptografar
- Rejeitar mensagens inválidas
- Exibir dados e resultado das verificações

## Link budget

Além da comunicação, dimensionar potência da estação, latência e RTT conforme órbita/banda/antena atribuídas à equipe (tabela do enunciado — equipes 1–7: GEO/LEO, bandas C/L/Ka/Ku).

## Entregáveis previstos

- Código-fonte: firmware ESP32 + cliente da estação
- Memória de cálculo do link budget
- Especificação técnica do protocolo com exemplos

## Pipeline do firmware ESP32 (ordem oficial)

Definida pela equipe; implementar **nesta ordem**, sem pular:

1. **Wi-Fi** ✓ — IP fixo `192.168.30.52`
2. **MQTT** ✓ — conectar ao broker como publisher (Miguel; sem subscribe)
3. **Gerar dados** — telemetria bruta (pendente)
4. **Criptografia** — AES-CBC e/ou HMAC-SHA256 (mbedTLS; pendente)
5. **Montar pacote** — payload final ≤ 16 bytes (`docs/protocolo/`; pendente)
6. **Publicar** — `publish` da telemetria (pendente)

## Divisão de trabalho

Ver [`EQUIPE.md`](EQUIPE.md). Responsável pelo satélite (ESP32): **Miguel Julidori** — etapa 2/6 MQTT.

## Decisões da equipe

| Item | Decisão |
|------|---------|
| Número da equipe | _a definir_ |
| Órbita / banda / antena | _a definir_ |
| Conteúdo da telemetria | _a definir_ |
| Broker MQTT | Mosquitto em PC Linux/Raspberry Pi |
| IP do broker | _a definir_; no ESP: variável `mqtt_broker` no `.ino`; no PC: `mqtt_config.py` |
| Porta MQTT | TCP 1883 (LAN isolada; sem TLS nesta etapa) |
| Sentido do enlace | Unidirecional: ESP publish (TX), PC subscribe (RX) |
| Tópico de telemetria | `satellite/esp32-01/telemetry` |
| Tópico de telecomando | **não usado** (sem RX no ESP) |
| Mecanismo de autenticação de mensagem | HMAC-SHA256 (planejado; mbedTLS) |
| Mecanismo de confidencialidade | AES-CBC (planejado; mbedTLS) |
| Formato do payload (16 B) | _a definir_ (`docs/protocolo/`) |
| Progresso firmware | **2/6 — Wi-Fi + MQTT connect (publish-only)** |

## Fontes

- `docs/referencia/Estacao_Terrena_Satelite_Fase1.pdf`
- `docs/referencia/Planejamento_sistema_ESP32_HMAC_AES.pdf`
