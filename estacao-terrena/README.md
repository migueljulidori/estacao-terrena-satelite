# Broker MQTT Mosquitto — computador Linux

## Topologia atual

- **Computador 1 (Linux):** executa o broker Mosquitto.
- **Computador 2:** executa a aplicação Python da estação terrena; é cliente MQTT, não broker.
- **ESP32:** representa o satélite simulado e é cliente MQTT.
- Os três equipamentos devem estar na mesma LAN ou possuir rota entre si.
- Broker: Mosquitto, TCP 1883.
- O IP do broker é o IPv4 LAN do Computador 1. Confirme com `hostname -I`.
- O IP `192.168.30.52` pertence ao ESP32 e não deve ser usado como endereço do broker.

Não há Raspberry Pi na arquitetura atual.

## 1. Instalação (Debian/Ubuntu e derivados)

No computador Linux que será o broker:

```bash
sudo apt update
sudo apt install mosquitto mosquitto-clients -y
sudo systemctl enable mosquitto
sudo systemctl start mosquitto
sudo systemctl status mosquitto
sudo systemctl is-active mosquitto
hostname -I
```

Se a distribuição não usar `apt`, os comandos de instalação precisam ser adaptados.

**Validação inicial:** o serviço deve aparecer como ativo (`active`) e o comando `hostname -I` deve mostrar o endereço IPv4 da interface conectada à LAN da equipe. Registre esse IP; ele será usado nos dois clientes.

> Estado em 2026-11-13: instalação e inicialização ainda aguardam confirmação no computador Linux. Não presumir que o broker já está ativo.

## 2. Usuários e permissões MQTT

Depois de confirmar que o serviço está funcionando, crie os usuários:

```bash
sudo mosquitto_passwd -c /etc/mosquitto/passwd satellite
sudo mosquitto_passwd /etc/mosquitto/passwd groundstation
```

O primeiro comando cria o arquivo de senhas; o segundo acrescenta o outro usuário. Não use `-c` no segundo comando, pois isso recriaria o arquivo.

Copie `mosquitto.conf.example` para `/etc/mosquitto/conf.d/estacao.conf` e `acl.example` para `/etc/mosquitto/acl`. Ajuste permissões dos arquivos para que o serviço Mosquitto possa lê-los e reinicie o serviço. Não versione senhas reais.

A configuração de exemplo usa autenticação obrigatória (`allow_anonymous false`) e ACL para limitar os tópicos de cada cliente.

## 3. Fluxos MQTT

| Sentido | Publisher | Subscriber | Tópico |
|---|---|---|---|
| Satélite → estação | ESP32 (`satellite`) | Python (`groundstation`) | `satellite/esp32-01/telemetry` |
| Estação → satélite | Python (`groundstation`) | ESP32 (`satellite`) | `satellite/esp32-01/command` |

Permissões na ACL:
- `satellite`: publica em `telemetry` e assina `command`.
- `groundstation`: assina `telemetry` e publica em `command`.

## 4. Configuração dos clientes

- Firmware: atualizar `mqtt_broker` em `firmware-esp32/satelite/mqtt_mod.h` com o IP LAN confirmado do computador Linux.
- Python: copiar `cliente-pc/mqtt_config.example.py` para `cliente-pc/mqtt_config.py` e definir o mesmo IP em `BROKER_HOST`.
- Preencher as credenciais locais correspondentes aos usuários criados no Mosquitto.
- Não confundir o endereço do broker com o IP do ESP32.

## 5. Testes

Primeiro confirme a conectividade de rede entre o computador Python e o broker. Depois teste cada direção com os utilitários `mosquitto_sub` e `mosquitto_pub` ou execute o cliente Python.

Resultado esperado:
1. ESP32 publica `TESTE-N` a cada 5 segundos.
2. Aplicação Python recebe e exibe a telemetria.
3. Aplicação Python publica um comando de até 16 bytes.
4. ESP32 recebe o comando e o exibe no Monitor Serial.

O firmware atual apenas imprime os comandos recebidos; não executa ações físicas. A validação dos dois sentidos ainda está pendente.

## Segurança

- Restrinja a porta 1883 à LAN da equipe por firewall; não exponha o broker à internet.
- MQTT sem TLS não cifra credenciais nem mensagens em trânsito.
- Autenticação MQTT e ACL controlam acesso ao broker, mas não substituem os mecanismos de autenticação/integridade e confidencialidade da camada de aplicação previstos para a próxima fase.
