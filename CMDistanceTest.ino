// Adapted from the NewPing library example sketch

// Import the New Ping library we installed earlier
#include <NewPing.h>

#define TRIGGER_PIN  7  // The digital pin tied to trig
#define ECHO_PIN     8  // The digital pin tied to echo
#define MAX_DISTANCE 300 // Maximum distance we want to ping for (in centimeters) 
// Maximum sensor distance is 400-500cm

// Initialize the distance sensor
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

// Initialize Serial
void setup() {
  Serial.begin(9600);
}

void loop() {
  // Wait 50ms between pings (about 20 pings/sec)
  // 29ms should be the shortest delay between pings
  delay(50);

  // Print sensor readings in cm
  // A 0 means that the reading is outside the MAX_DISTANCE
  Serial.print("Ping: ");
  Serial.print(sonar.ping_cm());
  Serial.println("cm");
}
