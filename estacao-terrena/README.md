# Estação terrena — broker MQTT Mosquitto

Esta pasta contém a configuração de referência do broker da estação terrena.

## Topologia

- ESP32: cliente MQTT `SAT-01`, IP fixo `192.168.30.52`.
- PC/Raspberry Pi: executa o broker Mosquitto e a aplicação Python de operação.
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

## Tópicos e permissões

| Usuário | Permissão | Tópico |
|---|---|---|
| `satellite` | Publicar | `satellite/esp32-01/telemetry` |
| `groundstation` | Assinar | `satellite/esp32-01/telemetry` |
| `groundstation` | Publicar | `satellite/esp32-01/command` |
| `satellite` | Assinar | `satellite/esp32-01/command` |

A ACL de exemplo já contém essas quatro permissões. Confirme que ela foi copiada para `/etc/mosquitto/acl`.

## Teste manual do broker

Receber telemetria:

```bash
mosquitto_sub -h IP_DO_BROKER -p 1883 -u groundstation -P 'SENHA_DO_USUARIO' -t 'satellite/esp32-01/telemetry' -v
```

Publicar um comando de teste (payload abaixo de 16 bytes):

```bash
mosquitto_pub -h IP_DO_BROKER -p 1883 -u groundstation -P 'SENHA_DO_USUARIO' -t 'satellite/esp32-01/command' -m 'PING'
```

O ESP32 deve exibir o comando no Monitor Serial. Para evitar expor a senha no histórico do terminal, prefira usar o arquivo local de configuração da aplicação Python ou um método de autenticação interativo.

## Rede e segurança

- Permita a porta TCP 1883 apenas na rede local isolada da equipe.
- Não encaminhe a porta 1883 para a internet.
- MQTT nesta configuração não usa TLS; credenciais e mensagens trafegam sem criptografia de transporte.
- Usuário/senha do broker controlam acesso ao broker, mas **não substituem** HMAC/AES do protocolo da aplicação.
- A segurança criptográfica das mensagens permanece como Fase 2 do projeto.
