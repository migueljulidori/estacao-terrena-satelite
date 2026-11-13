import unittest

from crypto_protocol import (
    CryptoError, TYPE_ACK, TYPE_PING, TYPE_SET_NAME, TYPE_STATUS,
    TYPE_TELEMETRY, open_packet, seal_packet,
)
from mqtt_client import decode_packet, validate_command

AES = b"TEST-KEY-AES-001"
HMAC = b"TEST-KEY-HMAC001"


class CryptoProtocolTests(unittest.TestCase):
    def test_encrypt_decrypt_round_trip(self):
        data = bytes([87, 0xF5, 0x00, 0xB6, 0x00, 187])
        encrypted = seal_packet(TYPE_TELEMETRY, 9, data, AES, HMAC)
        self.assertEqual(len(encrypted), 16)
        self.assertEqual(open_packet(encrypted, AES, HMAC), (TYPE_TELEMETRY, 9, data))

    def test_rejects_modified_ciphertext(self):
        encrypted = bytearray(seal_packet(TYPE_PING, 1, bytes(6), AES, HMAC))
        encrypted[7] ^= 0x01
        with self.assertRaises(CryptoError):
            open_packet(bytes(encrypted), AES, HMAC)

    def test_rejects_wrong_hmac_key(self):
        encrypted = seal_packet(TYPE_STATUS, 2, bytes(6), AES, HMAC)
        with self.assertRaises(CryptoError):
            open_packet(encrypted, AES, b"OTHER-HMAC-KEY!!")

    def test_rejects_wrong_length(self):
        with self.assertRaises(CryptoError):
            open_packet(b"short", AES, HMAC)

    def test_rejects_invalid_key_length(self):
        with self.assertRaises(CryptoError):
            seal_packet(TYPE_PING, 0, bytes(6), b"short", HMAC)

    def test_decoder_interprets_telemetry_after_authentication(self):
        data = bytes([85]) + (253).to_bytes(2, "little", signed=True)
        data += (-45).to_bytes(2, "little", signed=True) + bytes([203])
        packet = seal_packet(TYPE_TELEMETRY, 7, data, AES, HMAC)
        result = decode_packet(packet, AES, HMAC)
        self.assertTrue(result["valid"])
        self.assertEqual(result["battery"], 85)
        self.assertEqual(result["temp_internal"], 25.3)
        self.assertEqual(result["temp_external"], -4.5)
        self.assertEqual(result["solar"], 812)

    def test_decoder_rejects_invalid_hmac(self):
        packet = seal_packet(TYPE_TELEMETRY, 7, bytes(6), AES, HMAC)
        wrong_key_result = decode_packet(packet, AES, b"OTHER-HMAC-KEY!!")
        self.assertFalse(wrong_key_result["valid"])


class CommandTests(unittest.TestCase):
    def test_supported_commands_are_converted_to_binary_protocol(self):
        self.assertEqual(validate_command("PING"), (TYPE_PING, bytes(6), None))
        self.assertEqual(validate_command("STATUS"), (TYPE_STATUS, bytes(6), None))
        self.assertEqual(validate_command("NOME=Alpha"), (TYPE_SET_NAME, b"Alpha\0", None))

    def test_name_limit_is_six_ascii_bytes(self):
        kind, data, error = validate_command("NOME=ABCDEF")
        self.assertEqual(kind, TYPE_SET_NAME)
        self.assertEqual(data, b"ABCDEF")
        self.assertIsNone(error)
        self.assertIsNotNone(validate_command("NOME=ABCDEFG")[2])

    def test_rejects_non_ascii_and_unknown_commands(self):
        self.assertIsNotNone(validate_command("NOME=Olá")[2])
        self.assertIsNotNone(validate_command("REBOOT")[2])


if __name__ == "__main__":
    unittest.main()
