# Passo a passo — como enviar atualizações

Guia para **todos os colaboradores** do repositório  
https://github.com/migueljulidori/estacao-terrena-satelite

Leia na ordem. Na primeira vez, faça as seções 1 a 3. Nas próximas mudanças, use a seção 4.

---

Antes de codar: leia também [`EQUIPE.md`](EQUIPE.md) e **coloque seu nome** na parte que você vai fazer.

## 1. Aceitar o convite

1. O dono do repositório (Miguel) convida você em **Settings → Collaborators**.
2. Você recebe um e-mail do GitHub **ou** vê o convite em: https://github.com/notifications
3. Clique em **Accept invitation**.
4. Só depois disso o repositório aparece na sua conta.

Sem aceitar o convite, o clone e o push **não funcionam** (repo privado).

---

## 2. Configurar o Git (só na primeira vez, na sua máquina)

Abra o terminal e rode (troque pelo seu nome e e-mail do GitHub):

```bash
git config --global user.name "Seu Nome"
git config --global user.email "seu@email.com"
```

Para autenticar no GitHub, escolha uma opção:

- **GitHub CLI:** `gh auth login`
- Ou use o login que o Git pedir no navegador ao fazer `git push`
- Ou configure uma chave SSH / Personal Access Token

---

## 3. Baixar (clonar) o repositório (só na primeira vez)

```bash
git clone https://github.com/migueljulidori/estacao-terrena-satelite.git
cd estacao-terrena-satelite
```

Agora você tem uma cópia local igual à do GitHub.

---

## 4. Enviar uma atualização (fazer toda vez que mudar algo)

### Passo 4.1 — Atualizar sua cópia com o que já está no GitHub

```bash
cd estacao-terrena-satelite
git checkout main
git pull origin main
```

Isso evita trabalhar em cima de uma versão antiga.

### Passo 4.2 — Criar uma branch para a sua tarefa

Não edite direto na `main`. Crie uma branch com nome claro:

```bash
git checkout -b feature/nome-da-sua-tarefa
```

Exemplos de nome:

- `feature/firmware-hmac`
- `feature/cliente-mqtt`
- `docs/protocolo-16-bytes`
- `fix/correcao-payload`

### Passo 4.3 — Fazer as alterações

Edite os arquivos do projeto (código, docs, etc.).

**Obrigatório em toda contribuição:**

1. Abrir `ATUALIZACOES.md` e **adicionar uma entrada nova no topo** do histórico (use o modelo que já está no arquivo).
2. Se a mudança alterar requisito, protocolo ou decisão da equipe, atualizar também `CONTEXTO.md`.

### Passo 4.4 — Preparar e gravar o commit

```bash
git add .
git status
git commit -m "Descrição clara do que foi feito"
```

Exemplo de mensagem boa: `Adiciona geração HMAC-SHA256 no firmware ESP32`

### Passo 4.5 — Enviar a branch para o GitHub

```bash
git push -u origin feature/nome-da-sua-tarefa
```

(Use o mesmo nome de branch que você criou no passo 4.2.)

### Passo 4.6 — Abrir um Pull Request (PR)

1. Abra: https://github.com/migueljulidori/estacao-terrena-satelite
2. Deve aparecer um aviso **Compare & pull request** — clique nele  
   (ou: aba **Pull requests** → **New pull request**)
3. Confira: base = `main` ← compare = sua branch
4. Título e descrição: diga o que mudou e por quê
5. Peça para **pelo menos um colega revisar**
6. Clique em **Create pull request**

### Passo 4.7 — Merge (juntar na main)

1. Depois da revisão (aprovação), clique em **Merge pull request**
2. Confirme o merge
3. (Opcional) delete a branch no GitHub

### Passo 4.8 — Atualizar sua máquina de novo

```bash
git checkout main
git pull origin main
```

Pronto: sua mudança está na `main` e **todo o grupo** vê ao fazer `git pull`.

---

## Checklist rápido antes do Pull Request

- [ ] Trabalhei em uma **branch**, não na `main`
- [ ] Rodei `git pull` na `main` antes de começar
- [ ] Atualizei `ATUALIZACOES.md`
- [ ] Atualizei `CONTEXTO.md` (se mudou requisito/protocolo/decisão)
- [ ] **Não** committei senhas, chaves Wi-Fi, HMAC, AES ou `.env` com segredos
- [ ] Fiz `git push` da minha branch
- [ ] Abri o Pull Request no GitHub

---

## Como só acompanhar o que os outros fizeram

```bash
cd estacao-terrena-satelite
git checkout main
git pull origin main
```

Leia também `ATUALIZACOES.md` para o histórico em português do que mudou.

No site: https://github.com/migueljulidori/estacao-terrena-satelite/pulls (PRs) e a aba **Commits**.

---

## Se der conflito

Duas pessoas editaram o mesmo arquivo. Na sua branch:

```bash
git checkout main
git pull origin main
git checkout feature/nome-da-sua-tarefa
git merge main
```

1. Abra os arquivos marcados com conflito e resolva.
2. Depois:

```bash
git add .
git commit -m "Resolve conflito com main"
git push
```

Combinem no grupo quem é responsável por cada pasta para reduzir conflitos.

---

## Pastas do projeto

| Pasta | Conteúdo |
|-------|----------|
| `firmware-esp32/` | Código do satélite (ESP32) |
| `cliente-pc/` | Cliente MQTT da estação (PC) |
| `estacao-terrena/` | Broker Mosquitto, scripts, SSH |
| `docs/protocolo/` | Formato do payload (≤ 16 bytes) |
| `docs/link-budget/` | Cálculos de enlace |
| `docs/referencia/` | PDFs oficiais (não alterar sem acordo) |

---

## Segredos (nunca enviar)

Não faça commit de:

- Senhas Wi-Fi, SSH, broker MQTT
- Chaves HMAC / AES / certificados
- Arquivos `.env` com credenciais reais

Use `*.example` e mantenha valores reais só na sua máquina (já ignorados pelo `.gitignore`).

---

## Dúvidas comuns

| Problema | O que fazer |
|----------|-------------|
| `Repository not found` | Aceitar o convite de colaborador |
| Pediu usuário/senha no push | Autenticar com `gh auth login` ou token/SSH |
| Não aparece “Compare & pull request” | Abrir **Pull requests → New pull request** e selecionar sua branch |
| Minha mudança não aparece para os outros | Falta o merge do PR na `main` (ou os outros não fizeram `git pull`) |
