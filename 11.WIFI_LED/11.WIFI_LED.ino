
#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "ESP32_LED_Control";
const char* password = "12345678";

const int LED_PIN = 18;
bool ledState = false;

WebServer server(80);

void handleRoot() {
    String html = R"rawliteral(
    <!DOCTYPE html>
    <html>
    <head>
        <meta name="viewport" content="width=device-width, initial-scale=1">
        <title>ESP32 LED Control</title>
        <style>
            body {
                text-align: center;
                font-family: Arial;
                margin-top: 60px;
            }
            button {
                padding: 15px 35px;
                font-size: 20px;
                margin: 10px;
                cursor: pointer;
            }
        </style>
    </head>

    <body>
        <h1>ESP32 LED Control</h1>

        <h2>LED is OFF</h2>

        <a href="/on">
            <button>LED ON</button>
        </a>

        <a href="/off">
            <button>LED OFF</button>
        </a>

    </body>
    </html>
    )rawliteral";

    html.replace(
        "LED is OFF",
        ledState ? "LED is ON" : "LED is OFF"
    );

    server.send(200, "text/html", html);
}

void handleLEDOn() {
    ledState = true;
    digitalWrite(LED_PIN, HIGH);

    server.sendHeader("Location", "/");
    server.send(303);
}


void handleLEDOff() {
    ledState = false;
    digitalWrite(LED_PIN, LOW);

    server.sendHeader("Location", "/");
    server.send(303);
}

void setup() {
    Serial.begin(115200);

    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);

    WiFi.mode(WIFI_AP);

    WiFi.softAP(ssid, password);

    Serial.println("Access Point Started!");

    Serial.print("IP Address: ");
    Serial.println(WiFi.softAPIP());

    
    server.on("/", handleRoot);
    server.on("/on", handleLEDOn);
    server.on("/off", handleLEDOff);

    server.begin();

    Serial.println("Web Server Started!");
}

void loop() {
    server.handleClient();
}
