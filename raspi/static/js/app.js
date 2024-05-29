const host = window.location.host;
const port = 9001;
const client = mqtt.connect(`mqtt://${host}:${port}`);

const vooruit = document.getElementById("vooruit");
const achteruit = document.getElementById("achteruit");
const links = document.getElementById("links");
const rechts = document.getElementById("rechts");
const stopp = document.getElementById("stop");
const sirene = document.getElementById("sirene");
const servoInput = document.getElementById("servoinput");
const servoInput2 = document.getElementById("servoinput2");
const servoValueDisplay = document.getElementById("servoValueDisplay");
const servoValueDisplay2 = document.getElementById("servoValueDisplay2");
const modeToggle = document.getElementById("modeToggle");
const ctx = document.getElementById('myChart');
const startOpnemen = document.getElementById("startOpnemen");
const stopOpnemen = document.getElementById("stopOpnemen");
const savedRoutesContainer = document.getElementById("savedRoutesContainer");

let check = "";
let msg = 0;
const controls = document.getElementById("controls");
let isOpnemen = false;
let route = [];
let lastAction = null;
let routeName = "";

// MQTT Connect
client.on("connect", () => {
    client.subscribe("real_unique_topic_2", (err) => {
        if (!err) {
            client.publish("presence", "Hello mqtt");
        }
    });
});

// Graph
document.addEventListener('DOMContentLoaded', (event) => {
    toggleCheck();
    const ctx = document.getElementById('myChart').getContext('2d');
    const vochtigheidData = {
        labels: [],
        datasets: [{
            label: 'Vochtigheid (%)',
            data: [],
            backgroundColor: 'rgba(54, 162, 235, 0.2)',
            borderColor: 'rgba(54, 162, 235, 1)',
            borderWidth: 1
        }]
    };

    const vochtigheidGrafiek = new Chart(ctx, {
        type: 'line',
        data: vochtigheidData,
        options: {
            scales: {
                x: {
                    type: 'time',
                    time: {
                        unit: 'minute'
                    },
                    title: {
                        display: true,
                        text: 'Tijd'
                    }
                },
                y: {
                    beginAtZero: true,
                    title: {
                        display: true,
                        text: 'Vochtigheid (%)'
                    }
                }
            }
        }
    });

    function voegVochtigheidToe(waarde) {
        const tijdstip = new Date();
        vochtigheidGrafiek.data.labels.push(tijdstip);
        vochtigheidGrafiek.data.datasets[0].data.push(waarde);
        vochtigheidGrafiek.update();
    }

    client.on("message", (topic, message) => {
        if (topic === "real_unique_topic_2") {
            msg = parseInt(message.toString());
            voegVochtigheidToe(msg);

            if (check == "auto" && msg <= 50) {
                client.publish("topic", "motor/plant1");
                console.log("auto + droog");
            } else if (check == "auto" && msg > 50) {
                console.log("auto + nat");
            } else if (check == "manual" && msg <= 50) {
                console.log("manual + droog");
            } else if (check == "manual" && msg > 50) {
                console.log("manual + nat");
            }
        }
    });

    loadSavedRoutes();
});

modeToggle.addEventListener("change", function() {
    toggleCheck();
});

startOpnemen.addEventListener("click", () => {
    routeName = prompt("Geef een naam voor de route:");
    if (routeName) {
        startRouteOpnemen();
    }
});
stopOpnemen.addEventListener("click", stopRouteOpnemen);

function startRouteOpnemen() {
    isOpnemen = true;
    route = [];
    lastAction = null; // Reset last action when starting a new recording
    startOpnemen.classList.add("hidden");
    stopOpnemen.classList.remove("hidden");
    console.log("Opnemen gestart");
}

function stopRouteOpnemen() {
    isOpnemen = false;
    startOpnemen.classList.remove("hidden");
    stopOpnemen.classList.add("hidden");
    if (routeName) {
        saveRoute(routeName, route);
    }
    console.log("Opnemen gestopt");
    console.log("Opgenomen route:", route);
}

function recordAction(action) {
    if (isOpnemen && action !== lastAction) {
        const timestamp = new Date().getTime();
        route.push({ action, timestamp });
        lastAction = action;
    }
}

function routeUitvoeren(route) {
    if (route.length === 0) {
        console.log("Geen route opgenomen");
        return;
    }
    console.log("Uitvoeren van route:", route);

    let index = 0;

    function executeNextAction() {
        if (index < route.length) {
            const { action, timestamp } = route[index];
            console.log("Uitvoeren actie:", action);

            switch (action) {
                case "vooruit":
                    vooruitf();
                    break;
                case "achteruit":
                    achteruitf();
                    break;
                case "links":
                    linksf();
                    break;
                case "rechts":
                    rechtsf();
                    break;
                case "stop":
                    stopf();
                    break;
                case "sirene":
                    sirene.click();
                    break;
            }

            index++;
            if (index < route.length) {
                const nextTimestamp = route[index].timestamp;
                const delay = nextTimestamp - timestamp;
                setTimeout(executeNextAction, delay);
            } else {
                console.log("Route voltooid");
            }
        }
    }

    executeNextAction();
}

