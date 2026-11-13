"""Espelho Python do contrato AES-128-CBC + HMAC-SHA256 do firmware ESP32.

O formato e o IV derivado seguem docs/protocolo/README.md da branch main.
Este módulo não armazena chaves: elas vêm do mqtt_config.py local (ignorado pelo Git).
"""
import hashlib
import hmac

from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes

PACKET_SIZE = 16
DATA_SIZE = 6
MAC_SIZE = 8
TYPE_TELEMETRY = 0x01
TYPE_ACK = 0x02
TYPE_PING = 0x10
TYPE_STATUS = 0x12
TYPE_SET_NAME = 0x13


class CryptoError(ValueError):
    """Pacote criptográfico inválido ou incompatível."""


def _validate_key(key, label):
    if not isinstance(key, bytes) or len(key) != 16:
        raise CryptoError(f"A chave {label} precisa ter exatamente 16 bytes ASCII.")
    return key


def derive_iv(aes_key):
    aes_key = _validate_key(aes_key, "AES")
    return hashlib.sha256(b"SAT-IV" + aes_key).digest()[:16]


def _mac(data, hmac_key):
    hmac_key = _validate_key(hmac_key, "HMAC")
    return hmac.new(hmac_key, data, hashlib.sha256).digest()[:MAC_SIZE]


def seal_packet(kind, sequence, data, aes_key, hmac_key):
    """Monta tipo|seq|6B dados|8B HMAC e cifra o bloco AES-CBC de 16 bytes."""
    if len(data) != DATA_SIZE:
        raise CryptoError("O campo de dados precisa ter exatamente 6 bytes.")
    if not 0 <= kind <= 255 or not 0 <= sequence <= 255:
        raise CryptoError("Tipo e sequência precisam caber em um byte.")
    clear_head = bytes((kind, sequence)) + bytes(data)
    clear_packet = clear_head + _mac(clear_head, hmac_key)
    encryptor = Cipher(
        algorithms.AES(_validate_key(aes_key, "AES")),
        modes.CBC(derive_iv(aes_key)),
    ).encryptor()
    ciphertext = encryptor.update(clear_packet) + encryptor.finalize()
    if len(ciphertext) != PACKET_SIZE:
        raise CryptoError("Falha interna: ciphertext não tem 16 bytes.")
    return ciphertext


def open_packet(ciphertext, aes_key, hmac_key):
    """Decifra e valida HMAC antes de devolver tipo, sequência e dados."""
    if len(ciphertext) != PACKET_SIZE:
        raise CryptoError("Ciphertext deve ter exatamente 16 bytes.")
    decryptor = Cipher(
        algorithms.AES(_validate_key(aes_key, "AES")),
        modes.CBC(derive_iv(aes_key)),
    ).decryptor()
    clear = decryptor.update(ciphertext) + decryptor.finalize()
    head, received_mac = clear[:8], clear[8:16]
    expected_mac = _mac(head, hmac_key)
    if not hmac.compare_digest(received_mac, expected_mac):
        raise CryptoError("HMAC inválido: mensagem rejeitada.")
    return clear[0], clear[1], clear[2:8]
