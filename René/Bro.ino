#include <Dhcp.h>
#include <Dns.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include <EthernetServer.h>
//#include <EthernetUdp.h>
//#include <util.h>
// --------------------------------------------------------------------------- Motors
int motor[] = {2, 3};
// --------------------------------------------------------------------------- Connection
// if we use dhcp to get IP, then we can use this connection to display the result of the dhcp request.
int connection;
// Defining the mac address to use for the board.
byte mac[] = {0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE};
// The client wich will connect to the server.
EthernetClient client;
// When not using Azure server, we set the ip of the server static, wich we will connect to.
IPAddress server(192, 168, 1, 100);
// Using this if we dont want to use dhcp to get ip for the Arduion board.
IPAddress ip(192, 168, 1, 15);
IPAddress gateway(192,168,1, 1);
IPAddress subnet(255, 255, 255, 0);
// --------------------------------------------------------------------------- Methods
// The string of methods that this arduino is holding, and should sent to the server.
String methodStr = "bro#OpenAndClose,0,0,1,0,,#Open,0,0,1,0,Open,1#Close,0,0,1,0,Close,1";
// --------------------------------------------------------------------------- Setup
void setup() 
{
  // Starting serial
  Serial.begin(9600);
  // Setup motors
  pinMode(motor[0], OUTPUT);
  pinMode(motor[1], OUTPUT);
  // Just starting the ethernet port and setting mac.
  Ethernet.begin(mac);
  // Letting the Ethernet port finishing initializing before we try to do the connection. 
  delay(1000);

  Serial.println("Trying to connect...");
  // Connecting to the server.
  int connectionStatus = connectToServer();
  if(connectionStatus == 1) 
    {
      Serial.println("connected");
    } 
  else if(connectionStatus == 0) 
    {
      Serial.println("connection failed");
    }
}

void loop() 
{
  //renews the IP lease - returning a byte with the following:
  //0: nothing happened
  //1: renew failed
  //2: renew success
  //3: rebind fail
  //4: rebind success
  byte result = Ethernet.maintain(); 

  String methodToCall;

  if (client.available()) 
    {
      // Reads for 5 sec from server.
      client.setTimeout(5000);
      Serial.println("Reading");
      methodToCall = client.readString();

      // Removes first 2 char in string (Blanks from server)
      methodToCall.remove(0,2);
      Serial.println("Method: " + methodToCall);
      
      if (methodToCall == "OpenAndClose,1") // Check Method
        {
          OpenAndClose();
        }
      else if (methodToCall == "Open,1,Open,1") // Check Method
        {
          Open();
        }
      else if (methodToCall == "Close,1,Close,1") // Check Method
        {
          Close();
        }
      methodToCall = "";
    }
  // Close connecting if not connect    
  if (!client.connected()) 
    {
      Serial.println("disconnecting.");
      client.stop();
      for(;;)
      ;
    }
}
void OpenAndClose()
{
  // Starts Motors!
  Serial.println("Lukker op!"); 
  analogWrite(motor[0], 0); 
  analogWrite(motor[1], 140); 

  // Wait to Close
  Serial.println("Venter..");
  delay(30000);

  // Close Motors!
  Serial.println("Lukker ned!");
  digitalWrite(motor[0], LOW); 
  digitalWrite(motor[1], LOW);
}
void Close()
{
  // Close Motors!
  Serial.println("Lukker ned!");
  digitalWrite(motor[0], LOW); 
  digitalWrite(motor[1], LOW);
}
void Open()
{
  // Starts Motors!
  Serial.println("Lukker op!"); 
  analogWrite(motor[0], 0); 
  analogWrite(motor[1], 140); 
}
// The connection method when we want to connect to the server, is called inside setup().
int connectToServer() 
{ 
  if (client.connect(server, 9000)) 
    {
      // Trying to sent method string to the server.
      byte bytesSent = client.print(methodStr + "\n");
      // Returningn status on the connection.
      return 1;
    } 
  else 
    {
      // Returningn status on the connection.
      return 0; 
    }
}
