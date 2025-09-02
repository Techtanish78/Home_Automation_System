#include <DHT.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "thingProperties.h"

#define DHTPIN 2         // D4 on NodeMCU
#define DHTTYPE DHT11
#define LEDPIN 5         // D1 on NodeMCU

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  delay(2000);

  Serial.println("Initializing...");

  initProperties(); // Arduino IoT Cloud properties
  pinMode(LEDPIN, OUTPUT);

  dht.begin();
  delay(2000); // Give DHT time to stabilize

  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  setDebugMessageLevel(4);
  ArduinoCloud.printDebugInfo();

  Serial.println("Setup complete.");
}

void loop() {
  ArduinoCloud.update();
  readSensor();
}

void readSensor() {
  Serial.println("Reading sensor values...");

  float t = dht.readTemperature();
  float h = dht.readHumidity();

  if (!isnan(t)) {
    temp = t;
    Serial.print("Temperature: ");
    Serial.print(temp);
    Serial.println(" °C");

    if (temp > 31) {
      sendToIFTTT(temp);
    }
  } else {
    Serial.println("Failed to read temperature");
  }

  if (!isnan(h)) {
    humid = h;
    Serial.print("Humidity: ");
    Serial.print(humid);
    Serial.println(" %");
  } else {
    Serial.println("Failed to read humidity");
  }

  delay(5000); // Delay between readings
}

void onLEDChange() {
  Serial.print("LED State: ");
  Serial.println(lED);
  digitalWrite(LEDPIN, lED ? HIGH : LOW);
}
