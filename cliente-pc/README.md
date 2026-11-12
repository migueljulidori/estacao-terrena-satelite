# Cliente PC (estação terrena) — precisa de responsável

**Status:** pasta reservada.  
**Ação:** quem for ficar com esta parte, coloque seu nome em [`EQUIPE.md`](../EQUIPE.md) e comece aqui.

## Objetivo

Programa no PC que:

1. Conecta ao **broker MQTT** da estação terrena
2. Assina o tópico do satélite da equipe
3. Interpreta o **payload ≤ 16 bytes**
4. Verifica autenticidade (HMAC) e/ou descriptografa (AES)
5. Mostra os dados e rejeita mensagens inválidas

## Como começar (sugestão)

1. Anote seu nome em `EQUIPE.md` (via branch + PR).
2. Escolha a linguagem (ex.: Python + `paho-mqtt`).
3. Crie um esqueleto que só conecta ao broker e imprime o payload bruto.
4. Registre em `ATUALIZACOES.md` e envie o PR.
5. Evolua junto com o protocolo (`docs/protocolo/`) e o firmware (`firmware-esp32/`).

## Dependências

- Broker rodando (pasta `estacao-terrena/`)
- Firmware publicando (Miguel — Wi-Fi já em andamento; MQTT na próxima etapa)

Não committe senhas ou chaves. Use arquivos `*.example`.
