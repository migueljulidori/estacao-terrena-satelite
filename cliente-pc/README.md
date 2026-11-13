# Cliente PC (estação terrena) — MQTT bidirecional

**Responsável: Manuela Lino**

O cliente Python conecta ao broker Mosquitto, recebe telemetria/ACK do ESP32 e publica telecomandos. Ele também serve o painel web local, que acompanha as mensagens em tempo real.

## Fluxo MQTT

| Direção | Tópico | Operação do PC |
|---|---|---|
| ESP32 → PC | `satellite/esp32-01/telemetry` | Subscriber |
| PC → ESP32 | `satellite/esp32-01/command` | Publisher |

## Instalação

Na pasta `cliente-pc/`:

```bash
python3 -m venv .venv
source .venv/bin/activate
pip install -r requirements.txt
cp mqtt_config.example.py mqtt_config.py
```

Edite `mqtt_config.py` com o IP real da máquina que executa o Mosquitto, usuário e senha do cliente `groundstation`. Se já existir um `mqtt_config.py` antigo, acrescente `TOPIC_COMMAND`, `WEB_HOST` e `WEB_PORT` conforme o exemplo (ou atualize-o a partir do exemplo). Nunca versione credenciais reais.

## Executar cliente e painel

```bash
python3 mqtt_client.py
```

Abra no navegador da própria máquina:

```
http://127.0.0.1:8765
```

Mantenha o terminal aberto. O servidor local entrega o painel e recebe os eventos MQTT; não abra o HTML diretamente como arquivo.

## Testes de telecomando

Pelo painel, use os botões **PING**, **SOLICITAR STATUS** e o formulário de nome. Os comandos de texto aceitos pelo firmware atual são:

- `PING`: testa o enlace; o ESP deve devolver ACK.
- `STATUS`: solicita telemetria imediata e ACK.
- `NOME=Alpha`: altera o nome do satélite; o firmware atual aceita até 11 caracteres nesse formato de texto (o comando inteiro precisa caber em 16 bytes).

Também é possível publicar pelo terminal, em outra janela:

```bash
mosquitto_pub -h IP_DO_BROKER -p 1883 -u groundstation -P 'SENHA' \
  -t 'satellite/esp32-01/command' -m 'PING'
```

## O que o cliente interpreta

O firmware atual envia pacotes de 16 bytes:

- Tipo `0x01`: telemetria — ID, sequência, bateria, temperatura interna/externa, irradiação solar e início do nome.
- Tipo `0x02`: ACK — ID, sequência, código do comando confirmado e início do nome.

O painel atualiza os indicadores quando recebe telemetria e identifica ACKs no registro de mensagens. Pacotes com tamanho ou tipo inesperado são sinalizados como inválidos.

## Limites desta etapa

- Os valores gerados pelo firmware ainda são simulados.
- HMAC/AES ainda não estão implementados; este cliente não declara autenticidade nem confidencialidade criptográfica.
- A comunicação usa MQTT TCP na porta 1883, conforme a configuração atual da LAN de testes.
- O painel escuta somente em `127.0.0.1`.

## Diagnóstico rápido

- **Conexão recusada/timeout:** confira se o Mosquitto está ativo, o IP/porta e se o PC está na mesma rede.
- **Not authorized:** confira usuário/senha e ACL do broker. O usuário `groundstation` precisa de leitura em `telemetry` e escrita em `command`.
- **Comando publicado, sem ACK:** confira se o ESP32 está conectado ao mesmo broker, inscrito em `command` e com o firmware bidirecional atualizado.
- **Painel não atualiza:** acesse pelo endereço `http://127.0.0.1:8765` com o processo Python em execução.

Encerre com Ctrl+C no terminal.
