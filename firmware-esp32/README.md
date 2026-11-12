# Firmware ESP32 (satélite) — parte do Miguel

Responsável: **Miguel Julidori** (`firmware-esp32/`).

O ESP32 simula o **satélite**: entra na Wi-Fi da turma, depois usa o broker MQTT da estação terrena para enviar telemetria com segurança (planejamento HMAC/AES nos PDFs).

## Ordem oficial (não pular etapas)

| # | Etapa | Status | Observação |
|---|--------|--------|------------|
| 1 | **Wi-Fi** | **Atual** | Conectar e manter enlace na LAN da turma |
| 2 | MQTT | Próxima | Cliente conectado ao broker da estação |
| 3 | Gerar dados | Depois | Telemetria (energia, temp., etc. — a definir) |
| 4 | Criptografia | Depois | AES-CBC e/ou HMAC-SHA256 (mbedTLS) |
| 5 | Montar pacote | Depois | Payload final ≤ **16 bytes** (protocolo da equipe) |
| 6 | Publicar | Depois | `publish` no tópico MQTT do satélite |

Alinhado ao PDF de planejamento (mensagem → proteção → montagem → publicação MQTT) e ao enunciado da Fase 1 (Wi-Fi + broker na estação, payload ≤ 16 B).

## Etapa atual: Wi-Fi

Código em `satelite/`: conecta, reconecta se cair, loga IP e RSSI no Serial.

## Como gravar no ESP32 (Arduino IDE)

1. Instale o suporte à placa **ESP32** no Arduino IDE.
2. Credenciais locais:

```bash
cd firmware-esp32/satelite
cp secrets.example.h secrets.h
# edite secrets.h com SSID e senha da rede da turma
```

3. Abra `satelite/satelite.ino`.
4. Selecione a placa ESP32 e a porta serial.
5. Compile, grave e abra o **Serial Monitor** em **115200**.

Esperado: `[WiFi] Conectado`, IP e RSSI a cada 5 s. Só avançamos para MQTT quando isso estiver estável.

## Arquivos

| Arquivo | Função |
|---------|--------|
| `satelite/satelite.ino` | Pipeline e etapa atual (Wi-Fi) |
| `satelite/config.h` | Constantes (ID, tempos) |
| `satelite/wifi_link.h` | Conectar / manter / logar Wi-Fi |
| `satelite/secrets.example.h` | Modelo de SSID/senha |
| `satelite/secrets.h` | Credenciais reais (**não commitadas**) |

## Contexto

- `CONTEXTO.md` e `docs/referencia/` (PDFs Fase 1 + planejamento HMAC/AES)
- Colaboração: `EQUIPE.md` + `CONTRIBUTING.md` + `ATUALIZACOES.md`
