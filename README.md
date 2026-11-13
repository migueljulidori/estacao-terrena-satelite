# Estação Terrena de Comunicação via Satélite

Projeto Integrado – Telecomunicações (3º Trimestre 2026).

Simulação de uma estação terrena que se comunica com satélites (nós ESP32) via MQTT, com dimensionamento de potência de um enlace GEO e avaliação da segurança da comunicação e do acesso de gerência.

## Status do projeto

- **Comunicação:** código MQTT bidirecional de teste (ESP32 ↔ Mosquitto ↔ cliente Python).
- **Telemetria:** atualmente `TESTE-N`; dados operacionais reais ainda serão definidos.
- **Segurança das mensagens:** autenticação e confidencialidade pendentes de definição/implementação.
- **Gerência:** SSH na Raspberry Pi, restrito à LAN/VPN; configuração e evidências pendentes.
- **Link budget:** fórmulas e parâmetros de referência documentados; banda, taxa de dados e potência ainda precisam ser calculadas.
- **Desafio hacking:** etapa posterior à validação, somente na rede isolada autorizada.

## Comece por aqui

1. `CONTRIBUTING.md` — colaboração Git.
2. `EQUIPE.md` — responsabilidades.
3. `CONTEXTO.md` — requisitos e estado geral.
4. `ATUALIZACOES.md` — histórico de alterações.

## Estrutura

- `firmware-esp32/` — firmware dos satélites ESP32.
- `cliente-pc/` — cliente MQTT Python da estação.
- `estacao-terrena/` — exemplos Mosquitto, usuários e ACL.
- `docs/protocolo/` — definição do protocolo de mensagens.
- `docs/link-budget/` — memória de cálculo do enlace GEO.
- `docs/referencia/` — enunciados e documentos de referência.

## Segurança

Não versionar senhas, chaves HMAC/AES, credenciais Wi-Fi/SSH nem arquivos locais de configuração. Usar arquivos `*.example` como modelo e manter valores reais fora do Git.

Todos os testes de segurança devem ocorrer exclusivamente na rede isolada e nos dispositivos autorizados da atividade.
