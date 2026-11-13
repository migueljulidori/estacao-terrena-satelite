# Contexto do projeto

Documento vivo do Projeto Integrado. Atualize quando requisitos e decisões mudarem; registre as alterações em [`ATUALIZACOES.md`](ATUALIZACOES.md).

Fonte atual: [`docs/referencia/PI_3T_Sistema_Comunicacao_Satelite.pdf`](docs/referencia/PI_3T_Sistema_Comunicacao_Satelite.pdf).

## Objetivo geral

Simular um satélite geoestacionário por meio de módulos ESP32 que se comunicam com uma estação terrestre representada por uma Raspberry Pi, dimensionar a potência necessária para um enlace GEO real e analisar a segurança da comunicação e do acesso de gerência (SSH).

## Cenário e arquitetura

| Papel | Hardware / software | Função |
|---|---|---|
| Estação terrena | Raspberry Pi | Broker MQTT Mosquitto |
| Satélites simulados | ESP32 | Publicar telemetria e receber comandos/configuração |
| Aplicação de apoio | Python / cliente MQTT | Receber e interpretar telemetria, enviar comandos e validar segurança |
| Plano de gerência | SSH na LAN ou VPN | Manutenção da estação; não expor SSH diretamente à internet |

A operação prática ocorre na rede isolada fornecida pelos professores.

## Planos de acesso

- **Operação (espaço–terra):** enlace MQTT bidirecional, com segurança aplicada às mensagens.
- **Gerência (manutenção):** SSH para acesso à Raspberry Pi, autenticado por usuário e senha, somente pela LAN ou VPN.

## MQTT — enlace bidirecional

O Mosquitto na estação é o broker. O programa Python é um cliente MQTT, não o broker.

| Direção | Publisher | Subscriber | Tópico |
|---|---|---|---|
| Satélite → estação | ESP32 | Aplicação Python | `satellite/esp32-01/telemetry` |
| Estação → satélite | Aplicação Python | ESP32 | `satellite/esp32-01/command` |

Estado do código: o ESP32 publica telemetria de teste `TESTE-N`, assina o tópico de comando; o cliente Python recebe telemetria e publica comandos de teste. O comando recebido pelo firmware é apenas exibido no Monitor Serial, sem atuação física nesta etapa.

## Protocolo da equipe

O formato é livre, mas deve documentar:
- identificação do satélite de origem;
- identificação do tipo de mensagem;
- dados úteis de telemetria e/ou comando;
- campos complementares necessários à segurança (por exemplo, IV e HMAC);
- tamanho, representação, significado e regras de decodificação de cada campo.

**Atenção ao limite de 16 bytes:** ele existe no código de teste legado, mas não aparece como requisito explícito no PDF atualizado do PI 3T. Mantê-lo temporariamente no firmware/cliente até confirmar com professor e equipe; não assumir que é uma exigência oficial do enunciado atual.

## Segurança obrigatória

O PI 3T exige, no mínimo:

1. **Autenticação das mensagens:** verificar que a mensagem veio de entidade autorizada e que o conteúdo não foi alterado.
2. **Confidencialidade:** impedir que terceiros que capturem o tráfego interpretem o conteúdo protegido.

A equipe deve definir algoritmo/modo, geração e distribuição das chaves, campos protegidos, processo de verificação/descriptografia e comportamento diante de mensagens inválidas. HMAC-SHA256 e AES são opções que já foram discutidas, mas o algoritmo, o modo de operação e o formato final ainda precisam ser formalmente definidos e implementados. Não considerar a segurança concluída.

## Pipeline atual do firmware

1. Wi-Fi — implementado; ESP32 usa IP fixo `192.168.30.52`.
2. MQTT — conexão, publish de telemetria de teste e subscribe de comando implementados no código.
3. Geração de dados — atualmente apenas `TESTE-N`; telemetria útil ainda pendente.
4. Autenticação e confidencialidade — pendentes.
5. Montagem/decodificação do pacote — pendente.
6. Publicação/recepção do pacote final protegido — pendente.

## Link budget — enlace GEO

Usar as fórmulas do enunciado atualizado:

- Raio da Terra: (R_e = 6.378\,km).
- Raio orbital GEO: (R_s = R_e + 35.786 = 42.164\,km).
- Distância inclinada: (d = \sqrt{R_s^2 - R_e^2\cos^2 E} - R_e\,sen E).
- Perda de espaço livre: (L(dB)=92,45+20\log_{10}(f_{GHz})+20\log_{10}(d_{km})).
- Ganho da antena da estação: (G(dBi)\approx17,8+20\log_{10}(D_m)+20\log_{10}(f_{GHz})), com eficiência aproximada de 60%.
- (C/N_0=EIRP+(G/T)_{sat}-L(FSPL)-L_{atm}-k), com (EIRP=P_t+G_t-L_{alim}) e (k=-228,6\,dBW/(K\cdot Hz)).
- Requisito: (C/N_0\geq(E_b/N_0)_{req}+10\log_{10}(R_b)+margem).

A equipe ainda precisa escolher frequência/banda e taxa de dados. O enunciado sugere antena de 2,4 m, (G/T=0\,dB/K), perdas atmosféricas de 2 dB e margem de 3 dB. O cálculo final deve resolver (P_t).

## Desafio hacking e limites éticos

Depois de implementar e validar a comunicação:
- avaliar o acesso SSH à estação;
- capturar/inspecionar o tráfego MQTT;
- avaliar autenticação, confidencialidade, gerenciamento de chaves e validação;
- testar alteração, replay e injeção de mensagens somente no ambiente autorizado.

Todos os testes devem ocorrer exclusivamente na rede isolada e nos dispositivos da atividade. Não testar redes ou sistemas externos. Considerar Lei 12.737/2012 e LGPD.

## Entregáveis

- Código ESP32 e cliente MQTT Python.
- Fluxogramas da lógica do satélite e do cliente.
- Especificação byte a byte do protocolo.
- Explicação dos mecanismos de segurança e gestão de chaves.
- Memória de cálculo do link budget, incluindo geometria, FSPL, ganho, C/N₀ e potência (P_t).
- Evidências de testes com mensagens válidas e inválidas.
- Análise dos resultados e do desafio hacking, com vulnerabilidades, impactos e mitigações.

## Divisão de responsabilidades

Consultar [`EQUIPE.md`](EQUIPE.md). Não assumir como concluída uma parte sem implementação e evidências.

## Decisões ainda pendentes

- Frequência/banda de operação e taxa de dados.
- Parâmetros finais da antena e perdas de alimentação.
- Formato e conteúdo útil das mensagens.
- Algoritmo/modo criptográfico e estratégia de chaves.
- Confirmação do limite de 16 bytes, que não está explícito no PDF atualizado.
