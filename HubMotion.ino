// Will work only with ESP8266 firmware 0.9.2.2 or higher
// Some particulars of this sketch are for the Cactus Micro board, 
// but it can be adapted for any Arduino
// Special thanks to David Reeves for his help

////////////////////////////
/////// Motion Sensor //////
////////////////////////////

// The pin OUT is connected to
int motionPin = 7;
 

////////////////////////////
// Initial State Streamer //
////////////////////////////
 
// Your hub's IP address to configure the direct connection
String hubIP = "Hub_IP";
int port = 8080;
// Bucket key - using the hub, this will be the name of your bucket
String bucketKey = "Hub_Motion";


///////////////////////////
// Event Signal Settings //
///////////////////////////

// How many signals are in your stream? You can have as few or as many as you want
const int NUM_SIGNALS = 1;
// What are the names of your signals (i.e. "Temperature", "Humidity", etc.)
String signalName[NUM_SIGNALS] = {":wave:Motion"};
// This array is to store our signal data later
String signalData[NUM_SIGNALS];
 
 
////////////////////////////
///// WiFi Credentials /////
////////////////////////////

// WiFi SSID
#define SSID "PiOT Workshop"
// WiFi password
#define PASS "raspberry"
 
 
////////////////////////////
/// Cactus Micro Settings //
////////////////////////////

// CH_PD pin - Used by Cactus Micro to enable ESP8266
#define RESET 13
// GPIO0
#define GPIO0 5 
// RST pin - ESP8266 reset pin
#define RST 5
 

////////////////////////////
//// AT Command Setting ////
////////////////////////////

// Timeout in milliseconds
#define TIMEOUT     5000
#define CONTINUE    false
#define HALT        true


// This only runs once at the very beginning
void setup()  {
 
  Serial.begin(9600);
  // Communication with ESP8266
  Serial1.begin(9600);
  
  // Enable RST pin
  digitalWrite(RST, 1);
  // Enable GPIO1 pin to prevent bootloader mode
  digitalWrite(GPIO0, 1);
  // Enable CH_PD pin
  pinMode(RESET, OUTPUT);
  digitalWrite(RESET, 1);     
  // Wait for chip enable
  delay(2000);
  
  Serial.println("ESP8266 A0 Monitor");
  reset();
  // Wait for reset to complete
  delay(5000);
 
  // Reset & test if the module is ready 
  echoCommand("AT+RST", "Ready", HALT); 
  delay(5000);
  echoCommand("AT+CSYSWDTENABLE", "WDT Enabled", HALT);
  delay(500);
  Serial.println("Module is ready.");
  
  // Set up connection modes
  // Retrieves the firmware ID (version number) of the module.
  echoCommand("AT+GMR", "OK", CONTINUE);
  // Station mode    
  echoCommand("AT+CWMODE=3", "", HALT);
  // Allow one connection    
  echoCommand("AT+CIPMUX=1", "", HALT);    
 
  // Connect to the wifi
  boolean connection_established = false;
  for(int i=0;i<5;i++)
  {
    if(connectWiFi())
    {
      connection_established = true;
      delay(5000);
      break;
    }
  }
  if (!connection_established) errorHalt("Connection failed");
  delay(5000);
 
  // Echo IP address.
  echoCommand("AT+CIFSR", "", HALT);         
  
}
 
// This repeats
void loop() 
{
  // Reset ESP8266 each time around
  reset();                                
  delay(2000);
 
  // The takeMotion() function reads from the sensor
  takeMotion();

  // The toHub() function streams our events
  while(!toHub());   
}
 
 
 
// Here are the data bucket creation, event data posting, error handling,
// and WiFi connection functions
// They do not need to be edited - everything you would need to change for 
// your project can be found above

// Read from the sensor
void takeMotion()
{
  pinMode(motionPin, INPUT);
  int motion = digitalRead(motionPin);
  
  if (motion == HIGH)
  {
    signalData[0] = String(":runner:");
  }
  else
  {
    signalData[0] = String(":no_pedestrians:");
  }  
}

