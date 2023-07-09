#include <WiFi.h>
#include <SPI.h>

#define CONNECTION_TIMEOUT 10

const char* ssid = "pointbreak22";
const char* password = "64Nevipa";

int keyIndex = 0;                             // Network key index
int status = WL_IDLE_STATUS;

int timer = 0;
int lastTimer = 0;
int checkInterval = 10000;

char thisChar = 0;

IPAddress ip;
IPAddress dns;
IPAddress gateway;
IPAddress subnet;

WiFiServer server(5001);

bool alreadyConnected = false;                // whether or not the client was connected previously

void setup()
{
    Serial.begin(115200);
    delay(1000);


    WiFi.mode(WIFI_STA); //Optional
    // Check for the presence of the shield:
     if (WiFi.status() == WL_NO_SHIELD) 
     {
        Serial.println("WiFi shield not present"); 
        // don't continue:
        while(true);
     } 
    
    
    //status = WiFi.begin(ssid, password);
    
    Serial.println("\nConnecting");
    int timeout_counter = 0;

    while ( status != WL_CONNECTED) { 
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:    
    status = WiFi.begin(ssid, password);

    // wait 10 seconds for connection:
    delay(10000);
  }

    server.begin();

    printWifiStatus();

}

void loop()
{
  WiFiClient client = server.available();   // Wait for a new client

  timer = millis(); // timer equals current time

  // When the client sends the first byte, say hello:
  if (client) // if there is a client
  { 
    if (!alreadyConnected) //and it's not already connected
    { 
      client.flush();          // clear out the input buffer
      Serial.println("Client has connected to Arduino");
      client.println("Hello Client, From Server!"); 
      alreadyConnected = true; // remember the client is now connected
    } 
  } 
  else // if there is no client
  { 
    if (alreadyConnected)   // if there has just been a client, set alreadyConnected to false
    {      
      alreadyConnected = false;
    }

    alreadyConnected = false;
    if (timer - lastTimer > checkInterval) 
    { // if the current time minus the last time is more than the check interval
      Serial.println("No client available"); 
      lastTimer = timer;
    } 
    
  }
 
  if (client.available()) // if a client is available
  {   
    thisChar = client.read();   // Read the bytes incoming from the client:
    client.flush();  
      
    if (thisChar == 49)  // if the character is a 1
    {                 
      Serial.print("Light on"); 
      client.println("Light on"); // tell Unity light is on
    } 
    else 
    {
      if (thisChar == 48) // if the character is a 0
      {      
        Serial.print("Light off");       // tell Unity light is off
        client.println("Light off");     // switch light off     
      } 
      else 
      {
        Serial.print("incorrect command"); 
        server.write("incorrect command");
      }
    }
  }
  
}

void printWifiStatus()
{
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print local IP address of the WiFi network (gateway)
  gateway = WiFi.gatewayIP();
  Serial.print("Gateway: ");
  Serial.println(gateway);

  //print the subnet mask
  Serial.print("Subnet: ");
  Serial.println(subnet);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
