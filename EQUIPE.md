# Equipe — quem faz o quê

Repositório compartilhado. Cada pessoa deve assumir uma parte, colocar seu nome na tabela e enviar o trabalho pelo GitHub (branch + Pull Request), sempre atualizando `ATUALIZACOES.md`.

## Papéis

| Parte | Pasta | Responsável | Status |
|---|---|---|---|
| Firmware satélite (ESP32) | `firmware-esp32/` | **Miguel Julidori** | Wi-Fi + MQTT bidirecional + telemetria de teste |
| Cliente PC (aplicação MQTT) | `cliente-pc/` | A definir | Subscribe de telemetria + publish de comandos implementados |
| Estação terrena (broker Mosquitto / SSH) | `estacao-terrena/` | A definir | Configuração de referência e ACL documentadas |
| Protocolo (payload ≤ 16 bytes) | `docs/protocolo/` | A definir | Tópicos definidos; formato dos payloads pendente |
| Link budget (potência, latência, RTT) | `docs/link-budget/` | **JH** | Cálculos pendentes |
| Documentação / consolidação | `CONTEXTO.md`, relatórios | Apoio de todos | Atualizar em cada mudança de requisito |

## Entregas por parte

### Firmware ESP32

- Conectar ao Wi-Fi e ao Mosquitto.
- Publicar telemetria em `satellite/esp32-01/telemetry`.
- Assinar `satellite/esp32-01/command`.
- Validar o limite de 16 bytes.

### Aplicação PC

- Conectar ao broker.
- Assinar telemetria.
- Publicar comandos de até 16 bytes.
- Interpretar o protocolo, verificar HMAC/AES e exibir dados quando essas etapas forem implementadas.

### Estação terrena

- Instalar/configurar Mosquitto na LAN.
- Documentar IP, portas, usuários e ACL, sem versionar senhas.
- Documentar acesso SSH local.

### Protocolo

- Definir campos do payload, soma ≤ 16 bytes, incluindo segurança.
- Documentar tabela byte a byte, exemplos e regras de validação.
- Alinhar firmware e aplicação PC.

### Link budget

- Definir órbita/banda/antena da equipe.
- Calcular FSPL, ganhos, potência de transmissão, latência e RTT.
- Registrar fontes e memória de cálculo.

## Regra de ouro

Ninguém sobrescreve a parte de outro colaborador sem combinar. Mudanças entram por Pull Request para revisão do grupo.
