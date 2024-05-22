
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
  

    