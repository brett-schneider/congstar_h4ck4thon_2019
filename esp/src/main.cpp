#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>
#include <TM1638lite.h>


const char* ssid            = "";
const char* password        = "";

const char* restURL         = "";

const int   baud            = 115200;

const int   wifiDelay       = 500;
const int   refreshDelay    = 1000;

String alt_bestand;

// D5 grau, orange, strobe
// D6 weiß, gelb, clock
// D7 blau, grün, data

// strobe, clock, data
TM1638lite tm(4,5,16);

void setup() {
  tm.reset();

  Serial.begin(baud);
  Serial.flush();
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status

    HTTPClient http;
    http.begin(restURL); //Connecting HTTP
    
    if (http.GET() > 0) { //Check the returning code
      DynamicJsonDocument doc(1024);
      DeserializationError error = deserializeJson(doc, http.getString());
      if(error) {
        Serial.println("[HTTP] Failed to deserialize JSON");
        tm.displayText("ERROR422");
        alt_bestand = "ERROR422";
      } else {
        String bestand = doc["bestand"];

        if(alt_bestand != bestand) {
          alt_bestand = bestand;
          Serial.print("Neuer Bestand: ");
          Serial.println(bestand);
        }
        
        tm.displayText(bestand);
      }
    } else {
      Serial.print("[HTTP] Failed to connect to ");
      Serial.println(restURL);
      tm.displayText("ERROR408");
      alt_bestand = "ERROR408";
    }
     
    http.end();

    delay(refreshDelay); //Timout
  } else {
    Serial.print("[WIFI] Connecting..");

    WiFi.begin(ssid, password); // Connecting to WIFI
    tm.displayText("CONNECT");
    while (WiFi.status() != WL_CONNECTED) {
      delay(wifiDelay);
      Serial.print(".");
    }
    Serial.println("Connected"); //WiFi.localIP()
    tm.reset();
  }
}
