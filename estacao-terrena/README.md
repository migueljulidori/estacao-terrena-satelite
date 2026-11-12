# Estação terrena (broker e gerência) — precisa de responsável

**Status:** pasta reservada.  
**Ação:** quem for ficar com esta parte, coloque seu nome em [`EQUIPE.md`](../EQUIPE.md) e comece aqui.

## Objetivo

- Configurar o **broker MQTT** (ex.: Mosquitto) na máquina/Raspberry da estação
- Documentar IP, porta e como o ESP32 e o cliente PC se conectam
- Notas de acesso **SSH** local (sem expor senhas no Git)

## Como começar (sugestão)

1. Anote seu nome em `EQUIPE.md` (via branch + PR).
2. Documente a instalação do Mosquitto (Ubuntu/Raspberry).
3. Crie `mosquitto.conf.example` e um `README` com IP de teste da LAN da turma.
4. Registre em `ATUALIZACOES.md` e envie o PR.

O firmware (Miguel) precisa do IP/porta do broker na próxima etapa (MQTT).
