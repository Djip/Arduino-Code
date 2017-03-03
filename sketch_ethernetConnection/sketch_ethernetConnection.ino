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
String methodStr = "lampeSLAPAF#test123,0,0,1,0,,#test321,0,0,1,0,lampe,5";
// String methodStr = "Lampe";

// When we need to get buffer lengt we can store it here.
byte bufferLength;

// This will hold group methods.
char methodArray[] = {};

// Setting up the Arduino
void setup() {

  // Starting serial
  Serial.begin(9600);

  Serial.println("Checking buffer length when declared to 0");
  Serial.println(bufferLength);
  
  // Checking that we are inside setup.
  Serial.println("We are inside setup method!");

  // Just starting the ethernet port and setting mac, ip, gateway, subnetmask.
  Ethernet.begin(mac, ip, gateway, subnet);

  // Getting the size of the buffer.
  bufferLength = sizeof(methodStr);

  Serial.println("Buffer length after sizeof");
  Serial.println(bufferLength);
  
  // Converting method string to charArray to sent.
  methodStr.toCharArray(methodArray, bufferLength);

  // If we where using dhcp we could print out the status of the lease.
  // This should be used where we begin the Ethernet.begin like this connection = Ethernet.begin();
  // returns state of connection 1 for succes 0 for failure
  // Serial.println(connection);

  // Letting the Ethernet port finishing initializing before we try to do the connection. 
  delay(5000);

  // Connecting to the server.
  connectToServer();

  if(client.available()) 
  {
      Serial.println("Connection is OK");
  } else {
      Serial.println("Woops we dont have connection");
      client.stop();
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
void connectToServer() {
  
  Serial.println("Trying to connect...");
  
  if (client.connect(server, 1000)) {
    Serial.println("connected");

    Serial.flush();
    // Trying to sent method string to the server.
    //byte bytesSent = client.write(methodArray, 6);
    byte bytesSent = client.print(methodStr);
    // Serial.println(client.write(methodArray, bufferLength));
    Serial.println(bytesSent);

  } else {
    
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
  
  }
}

