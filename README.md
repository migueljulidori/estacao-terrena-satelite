# Estação Terrena de Comunicação via Satélite

Projeto Integrado – Telecomunicações (3º Trimestre 2026).

Simulação de uma estação terrena que se comunica com um satélite simulado (ESP32) por MQTT, com dimensionamento de enlace GEO e avaliação de segurança.

## Arquitetura atual

- **Computador 1 (Linux):** hospeda o broker MQTT Mosquitto.
- **Computador 2:** executa a aplicação Python da estação terrena, como cliente MQTT.
- **ESP32:** representa o satélite simulado e também atua como cliente MQTT.
- **Rede:** os três equipamentos precisam estar na mesma LAN ou possuir rota entre si.
- **Broker:** Mosquitto, porta TCP 1883.
- **IP do broker:** ainda precisa ser confirmado no computador Linux com `hostname -I`. Não confundir com o IP fixo do ESP32 (`192.168.30.52`).

Não há Raspberry Pi na arquitetura atual.

## Status

- Código preparado para comunicação MQTT bidirecional:
  - ESP32 publica telemetria e assina comandos.
  - Aplicação Python assina telemetria e publica comandos.
- Tópicos:
  - Telemetria: `satellite/esp32-01/telemetry`.
  - Comandos: `satellite/esp32-01/command`.
- Telemetria de teste: `TESTE-N`, enviada pelo ESP32 a cada 5 segundos.
- Comandos recebidos são exibidos no Monitor Serial; ainda não acionam hardware.
- **Pendente:** instalar/verificar o Mosquitto no computador Linux, confirmar seu IP LAN, configurar os clientes e validar os dois sentidos da comunicação.
- A comunicação ainda não deve ser considerada validada fisicamente até a realização desses testes.

## Documentação

1. `estacao-terrena/README.md` — instalação e configuração do broker.
2. `ATUALIZACOES.md` — histórico de mudanças.
3. `CONTEXTO.md` — arquitetura, requisitos e estado atual.
4. `EQUIPE.md` — divisão de responsabilidades.

## Segurança

Não envie credenciais Wi-Fi, MQTT, SSH ou chaves criptográficas ao Git. Use arquivos locais ignorados pelo Git e modelos `*.example`. MQTT na porta 1883 sem TLS deve ser usado somente na LAN isolada da equipe; não exponha o broker à internet.
