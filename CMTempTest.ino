// Adapted from the example sketch by ladyada, public domain

// Import the Temperature/Humidity library we installed earlier
#include "DHT.h"

#define DHTPIN 7     // The digital pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

// Initialize DHT sensor
DHT dht(DHTPIN, DHTTYPE);

// Initialize Serial and the DHT sensor
void setup() {
  Serial.begin(9600);
  Serial.println("DHT22 test!");

  dht.begin();
}

void loop() {
  // Wait a few seconds between measurements.
  delay(2000);

  // Read humidity as percent
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);
  
  // Print all of our readings to the Serial monitor
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print(f);
  Serial.print(" *F\t");
  Serial.print("Heat index: ");
  Serial.print(hic);
  Serial.print(" *C ");
  Serial.print(hif);
  Serial.println(" *F");
}
