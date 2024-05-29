//var
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
const ctx = document.getElementById("myChart");
let check = "";
let msg = 0;
const controls = document.getElementById("controls");

//mqtt read
client.on("connect", () => {
  client.subscribe("real_unique_topic_2", (err) => {
    if (!err) {
      client.publish("presence", "Hello mqtt");
    }
  });
});
//graph
document.addEventListener("DOMContentLoaded", (event) => {
  // Initialiseer de Chart.js grafiek
  toggleCheck();
  const ctx = document.getElementById("myChart").getContext("2d");
  const vochtigheidData = {
    labels: [], // Tijdstippen waarop de waarden gemeten worden
    datasets: [
      {
        label: "Vochtigheid (%)",
        data: [], // Vochtigheidswaarden
        backgroundColor: "rgba(54, 162, 235, 0.2)",
        borderColor: "rgba(54, 162, 235, 1)",
        borderWidth: 1,
      },
    ],
  };

  const vochtigheidGrafiek = new Chart(ctx, {
    type: "line",
    data: vochtigheidData,
    options: {
      scales: {
        x: {
          type: "time",
          time: {
            unit: "minute",
          },
          title: {
            display: true,
            text: "Tijd",
          },
        },
        y: {
          beginAtZero: true,
          title: {
            display: true,
            text: "Vochtigheid (%)",
          },
        },
      },
    },
  });

  // Functie om nieuwe vochtigheidswaarde toe te voegen
  function voegVochtigheidToe(waarde) {
    const tijdstip = new Date();
    vochtigheidGrafiek.data.labels.push(tijdstip);
    vochtigheidGrafiek.data.datasets[0].data.push(waarde);
    vochtigheidGrafiek.update();
  }

  // // Simuleer het ontvangen van vochtigheidswaarden
  // setInterval(() => {
  //     const nieuweWaarde = Math.random() * 100;  // Simuleer een vochtigheidswaarde tussen 0 en 100
  //     voegVochtigheidToe(nieuweWaarde);
  // }, 2000);  // Elke 2 seconden een nieuwe waarde

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
});

modeToggle.addEventListener("change", function () {
  toggleCheck();
});

// if( msg >= 0 && msg <= 50) {
//     console.log("droog");
//     client.publish("topic", "motor/plant1");

// }else{
//     console.log(check);
//     console.log("nat");
// }

//      modeToggle.addEventListener("change", function() {
//     if (modeToggle.checked) {
//         console.log("Auto");
//         client.on("message", (topic, message) => {
//             if (topic === "real_unique_topic_2") {
//                 const msg = parseInt(message.toString());
//                 console.log(msg);
//                 if (msg >= 0 && msg <= 50) {
//                     console.log("droog");
//                     client.publish("topic", "motor/plant1");

//                 } else if (msg > 50 && msg <= 400) {
//                     console.log("nat");
//                 }
//             }
//         });

//     } else {
//         console.log("Manual");
//     }
// });

var mcVooruit = new Hammer(vooruit);
var mcAchteruit = new Hammer(achteruit);
var mcLinks = new Hammer(links);
var mcRechts = new Hammer(rechts);

function vooruitf() {
  client.publish("topic", "motor/vooruit");
  console.log("Vooruit");
}

function stopf() {
  client.publish("topic", "motor/stop");
  console.log("Stop");
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

//mqtt write
vooruit.addEventListener("mousedown", vooruitf);
vooruit.addEventListener("mouseup", stopf);
mcVooruit.on("press", vooruitf);
mcVooruit.on("pressup", stopf);
achteruit.addEventListener("mousedown", achteruitf);
achteruit.addEventListener("mouseup", stopf);
mcAchteruit.on("press", achteruitf);
mcAchteruit.on("pressup", stopf);
links.addEventListener("mousedown", linksf);
links.addEventListener("mouseup", stopf);
mcLinks.on("press", linksf);
mcLinks.on("pressup", stopf);
rechts.addEventListener("mousedown", rechtsf);
rechts.addEventListener("mouseup", stopf);
mcRechts.on("press", rechtsf);
mcRechts.on("pressup", stopf);

// achteruit.addEventListener("mouseup", function() {
//     client.publish("topic", "motor/stop");
//     console.log("Stop");
// }
// );
// function achteruitf() {
//     client.publish("topic", "motor/achteruit");
//     console.log("Achteruit");
// }

// links.addEventListener("mousedown", function() {
//     client.publish("topic", "motor/links");
//     console.log("Links");
// }
// );
// links.addEventListener("mouseup", function() {
//     client.publish("topic", "motor/stop");
//     console.log("Stop");
// }
// );

// rechts.addEventListener("mousedown", function() {
//     client.publish("topic", "motor/rechts");
//     console.log("Rechts");
// }
// );
// rechts.addEventListener("mouseup", function() {
//     client.publish("topic", "motor/stop");
//     console.log("Stop");
// }
// );

stopp.addEventListener("click", function () {
  client.publish("topic", "motor/stop");
  console.log("Stop");
});

sirene.addEventListener("click", function () {
  client.publish("topic", "motor/sirene");
  console.log("Stop");
});

servoInput.addEventListener("input", function () {
  const servoValue = parseInt(servoInput.value);
  if (servoValue >= 0 && servoValue <= 180) {
    client.publish("topic", `servo/${servoValue}`);
    console.log(servoValue);
    servoValueDisplay.textContent = servoValue;
  }
});
servoInput2.addEventListener("input", function () {
  const servoValue2 = parseInt(servoInput2.value);
  if (servoValue2 >= 0 && servoValue2 <= 180) {
    client.publish("topic", `servo/${servoValue2}`);
    console.log(servoValue2);
    servoValueDisplay2.textContent = servoValue2;
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
