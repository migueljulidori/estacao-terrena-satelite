import unittest

from mqtt_client import decode_packet, validate_command


class PacketDecoderTests(unittest.TestCase):
    def test_decodes_telemetry(self):
        packet = bytearray(16)
        packet[0:4] = bytes([1, 0x01, 7, 85])
        packet[4:6] = (253).to_bytes(2, "little", signed=True)
        packet[6:8] = (-45).to_bytes(2, "little", signed=True)
        packet[8:10] = (812).to_bytes(2, "little")
        packet[10:16] = b"Alpha\0\0"

        result = decode_packet(bytes(packet))

        self.assertTrue(result["valid"])
        self.assertEqual(result["type"], "telemetry")
        self.assertEqual(result["battery"], 85)
        self.assertEqual(result["temp_internal"], 25.3)
        self.assertEqual(result["temp_external"], -4.5)
        self.assertEqual(result["solar"], 812)
        self.assertEqual(result["name"], "Alpha")

    def test_decodes_ack(self):
        packet = bytearray(16)
        packet[0:4] = bytes([1, 0x02, 0, 0x10])
        packet[10:16] = b"SAT-01"

        result = decode_packet(bytes(packet))

        self.assertTrue(result["valid"])
        self.assertEqual(result["type"], "ack")
        self.assertEqual(result["command"], "PING")

    def test_rejects_wrong_size(self):
        result = decode_packet(b"short")
        self.assertFalse(result["valid"])

    def test_rejects_unknown_packet_type(self):
        packet = bytearray(16)
        packet[1] = 0x7F
        result = decode_packet(bytes(packet))
        self.assertFalse(result["valid"])


class CommandValidationTests(unittest.TestCase):
    def test_accepts_supported_commands(self):
        for command in ("PING", "STATUS", "NOME=Alpha"):
            payload, error = validate_command(command)
            self.assertIsNone(error)
            self.assertEqual(payload.decode("ascii"), command)

    def test_rejects_unknown_command(self):
        payload, error = validate_command("REBOOT")
        self.assertIsNone(payload)
        self.assertIsNotNone(error)

    def test_rejects_command_over_16_bytes(self):
        payload, error = validate_command("NOME=123456789012")
        self.assertIsNone(payload)
        self.assertIsNotNone(error)

    def test_rejects_empty_name(self):
        payload, error = validate_command("NOME=")
        self.assertIsNone(payload)
        self.assertIsNotNone(error)


if __name__ == "__main__":
    unittest.main()
