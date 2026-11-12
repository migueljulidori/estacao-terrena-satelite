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

### 2026-11-12 — Simplifica firmware: um único .ino com SSID/senha

- **Autor(es):** Miguel Julidori
- **Branch / PR:** `main`
- **Áreas:** `firmware-esp32/satelite/`
- **O que mudou:**
  - Código Wi-Fi só em `satelite.ino` (`ssid` e `senha` diretos)
  - Removidos `config.h`, `wifi_link.*`, `secrets.example.h`
- **Por quê:** pedir organização mínima, sem modularização prematura
- **Como testar / impacto:** editar ssid/senha no .ino, gravar, Serial 115200

### 2026-11-12 — Modulariza o firmware Wi-Fi (.h / .cpp)

- **Autor(es):** Miguel Julidori
- **Branch / PR:** `main`
- **Áreas:** `firmware-esp32/satelite/`
- **O que mudou:**
  - `satelite.ino` só orquestra (`setup`/`loop`)
  - `wifi_link.h` + `wifi_link.cpp` com a lógica Wi-Fi
  - `config.h` com constantes; `secrets.example.h` para credenciais
- **Por quê:** organização modular clara, sem esconder implementação só em header inline
- **Como testar / impacto:** abrir a pasta `satelite/` no Arduino IDE; criar `secrets.h` e gravar

### 2026-11-12 — Simplifica o sketch ESP32 (Wi-Fi no .ino)

- **Autor(es):** Miguel Julidori
- **Branch / PR:** `main`
- **Áreas:** `firmware-esp32/satelite/`
- **O que mudou:**
  - Lógica Wi-Fi passou a ficar no `satelite.ino` (estilo Arduino claro)
  - Removidos `wifi_link.h` e `config.h` (estrutura confusa com inline em header)
  - Mantido só `secrets.example.h` / `secrets.h` para credenciais
- **Por quê:** o código anterior não ficava legível nem alinhado ao planejamento (funções e variáveis no sketch)
- **Como testar / impacto:** mesma etapa Wi-Fi; gravar `satelite.ino` após criar `secrets.h`

### 2026-11-12 — Ordem oficial do pipeline do firmware

- **Autor(es):** Miguel Julidori
- **Branch / PR:** `main`
- **Áreas:** `firmware-esp32/`, `CONTEXTO.md`, `EQUIPE.md`
- **O que mudou:**
  - Fixou a ordem: Wi-Fi → MQTT → gerar dados → criptografia → montar pacote → publicar
  - Comentários do sketch e docs alinhados aos PDFs; etapa ativa permanece só Wi-Fi
- **Por quê:** evitar pular etapas e manter o grupo no mesmo plano de evolução
- **Como testar / impacto:** continuar validando Wi-Fi; MQTT só na próxima etapa

### 2026-11-12 — Início do firmware ESP32 (Wi-Fi) + papéis da equipe

- **Autor(es):** Miguel Julidori
- **Branch / PR:** `main`
- **Áreas:** `firmware-esp32/`, `EQUIPE.md`, READMEs das pastas, `README.md`, `CONTEXTO.md`
- **O que mudou:**
  - Sketch do satélite com conexão Wi-Fi, reconexão e log de IP/RSSI
  - `secrets.example.h` (credenciais locais fora do Git)
  - `EQUIPE.md` pedindo que cada pessoa assuma sua parte
  - READMEs de `cliente-pc/`, `estacao-terrena/`, protocolo e link-budget com guia de início
- **Por quê:** começar a Fase 1 pelo enlace Wi-Fi do ESP32 e organizar o trabalho colaborativo
- **Como testar / impacto:** copiar `secrets.example.h` → `secrets.h`, gravar `satelite.ino` no ESP32, Serial 115200; colegas devem preencher `EQUIPE.md`

### 2026-11-12 — Passo a passo de colaboração no GitHub

- **Autor(es):** Miguel Julidori
- **Branch / PR:** `main`
- **Áreas:** `CONTRIBUTING.md`, `README.md`
- **O que mudou:**
  - Reescreveu `CONTRIBUTING.md` como guia passo a passo (convite, clone, branch, commit, push, PR, merge, conflitos)
  - Destacou o link do guia no `README.md`
- **Por quê:** orientar a equipe sobre como enviar atualizações e manter o repositório sincronizado
- **Como testar / impacto:** qualquer colaborador deve seguir `CONTRIBUTING.md` antes de contribuir

### 2026-11-12 — Criação do repositório compartilhado

- **Autor(es):** Miguel Julidori
- **Branch / PR:** `main` (commit inicial)
- **Áreas:** raiz do projeto, `docs/referencia`, estrutura de pastas
- **O que mudou:**
  - Criação do repositório privado no GitHub
  - README, CONTEXTO, ATUALIZACOES e CONTRIBUTING
  - Pastas `firmware-esp32/`, `cliente-pc/`, `estacao-terrena/`, `docs/`
  - Inclusão dos PDFs oficiais em `docs/referencia/`
- **Por quê:** base colaborativa para o Projeto Integrado (estação terrena / satélite ESP32)
- **Como testar / impacto:** clonar o repo e seguir `CONTRIBUTING.md`; implementação começa nas próximas etapas
