const host = window.location.host;
const port = 9001;
const messages = document.getElementById("messages");
const client = mqtt.connect(`mqtt://${host}:${port}`);

client.on("connect", () => {
  client.subscribe("topic", (err) => {
    if (!err) {
      client.publish("presence", "Hello mqtt");
    }
  });
});

// client.on("message", (topic, message) => {
//   const msg = message.toString();
// const time = new Date().toLocaleTimeString();
// const li = document.createElement("li");
// li.textContent = `${time} - ${msg}`;
// messages.prepend(li);
//});

const vooruit = document.getElementById("vooruit");
const achteruit = document.getElementById("achteruit");
const links = document.getElementById("links");
const rechts = document.getElementById("rechts");
const stopp = document.getElementById("stop");
const sirene = document.getElementById("sirene");

vooruit.addEventListener("click", function () {
  client.publish("topic", "vooruit");
  console.log("Vooruit");
});

achteruit.addEventListener("click", function () {
  client.publish("topic", "achteruit");
  console.log("Achteruit");
});
links.addEventListener("click", function () {
  client.publish("topic", "links");
  console.log("Links");
});
rechts.addEventListener("click", function () {
  client.publish("topic", "rechts");
  console.log("Rechts");
});
stopp.addEventListener("click", function () {
  client.publish("topic", "stop");
  console.log("Stop");
});
sirene.addEventListener("click", function () {
  client.publish("topic", "sirene");
  console.log("Sirene");
});
