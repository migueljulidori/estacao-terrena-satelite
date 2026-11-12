# Contexto do projeto

Documento vivo com o entendimento do Projeto Integrado. Atualize este arquivo quando requisitos, decisões de protocolo ou escopo da equipe mudarem. Registre a mudança também em [`ATUALIZACOES.md`](ATUALIZACOES.md).

## Objetivo geral

Simular uma estação terrena que se comunica com satélites (nós ESP32) por enlace sem fio, dimensionando a potência necessária para um enlace geoestacionário/orbital real e analisando a segurança da comunicação e do acesso de gerência.

## Objetivos específicos

- Comunicação estação–satélite por Wi-Fi, com a estação terrena atuando como **broker MQTT**.
- Dimensionar a potência de transmissão (link budget).
- Compreender acesso de gerência via **SSH** na rede local.
- Implementar e avaliar segurança da comunicação em ambiente controlado.
- Relacionar com legislação brasileira (Lei 12.737/2012 e LGPD).

## Cenário

| Papel | Hardware / software | Função |
|---|---|---|
| Estação terrena | PC Linux ou Raspberry Pi | Broker MQTT Mosquitto e aplicação de operação |
| Satélite | ESP32 | Publica telemetria e recebe comandos |
| Aplicação PC | Python + Paho MQTT | Assina telemetria e publica comandos |
| Gerência | SSH na LAN | Manutenção local, sem exposição à internet |

Rede isolada da turma — sem internet e sem dispositivos externos.

## Fases

| Fase | Foco |
|---|---|
| **Fase 1** | Comunicação satélite ↔ estação remota |
| **Fase 2** | Segurança da implementação |

## Protocolo MQTT

Broker: **Mosquitto** em PC Linux ou Raspberry Pi, porta TCP `1883`. O IP da estação deve ser configurado no firmware e no arquivo local `mqtt_config.py`.

O enlace é **bidirecional**:

| Direção | Publisher | Subscriber | Tópico |
|---|---|---|---|
| ESP32 → estação | ESP32 | Aplicação PC | `satellite/esp32-01/telemetry` |
| Estação → ESP32 | Aplicação PC | ESP32 | `satellite/esp32-01/command` |

O Mosquitto é o broker central. A aplicação Python não é broker: ela mantém uma assinatura de telemetria e publica comandos no tópico de comando.

### Requisitos do payload

- Payload máximo: **16 bytes** por mensagem da aplicação. Cabeçalhos MQTT não entram nessa contagem.
- Formato livre (binário ou textual), a ser definido pela equipe.
- Elementos de identificação, tipo, contador, autenticação e criptografia contam dentro dos 16 bytes.
- Mensagens vazias ou maiores que 16 bytes devem ser rejeitadas pelos clientes.

Nesta etapa, o ESP32 envia `TESTE-N`; a aplicação exibe tamanho, HEX e texto. A aplicação permite enviar comandos textuais de até 16 bytes; o firmware os exibe no Monitor Serial, sem executar ações físicas ainda.

TLS no broker não é obrigatório nesta etapa de LAN isolada. HMAC/AES entram depois no payload da aplicação.

## Segurança (requisitos)

### a) Autenticação de mensagens

Permitir que o cliente PC verifique origem autorizada e integridade. Planejamento de apoio: HMAC-SHA256 com mbedTLS no ESP32.

Especificar mecanismo, geração/distribuição de chaves, campos protegidos, verificação no PC e comportamento ante mensagens inválidas.

### b) Confidencialidade

Impedir leitura por terceiros no tráfego. Planejamento de apoio: AES-CBC com mbedTLS.

Especificar algoritmo/modo, gestão de chaves e descriptografia no PC. Se houver só criptografia sem autenticação, documentar a limitação. Considerar proteção contra replay.

## Aplicação da estação terrena

- Conectar ao broker MQTT da estação.
- Assinar `satellite/esp32-01/telemetry`.
- Interpretar protocolo, verificar autenticidade e/ou descriptografar.
- Rejeitar mensagens inválidas.
- Exibir os dados e os resultados das verificações.
- Publicar comandos em `satellite/esp32-01/command`.

## Link budget

Além da comunicação, dimensionar potência da estação, latência e RTT conforme órbita/banda/antena atribuídas à equipe.

## Entregáveis previstos

- Firmware ESP32.
- Aplicação MQTT da estação terrena.
- Memória de cálculo do link budget.
- Especificação técnica do protocolo com exemplos.

## Pipeline do firmware ESP32

1. **Wi-Fi** ✓ — IP fixo `192.168.30.52`.
2. **MQTT** ✓ — conexão, publish de telemetria e subscribe de comandos.
3. **Gerar dados** — atualmente teste `TESTE-N`; telemetria real depois.
4. **Criptografia** — AES-CBC e/ou HMAC-SHA256 (mbedTLS; pendente).
5. **Montar pacote** — payload final ≤ 16 bytes (`docs/protocolo/`; pendente).
6. **Publicar** — teste já implementado; pacote final depois.

## Divisão de trabalho

Ver [`EQUIPE.md`](EQUIPE.md). Firmware satélite (ESP32): Miguel Julidori.

## Decisões atuais

| Item | Decisão |
|---|---|
| Broker MQTT | Mosquitto em PC Linux/Raspberry Pi |
| IP do broker | A definir; configurar no firmware e em `mqtt_config.py` |
| Porta MQTT | TCP 1883, LAN isolada, sem TLS nesta etapa |
| Sentido do enlace | Bidirecional |
| Tópico de telemetria | `satellite/esp32-01/telemetry` |
| Tópico de comando | `satellite/esp32-01/command` |
| Limite do payload | 16 bytes por mensagem |
| Autenticação de mensagem | HMAC-SHA256 planejado |
| Confidencialidade | AES-CBC planejado |
| Formato do payload | A definir em `docs/protocolo/` |

## Fontes

- `docs/referencia/Estacao_Terrena_Satelite_Fase1.pdf`
- `docs/referencia/Planejamento_sistema_ESP32_HMAC_AES.pdf`
