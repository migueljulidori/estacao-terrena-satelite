(() => {
    const $ = (id) => document.getElementById(id);
    let messageCount = 0;

    function setConnection(connected, message) {
        $("connection-text").textContent = connected ? "CONECTADO AO BROKER" : "DESCONECTADO";
        $("connection-dot").style.background = connected ? "var(--green)" : "var(--yellow)";
        $("payload-detail").textContent = message || (connected ? "Conexão MQTT ativa." : "Aguardando conexão MQTT.");
    }

    function formatTemperature(value) {
        return `${Number(value).toFixed(1).replace(".", ",")} °C`;
    }

    function updateTelemetry(data) {
        $("battery").textContent = `${data.battery}%`;
        $("temp-in").textContent = formatTemperature(data.temp_internal);
        $("temp-out").textContent = formatTemperature(data.temp_external);
        $("solar").textContent = data.solar.toLocaleString("pt-BR");

        $("table-battery").textContent = `${data.battery}%`;
        $("table-temp-in").textContent = formatTemperature(data.temp_internal);
        $("table-temp-out").textContent = formatTemperature(data.temp_external);
        $("table-solar").textContent = `${data.solar.toLocaleString("pt-BR")} W/m²`;
        $("battery-bar").style.width = `${Math.max(0, Math.min(100, data.battery))}%`;

        $("payload-kind").textContent = `TELEMETRIA RECEBIDA — ${data.name}`;
        $("payload-detail").textContent = `Satélite ${data.sat_id} | sequência ${data.sequence}`;
    }

    function handleMessage(data) {
        messageCount += 1;
        $("messages").textContent = messageCount;
        $("last-update").textContent = new Date().toLocaleTimeString("pt-BR");

        $("payload").textContent = data.hex || "Sem payload";
        if (!data.valid) {
            $("payload-kind").textContent = "MENSAGEM REJEITADA";
            $("payload-detail").textContent = data.error || "Formato inválido.";
            return;
        }
        if (data.type === "telemetry") {
            updateTelemetry(data);
        } else if (data.type === "ack") {
            $("payload-kind").textContent = "ACK RECEBIDO";
            $("payload-detail").textContent = `Comando confirmado: ${data.command} | satélite: ${data.name}`;
        }
    }

    async function sendCommand(command) {
        const feedback = $("command-feedback");
        feedback.textContent = `Enviando: ${command}...`;
        try {
            const response = await fetch("/api/command", {
                method: "POST",
                headers: { "Content-Type": "application/json" },
                body: JSON.stringify({ command })
            });
            const result = await response.json();
            if (!response.ok || !result.ok) {
                throw new Error(result.error || "Não foi possível enviar o comando.");
            }
            feedback.textContent = `Publicado em ${result.topic}: ${result.command}`;
        } catch (error) {
            feedback.textContent = `Falha: ${error.message}`;
        }
    }

    document.querySelectorAll("[data-command]").forEach((button) => {
        button.addEventListener("click", () => sendCommand(button.dataset.command));
    });

    $("name-form").addEventListener("submit", (event) => {
        event.preventDefault();
        const name = $("satellite-name").value.trim();
        if (!name) return;
        sendCommand(`NOME=${name}`);
    });

    const events = new EventSource("/events");
    events.onmessage = (event) => {
        try {
            const data = JSON.parse(event.data);
            if (data.event === "connection") {
                setConnection(data.connected, data.message);
            } else if (data.event === "message") {
                handleMessage(data);
            } else if (data.event === "command" && data.sent) {
                $("command-feedback").textContent = `Comando publicado: ${data.command}`;
            }
        } catch (error) {
            console.error("Evento inválido recebido do cliente MQTT:", error);
        }
    };
    events.onerror = () => setConnection(false, "Conexão com o serviço local interrompida.");

    function updateClock() {
        $("clock").textContent = new Date().toLocaleString("pt-BR");
    }
    updateClock();
    setInterval(updateClock, 1000);
})();
