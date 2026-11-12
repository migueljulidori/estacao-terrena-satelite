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
| Estação terrena | PC Linux ou Raspberry Pi | Broker MQTT (ex.: Mosquitto), cliente de operação |
| Satélite | ESP32 | Publica telemetria e recebe telecomandos |
| Gerência | SSH na LAN | Manutenção local (sem exposição à internet) |

Rede isolada da turma — sem internet e sem dispositivos externos.

## Fases

| Fase | Foco |
|------|------|
| **Fase 1** | Comunicação satélite ↔ estação remota |
| **Fase 2** | Segurança da implementação |

## Protocolo de comunicação (requisitos)

- Transporte: **MQTT**
- **Payload máximo: 16 bytes** por mensagem MQTT
- Formato livre (binário / numérico), definido pela equipe
- Documentar: identificação do satélite, tipo de mensagem, campos, tamanhos e regras de decodificação
- Elementos de segurança **contam** no limite de 16 bytes

Conteúdo típico de telemetria (exemplos do enunciado): energia, temperatura interna/externa, irradiação solar, etc.

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

## Decisões da equipe (preencher)

| Item | Decisão |
|------|---------|
| Número da equipe | _a definir_ |
| Órbita / banda / antena | _a definir_ |
| Conteúdo da telemetria | _a definir_ |
| Mecanismo de autenticação | _a definir_ |
| Mecanismo de confidencialidade | _a definir_ |
| Formato do payload (16 B) | _a definir_ |
| Tópicos MQTT | _a definir_ |

## Fontes

- `docs/referencia/Estacao_Terrena_Satelite_Fase1.pdf`
- `docs/referencia/Planejamento_sistema_ESP32_HMAC_AES.pdf`
