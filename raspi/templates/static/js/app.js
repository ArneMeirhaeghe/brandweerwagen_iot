document.addEventListener('DOMContentLoaded', function() {
    const host = window.location.host;
    const port = 9001; // Ensure this port is open for WebSocket connections on your MQTT broker
    const messages = document.getElementById("messages");
    const client = mqtt.connect(`wss://${host}:${port}`); // Using WebSocket Secure (wss)

    client.on("connect", () => {
        console.log("Connected to MQTT Broker!");
        client.subscribe("arduino/jannesiscool", (err) => {
            if (!err) {
                client.publish("presence", "Hello mqtt");
            } else {
                console.error("Subscribe error:", err);
            }
        });
    });

    client.on("message", (topic, message) => {
        const msg = message.toString();
        const time = new Date().toLocaleTimeString();
        const li = document.createElement("li");
        li.textContent = `${time} - ${msg}`;
        messages.prepend(li); // This ensures the latest message is always at the top
    });

    client.on("error", function (err) {
        console.error('Connection to MQTT Broker failed:', err);
    });
});
