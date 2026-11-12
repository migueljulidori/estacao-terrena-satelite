# Estação terrena — broker MQTT Mosquitto

Esta pasta contém a configuração de referência do broker da estação terrena.

## Topologia

- ESP32: cliente MQTT `satellite-esp32-01`, IP fixo `192.168.30.52`.
- PC/Raspberry Pi: executa o broker Mosquitto.
- Porta MQTT: `1883`.
- IP do broker: **a definir pela equipe**. Use o IP LAN real da máquina que executa o Mosquitto; não confunda com o IP do ESP32.

## Instalação (Ubuntu/Raspberry Pi OS)

```bash
sudo apt update
sudo apt install mosquitto mosquitto-clients
sudo systemctl enable --now mosquitto
```

## Configuração

1. Copie `mosquitto.conf.example` para `/etc/mosquitto/conf.d/estacao.conf`.
2. Crie os usuários do broker (o terminal solicitará as senhas):

```bash
sudo mosquitto_passwd -c /etc/mosquitto/passwd satellite
sudo mosquitto_passwd /etc/mosquitto/passwd groundstation
```

3. Copie `acl.example` para `/etc/mosquitto/acl`.
4. Reinicie e confira o serviço:

```bash
sudo systemctl restart mosquitto
sudo systemctl status mosquitto
```

Não coloque senhas reais nos arquivos versionados. O arquivo `passwd` deve permanecer no computador da estação e ter permissões de leitura adequadas para o serviço Mosquitto.

## Tópicos iniciais

| Direção | Tópico | Permissão |
|---|---|---|
| ESP32 → PC (TX) | `satellite/esp32-01/telemetry` | `satellite` publica; `groundstation` assina |

Enlace **unidirecional**: sem tópico de telecomando.

## Teste manual do broker

Assine a telemetria (RX no PC):

```bash
mosquitto_sub -h IP_DO_BROKER -p 1883 -u groundstation -P 'SENHA' -t 'satellite/esp32-01/telemetry' -v
```

Quando o ESP publicar (etapa 6), a mensagem aparece aqui.

Evite usar `-P` com senha real em terminais compartilhados: o valor pode ficar no histórico do shell. Prefira ferramentas que solicitem a senha interativamente ou configure o cliente localmente.

## Rede e segurança

- Permita a porta TCP 1883 apenas na rede local isolada da equipe.
- Não encaminhe a porta 1883 para a internet.
- MQTT nesta configuração não usa TLS; credenciais e mensagens trafegam sem criptografia de transporte.
- Usuário/senha do broker controlam acesso ao broker, mas **não substituem** HMAC/AES do protocolo da aplicação.
- A segurança criptográfica das mensagens permanece como Fase 2 do projeto.
