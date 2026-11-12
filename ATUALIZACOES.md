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

### 2026-11-12 — Implementa etapa MQTT inicial (firmware, broker e cliente PC)

- **Autor(es):** Elias / implementação assistida por ChatGPT
- **Branch / PR:** `feature/etapa-mqtt` (PR a abrir)
- **Áreas:** `firmware-esp32/`, `cliente-pc/`, `estacao-terrena/`, `docs/protocolo/`, `CONTEXTO.md`, `README.md`, `.gitignore`
- **O que mudou:**
  - ESP32 conecta ao broker Mosquitto, assina o tópico de telecomando e rejeita payloads acima de 16 bytes.
  - Cliente Python conecta, assina telemetria e publica telecomandos de teste limitados a 16 bytes.
  - Adicionados exemplos de configuração Mosquitto, ACL e arquivos locais de credenciais ignorados pelo Git.
  - Documentados tópicos iniciais e pendências de IP do broker e formato binário.
- **Por quê:** avançar a Fase 1 na ordem oficial sem antecipar geração de dados, criptografia ou pacote.
- **Como testar / impacto:** instalar PubSubClient no Arduino IDE; instalar dependência Python; configurar IP e credenciais localmente; testar publicação de comando e recepção no Serial. Testes físicos ainda pendentes.

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
