# Equipe — responsabilidades

Repositório compartilhado. Cada integrante deve registrar sua responsabilidade e enviar alterações por branch + Pull Request, atualizando `ATUALIZACOES.md`.

## Papéis

| Parte | Pasta | Responsável atual | Status |
|---|---|---|---|
| Firmware satélite (ESP32) | `firmware-esp32/` | **Miguel Julidori** | Wi-Fi + MQTT TX/RX + telemetria de teste; segurança e pacote final pendentes |
| Cliente Python | `cliente-pc/` | _definir_ | Cliente bidirecional de teste; interpretação e verificação criptográfica pendentes |
| Estação terrena (Mosquitto / SSH) | `estacao-terrena/` | _definir_ | Exemplos Mosquitto/ACL documentados; instalação e SSH precisam de evidências |
| Protocolo | `docs/protocolo/` | _definir_ | Tópicos definidos; formato final byte a byte pendente |
| Link budget GEO | `docs/link-budget/` | **JH** | Responsabilidade indicada; parâmetros e memória de cálculo pendentes |
| Consolidação / relatório | documentos | Todos | Fluxogramas, evidências e relatório final pendentes |

## Entregas por frente

### Firmware ESP32
1. Wi-Fi.
2. MQTT bidirecional.
3. Telemetria útil.
4. Autenticação e confidencialidade.
5. Montagem e interpretação do pacote final.
6. Testes com mensagens válidas/inválidas.

### Cliente Python
- Conectar ao broker Mosquitto da estação.
- Assinar telemetria e publicar comandos.
- Interpretar os campos do protocolo.
- Verificar autenticidade/integridade e descriptografar.
- Rejeitar mensagens inválidas e apresentar o resultado das verificações.

### Estação terrena
- Instalar/configurar Mosquitto na Raspberry Pi.
- Configurar usuários e ACL.
- Configurar acesso SSH por usuário/senha, restrito à LAN/VPN.
- Registrar evidências de operação e segurança.

### Protocolo
- Definir mensagens de telemetria e comando/configuração.
- Documentar ID de origem, tipo, dados úteis, campos de segurança, tamanhos e codificação.
- Definir procedimento de chaves e comportamento diante de mensagens inválidas.

### Link budget
- Escolher banda/frequência e taxa de dados.
- Definir parâmetros de antena, elevação e perdas.
- Calcular geometria, FSPL, ganho, C/N₀ e potência (P_t).
- Entregar memória de cálculo com unidades e fontes.

## Regra de colaboração

Não sobrescrever o trabalho de outro integrante sem combinar. Registrar mudanças e abrir Pull Request para revisão do grupo.
