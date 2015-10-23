// The digital pin we're connected to
int motionPin = 7;

// Initialize Serial and the motion sensor pin
void setup() {
  Serial.begin(9600);
  pinMode(motionPin, INPUT);
}

void loop() {
  // Read from the sensor
  int motion = digitalRead(motionPin);

  // When motion is detected, the sensor returns a "1"
  if (motion == 1)
  	Serial.println("Motion Detected");
  else
  	Serial.println("No Movement");

  // Wait 2 seconds between measurements
  delay(2000);
}
