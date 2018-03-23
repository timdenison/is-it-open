
// Import required libraries
#include <WiFi.h>
#include <aREST.h>

// Create aREST instance
aREST rest = aREST();

// WiFi parameters
const char* ssid = "SSID";
const char* password = "PASSWORD";

// Create an instance of the server
WiFiServer server(80);

// Variables to be exposed to the API
bool occupied;

// Declare functions to be exposed to the API
int inUse(String command);

// Button pin to detect if button is pressed
const int BUTTON_PIN = 0;

void setup()
{
  
  // Start Serial
  Serial.begin(115200);

  // Init variables and expose them to REST API
  occupied = false;
  rest.variable("occupied",&occupied);

  // Function to be exposed
  rest.function("occupied", inUse);

  // Give name & ID to the device (ID should be 6 characters long)
  rest.set_id("1");
  rest.set_name("esp32");

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());

}

void loop() {
  
  // Handle REST calls
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  while(!client.available()){
    delay(1);
  }
  rest.handle(client);

  occupied = (inUse("") == 1) ? true : false;
}

// Custom function for reading if button is pressed
int inUse(String command) {
  return (digitalRead(BUTTON_PIN) == LOW) ? 1 : 0;
}
