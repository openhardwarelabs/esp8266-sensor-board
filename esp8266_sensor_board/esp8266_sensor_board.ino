// Import required libraries
#include <ESP8266WiFi.h>
#include <aREST.h>
#include <aREST_UI.h>
#include "DHT.h"

// DHT 11 sensor
#define DHTPIN 5
#define DHTTYPE DHT11 

// Create aREST instance
aREST_UI rest = aREST_UI();

// WiFi parameters
const char* ssid = "your_WiFi_SSID";
const char* password = "your_WiFi_password";

// The port to listen for incoming TCP connections 
#define LISTEN_PORT           80

// Create an instance of the server
WiFiServer server(LISTEN_PORT);

// DHT sensor
DHT dht(DHTPIN, DHTTYPE, 15);

// Variables to be exposed to the API
int temperature;
float humidity;

void setup(void)
{  
  // Start Serial
  Serial.begin(115200);
  
  // Init variables and expose them to REST API
  rest.variable("temperature",&temperature);
  rest.variable("humidity",&humidity);

  // Labels
  rest.label("temperature");
  rest.label("humidity");
    
  // Give name and ID to device
  rest.set_id("1");
  rest.set_name("esp8266");
  
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

  // Init sensor
  dht.begin();
  
}

void loop() {

  // Make measurements
  humidity = (int)dht.readHumidity();
  temperature = (int)dht.readTemperature();

  // Handle REST calls
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  while(!client.available()){
    delay(1);
  }
  rest.handle(client);
 
}
