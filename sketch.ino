#include <WiFi.h>
#include "ThingSpeak.h"
#include "DHTesp.h"
#include "config.h"

#define WIFI_SSID "Wokwi-GUEST"
#define WIFI_PASSWORD ""

DHTesp dhtSensor;
WiFiClient client;

void setup() {
  Serial.begin(115200);
  dhtSensor.setup(15, DHTesp::DHT22);

  Serial.print("Connecting to Wi-Fi");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" Connected!");

  ThingSpeak.begin(client);
}

void loop() {
  TempAndHumidity data = dhtSensor.getTempAndHumidity();
  float temperature = data.temperature;
  float humidity = data.humidity;

  Serial.println("=== Sensor Data ===");
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  ThingSpeak.setField(1, temperature);
  ThingSpeak.setField(2, humidity);

  int response = ThingSpeak.writeFields(channelNumber, APIKey);
  if (response == 200) {
    Serial.println("Data sent to ThingSpeak successfully!");
  } else {
      Serial.print("Oops, something went wrong. Couldn't send data. Error code: ");
      Serial.println(response);
  }

  delay(15000);
}
