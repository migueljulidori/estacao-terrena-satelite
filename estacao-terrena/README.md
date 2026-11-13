# Estação terrena — broker MQTT Mosquitto

Conforme o PI 3T atualizado, a Raspberry Pi representa a estação terrena e hospeda o servidor MQTT Mosquitto. O computador executa a aplicação Python cliente MQTT. O ESP32 representa o satélite simulado.

## Topologia
- Raspberry Pi: broker Mosquitto, porta TCP 1883.
- ESP32: cliente MQTT; IP fixo atual 192.168.30.52.
- Computador: aplicação Python; recebe telemetria e publica comandos.
- IP do broker: usar o endereço LAN real da Raspberry Pi. O IP 192.168.30.52 pertence ao ESP32, não deve ser confundido com o broker.

Não é necessário trocar o Mosquitto por outro broker. É necessário confirmar que o serviço está instalado e ativo na Raspberry Pi.

## Instalação na Raspberry Pi
```bash
sudo apt update
sudo apt install mosquitto mosquitto-clients
sudo systemctl enable --now mosquitto
```

## Usuários e ACL
```bash
sudo mosquitto_passwd -c /etc/mosquitto/passwd satellite
sudo mosquitto_passwd /etc/mosquitto/passwd groundstation
```

Copie mosquitto.conf.example para /etc/mosquitto/conf.d/estacao.conf e acl.example para /etc/mosquitto/acl. Depois reinicie o serviço com sudo systemctl restart mosquitto. Não versione senhas reais.

## Fluxos MQTT exigidos
| Sentido | Cliente que publica | Cliente que assina | Tópico |
|---|---|---|---|
| Satélite → estação | ESP32 | Aplicação Python | satellite/esp32-01/telemetry |
| Estação → satélite | Aplicação Python | ESP32 | satellite/esp32-01/command |

A ACL de exemplo deve permitir as quatro operações: usuário satellite publica telemetry e assina command; usuário groundstation assina telemetry e publica command.

## Configuração dos clientes
- No firmware ESP32, mqtt_broker deve receber o IP LAN da Raspberry Pi.
- Em cliente-pc/mqtt_config.py, BROKER_HOST deve receber o mesmo IP.
- Não use o IP do ESP32 como IP do broker.

## Testes manuais
Receber telemetria no computador:
```bash
mosquitto_sub -h IP_DA_RASPBERRY_PI -p 1883 -u groundstation -P 'SENHA' -t 'satellite/esp32-01/telemetry' -v
```

Enviar comando ao ESP32:
```bash
mosquitto_pub -h IP_DA_RASPBERRY_PI -p 1883 -u groundstation -P 'SENHA' -t 'satellite/esp32-01/command' -m 'PING'
```

O firmware atual apenas mostra o comando no Monitor Serial; ainda não executa ações físicas.

## Segurança
- Restrinja a porta 1883 à rede isolada da equipe; não exponha o broker à internet.
- MQTT sem TLS não cifra credenciais nem mensagens em trânsito.
- Usuário e senha MQTT não substituem autenticação e confidencialidade no protocolo de aplicação (HMAC/AES).