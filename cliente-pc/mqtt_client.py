"""
Cliente da estação terrena: MQTT bidirecional, AES/HMAC e painel web.

O contrato criptográfico espelha firmware-esp32/satelite/cripto_mod.h da main:
16 bytes de ciphertext = AES-128-CBC(tipo|seq|dados[6]|HMAC[8]).
"""
import json
import mimetypes
import queue
import sys
import threading
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer
from pathlib import Path
from urllib.parse import unquote

import paho.mqtt.client as mqtt

from crypto_protocol import (
    CryptoError, TYPE_ACK, TYPE_PING, TYPE_SET_NAME, TYPE_STATUS,
    TYPE_TELEMETRY, open_packet, seal_packet,
)

try:
    import mqtt_config as config
except ImportError:
    config = None

BASE_DIR = Path(__file__).resolve().parent
WEBSITE_DIR = BASE_DIR / "website"
TOPIC_COMMAND = getattr(config, "TOPIC_COMMAND", "satellite/esp32-01/command")
WEB_HOST = getattr(config, "WEB_HOST", "127.0.0.1")
WEB_PORT = getattr(config, "WEB_PORT", 8765)
MAX_PAYLOAD_BYTES = getattr(config, "MAX_PAYLOAD_BYTES", 16)
AES_KEY = getattr(config, "AES_KEY", "").encode("ascii") if config else b""
HMAC_KEY = getattr(config, "HMAC_KEY", "").encode("ascii") if config else b""
subscribers = []
subscribers_lock = threading.Lock()
mqtt_client = None
command_sequence = 0


def broadcast(event):
    data = json.dumps(event, ensure_ascii=False)
    with subscribers_lock:
        targets = list(subscribers)
    for subscriber in targets:
        try:
            subscriber.put_nowait(data)
        except queue.Full:
            try:
                subscriber.get_nowait()
                subscriber.put_nowait(data)
            except queue.Empty:
                pass


def decode_packet(payload, aes_key=None, hmac_key=None):
    """Decifra AES-CBC e valida HMAC antes de interpretar os campos."""
    aes_key = AES_KEY if aes_key is None else aes_key
    hmac_key = HMAC_KEY if hmac_key is None else hmac_key
    if len(payload) != MAX_PAYLOAD_BYTES:
        return {"valid": False, "error": f"Ciphertext deve ter {MAX_PAYLOAD_BYTES} bytes."}
    try:
        kind, sequence, data = open_packet(payload, aes_key, hmac_key)
    except CryptoError as error:
        return {"valid": False, "error": str(error)}

    if kind == TYPE_TELEMETRY:
        battery = data[0]
        temp_internal = int.from_bytes(data[1:3], "little", signed=True) / 10
        temp_external = int.from_bytes(data[3:5], "little", signed=True) / 10
        solar = data[5] * 4
        return {
            "valid": True, "type": "telemetry", "sat_id": 1,
            "sequence": sequence, "battery": battery,
            "temp_internal": temp_internal, "temp_external": temp_external,
            "solar": solar, "name": "SAT-01",
            "security": "AES-128-CBC + HMAC-SHA256 OK",
        }

    if kind == TYPE_ACK:
        command_names = {TYPE_PING: "PING", TYPE_STATUS: "STATUS", TYPE_SET_NAME: "SET_NOME"}
        name = data[1:6].split(b"\0", 1)[0].decode("ascii", errors="replace")
        return {
            "valid": True, "type": "ack", "sat_id": 1, "sequence": sequence,
            "command": command_names.get(data[0], f"0x{data[0]:02X}"),
            "name": name or "SAT-01",
            "security": "AES-128-CBC + HMAC-SHA256 OK",
        }

    return {"valid": False, "error": f"Tipo de pacote desconhecido: 0x{kind:02X}"}


