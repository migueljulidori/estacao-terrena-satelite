# Cliente PC (estação terrena) — MQTT, AES e HMAC

**Responsável: Manuela Lino**

O cliente conecta ao Mosquitto, recebe telemetria/ACK, autentica e decifra os pacotes do ESP32, publica comandos cifrados e atualiza o painel web local.

## Contrato criptográfico espelhado do firmware

O formato implementado em `crypto_protocol.py` segue `docs/protocolo/README.md` da branch `main` e `firmware-esp32/satelite/cripto_mod.h`:

- AES-128-CBC, bloco/ciphertext de 16 bytes.
- HMAC-SHA256 truncado aos 8 primeiros bytes.
- Bloco claro: `tipo (1) | sequência (1) | dados (6) | HMAC (8)`.
- HMAC calculado sobre os 8 primeiros bytes do bloco claro.
- IV derivado: `SHA256("SAT-IV" || chave_AES)[0:16]`.
- Sem padding: o bloco claro já tem exatamente 16 bytes.
- O cliente só interpreta e encaminha à dashboard um pacote depois de decifrar e validar o HMAC.

Tipos: `0x01` telemetria; `0x02` ACK; `0x10` PING; `0x12` STATUS; `0x13` SET_NOME.

### Campos de dados da telemetria (6 bytes)

| Byte | Campo | Codificação |
|---|---|---|
| 0 | Bateria | percentual |
| 1–2 | Temperatura interna | int16 little-endian, décimos de °C |
| 3–4 | Temperatura externa | int16 little-endian, décimos de °C |
| 5 | Irradiação solar | valor dividido por 4; o PC multiplica por 4 |

ACK: byte 0 identifica o comando; bytes 1–5 contêm o início do nome do satélite.

### Comandos enviados pelo painel

- `PING` e `STATUS`: dados zerados.
- `NOME=Alpha`: tipo `0x13`, nome ASCII de até 6 bytes, preenchido com NUL até completar os seis bytes.
- Cada comando é selado com HMAC e AES antes de ser publicado em `satellite/esp32-01/command`.

## Instalação

Na pasta `cliente-pc/`:

```bash
python3 -m venv .venv
source .venv/bin/activate
pip install -r requirements.txt
cp mqtt_config.example.py mqtt_config.py
```

Edite `mqtt_config.py`:
1. Configure IP, usuário e senha do broker.
2. Configure `AES_KEY` e `HMAC_KEY` com os mesmos 16 bytes ASCII usados pelo firmware em `cripto_mod.h`.
3. Não compartilhe nem versione `mqtt_config.py`.

As chaves atualmente presentes no código da branch `main` já estão publicadas no repositório. Elas não devem ser consideradas secretas. Para uso real, a equipe precisa gerar novas chaves e atualizar o firmware e o arquivo local do PC em conjunto, sem publicar as chaves.

## Execução

```bash
python3 -m unittest -v test_mqtt_client.py
python3 mqtt_client.py
```

Mantenha o terminal aberto e acesse `http://127.0.0.1:8765`. A dashboard mostra os dados somente após validação do HMAC. Mensagens com tamanho incorreto, chave incorreta ou HMAC inválido são rejeitadas.

## Limitações de segurança que permanecem

Esta implementação reproduz o contrato atual do firmware para permitir interoperabilidade, mas **não representa uma garantia criptográfica forte**:

- O IV é determinístico e reutilizado para todas as mensagens com a mesma chave. AES-CBC exige IV imprevisível e único por mensagem.
- O firmware guarda as chaves no código versionado da branch `main`; portanto, as chaves atuais estão expostas a quem acessa o repositório.
- O protocolo usa sequência de 1 byte, mas ainda não implementa janela anti-replay nem persistência de sequência.
- O HMAC truncado a 8 bytes é o formato escolhido pelo firmware atual; não altere um lado isoladamente.

A correção robusta desses pontos exige uma nova versão conjunta do protocolo e do firmware. Enquanto isso, use somente a rede isolada de testes.

## Diagnóstico

- **HMAC inválido:** confira se AES_KEY e HMAC_KEY são idênticas às do firmware e se o ESP usa a implementação criptográfica da branch `main`.
- **Nenhuma mensagem decifrada:** confirme que o ESP publica ciphertext de 16 bytes (Serial deve indicar TX com HMAC+AES).
- **Comando sem ACK:** confira se o firmware executa `comandoProcessar` e se está com `CRIPTO_ACEITAR_CLARO` habilitado/desabilitado conforme a etapa de integração. O cliente Python publica comandos binários cifrados, não texto em claro.
- **Nome rejeitado:** o pacote binário atual comporta até 6 caracteres ASCII.

Encerre com Ctrl+C no terminal.
