#include <WiFi.h>
#include <PubSubClient.h>


//next target : To get the status of the device irrespective of method of operation - 14/05/2026
//additional target : To use firebase for res management and also publishing the respective espID for future refernce.

/* =========================
   WIFI CONFIG
   ========================= */

const char* ssid = "3a";
const char* password = "monk1234";

/* =========================
   MQTT CONFIG
   ========================= */

const char* mqtt_server = "10.23.124.145";

/* =========================
   OBJECTS
   ========================= */

WiFiClient espClient;
PubSubClient client(espClient);

/* =========================
   HARDWARE
   ========================= */

const int ledPin = 13;

/* =========================
   WIFI SETUP
   ========================= */

void setup_wifi() {

  Serial.println();
  Serial.println("Connecting to WiFi...");

  WiFi.mode(WIFI_STA);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {

    delay(500);

    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi Connected!");
  Serial.print("ESP32 IP: ");
  Serial.println(WiFi.localIP());
}

/* =========================
   MQTT CALLBACK
   ========================= */

void callback(char* topic, byte* payload, unsigned int length) {

  Serial.print("Message received on topic: ");
  Serial.println(topic);

  String message = "";

  for (int i = 0; i < length; i++) {

    message += (char)payload[i];
  }

  Serial.print("Message: ");
  Serial.println(message);

  /* ---------- CONTROL LED ---------- */

  if (message == "ON") {
   digitalWrite(ledPin, LOW);

    Serial.println("LED TURNED OFF");

  }

  else if (message == "OFF") {

        digitalWrite(ledPin, HIGH);

    Serial.println("LED TURNED ON");
  }
}

/* =========================
   MQTT RECONNECT
   ========================= */

void reconnect() {

  while (!client.connected()) {

    Serial.println("Attempting MQTT connection...");

    if (client.connect("ESP32Client")) {

      Serial.println("MQTT Connected!");

      client.subscribe("home/control");

      Serial.println("Subscribed to: home/control");

    } else {

      Serial.print("Failed, rc=");
      Serial.println(client.state());

      delay(2000);
    }
  }
}

/* =========================
   SETUP
   ========================= */

void setup() {

  Serial.begin(115200);

  pinMode(ledPin, OUTPUT);

  digitalWrite(ledPin, LOW);

  setup_wifi();

  client.setServer(mqtt_server, 1883);

  client.setCallback(callback);
}

/* =========================
   LOOP
   ========================= */

void loop() {

  if (WiFi.status() != WL_CONNECTED) {

    setup_wifi();
  }

  if (!client.connected()) {

    reconnect();
  }

  client.loop();
}