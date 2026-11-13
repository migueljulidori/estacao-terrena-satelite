# Atualizações

Histórico obrigatório de mudanças do projeto. Toda alteração relevante deve ser registrada no topo, indicando o que mudou, por quê e como validar.

## Histórico

### 2026-11-13 — Organização da Parte 2 conforme enunciado atualizado

- **Autor(es):** Elias / implementação assistida
- **Branch / PR:** `feature/parte-2-organizacao`
- **Áreas:** `CONTEXTO.md`, `EQUIPE.md`, `cliente-pc/`, `docs/protocolo/`, `docs/link-budget/`
- **O que mudou:**
  - Alinhamento do contexto aos requisitos atualizados: dois planos de acesso, SSH, autenticação, confidencialidade, desafio hacking e entregáveis.
  - Cliente Python documentado e preparado para subscribe de telemetria e publish de comandos.
  - Inclusão das fórmulas GEO e dos parâmetros fornecidos no enunciado.
  - Registro explícito de que o limite de 16 bytes é provisório, pois não aparece no PDF atualizado.
- **Por quê:** iniciar a Parte 2 com requisitos, pendências e responsabilidades rastreáveis.
- **Como testar / impacto:** validar cliente com broker e ESP32 na rede isolada; revisar parâmetros do link budget antes de calcular (P_t). Código e segurança ainda exigem testes físicos.

### 2026-11-12 — Comunicação MQTT bidirecional (implementação em branch)

- **Autor(es):** Elias / implementação assistida
- **Branch / PR:** `feature/mqtt-bidirecional` (PR #2)
- **Áreas:** firmware ESP32, cliente Python e documentação
- **O que mudou:** ESP32 publica telemetria e assina comandos; cliente Python recebe telemetria e publica comandos de teste.
- **Por quê:** atender ao enlace bidirecional exigido no PI 3T.
- **Como testar / impacto:** enviar `TESTE-N) para o PC e publicar `PING` de volta; confirmar RX no Monitor Serial do ESP32.

### 2026-11-12 — Módulo de dados com publish de teste

- **Autor(es):** Miguel Julidori
- **Áreas:** `dados_mod.h`, `mqtt_mod.h`, `satelite.ino`
- **O que mudou:** envio de `TESTE-N` a cada 5 segundos e validação do tamanho antes do publish.
- **Por quê:** testar o caminho ESP32 → broker → cliente.

### 2026-11-12 — Autenticação MQTT por usuário e senha

- **Autor(es):** Miguel Julidori
- **Áreas:** firmware ESP32 e configuração Mosquitto
- **O que mudou:** conexão MQTT com credenciais e ACL por tópico.
- **Por quê:** restringir publicação e assinatura por usuário.

### 2026-11-12 — Organização do firmware e rede

- **Autor(es):** Miguel Julidori
- **Áreas:** `firmware-esp32/satelite/`
- **O que mudou:** separação dos módulos Wi-Fi e MQTT, IP fixo do ESP32 e sketch de orquestração.
- **Por quê:** facilitar manutenção e testes por etapa.

### 2026-11-12 — Estrutura inicial do repositório e documentação

- **Autor(es):** Miguel Julidori e equipe
- **Áreas:** raiz, `firmware-esp32/`, `cliente-pc/`, `estacao-terrena/`, `docs/`
- **O que mudou:** estrutura colaborativa, READMEs, exemplos de configuração e instruções de contribuição.
- **Por quê:** organizar o Projeto Integrado e permitir trabalho paralelo.
