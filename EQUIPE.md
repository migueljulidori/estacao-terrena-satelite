# Equipe — quem faz o quê

Repositório compartilhado. **Cada pessoa deve assumir uma parte**, colocar o nome nesta tabela e enviar o trabalho pelo GitHub (branch + Pull Request), sempre atualizando `ATUALIZACOES.md`.

Se ainda não tiver parte, **escolha uma linha livre**, edite este arquivo com seu nome e abra um PR só com isso — assim o grupo sabe quem é responsável.

## Papéis

| Parte | Pasta | Responsável | Status |
|-------|--------|-------------|--------|
| Firmware satélite (ESP32) | `firmware-esp32/` | **Miguel Julidori** | Pipeline: Wi-Fi → MQTT → dados → cripto → pacote → publicar — **agora: MQTT (conectar)** |
| Cliente PC (MQTT + verificação + painel web) | `cliente-pc/` | **Manuela Lino** | Cliente MQTT bidirecional: recebe telemetria/ACK, publica comandos de teste e atualiza o painel web |
| Estação terrena (broker Mosquitto / SSH) | `estacao-terrena/` | _coloque seu nome_ | Aguardando responsável |
| Protocolo (payload ≤ 16 bytes) | `docs/protocolo/` | _coloque seu nome_ (pode ser em dupla) | Aguardando responsável |
| Link budget (potência, latência, RTT) | `docs/link-budget/` | **JH** | Aguardando responsável |
| Documentação / consolidação | `CONTEXTO.md`, relatórios | _coloque seu nome_ | Apoio de todos |

## Como “pegar” sua parte

1. Aceite o convite de colaborador (ver `CONTRIBUTING.md`).
2. Clone o repositório.
3. Crie uma branch, por exemplo: `docs/assumir-cliente-pc`
4. Edite **esta tabela** colocando seu nome na linha da parte.
5. Atualize o `README.md` da pasta correspondente (escreva que você é o responsável).
6. Adicione entrada em `ATUALIZACOES.md`.
7. Push + Pull Request.

## O que cada parte deve entregar (visão geral)

### `firmware-esp32/` (Miguel — em andamento)

1. Wi-Fi → 2. MQTT → 3. Gerar dados → 4. Criptografia → 5. Montar pacote → 6. Publicar  
Detalhes: `firmware-esp32/README.md` (só avançar a próxima etapa quando a atual estiver estável)

### `cliente-pc/` (Manuela — em andamento)

- Conectar ao broker MQTT da estação
- Assinar `satellite/esp32-01/telemetry` para receber telemetria e ACK
- Publicar `PING`, `STATUS` e `NOME=...` em `satellite/esp32-01/command`
- Interpretar o payload de até 16 bytes
- Verificar HMAC e/ou descriptografar AES, conforme o protocolo definido
- Rejeitar mensagens inválidas e apresentar os dados recebidos
- Manter o painel web de monitoramento e integrá-lo ao cliente de recepção

O cliente Python e o painel web são componentes da mesma área. O teste inicial cobre os dois sentidos MQTT, a decodificação dos pacotes de 16 bytes e os ACKs; HMAC/AES permanecem pendentes. Guia: `cliente-pc/README.md`

### `estacao-terrena/` (precisa de responsável)

- Instalar/configurar Mosquitto (ou equivalente) na LAN da turma
- Documentar IP do broker, portas e usuários (sem senhas reais no Git)
- Notas de acesso SSH local à estação

Comece documentando o passo a passo de instalação. Guia: `estacao-terrena/README.md`

### `docs/protocolo/` (precisa de responsável)

- Definir campos do payload (soma ≤ 16 bytes, incluindo segurança)
- Tabela byte a byte + exemplos
- Alinhar com firmware e cliente PC

### `docs/link-budget/` (precisa de responsável)

- Órbita/banda/antena da equipe (preencher em `CONTEXTO.md`)
- FSPL, ganhos, P_t, latência, RTT
- Memória de cálculo com fontes

## Regra de ouro

Ninguém sobrescreve a parte do outro sem combinar. Mudanças entram por **Pull Request** para o resto do grupo ver.
