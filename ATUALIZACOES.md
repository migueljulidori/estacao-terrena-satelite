# Atualizações

Histórico obrigatório de mudanças do projeto. Serve para controle e organização da equipe: qualquer pessoa deve conseguir entender **o que mudou**, **por quê** e **onde**, só lendo este arquivo.

## Regra da equipe (obrigatório)

**Toda alteração** que entrar no repositório (código, documentação, protocolo, cálculos, configs) deve:

1. Ser commitada em uma branch e enviada ao GitHub.
2. Incluir uma **nova entrada no topo** desta lista (mais recente primeiro).
3. Seguir o modelo abaixo.

Não misture várias mudanças não relacionadas na mesma entrada. Se for um ajuste pequeno, ainda assim registre — uma linha basta.

### Modelo de entrada

```markdown
### YYYY-MM-DD — Título curto da mudança

- **Autor(es):** nome(s)
- **Branch / PR:** nome-da-branch ou link do PR
- **Áreas:** ex. `firmware-esp32`, `cliente-pc`, `docs/protocolo`, `CONTEXTO.md`
- **O que mudou:** descrição objetiva (1–4 bullets)
- **Por quê:** motivo / requisito atendido
- **Como testar / impacto:** se aplicável
```

### Checklist antes de abrir o Pull Request

- [ ] Código ou docs da mudança estão no commit
- [ ] Esta entrada em `ATUALIZACOES.md` foi adicionada
- [ ] Se a decisão alterar requisitos/protocolo, `CONTEXTO.md` também foi atualizado
- [ ] Segredos (chaves Wi-Fi, HMAC, AES, senhas SSH) **não** foram commitados

---

## Histórico

### 2026-11-13 — Correção da topologia e preparação do broker em PC Linux

