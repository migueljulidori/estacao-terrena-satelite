#ifndef CONFIG_H
#define CONFIG_H

// Identificação do satélite (ajustar quando a equipe definir o protocolo)
static const char *SATELITE_ID = "SAT-01";

// Serial
static const unsigned long SERIAL_BAUD = 115200;

// Wi-Fi
static const unsigned long WIFI_RETRY_MS = 500;
static const unsigned long WIFI_STATUS_INTERVAL_MS = 5000;

// Próximas etapas (ainda não usadas nesta versão):
// - MQTT broker da estação terrena
// - Tópico de telemetria / telecomando
// - Chaves HMAC / AES (nunca hardcodar em repositório público de notas)

#endif