def validate_command(command):
    """Converte o texto da interface para tipo e seis bytes de dados do protocolo."""
    command = command.strip()
    upper = command.upper()
    if upper == "PING":
        return TYPE_PING, bytes(6), None
    if upper == "STATUS":
        return TYPE_STATUS, bytes(6), None
    if upper.startswith("NOME="):
        name = command[5:].strip()
        try:
            encoded = name.encode("ascii")
        except UnicodeEncodeError:
            return None, None, "O nome deve usar somente caracteres ASCII."
        if not encoded:
            return None, None, "Informe um nome depois de NOME=."
        if len(encoded) > 6:
            return None, None, "O protocolo binário permite nomes de até 6 caracteres."
        if any(byte < 32 or byte > 126 for byte in encoded):
            return None, None, "O nome contém caracteres não imprimíveis."
        return TYPE_SET_NAME, encoded.ljust(6, b"\0"), None
    return None, None, "Comandos aceitos: PING, STATUS ou NOME=nome (até 6 caracteres)."


def on_connect(client, userdata, flags, reason_code, properties):
    if reason_code == 0:
        print(f"Conectado ao broker {config.BROKER_HOST}:{config.BROKER_PORT}")
        client.subscribe(config.TOPIC_TELEMETRY, qos=0)
        print(f"Inscrito (RX): {config.TOPIC_TELEMETRY}")
        broadcast({"event": "connection", "connected": True, "message": "Conectado ao broker MQTT"})
    else:
        message = f"Falha na conexão MQTT. Código: {reason_code}"
        print(message)
        broadcast({"event": "connection", "connected": False, "message": message})


def on_disconnect(client, userdata, disconnect_flags, reason_code, properties):
    broadcast({"event": "connection", "connected": False, "message": "Desconectado do broker MQTT"})


def on_message(client, userdata, message):
    payload = message.payload
    print(f"RX {message.topic} | {len(payload)} byte(s) | {payload.hex(' ')}")
    decoded = decode_packet(payload)
    if not decoded["valid"]:
        print(f"Mensagem rejeitada: {decoded['error']}")
    else:
        print(f"Mensagem autenticada e decifrada: {decoded.get('type')} seq={decoded.get('sequence')}")
    broadcast({
        "event": "message", "topic": message.topic,
        "hex": payload.hex(" "), **decoded,
    })