- **Autor(es):** equipe / implementação assistida
- **Branch / PR:** `feature/mqtt-bidirecional` (PR #2)
- **Áreas:** `README.md`, `CONTEXTO.md`, `estacao-terrena/README.md`, `cliente-pc/mqtt_config.example.py`
- **O que mudou:**
  - Corrigida a arquitetura: um PC Linux separado hospeda o Mosquitto; outro PC executa o cliente Python; o ESP32 é o terceiro cliente MQTT. Removidas referências incorretas à Raspberry Pi.
  - Documentado o procedimento inicial de instalação/verificação do Mosquitto em Debian/Ubuntu e a necessidade de confirmar o IP LAN com `hostname -I`.
  - Atualizado o exemplo de configuração Python para deixar explícito que o broker está no computador Linux.
  - Registrado que o código está preparado para publish/subscribe nos dois sentidos, mas a instalação/configuração do broker e a validação física ainda estão pendentes.
- **Por quê:** refletir a topologia real definida pela equipe e orientar a próxima etapa de configuração sem confundir o IP do ESP32 com o IP do broker.
- **Como testar / impacto:** no computador Linux, executar os comandos de instalação e verificação em `estacao-terrena/README.md`; confirmar que Mosquitto está ativo e anotar o IPv4 LAN. Em seguida, configurar esse IP nos dois clientes e testar ESP32 → Python e Python → ESP32.


### 2026-11-13 — Alinhamento ao PI 3T atualizado: Raspberry Pi como broker

- **Autor(es):** Elias / implementação assistida
- **Branch / PR:** feature/mqtt-bidirecional (PR #2)
- **Áreas:** estacao-terrena/README.md, CONTEXTO.md
- **O que mudou:** Raspberry Pi definida explicitamente como host do Mosquitto; computador documentado como cliente Python; fluxos de telemetria e comando descritos conforme o requisito bidirecional.
- **Por quê:** eliminar a ambiguidade entre broker e aplicação e alinhar o repositório ao novo documento.
- **Como testar / impacto:** confirmar o IP LAN da Raspberry Pi e configurá-lo no firmware e em mqtt_config.py; testar telemetria e comando na rede isolada.

### 2026-11-12 — Comunicação MQTT bidirecional entre ESP32 e estação terrena

- **Autor(es):** Elias / implementação assistida
- **Branch / PR:** `feature/mqtt-bidirecional` (PR #2)
- **Áreas:** `firmware-esp32/`, `cliente-pc/`, `estacao-terrena/`, `docs/protocolo/`, `CONTEXTO.md`, `EQUIPE.md`, `README.md`
- **O que mudou:**
  - ESP32 mantém publish de telemetria e passa a assinar o tópico de comandos.
  - Aplicação Python recebe telemetria e publica comandos de até 16 bytes.
  - Documentação alinhada ao fluxo bidirecional e aos tópicos MQTT.
- **Por quê:** permitir que a estação receba dados e também envie comandos pelo mesmo broker.
- **Como testar / impacto:** executar aplicação Python, conferir `TESTE-N` e enviar `PING`; o comando deve aparecer no Monitor Serial do ESP32.

### 2026-11-12 — Módulo de dados com publish de teste

- **Autor(es):** Miguel Julidori
- **Branch / PR:** _você faz o commit_
- **Áreas:** `dados_mod.h`, `mqtt_mod.h`, `satelite.ino`
- **O que mudou:**
  - `dados_mod.h` envia `TESTE-N` a cada 5 s
  - `mqttPublicar` no tópico de telemetria (máx. 16 bytes)
- **Por quê:** validar comunicação TX ESP → RX PC antes da telemetria real
- **Como testar / impacto:** Serial mostra `TX ...`; no PC use `mosquitto_sub` no tópico telemetry

### 2026-11-12 — Autenticação usuário/senha no MQTT do ESP

- **Autor(es):** Miguel Julidori
- **Branch / PR:** _você faz o commit_
- **Áreas:** `firmware-esp32/satelite/mqtt_mod.h`
- **O que mudou:** `mqtt.connect` com `mqtt_user` e `mqtt_senha`
- **Por quê:** broker Mosquitto exige senha
- **Como testar / impacto:** preencher a senha real criada com `mosquitto_passwd`

### 2026-11-12 — Um arquivo por módulo (wifi / mqtt)

- **Autor(es):** Miguel Julidori
- **Branch / PR:** _você faz o commit_
- **Áreas:** `firmware-esp32/satelite/`
- **O que mudou:**
  - Só `wifi_mod.h` e `mqtt_mod.h` (sem pares `.cpp`)
  - Removidos `.cpp` e `secrets.example.h` não usados
- **Por quê:** menos abas no Arduino IDE; um arquivo por função
- **Como testar / impacto:** reabrir a pasta `satelite/` no IDE

### 2026-11-12 — ESP só publish (TX); PC só subscribe (RX)

- **Autor(es):** Miguel Julidori
- **Branch / PR:** _você faz o commit_
- **Áreas:** `mqtt_mod.cpp`, `CONTEXTO.md`, READMEs
- **O que mudou:**
  - Removidos `subscribe`, callback e tópico `command` do firmware
  - Documentado enlace unidirecional satélite → estação
- **Por quê:** decisão da equipe — sem troca bidirecional de dados
- **Como testar / impacto:** ESP só conecta ao broker; publicação ainda na etapa 6; PC deve só assinar `telemetry`

### 2026-11-12 — Separa módulos Wi-Fi e MQTT no firmware

- **Autor(es):** Miguel Julidori
- **Branch / PR:** _você faz o commit_
- **Áreas:** `firmware-esp32/satelite/`
- **O que mudou:**
  - `wifi_mod.h` / `wifi_mod.cpp` — conexão Wi-Fi e IP fixo
  - `mqtt_mod.h` / `mqtt_mod.cpp` — broker, assinatura e callback
  - `satelite.ino` só chama os módulos
- **Por quê:** organização clara por etapa (Wi-Fi e MQTT separados)
- **Como testar / impacto:** abrir pasta `satelite/` no Arduino IDE; ajustar broker em `mqtt_mod.cpp`

### 2026-11-12 — Firmware MQTT simples no .ino (integração)

- **Autor(es):** Miguel Julidori (com base no MQTT inicial da equipe)
- **Branch / PR:** `main`
- **Áreas:** `firmware-esp32/satelite/satelite.ino`
- **O que mudou:**
  - Sketch único: Wi-Fi + MQTT + assinatura de `satellite/esp32-01/command`
  - SSID/senha e `mqtt_broker` diretos no `.ino` (sem `secrets.h` no firmware)
  - Mantém limite de 16 bytes na recepção; sem publicar telemetria ainda
- **Por quê:** etapa 2/6 com código simples alinhado ao restante do time
- **Como testar / impacto:** PubSubClient; ajustar `mqtt_broker`; Serial `MQTT ... OK`

### 2026-11-12 — Implementa etapa MQTT inicial (firmware, broker e cliente PC)

- **Autor(es):** Elias / implementação assistida por ChatGPT
- **Branch / PR:** `feature/etapa-mqtt`
- **Áreas:** `firmware-esp32/`, `cliente-pc/`, `estacao-terrena/`, `docs/protocolo/`, `CONTEXTO.md`, `README.md`, `.gitignore`
- **O que mudou:**
  - ESP32 conecta ao broker Mosquitto, assina telecomando e rejeita payloads > 16 bytes
  - Cliente Python e exemplos Mosquitto/ACL
  - Tópicos iniciais documentados
- **Por quê:** avançar a Fase 1 na ordem oficial
- **Como testar / impacto:** ver READMEs de `firmware-esp32/`, `cliente-pc/` e `estacao-terrena/`

### 2026-11-12 — IP fixo do ESP32

- **Autor(es):** Miguel Julidori
- **Branch / PR:** `main`
- **Áreas:** `firmware-esp32/satelite/satelite.ino`
- **O que mudou:** `WiFi.config` com IP `192.168.30.52`, gateway/DNS `192.168.30.1`
- **Por quê:** endereço estável do satélite na LAN da turma
- **Como testar / impacto:** gravar e confirmar no Serial que o IP é 192.168.30.52

### 2026-11-12 — Credenciais Wi-Fi da turma no sketch

- **Autor(es):** Miguel Julidori
- **Branch / PR:** `main`
- **Áreas:** `firmware-esp32/satelite/satelite.ino`
- **O que mudou:** SSID/senha definidos diretamente no sketch
- **Por quê:** rede da turma para testes da etapa Wi-Fi
- **Como testar / impacto:** gravar no ESP32 e conferir IP no Serial 115200

### 2026-11-12 — Simplifica firmware: um único .ino com SSID/senha

- **Autor(es):** Miguel Julidori
- **Branch / PR:** `main`
- **Áreas:** `firmware-esp32/satelite/`
- **O que mudou:**
  - Código Wi-Fi só em `satelite.ino`
  - Removidos `config.h`, `wifi_link.*`, `secrets.example.h`
- **Por quê:** pedir organização mínima, sem modularização prematura
- **Como testar / impacto:** etapa Wi-Fi gravada pelo Arduino IDE

### 2026-11-12 — Modulariza o firmware Wi-Fi (.h / .cpp)

- **Autor(es):** Miguel Julidori
- **Branch / PR:** `main`
- **Áreas:** `firmware-esp32/satelite/`
- **O que mudou:** tentativa anterior de separar a lógica Wi-Fi em arquivos próprios.
- **Por quê:** organização modular clara.
- **Como testar / impacto:** histórico de organização; não representa a estrutura atual.

### 2026-11-12 — Simplifica o sketch ESP32 (Wi-Fi no .ino)

- **Autor(es):** Miguel Julidori
- **Branch / PR:** `main`
- **Áreas:** `firmware-esp32/satelite/`
- **O que mudou:** lógica Wi-Fi consolidada no sketch.
- **Por quê:** manter o código legível no estilo Arduino.
- **Como testar / impacto:** etapa Wi-Fi.

### 2026-11-12 — Ordem oficial do pipeline do firmware

- **Autor(es):** Miguel Julidori
- **Branch / PR:** `main`
- **Áreas:** `firmware-esp32/`, `CONTEXTO.md`, `EQUIPE.md`
- **O que mudou:** fixou a ordem Wi-Fi → MQTT → gerar dados → criptografia → montar pacote → publicar.
- **Por quê:** evitar pular etapas e manter a equipe alinhada.
- **Como testar / impacto:** continuar validando cada etapa antes de avançar.

### 2026-11-12 — Início do firmware ESP32 (Wi-Fi) + papéis da equipe

- **Autor(es):** Miguel Julidori
- **Branch / PR:** `main`
- **Áreas:** `firmware-esp32/`, `EQUIPE.md`, READMEs das pastas, `README.md`, `CONTEXTO.md`
- **O que mudou:** sketch Wi-Fi inicial e organização do trabalho colaborativo.
- **Por quê:** começar a Fase 1 pelo enlace Wi-Fi do ESP32.
- **Como testar / impacto:** gravar no ESP32 e conferir Serial 115200.

### 2026-11-12 — Passo a passo de colaboração no GitHub

- **Autor(es):** Miguel Julidori
- **Branch / PR:** `main`
- **Áreas:** `CONTRIBUTING.md`, `README.md`
- **O que mudou:** guia passo a passo de colaboração e Pull Requests.
- **Por quê:** orientar a equipe sobre como enviar atualizações.
- **Como testar / impacto:** seguir o guia antes de contribuir.

### 2026-11-12 — Criação do repositório compartilhado

- **Autor(es):** Miguel Julidori
- **Branch / PR:** `main` (commit inicial)
- **Áreas:** raiz do projeto, `docs/referencia`, estrutura de pastas
- **O que mudou:** criação da base colaborativa, documentação e PDFs de referência.
- **Por quê:** organizar o Projeto Integrado (estação terrena / satélite ESP32).
- **Como testar / impacto:** clonar o repositório e seguir `CONTRIBUTING.md`.
