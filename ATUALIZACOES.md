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
