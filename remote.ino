#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>

// WIFI
const char* ssid = "ssid";
const char* password = "password";

// IR-Send-Pin (GPIO2 = D4)
const uint16_t kIrLed = 2; 
IRsend irsend(kIrLed);

ESP8266WebServer server(80);

const char HTML_PAGE[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="de">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Audio Remote SC-7700</title>
    <style>
        body {
            background-color: #1a1a1a;
            font-family: 'Arial Black', Arial, sans-serif;
            display: flex;
            justify-content: center;
            align-items: center;
            min-height: 100vh;
            margin: 0;
            user-select: none;
        }

        .remote {
            width: 260px;
            background-color: #121212;
            border-radius: 4px;
            box-shadow: 0 10px 30px rgba(0,0,0,0.7), inset 0 2px 4px rgba(255,255,255,0.1);
            padding: 20px 15px 40px 15px;
            box-sizing: border-box;
            color: #ffffff;
            border: 1px solid #222;
        }

        .header {
            text-align: left;
            margin-bottom: 25px;
            position: relative;
        }
        .brand {
            color: #df9b42;
            font-size: 18px;
            letter-spacing: 1px;
            font-weight: 900;
            margin: 0;
        }
        .sub-brand {
            color: #8c8c8c;
            font-size: 8px;
            letter-spacing: 0.5px;
            margin: 2px 0 0 0;
            font-family: Arial, sans-serif;
        }

        .grid {
            display: grid;
            grid-template-columns: repeat(4, 1fr);
            gap: 12px 8px;
            margin-bottom: 15px;
        }

        .btn-group {
            display: flex;
            flex-direction: column;
            align-items: center;
            justify-content: flex-end;
        }

        .label {
            font-size: 8px;
            text-transform: uppercase;
            margin-bottom: 4px;
            letter-spacing: 0.5px;
            text-align: center;
            color: #ffffff;
            height: 10px;
        }

        .btn {
            width: 44px;
            height: 18px;
            background: linear-gradient(to bottom, #6e7073, #4a4b4d);
            border: 1px solid #333;
            border-radius: 2px;
            box-shadow: 0 2px 0px #222, inset 0 1px 0 rgba(255,255,255,0.2);
            cursor: pointer;
            outline: none;
            transition: transform 0.05s;
        }
        
        .btn:active {
            transform: translateY(2px);
            box-shadow: 0 0px 0px #222;
        }

        .label-gold { color: #df9b42; }
        .label-gray { color: #8c8c8c; }


        .surround-section {
            background-image: 
                linear-gradient(rgba(255,255,255,0.05) 1px, transparent 1px),
                linear-gradient(90deg, rgba(255,255,255,0.05) 1px, transparent 1px);
            background-size: 4px 4px;
            border-top: 1px solid #333;
            border-bottom: 1px solid #333;
            padding: 10px 15px;
            margin: 15px -15px;
            display: flex;
            align-items: center;
            justify-content: flex-end;
            gap: 20px;
            box-sizing: border-box;
        }
        .surround-title {
            color: #df9b42;
            font-size: 14px;
            letter-spacing: 2px;
            margin: 0;
            font-weight: bold;
        }

        .power-group {
            border: 1px solid #df9b42;
            padding: 4px;
            border-radius: 2px;
            background-color: rgba(223, 155, 66, 0.05);
        }

        .footer {
            border-top: 2px solid #555;
            padding-top: 8px;
            margin-top: 20px;
            text-align: left;
        }
        .footer-text {
            color: #8c8c8c;
            font-size: 10px;
            letter-spacing: 1px;
            margin: 0;
        }
    </style>
</head>
<body>

    <div class="remote">
        <div class="header">
            <h1 class="brand">SC 7700</h1>
            <p class="sub-brand">INFRARED REMOTE CONTROLLER</p>
        </div>

        <div class="grid" style="align-items: flex-end;">
            <div class="btn-group" style="height: 38px;"><span class="label label-gold" style="margin-bottom: 4px; line-height: 10px;">CD</span><button class="btn" onclick="sendCmd('CD')"></button></div>
            <div class="btn-group" style="height: 38px;"><span class="label label-gold" style="margin-bottom: 4px; line-height: 10px;">TUNER</span><button class="btn" onclick="sendCmd('TUNER')"></button></div>
            <div style="grid-column: span 2; display: grid; grid-template-columns: repeat(2, 1fr); height: 38px; align-items: flex-end;"><span class="label" style="grid-column: span 2; margin-bottom: 4px; line-height: 10px; letter-spacing: 1px; text-align: center; width: 100%;">▼ PRESET ▲</span>
                <button class="btn" style="justify-self: center;" onclick="sendCmd('CH_DOWN')"></button>
                <button class="btn" style="justify-self: center;" onclick="sendCmd('CH_UP')"></button>
            </div>
        </div>

        <div class="grid">
            <div class="btn-group"><span class="label label-gray">1</span><button class="btn" onclick="sendCmd('KEY_1')"></button></div>
            <div class="btn-group"><span class="label label-gray">2</span><button class="btn" onclick="sendCmd('KEY_2')"></button></div>
            <div class="btn-group"><span class="label label-gray">3</span><button class="btn" onclick="sendCmd('KEY_3')"></button></div>
            <div class="btn-group"><span class="label label-gray">4</span><button class="btn" onclick="sendCmd('KEY_4')"></button></div>
            
            <div class="btn-group"><span class="label label-gray">5</span><button class="btn" onclick="sendCmd('KEY_5')"></button></div>
            <div class="btn-group"><span class="label label-gray">6</span><button class="btn" onclick="sendCmd('KEY_6')"></button></div>
            <div class="btn-group"><span class="label label-gray">7</span><button class="btn" onclick="sendCmd('KEY_7')"></button></div>
            <div class="btn-group"><span class="label label-gray">8</span><button class="btn" onclick="sendCmd('KEY_8')"></button></div>
        </div>

        <div class="grid">
            <div class="btn-group"><span class="label label-gray">9</span><button class="btn" onclick="sendCmd('KEY_9')"></button></div>
            <div class="btn-group"><span class="label label-gray">0</span><button class="btn" onclick="sendCmd('KEY_0')"></button></div>
            
            <div></div>
            
            <div class="btn-group" style="align-items: flex-end;">
                <span class="label label-gray" style="width: 44px; text-align: center;">MEMORY</span>
                <button class="btn" onclick="sendCmd('MEMORY')"></button>
            </div>
        </div>

        <div class="grid">
            <div class="btn-group"><span class="label">REPEAT</span><button class="btn" onclick="sendCmd('REPEAT')"></button></div>
            <div class="btn-group"><span class="label">CALL</span><button class="btn" onclick="sendCmd('CALL')"></button></div>

            <div class="btn-group" style="align-items: flex-end; justify-content: center; padding-right: 4px;">
                <span class="label" style="font-size:7px; line-height:1; margin-bottom: 0; text-align: right;">STOP/<br>CLEAR</span>
            </div>

            <div class="btn-group">
                <span class="label">■</span>
                <button class="btn" onclick="sendCmd('STOP')"></button>
            </div>
        </div>

        <div class="grid" style="align-items: flex-end;">
            <div style="grid-column: span 2; display: grid; grid-template-columns: repeat(2, 1fr); height: 38px; align-items: flex-end;">

                <span class="label" style="grid-column: span 2; margin-bottom: 4px; line-height: 10px; letter-spacing: 0.5px; text-align: center; width: 100%;">&lt;&lt; -- APSS  -- &gt;&gt;</span>
                <button class="btn" style="justify-self: center;" onclick="sendCmd('PREV')"></button><button class="btn" style="justify-self: center;" onclick="sendCmd('NEXT')"></button></div>
            <div class="btn-group" style="height: 38px;"><span class="label" style="margin-bottom: 4px; line-height: 10px;">⏸</span><button class="btn" onclick="sendCmd('PAUSE')"></button></div>
            <div class="btn-group" style="height: 38px;"><span class="label label-gold" style="margin-bottom: 4px; line-height: 10px;">▶</span><button class="btn" onclick="sendCmd('PLAY')"></button></div>
        </div>


        <div class="grid">
            <div class="btn-group"><span class="label label-gold">PHONO</span><button class="btn" onclick="sendCmd('PHONO')"></button></div>
            <div class="btn-group"><span class="label label-gold">TAPE</span><button class="btn" onclick="sendCmd('TAPE')"></button></div>
            <div class="btn-group"><span class="label label-gold">AUX-1</span><button class="btn" onclick="sendCmd('AUX1')"></button></div>
            <div class="btn-group"><span class="label label-gold">AUX-2</span><button class="btn" onclick="sendCmd('AUX2')"></button></div>
        </div>

        
        <div class="surround-section">
            <div class="surround-title">SURROUND</div>
            <div class="btn-group">
                <span class="label" style="font-size: 8px; letter-spacing: 0px; white-space: nowrap;">OFF - 1 - 2</span>
                <button class="btn" onclick="sendCmd('SURROUND')"></button>
            </div>
        </div>
        
        <div class="grid" style="align-items: flex-end; margin-top: 15px;">
            <div class="btn-group" style="height: 42px; justify-content: flex-end;">
                <div class="power-group" style="padding: 4px; border: 1px solid #df9b42; border-radius: 2px; background-color: rgba(223, 155, 66, 0.05); display: flex; flex-direction: column; align-items: center;">
                    <span class="label label-gold" style="font-weight: bold; margin-bottom: 4px; height: 10px;">POWER</span>
                    <button class="btn" onclick="sendCmd('POWER')"></button>
                </div>
            </div>
            
            <div style="grid-column: span 2; display: flex; flex-direction: column; align-items: center; height: 42px; justify-content: flex-end; padding-bottom: 5px; box-sizing: border-box;">
                <span class="label" style="margin-bottom: 4px; letter-spacing: 1px; height: 10px;">▼ VOLUME ▲</span>
                <div style="display: flex; gap: 16px;">
                    <button class="btn" onclick="sendCmd('VOLDOWN')"></button>
                    <button class="btn" onclick="sendCmd('VOLUP')"></button>
                </div>
            </div>
            
            <div class="btn-group" style="height: 42px; justify-content: flex-end; padding-bottom: 5px; box-sizing: border-box;">
                <span class="label label-gold" style="margin-bottom: 4px; height: 10px;">MUTING</span>
                <button class="btn" onclick="sendCmd('MUTE')"></button>
            </div>

        </div>

        <div class="footer">
            <p class="footer-text">AUDIO SYSTEM</p>
        </div>
    </div>

    <script>
        function sendCmd(command) {
            const url = '/send?cmd=' + command;
            console.log('Send command:', command);

            fetch(url)
                .then(response => {
                    if (!response.ok) throw new Error('Server-error');
                    return response.text();
                })
                .then(data => console.log('Success:', data))
                .catch(error => console.error('Error:', error));
        }
    </script>

</body>
</html>
)=====";

void handleRoot() {
  server.send_P(200, "text/html", HTML_PAGE);
}

void handleSend() {
  if (server.hasArg("cmd")) {
    String command = server.arg("cmd");
    uint32_t code = 0;

    if (command == "CD")            code = 0x2762;
    else if (command == "TUNER")    code = 0x2662;
    else if (command == "CH_DOWN")  code = 0x2522;
    else if (command == "CH_UP")    code = 0x2622;
    else if (command == "KEY_1")    code = 0x1602;
    else if (command == "KEY_2")    code = 0x1502;
    else if (command == "KEY_3")    code = 0x1702;
    else if (command == "KEY_4")    code = 0x1482;
    else if (command == "KEY_5")    code = 0x1682;
    else if (command == "KEY_6")    code = 0x1582;
    else if (command == "KEY_7")    code = 0x1782;
    else if (command == "KEY_8")    code = 0x1442;
    else if (command == "KEY_9")    code = 0x1642;
    else if (command == "KEY_0")    code = 0x1542;
    else if (command == "MEMORY")   code = 0x1722;
    else if (command == "REPEAT")   code = 0x15B2;
    else if (command == "CALL")     code = 0x1622;
    else if (command == "STOP")     code = 0x1792;
    else if (command == "PREV")     code = 0x1712;
    else if (command == "NEXT")     code = 0x1612;
    else if (command == "PAUSE")    code = 0x1692;
    else if (command == "PLAY")     code = 0x1512;
    else if (command == "PHONO")    code = 0x24E2;
    else if (command == "TAPE")     code = 0x2562;
    else if (command == "AUX1")     code = 0x26E2;
    else if (command == "AUX2")     code = 0x261D;
    else if (command == "SURROUND") code = 0x2552;
    else if (command == "POWER")    code = 0x25A2;
    else if (command == "VOLDOWN")  code = 0x255D;
    else if (command == "VOLUP")    code = 0x24A2;
    else if (command == "MUTE")     code = 0x27A2;

    if (code != 0) {
      Serial.print("Send Sharp 15-Bit LIRC-Direct: ");
      Serial.print(command);
      Serial.print(" -> 0x");
      Serial.println(code, HEX);
      
      irsend.sendSharpRaw(code, 15);
      server.send(200, "text/plain", "OK");
    } else {
      server.send(400, "text/plain", "Command not found");
    }
  } else {
    server.send(400, "text/plain", "Parameter missing");
  }
}

void setup() {
  Serial.begin(115200);
  irsend.begin();

  WiFi.begin(ssid, password);
  Serial.println("");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.print("Connected! IP-Adress: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/send", handleSend);
  server.begin();
  Serial.println("Webserver active.");
}

void loop() {
  server.handleClient();
}
