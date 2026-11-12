# Firmware ESP32 (satélite) — parte do Miguel

Responsável inicial: **Miguel Julidori** (`firmware-esp32/`).

Este diretório simula o **satélite**: o ESP32 entra na rede Wi-Fi da turma/estação e, nas próximas etapas, publica telemetria no broker MQTT com segurança (HMAC/AES).

## Etapa atual

**Wi-Fi conectado e monitorado** (reconexão + log de IP/RSSI no Serial).

## Roadmap do firmware

| Etapa | Status | O que fazer |
|-------|--------|-------------|
| 1. Wi-Fi | Em andamento | Conectar na rede da turma (`secrets.h`) |
| 2. MQTT | Próxima | Conectar ao broker da estação e publicar/assinar |
| 3. Protocolo ≤ 16 B | Depois | Montar payload conforme `docs/protocolo/` |
| 4. HMAC-SHA256 | Depois | Autenticar mensagens (mbedTLS) |
| 5. AES | Depois | Confidencialidade (mbedTLS) |

Evoluímos **uma etapa por vez**, sempre registrando em `ATUALIZACOES.md` e enviando via branch + Pull Request (`CONTRIBUTING.md`).

## Como gravar no ESP32 (Arduino IDE)

1. Instale o suporte à placa **ESP32** no Arduino IDE.
2. Copie as credenciais locais:

```bash
cd firmware-esp32/satelite
cp secrets.example.h secrets.h
# edite secrets.h com SSID e senha da rede da turma
```

3. Abra `satelite/satelite.ino` no Arduino IDE.
4. Selecione a placa ESP32 e a porta serial.
5. Compile, grave e abra o **Serial Monitor** em **115200**.

Saída esperada: mensagens `[WiFi] Conectado`, IP local e RSSI a cada 5 s.

## Arquivos

| Arquivo | Função |
|---------|--------|
| `satelite/satelite.ino` | `setup` / `loop` do satélite |
| `satelite/config.h` | Constantes do satélite (ID, tempos) |
| `satelite/wifi_link.h` | Conectar, manter e logar o Wi-Fi |
| `satelite/secrets.example.h` | Modelo de SSID/senha |
| `satelite/secrets.h` | Credenciais reais (**não commitadas**) |

## Quem ajuda nesta pasta

Se for mexer no firmware, combine com o Miguel, use uma branch (`feature/...`) e atualize `ATUALIZACOES.md`. Não grave `secrets.h` no Git.
