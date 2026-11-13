# Firmware ESP32 — Miguel

ESP e estação usam o **mesmo** esquema: HMAC + AES-CBC, payload **16 B**.  
Contrato completo: `docs/protocolo/README.md` (para a Manuela espelhar no PC).

## Arquivos

| Arquivo | Função |
|---------|--------|
| `cripto_mod.h` | assinar / verificar / cifrar / decifrar |
| `dados_mod.h` | telemetria + ACK cifrados |
| `comando_mod.h` | RX: abre pacote e aplica comando |
| `mqtt_mod.h` / `wifi_mod.h` | enlace |
| `satelite.ino` | orquestra |

## Bibliotecas

- PubSubClient  
- mbedTLS (já no ESP32)

## Teste rápido

Grave o sketch → Serial: `TX telemetria (HMAC+AES)`.  
No broker a mensagem **não** é legível sem as chaves.

Comando em claro (transição): `NOME=Alpha` ainda funciona com aviso.  
Comando definitivo: PC manda 16 B já cifrados+HMAC.
