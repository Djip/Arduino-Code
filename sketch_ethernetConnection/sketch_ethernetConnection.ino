#include <Dhcp.h>
#include <Dns.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include <EthernetServer.h>
#include <EthernetUdp.h>

// if we use dhcp to get IP, then we can use this connection to display the result of the dhcp request.
int connection;

// Defining the mac address to use for the board.
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};

// The client wich will connect to the server.
EthernetClient client;

// When we is going to send to our Azure server we will use this server variable instead of the IP.
// char server[] = "http://citybuilder.azurewebsites.net/sample/hello";

// When not using Azure server, we set the ip of the server static, wich we will connect to.
IPAddress server(192, 168, 1, 108);

// Using this if we dont want to use dhcp to get ip for the Arduion board.
IPAddress ip(192, 168, 1, 15);
IPAddress gateway(192,168,1, 1);
IPAddress subnet(255, 255, 255, 0);


// The string of methods that this arduino is holding, and should sent to the server.
String methodStr = "lampe#test123,0,0,1,0,,#test321,0,0,1,0,lampe,5";

// Setting up the Arduino
void setup() {

  // Starting serial
  Serial.begin(9600);
 
  // Checking that we are inside setup.
  Serial.println("We are inside setup method!");

  // Just starting the ethernet port and setting mac, ip, gateway, subnetmask.
  Ethernet.begin(mac, ip, gateway, subnet);

  // If we where using dhcp we could print out the status of the lease.
  // This should be used where we begin the Ethernet.begin like this connection = Ethernet.begin();
  // returns state of connection 1 for succes 0 for failure
  // Serial.println(connection);

  // Letting the Ethernet port finishing initializing before we try to do the connection. 
  delay(1000);

  Serial.println("Trying to connect...");
  // Connecting to the server.
  int connectionStatus = connectToServer();

  if(connectionStatus == 1) {

      Serial.println("connected");
    
    } else if(connectionStatus == 0) {
        Serial.println("connection failed");
    }
    
  // Checking if the client connection is still open to the server, else we close it.
  if(client.available()) 
  {
      Serial.println("There is still connection to the server");
  } else { 
      client.stop();
      Serial.println("Connection Terminated");
  }

}

void loop() {
  
  //renews the IP lease - returning a byte with the following:
  //0: nothing happened
  //1: renew failed
  //2: renew success
  //3: rebind fail
  //4: rebind success
  byte result = Ethernet.maintain(); 
}

// The connection method when we want to connect to the server, is called inside setup().
int connectToServer() {
  
  if (client.connect(server, 1000)) {
    
    // Trying to sent method string to the server.
    byte bytesSent = client.print(methodStr);

    // Returningn status on the connection.
    return 1;

  } else {
    
    // Returningn status on the connection.
    return 0; 
  
  }
}

