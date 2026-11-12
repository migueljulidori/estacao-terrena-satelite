# Como colaborar

Este repositório é **privado**. Só quem for convidado como colaborador no GitHub consegue ver e alterar. Assim cada push/PR fica visível para o restante da equipe.

## 1. Convite no GitHub

O dono do repositório (Miguel) convida em:

**GitHub → Settings → Collaborators → Add people**

Cada pessoa aceita o convite no e-mail ou em https://github.com/notifications

## 2. Configurar o Git uma vez (na sua máquina)

```bash
git config --global user.name "Seu Nome"
git config --global user.email "seu@email.com"
```

Autenticação: use GitHub CLI (`gh auth login`) ou um Personal Access Token / SSH key.

## 3. Clonar

```bash
git clone https://github.com/migueljulidori/estacao-terrena-satelite.git
cd estacao-terrena-satelite
```

## 4. Fluxo de trabalho (melhor jeito para o grupo)

Trabalhar **sempre em branch**. `main` deve refletir só o que a equipe já revisou.

```bash
# Atualizar main
git checkout main
git pull origin main

# Criar branch para a tarefa
git checkout -b feature/descricao-curta

# ... editar arquivos ...

# Registrar a mudança
# 1) Editar ATUALIZACOES.md (nova entrada no topo)
# 2) Se mudou requisitos/protocolo, editar CONTEXTO.md

git add .
git status
git commit -m "Descrição clara do que foi feito"
git push -u origin feature/descricao-curta
```

No GitHub: **Compare & pull request** → descrever → pedir revisão de um colega → **Merge** após aprovação.

Depois do merge:

```bash
git checkout main
git pull origin main
```

## 5. O que cada pasta é

| Pasta | Quem costuma mexer | Conteúdo |
|-------|--------------------|----------|
| `firmware-esp32/` | firmware / embarcado | Código do satélite (ESP32) |
| `cliente-pc/` | software / aplicação | Cliente MQTT da estação |
| `estacao-terrena/` | infra / rede | Broker Mosquitto, scripts, SSH |
| `docs/protocolo/` | todos | Formato dos 16 bytes |
| `docs/link-budget/` | RF / cálculos | Memória de cálculo |
| `docs/referencia/` | — | PDFs oficiais (não alterar sem acordo) |

## 6. Segredos

**Nunca** commitar:

- Senhas Wi-Fi, SSH, broker MQTT
- Chaves HMAC / AES / certificados
- Arquivos `.env` com credenciais reais

Use arquivos de exemplo (`*.example`) e mantenha os valores reais só na máquina local (já cobertos pelo `.gitignore`).

## 7. Conflitos

Se duas pessoas editarem o mesmo arquivo:

```bash
git pull origin main
# resolver conflitos no editor
git add .
git commit -m "Resolve conflito em <arquivo>"
git push
```

Combinem no grupo quem é responsável por cada área para reduzir conflitos.

## 8. Documentação obrigatória

Toda contribuição relevante atualiza:

1. **`ATUALIZACOES.md`** — o que mudou (sempre)
2. **`CONTEXTO.md`** — só se a decisão/requisito/protocolo mudou
