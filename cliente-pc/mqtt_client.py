"""
Cliente da estação terrena: MQTT bidirecional + painel web local.

- recebe e decodifica telemetria/ACK do ESP32;
- publica PING, STATUS e NOME=... no tópico de comandos;
- serve o painel e atualiza o navegador via Server-Sent Events.
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

try:
    import mqtt_config as config
except ImportError:
    print("Arquivo mqtt_config.py nao encontrado.")
    print("Copie mqtt_config.example.py para mqtt_config.py e configure o broker.")
    sys.exit(1)

BASE_DIR = Path(__file__).resolve().parent
WEBSITE_DIR = BASE_DIR / "website"
subscribers = []
subscribers_lock = threading.Lock()
mqtt_client = None


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


def decode_packet(payload):
    """Decodifica o layout provisório de 16 bytes de firmware-esp32."""
    if len(payload) != MAX_PAYLOAD_BYTES:
        return {"valid": False, "error": f"Payload deve ter {config.MAX_PAYLOAD_BYTES} bytes."}

    sat_id, kind, sequence, value = payload[:4]
    name = payload[10:16].split(b"\\0", 1)[0].decode("ascii", errors="replace")

    if kind == 0x01:
        temp_internal = int.from_bytes(payload[4:6], "little", signed=True) / 10
        temp_external = int.from_bytes(payload[6:8], "little", signed=True) / 10
        solar = int.from_bytes(payload[8:10], "little", signed=False)
        return {
            "valid": True,
            "type": "telemetry",
            "sat_id": sat_id,
            "sequence": sequence,
            "battery": value,
            "temp_internal": temp_internal,
            "temp_external": temp_external,
            "solar": solar,
            "name": name or "SAT-01",
        }

    if kind == 0x02:
        command_names = {0x10: "PING", 0x12: "STATUS", 0x13: "SET_NOME"}
        return {
            "valid": True,
            "type": "ack",
            "sat_id": sat_id,
            "sequence": sequence,
            "command": command_names.get(value, f"0x{value:02X}"),
            "name": name or "SAT-01",
        }

    return {"valid": False, "error": f"Tipo de pacote desconhecido: 0x{kind:02X}"}


def validate_command(command):
    command = command.strip()
    try:
        encoded = command.encode("ascii")
    except UnicodeEncodeError:
        return None, "Use somente caracteres ASCII no comando."
    if not encoded or len(encoded) > config.MAX_PAYLOAD_BYTES:
        return None, f"O comando deve ter entre 1 e {config.MAX_PAYLOAD_BYTES} bytes."
    upper = command.upper()
    if upper not in ("PING", "STATUS") and not upper.startswith("NOME="):
        return None, "Comandos aceitos: PING, STATUS ou NOME=nome."
    if upper.startswith("NOME=") and len(command[5:].strip()) == 0:
        return None, "Informe um nome depois de NOME=."
    if any(ord(char) < 32 or ord(char) > 126 for char in command):
        return None, "O comando contém caracteres não imprimíveis."
    return encoded, None


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

    if len(payload) > config.MAX_PAYLOAD_BYTES:
        event = {
            "event": "message",
            "valid": False,
            "topic": message.topic,
            "hex": payload.hex(" "),
            "error": f"Mensagem rejeitada: excede {config.MAX_PAYLOAD_BYTES} bytes.",
        }
        broadcast(event)
        return

    decoded = decode_packet(payload)
    broadcast({
        "event": "message",
        "topic": message.topic,
        "hex": payload.hex(" "),
        **decoded,
    })


class DashboardHandler(BaseHTTPRequestHandler):
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
            try:
                self.wfile.write(b"event: ready\\ndata: {}\\n\\n")
                self.wfile.flush()
                while True:
                    try:
                        data = channel.get(timeout=15)
                        self.wfile.write(f"data: {data}\\n\\n".encode("utf-8"))
                    except queue.Empty:
                        self.wfile.write(b": keep-alive\\n\\n")
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
        if self.path != "/api/command":
            self.send_error(404)
            return
        try:
            size = int(self.headers.get("Content-Length", "0"))
            if size <= 0 or size > 1024:
                self._json(400, {"ok": False, "error": "Requisição inválida."})
                return
            body = json.loads(self.rfile.read(size).decode("utf-8"))
            command = body.get("command", "")
            payload, error = validate_command(command)
            if error:
                self._json(400, {"ok": False, "error": error})
                return
            if mqtt_client is None or not mqtt_client.is_connected():
                self._json(503, {"ok": False, "error": "Cliente MQTT não está conectado."})
                return
            result = mqtt_client.publish(TOPIC_COMMAND, payload, qos=0, retain=False)
            if result.rc != mqtt.MQTT_ERR_SUCCESS:
                self._json(503, {"ok": False, "error": "Falha ao publicar o comando MQTT."})
                return
            print(f"TX {config.TOPIC_COMMAND} | {payload.decode('ascii')}")
            broadcast({"event": "command", "command": payload.decode("ascii"), "sent": True})
            self._json(200, {"ok": True, "topic": config.TOPIC_COMMAND, "command": payload.decode("ascii")})
        except (ValueError, UnicodeDecodeError, json.JSONDecodeError):
            self._json(400, {"ok": False, "error": "JSON inválido."})

    def log_message(self, format, *args):
        # Mantém o terminal focado nos eventos MQTT.
        return


def main():
    global mqtt_client
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
    server = ThreadingHTTPServer((WEB_HOST, WEB_PORT), DashboardHandler)
    print(f"Painel: http://{config.WEB_HOST}:{config.WEB_PORT}")
    print(f"Publisher de comandos: {config.TOPIC_COMMAND}")
    print("Pressione Ctrl+C para encerrar.")

    try:
        server.serve_forever()
    except KeyboardInterrupt:
        print("\\nEncerrando...")
    finally:
        server.shutdown()
        server.server_close()
        mqtt_client.loop_stop()
        mqtt_client.disconnect()


if __name__ == "__main__":
    main()
