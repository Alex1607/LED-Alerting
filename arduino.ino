#include <WiFiClientSecure.h>
#include <ArduinoJson.h>

const char* ssid = "Routey McRouterFace";
const char* password = "SuperSecretPassword";

const char*  server = "api.greev.eu";

WiFiClientSecure client;

void setup() {
  Serial.begin(9600);
  delay(6000);

  Serial.print("Attempting to connect to SSID: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  uint8_t status = WiFi.waitForConnectResult();

  if (status != WL_CONNECTED) {
    Serial.print("Unable to connect to WiFi");
  }

  Serial.print("Connected to ");
  Serial.println(ssid);

  client.setInsecure();

  Serial.println("\nStarting connection to server...");
  if (!client.connect(server, 443))
    Serial.println("Connection failed!");
  else {
    Serial.println("Connected to server!");
    // Make a HTTP request:
    client.println("GET https://api.greev.eu/v2/stats/loginstreak/top?amount=1&offset=0 HTTP/1.0"); //TODO: Only for testing
    client.println("Host: api.greev.eu");
    client.println("Connection: close");
    client.println();

    while (client.connected()) {
      String line = client.readStringUntil('\n');
      if (line == "\r") {
        Serial.println("headers received");
        break;
      }
    }
    
    String response = "";
    while (client.available()) {
      char c = client.read();
      response += c;
    }

    DynamicJsonDocument doc(4096);
    DeserializationError error = deserializeJson(doc, response);
    Serial.println(error.c_str());
    if (!error) {
      Serial.println(doc.as<JsonArray>()[0].as<JsonObject>()["uuid"].as<String>());  //TODO: Only for testing, later I will have to make it use the object which indicated which led to light
    }

    client.stop();
  }
}

void loop() {
  delay(1000);
  Serial.print(".");
}
