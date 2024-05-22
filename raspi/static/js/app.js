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
    const check = "";


  //mqtt read
  client.on("connect", () => {
    client.subscribe("real_unique_topic_2", (err) => {
        if (!err) {
            client.publish("presence", "Hello mqtt");
        }
    });
    
});

 client.on("message", (topic, message) => {
     if (topic === "real_unique_topic_2") {
         const msg = parseInt(message.toString());
        voegVochtigheidToe(msg);
      
   }
});

modeToggle.addEventListener("change", function() {
    if (modeToggle.checked) {
        check = "auto";
        console.log("Auto");
    }
    else {
        check = "manual";
        console.log("Manual");
    }
});

if(check == "auto"&& msg >= 0 && msg <= 50) {
    console.log("droog");
    client.publish("topic", "motor/plant1");

}else{
    console.log(check);
    console.log("nat");
}

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
  

    //mqtt write
    vooruit.addEventListener("mousedown", function() {
        client.publish("topic", "motor/vooruit");                     
        console.log("Vooruit");
    }
    );
    vooruit.addEventListener("mouseup", function() {
        client.publish("topic", "motor/stop");
        console.log("Stop");
    }
    );

    achteruit.addEventListener("mousedown", function() {
        client.publish("topic", "motor/achteruit");
        console.log("Achteruit");
    }
    );
    achteruit.addEventListener("mouseup", function() {
        client.publish("topic", "motor/stop");
        console.log("Stop");
    }
    );

    links.addEventListener("mousedown", function() {
        client.publish("topic", "motor/links");
        console.log("Links");
    }
    );
    links.addEventListener("mouseup", function() {
        client.publish("topic", "motor/stop");
        console.log("Stop");
    }
    );

    rechts.addEventListener("mousedown", function() {
        client.publish("topic", "motor/rechts");
        console.log("Rechts");
    }
    );
    rechts.addEventListener("mouseup", function() {
        client.publish("topic", "motor/stop");
        console.log("Stop");
    }
    );

    stopp.addEventListener("click", function() {
        client.publish("topic", "motor/stop");
        console.log("Stop");
    }
    );
   
    sirene.addEventListener("click", function() {
        client.publish("topic", "motor/sirene");
        console.log("Stop");
    }
    );
  
    servoInput.addEventListener("input", function() {
        const servoValue = parseInt(servoInput.value);
        if (servoValue >= 0 && servoValue <= 180) {
        client.publish("topic", `servo/${servoValue}`);
        console.log(servoValue);
        servoValueDisplay.textContent = servoValue;
        }
    });
    servoInput2.addEventListener("input", function() {
        const servoValue2 = parseInt(servoInput2.value);
        if (servoValue2 >= 0 && servoValue2 <= 180) {
        client.publish("topic", `servo/${servoValue2}`);
        console.log(servoValue2);
        servoValueDisplay2.textContent = servoValue2;
        }
    }

    );  
    
   

  
    
   