function saveRoute(name, data) {
    const routes = JSON.parse(localStorage.getItem("routes")) || [];
    const existingRouteIndex = routes.findIndex(route => route.name === name);
    if (existingRouteIndex !== -1) {
        routes[existingRouteIndex].data = data;
    } else {
        routes.push({ name, data });
    }
    localStorage.setItem("routes", JSON.stringify(routes));
    window.location.reload(); // Reload the page to show the new route button
}

function loadSavedRoutes() {
    const routes = JSON.parse(localStorage.getItem("routes")) || [];
    savedRoutesContainer.innerHTML = '';
    routes.forEach(route => {
        const routeContainer = document.createElement('div');
        routeContainer.classList.add('route-container');
        
        const button = document.createElement('button');
        button.textContent = route.name;
        button.addEventListener('click', () => routeUitvoeren(route.data));

        const editButton = document.createElement('button');
        editButton.textContent = "Bewerken";
        editButton.addEventListener('click', () => editRoute(route.name));

        routeContainer.appendChild(button);
        routeContainer.appendChild(editButton);
        savedRoutesContainer.appendChild(routeContainer);
    });
}

function editRoute(name) {
    const routes = JSON.parse(localStorage.getItem("routes")) || [];
    const route = routes.find(route => route.name === name);
    if (route) {
        routeName = name;
        route.data = [];
        isOpnemen = true;
        startOpnemen.classList.add("hidden");
        stopOpnemen.classList.remove("hidden");
        console.log(`Opnemen gestart voor route: ${name}`);
    }
}

var mcVooruit = new Hammer(vooruit);
var mcAchteruit = new Hammer(achteruit);
var mcLinks = new Hammer(links);
var mcRechts = new Hammer(rechts);

function vooruitf() {
    client.publish("topic", "motor/vooruit");
    console.log("Vooruit");
}

function achteruitf() {
    client.publish("topic", "motor/achteruit");
    console.log("Achteruit");
}

function linksf() {
    client.publish("topic", "motor/links");
    console.log("Links");
}

function rechtsf() {
    client.publish("topic", "motor/rechts");
    console.log("Rechts");
}

function stopf() {
    client.publish("topic", "motor/stop");
    console.log("Stop");
}

vooruit.addEventListener("mousedown", () => { vooruitf(); recordAction("vooruit"); });
vooruit.addEventListener("mouseup", () => { stopf(); recordAction("stop"); });
mcVooruit.on("press", () => { vooruitf(); recordAction("vooruit"); });
mcVooruit.on("pressup", () => { stopf(); recordAction("stop"); });

achteruit.addEventListener("mousedown", () => { achteruitf(); recordAction("achteruit"); });
achteruit.addEventListener("mouseup", () => { stopf(); recordAction("stop"); });
mcAchteruit.on("press", () => { achteruitf(); recordAction("achteruit"); });
mcAchteruit.on("pressup", () => { stopf(); recordAction("stop"); });

links.addEventListener("mousedown", () => { linksf(); recordAction("links"); });
links.addEventListener("mouseup", () => { stopf(); recordAction("stop"); });
mcLinks.on("press", () => { linksf(); recordAction("links"); });
mcLinks.on("pressup", () => { stopf(); recordAction("stop"); });

rechts.addEventListener("mousedown", () => { rechtsf(); recordAction("rechts"); });
rechts.addEventListener("mouseup", () => { stopf(); recordAction("stop"); });
mcRechts.on("press", () => { rechtsf(); recordAction("rechts"); });
mcRechts.on("pressup", () => { stopf(); recordAction("stop"); });

stopp.addEventListener("click", function() {
    client.publish("topic", "motor/stop");
    console.log("Stop");
    recordAction("stop");
});

sirene.addEventListener("click", function() {
    client.publish("topic", "motor/sirene");
    console.log("Sirene");
    recordAction("sirene");
});

servoInput.addEventListener("input", function() {
    const servoValue = parseInt(servoInput.value);
    if (servoValue >= 0 && servoValue <= 180) {
        client.publish("topic", `servo/${servoValue}`);
        console.log(servoValue);
        servoValueDisplay.textContent = servoValue;
        recordAction(`servo/${servoValue}`);
    }
});

servoInput2.addEventListener("input", function() {
    const servoValue2 = parseInt(servoInput2.value);
    if (servoValue2 >= 0 && servoValue2 <= 180) {
        client.publish("topic", `servo/${servoValue2}`);
        console.log(servoValue2);
        servoValueDisplay2.textContent = servoValue2;
        recordAction(`servo/${servoValue2}`);
    }
});

function toggleCheck() {
    if (modeToggle.checked) {
        check = "auto";
        console.log("Auto");
        controls.classList.add("hidden");
    } else {
        check = "manual";
        console.log("Manual");
        controls.classList.remove("hidden");
    }
}