// Stream events to the hub
boolean toHub () 
{
  for (int i=0; i<NUM_SIGNALS; i++)
  {
    // Must be connected to your destination URL
    while(!connectService(hubIP,port)) {};
   
    // Get connection status 
    while(!echoCommand("AT+CIPSTATUS", "STATUS:3", CONTINUE)) 
    {
      echoCommand("AT+CIPCLOSE", "OK", CONTINUE);
      while(!connectService(hubIP,port)) {};
    };
 
  	  // Build HTTP request.
	  String toSend = "POST http://" + hubIP + "/" + bucketKey + "/" + signalName[i] + "\r\n";
	  String payload = signalData[i];
    toSend += "Content-Type: text/plain\r\n";
	  toSend += "Content-Length: "+String(payload.length())+"\r\n";
	  toSend += "\r\n";
	  toSend += payload;
	  
	  Serial.println(toSend);
	 
	  // Ready the module to receive raw data
	  if (!echoCommand("AT+CIPSEND="+String(toSend.length()), ">", CONTINUE))
	  {
	    echoCommand("AT+CIPCLOSE", "", CONTINUE);
	    Serial.println("Connection timeout.");
	    return false;
	  }
	  
	  // Send the raw HTTP request
	  // POST
	  if(!echoCommand(toSend,"20", CONTINUE)) return false;  
	  String toPrint = "Request ";
	  toPrint += (i + 1);
	  toPrint += " sent!";
	  Serial.println(toPrint);
    echoCommand("AT+CIPCLOSE", "OK", CONTINUE);
    delay(2000);

  }
   
  return true;
} 
 
// Print error message and loop stop.
void errorHalt(String msg)
{
  Serial.println(msg);
  Serial.println("HALT");
  while(true){};
}
 
// Read characters from WiFi module and echo to serial until keyword occurs or timeout.
boolean echoFind(String keyword)
{
  byte current_char   = 0;
  byte keyword_length = keyword.length();
  
  // Fail if the target string has not been sent by deadline.
  unsigned long deadline = millis() + TIMEOUT;
  while(millis() < deadline)
  {
    if (Serial1.available())
    {
      char ch = Serial1.read();
      Serial.write(ch);
 
      if (ch == keyword[current_char])
        if (++current_char == keyword_length)
        {
          Serial.println();
          return true;
        }
    }
  }
  // Timed out
  return false;  
}
 
// Read and echo all available module output.
// (Used when we're indifferent to "OK" vs. "no change" responses or to get around firmware bugs.)
void echoFlush()
  {while(Serial1.available()) Serial.write(Serial1.read());}
  
// Echo module output until 3 newlines encountered.
// (Used when we're indifferent to "OK" vs. "no change" responses.)
void echoSkip()
{
  echoFind("\n");        // Search for nl at end of command echo
  echoFind("\n");        // Search for 2nd nl at end of response.
  echoFind("\n");        // Search for 3rd nl at end of blank line.
}
 
// Send a command to the module and wait for acknowledgement string
// (or flush module output if no ack specified).
// Echoes all data received to the serial monitor.
boolean echoCommand(String cmd, String ack, boolean halt_on_fail)
{
  Serial1.println(cmd);
  #ifdef ECHO_COMMANDS
    Serial.print("--"); Serial.println(cmd);
  #endif
  
  // If no ack response specified, skip all available module output.
  if (ack == "")
    echoSkip();
  else
    // Otherwise wait for ack.
    // Timed out waiting for ack string
    if (!echoFind(ack))           
      if (halt_on_fail)
        // Critical failure halt.
        errorHalt(cmd+" failed");
      else
        // Let the caller handle it.
        return false;      
  // Ack blank or ack found      
  return true;                   
}
 
// Connect to the specified wireless network.
boolean connectWiFi()
{
  String cmd = "AT+CWJAP=\"" SSID "\",\"" PASS "\"";
  // Join Access Point
  if (echoCommand(cmd, "OK", CONTINUE)) 
  {
    Serial.println("Connected to WiFi.");
    return true;
  }
  else
  {
    Serial.println("Connection to WiFi failed.");
    return false;
  }
}
 
// Connect to a service; in our case, the hub at IP address on Port
boolean connectService(String service, int port) 
{  
  String serviceConnect = "AT+CIPSTART=\"TCP\",\"" + service + "\"," + port;
  //  Handle connection errors
  if (!echoCommand(serviceConnect, "Linked", CONTINUE)) {          
    if (echoCommand(serviceConnect, "ALREADY CONNECTED", CONTINUE)){
      return true;
    }
    if (echoCommand(serviceConnect, "busy p...", CONTINUE)) {
      reset();
      delay(5000);
    }
    if (echoCommand(serviceConnect, "ERROR", CONTINUE)) {
      reset();
      delay(5000);
    }
    delay(2000);
    return false;
  }
  delay(2000);
  return true;
}
 
// Reset the ESP8266 Chip
void reset()
{
  digitalWrite(RESET,LOW);
  delay(1000);
  digitalWrite(RESET,HIGH);
  delay(1000);
}
