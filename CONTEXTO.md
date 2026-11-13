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
|---|---|---|
| Estação terrena | PC Linux dedicado ao broker | Broker MQTT Mosquitto e aplicação de operação |
| Satélite | ESP32 | Publica telemetria e recebe comandos |
| Aplicação PC | Computador com Python/Paho MQTT | Assina telemetria e publica comandos |
| Gerência | SSH na LAN | Manutenção local, sem exposição à internet |

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

## MQTT — enlace bidirecional

Broker: **Mosquitto** em PC Linux dedicado ao broker, porta TCP `1883`. O IP da estação ainda deve ser definido (`mqtt_broker` no firmware e `mqtt_config.py` no PC).

O ESP32 publica telemetria e assina comandos; a aplicação PC assina telemetria e publica comandos.

| Direção | Publisher | Subscriber | Tópico |
|---|---|---|---|
| ESP32 → PC | ESP32 | PC | `satellite/esp32-01/telemetry` |
| PC → ESP32 | PC | ESP32 | `satellite/esp32-01/command` |

O payload de cada mensagem da aplicação deve ter no máximo 16 bytes. Nesta etapa, o ESP32 exibe comandos válidos no Monitor Serial, sem executar ações físicas.

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

- Conectar ao broker MQTT da estação.
- Assinar o tópico de telemetria do satélite.
- Publicar comandos no tópico de telecomando.
- Interpretar protocolo, verificar autenticidade e/ou descriptografar.
- Rejeitar mensagens inválidas.
- Exibir dados e resultado das verificações.

## Link budget

Além da comunicação, dimensionar potência da estação, latência e RTT conforme órbita/banda/antena atribuídas à equipe (tabela do enunciado — equipes 1–7: GEO/LEO, bandas C/L/Ka/Ku).

## Entregáveis previstos

- Código-fonte: firmware ESP32 + cliente da estação
- Memória de cálculo do link budget
- Especificação técnica do protocolo com exemplos

## Pipeline do firmware ESP32 (ordem oficial)

Definida pela equipe; implementar **nesta ordem**, sem pular:

1. **Wi-Fi** ✓ — IP fixo `192.168.30.52`
2. **MQTT** ✓ — conectar ao broker, publicar telemetria e assinar comandos
3. **Gerar dados** ← teste `TESTE-N` via `dados_mod.h` (telemetria real depois)
4. **Criptografia** — AES-CBC e/ou HMAC-SHA256 (mbedTLS; pendente)
5. **Montar pacote** — payload final ≤ 16 bytes (`docs/protocolo/`; pendente)
6. **Publicar** — já usado no teste (`mqttPublicar`); versão final com pacote criptografado depois

## Divisão de trabalho

Ver [`EQUIPE.md`](EQUIPE.md). Responsável pelo satélite (ESP32): **Miguel Julidori** — etapa 2/6 MQTT.

## Decisões da equipe

| Item | Decisão |
|------|---------|
| Número da equipe | _a definir_ |
| Órbita / banda / antena | _a definir_ |
| Conteúdo da telemetria | _a definir_ |
| Broker MQTT | Mosquitto em PC Linux separado |
| IP do broker | _a definir_; no ESP: variável `mqtt_broker` no `.ino`; no PC: `mqtt_config.py` |
| Porta MQTT | TCP 1883 (LAN isolada; sem TLS nesta etapa) |
| Sentido do enlace | Bidirecional: ESP publica telemetria e assina comandos; PC faz o inverso |
| Tópico de telemetria | `satellite/esp32-01/telemetry` |
| Tópico de telecomando | `satellite/esp32-01/command` |
| Mecanismo de autenticação de mensagem | HMAC-SHA256 (planejado; mbedTLS) |
| Mecanismo de confidencialidade | AES-CBC (planejado; mbedTLS) |
| Formato do payload (16 B) | _a definir_ (`docs/protocolo/`) |
| Progresso firmware | **MQTT bidirecional + telemetria de teste** |

## Fontes

- `docs/referencia/Estacao_Terrena_Satelite_Fase1.pdf`
- `docs/referencia/Planejamento_sistema_ESP32_HMAC_AES.pdf`

## Atualização de arquitetura — 2026-11-13

A topologia confirmada pela equipe utiliza um computador Linux dedicado para hospedar o broker MQTT Mosquitto. Um segundo computador executa somente a aplicação Python cliente MQTT. O ESP32 é o terceiro cliente da rede. Não há Raspberry Pi nesta configuração.

O enlace deve ser bidirecional:
- ESP32 publica telemetria no tópico satellite/esp32-01/telemetry; a aplicação Python assina esse tópico.
- Aplicação Python publica comandos no tópico satellite/esp32-01/command; o ESP32 assina esse tópico.

O endereço 192.168.30.52 é o IP fixo do ESP32. O IP do broker deve ser o endereço LAN real do computador Linux e precisa ser confirmado antes do teste. Esse mesmo IP deve ser configurado em mqtt_broker no firmware e em BROKER_HOST no arquivo local mqtt_config.py.

Estado: o firmware da branch feature/mqtt-bidirecional foi preparado para publicar telemetria e receber comandos; o cliente Python foi preparado para receber telemetria e publicar comandos. A instalação/verificação do Mosquitto no computador Linux e a validação física dos dois sentidos ainda estão pendentes. Atualmente, o firmware apenas imprime comandos recebidos no Monitor Serial.

## Atualização de arquitetura — 2026-11-13

A arquitetura foi esclarecida: o broker Mosquitto roda em um computador Linux dedicado, separado do computador que executa a aplicação Python. Não existe Raspberry Pi nesta configuração.

| Equipamento | Papel |
|---|---|
| Computador Linux 1 | Broker Mosquitto |
| Computador 2 | Aplicação Python cliente MQTT |
| ESP32 | Satélite simulado e cliente MQTT |

Os três equipamentos devem estar na mesma LAN ou possuir rota entre si. O endereço do broker ainda precisa ser confirmado no computador Linux usando `hostname -I`; não usar o IP fixo do ESP32 (`192.168.30.52`) como IP do broker.

### Estado do ambiente em 2026-11-13

O código dos dois clientes está preparado para publish/subscribe bidirecional. A configuração/instalação do Mosquitto no computador Linux e o teste real de conectividade ainda estão pendentes de confirmação.

Procedimento inicial documentado em `estacao-terrena/README.md`: instalar `mosquitto` e `mosquitto-clients` (Debian/Ubuntu), habilitar/iniciar o serviço, conferir `systemctl status mosquitto` e obter o IP LAN com `hostname -I`. Depois, configurar o mesmo IP no firmware e no arquivo local `cliente-pc/mqtt_config.py`.
