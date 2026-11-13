# Estação Terrena de Comunicação via Satélite

Projeto Integrado – Telecomunicações (3º Trimestre 2026).

Simulação de uma estação terrena que se comunica com satélites (nós ESP32) por MQTT, com dimensionamento de enlace GEO e avaliação de segurança.

## Arquitetura
- **Raspberry Pi:** estação terrena que hospeda o broker MQTT Mosquitto.
- **ESP32:** satélite simulado; publica telemetria e recebe comandos.
- **Computador com Python:** aplicação cliente MQTT; recebe telemetria e publica comandos.
- **Broker:** Mosquitto. O PI 3T atualizado não exige substituição do broker.
- **IP do broker:** endereço LAN real da Raspberry Pi, ainda a confirmar. O IP 192.168.30.52 é do ESP32.

## Status
- Enlace MQTT bidirecional preparado no código: ESP32 publica telemetry e assina command; cliente Python assina telemetry e publica command.
- Telemetria atual de teste: TESTE-N.
- Comandos recebidos pelo ESP32 são exibidos no Monitor Serial; ainda não acionam hardware.
- Próxima validação: configurar o IP real da Raspberry Pi no firmware e em cliente-pc/mqtt_config.py e testar os dois sentidos na rede isolada.
- Próximas etapas do projeto: definir o protocolo de mensagens, implementar autenticação/integridade e confidencialidade, testar mensagens válidas/inválidas e concluir o link budget GEO.

## Para a equipe
1. CONTRIBUTING.md — fluxo de contribuição.
2. EQUIPE.md — divisão de responsabilidades.
3. ATUALIZACOES.md — histórico de mudanças.
4. CONTEXTO.md — arquitetura, requisitos e estado atual.

## Segurança
Não envie credenciais Wi-Fi, MQTT, SSH ou chaves criptográficas ao Git. Use arquivos locais ignorados pelo Git e modelos de configuração *.example. Faça os testes somente na rede isolada e nos dispositivos autorizados.