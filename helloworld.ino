void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); // start the Serial monitor
  delay(2000); // wait 2 seconds
  Serial.println("Hello World!"); // print to the Serial display
  delay(2000); // wait 2 seconds
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Hey."); // print to the Serial display
  delay(2000); // wait 2 seconds
}
