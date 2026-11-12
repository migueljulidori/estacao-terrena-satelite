# Cliente PC (estação terrena) — somente RX

**Responsável: Manuela Lino**

O cliente PC recebe a telemetria publicada pelo ESP32. O enlace é unidirecional: o ESP32 transmite (TX) e o PC recebe (RX); este componente não envia telecomandos.

## Responsabilidades

1. Conectar ao broker Mosquitto da estação terrena.
2. Assinar `satellite/esp32-01/telemetry`.
3. Interpretar o payload do projeto (limite atual: 16 bytes).
4. Implementar a verificação HMAC e/ou descriptografia AES de acordo com o protocolo definido pela equipe.
5. Rejeitar mensagens inválidas.
6. Exibir os dados recebidos no cliente e no painel web.

## Cliente MQTT em Python

### Instalação

```bash
python3 -m venv .venv
source .venv/bin/activate
pip install -r requirements.txt
cp mqtt_config.example.py mqtt_config.py
```

Edite `mqtt_config.py` com o endereço do broker e as credenciais locais. Não versione esse arquivo nem compartilhe senhas.

### Execução

```bash
python mqtt_client.py
```

O cliente aguarda mensagens do ESP. Use Ctrl+C para sair.

## Painel web

A interface está em `website/`:

- `index.html`: estrutura do painel de monitoramento.
- `style.css`: identidade visual e layout responsivo.
- `script.js`: comportamento da interface.
- `assets/terra-estacao.png`: imagem usada no painel.

### Como abrir

Abra `website/index.html` em um navegador. Nesta etapa, o painel é uma interface estática: os números apresentados são valores demonstrativos e ainda não representam telemetria recebida pelo MQTT. A conexão entre o cliente Python e o painel, assim como a atualização em tempo real, ainda precisa ser implementada.

## Próximas etapas

- Validar a recepção MQTT no cliente Python usando o tópico oficial.
- Definir o formato do payload com a equipe de protocolo.
- Implementar interpretação e validação criptográfica.
- Integrar os dados válidos ao painel web.
- Exibir claramente o estado da conexão e mensagens rejeitadas.

**Segurança:** não envie credenciais Wi-Fi, MQTT, SSH, chaves HMAC/AES ou arquivos locais de configuração ao Git.