class DashboardHandler(BaseHTTPRequestHandler):
    protocol_version = "HTTP/1.1"

    def _json(self, status, body):
        data = json.dumps(body, ensure_ascii=False).encode("utf-8")
        self.send_response(status)
        self.send_header("Content-Type", "application/json; charset=utf-8")
        self.send_header("Content-Length", str(len(data)))
        self.end_headers()
        self.wfile.write(data)

    def do_GET(self):
        if self.path == "/events":
            self.send_response(200)
            self.send_header("Content-Type", "text/event-stream; charset=utf-8")
            self.send_header("Cache-Control", "no-cache")
            self.send_header("Connection", "keep-alive")
            self.end_headers()
            channel = queue.Queue(maxsize=30)
            with subscribers_lock:
                subscribers.append(channel)
            connected = mqtt_client is not None and mqtt_client.is_connected()
            channel.put_nowait(json.dumps({
                "event": "connection", "connected": connected,
                "message": "Conectado ao broker MQTT" if connected else "Aguardando conexão MQTT",
            }, ensure_ascii=False))
            try:
                self.wfile.write(b"event: ready\ndata: {}\n\n")
                self.wfile.flush()
                while True:
                    try:
                        data = channel.get(timeout=15)
                        self.wfile.write(f"data: {data}\n\n".encode("utf-8"))
                    except queue.Empty:
                        self.wfile.write(b": keep-alive\n\n")
                    self.wfile.flush()
            except (BrokenPipeError, ConnectionResetError):
                pass
            finally:
                with subscribers_lock:
                    if channel in subscribers:
                        subscribers.remove(channel)
            return

        relative = unquote(self.path.split("?", 1)[0]).lstrip("/") or "index.html"
        target = (WEBSITE_DIR / relative).resolve()
        if WEBSITE_DIR.resolve() not in target.parents and target != WEBSITE_DIR.resolve():
            self.send_error(403)
            return
        if not target.is_file():
            self.send_error(404)
            return
        data = target.read_bytes()
        content_type = mimetypes.guess_type(target.name)[0] or "application/octet-stream"
        if content_type.startswith("text/") or target.suffix in (".js", ".css"):
            content_type += "; charset=utf-8"
        self.send_response(200)
        self.send_header("Content-Type", content_type)
        self.send_header("Content-Length", str(len(data)))
        self.end_headers()
        self.wfile.write(data)

    def do_POST(self):
        global command_sequence
        if self.path != "/api/command":
            self.send_error(404)
            return
        try:
            size = int(self.headers.get("Content-Length", "0"))
            if size <= 0 or size > 1024:
                self._json(400, {"ok": False, "error": "Requisição inválida."})
                return
            body = json.loads(self.rfile.read(size).decode("utf-8"))
            kind, data, error = validate_command(body.get("command", ""))
            if error:
                self._json(400, {"ok": False, "error": error})
                return
            if mqtt_client is None or not mqtt_client.is_connected():
                self._json(503, {"ok": False, "error": "Cliente MQTT não está conectado."})
                return
            ciphertext = seal_packet(kind, command_sequence, data, AES_KEY, HMAC_KEY)
            result = mqtt_client.publish(TOPIC_COMMAND, ciphertext, qos=0, retain=False)
            if result.rc != mqtt.MQTT_ERR_SUCCESS:
                self._json(503, {"ok": False, "error": "Falha ao publicar o comando MQTT."})
                return
            sequence_sent = command_sequence
            command_sequence = (command_sequence + 1) & 0xFF
            command_label = {TYPE_PING: "PING", TYPE_STATUS: "STATUS", TYPE_SET_NAME: "SET_NOME"}[kind]
            print(f"TX {TOPIC_COMMAND} | AES+HMAC | seq={sequence_sent} | {ciphertext.hex(' ')}")
            broadcast({"event": "command", "command": command_label, "sent": True})
            self._json(200, {"ok": True, "topic": TOPIC_COMMAND, "command": command_label})
        except (ValueError, UnicodeDecodeError, json.JSONDecodeError, CryptoError) as error:
            self._json(400, {"ok": False, "error": str(error) or "JSON inválido."})

    def log_message(self, format, *args):
        return


class DashboardServer(ThreadingHTTPServer):
    daemon_threads = True


def main():
    global mqtt_client
    if config is None:
        print("Arquivo mqtt_config.py não encontrado.")
        print("Copie mqtt_config.example.py para mqtt_config.py e configure o broker e as chaves.")
        sys.exit(1)
    if len(AES_KEY) != 16 or len(HMAC_KEY) != 16:
        print("Configure AES_KEY e HMAC_KEY com exatamente 16 caracteres ASCII em mqtt_config.py.")
        sys.exit(1)

    mqtt_client = mqtt.Client(
        mqtt.CallbackAPIVersion.VERSION2,
        client_id=config.CLIENT_ID,
        protocol=mqtt.MQTTv311,
    )
    mqtt_client.username_pw_set(config.MQTT_USERNAME, config.MQTT_PASSWORD)
    mqtt_client.on_connect = on_connect
    mqtt_client.on_disconnect = on_disconnect
    mqtt_client.on_message = on_message

    print(f"Conectando ao broker {config.BROKER_HOST}:{config.BROKER_PORT}...")
    try:
        mqtt_client.connect(config.BROKER_HOST, config.BROKER_PORT, keepalive=30)
    except OSError as error:
        print(f"Não foi possível conectar ao broker: {error}")
        sys.exit(1)

    mqtt_client.loop_start()
    server = DashboardServer((WEB_HOST, WEB_PORT), DashboardHandler)
    print(f"Painel: http://{WEB_HOST}:{WEB_PORT}")
    print(f"Publisher criptográfico: {TOPIC_COMMAND}")
    print("Pressione Ctrl+C para encerrar.")

    try:
        server.serve_forever()
    except KeyboardInterrupt:
        print("\nEncerrando...")
    finally:
        server.shutdown()
        server.server_close()
        mqtt_client.loop_stop()
        mqtt_client.disconnect()


if __name__ == "__main__":
    main()
