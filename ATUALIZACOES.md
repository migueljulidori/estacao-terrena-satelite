### 2026-11-12 — Assunção do cliente PC e inclusão do painel web

- **Autor(es):** Manuela Lino
- **Branch / PR:** `feature/cliente-pc-manuela` / PR a abrir
- **Áreas:** `cliente-pc/`, `EQUIPE.md`, `README.md`
- **O que mudou:**
  - Responsabilidade por toda a área `cliente-pc/` atribuída a Manuela Lino.
  - Documentadas as tarefas de recepção MQTT, interpretação do payload, verificação de segurança e apresentação dos dados.
  - Adicionada a estrutura do painel web ao README principal e ao guia do cliente PC.
  - Incluída a interface web inicial em `cliente-pc/website/`.
- **Por quê:** centralizar a responsabilidade pelo cliente de recepção e pela interface de monitoramento.
- **Como testar / impacto:** abrir `cliente-pc/website/index.html` no navegador. A interface ainda é estática; a conexão MQTT e a atualização em tempo real permanecem pendentes.

---

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
- **O que mudou:** só `wifi_mod.h` e `mqtt_mod.h`; removidos arquivos não usados.
- **Por quê:** menos abas no Arduino IDE; um arquivo por função.
- **Como testar / impacto:** reabrir a pasta `satelite/` no IDE.

### 2026-11-12 — ESP só publish (TX); PC só subscribe (RX)

- **Autor(es):** Miguel Julidori
- **Branch / PR:** _você faz o commit_
- **Áreas:** firmware, `CONTEXTO.md`, READMEs
- **O que mudou:** removidos subscribe e callback do firmware; documentado enlace unidirecional.
- **Por quê:** decisão da equipe — sem troca bidirecional de dados.
- **Como testar / impacto:** ESP publica telemetria; PC assina o tópico.

### 2026-11-12 — Implementa etapa MQTT inicial (firmware, broker e cliente PC)

- **Autor(es):** Elias / implementação assistida por ChatGPT
- **Branch / PR:** `feature/etapa-mqtt`
- **Áreas:** `firmware-esp32/`, `cliente-pc/`, `estacao-terrena/`, `docs/protocolo/`, `CONTEXTO.md`, `README.md`, `.gitignore`
- **O que mudou:** cliente Python e exemplos Mosquitto/ACL, além da conexão inicial MQTT.
- **Por quê:** avançar a Fase 1 na ordem oficial.
- **Como testar / impacto:** ver READMEs das pastas envolvidas.

