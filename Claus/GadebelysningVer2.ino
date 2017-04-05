/* Gadebelysnings System
 *  Use a Photoresistor to monitor lightstrength
 *  and turn LEDs On and Off 
 *  Dev: Claus Olsen // Date 16/2-2017 //
 */

#include <Dhcp.h>
#include <Dns.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include <EthernetServer.h>

// Enter a MAC address for your controller below.
byte mac[] = {0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02};

// Initialize the Ethernet client
EthernetClient client;


// set the ip of the server static
IPAddress server(192, 168, 1, 100);
// Using this if we dont want to use dhcp
IPAddress ip(192, 168, 1, 15);
IPAddress gateway(192,168,1, 1);
IPAddress subnet(255, 255, 255, 0);


//Constants
const int photoResistor1 = A0;
const int photoResistor2 = A1;
const int photoResistor3 = A2;
const int photoResistor4 = A3;

const int lampe1 = 2;
const int lampe2 = 3;
const int lampe3 = 5;
const int lampe4 = 7;

int photoResistorValue1;
int photoResistorValue2;
int photoResistorValue3;
int photoResistorValue4;

long randomNumber;

String methodStr = "gadelamper#turnAllOn,0,0,1,0,,#turnAllOff,0,0,1,0,,#turnOneOn,0,0,1,0,1,5";

// put your setup code here, to run once:
void setup() {
  

  // serial communications
  Serial.begin(9600);

 // start the Ethernet connection:
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
  
  // print my local IP address:
  printIPAddress();
  
  pinMode(lampe1, OUTPUT);
  pinMode(lampe2, OUTPUT);
  pinMode(lampe3, OUTPUT);
  pinMode(lampe4, OUTPUT);
  
  
  pinMode(photoResistor1, INPUT);
  pinMode(photoResistor2, INPUT);
  pinMode(photoResistor3, INPUT);
  pinMode(photoResistor4, INPUT);
 
  randomSeed(42);
}

void loop() {
  
  // put your main code here, to run repeatedly:
  
    
  photoResistorValue1 = analogRead(photoResistor1);
  photoResistorValue2 = analogRead(photoResistor2);
  photoResistorValue3 = analogRead(photoResistor3);
  photoResistorValue4 = analogRead(photoResistor4);
 
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
	
  lightstrength();
  
}

// Photoresistor turn lamps on and off
void lightstrength(){
	if(photoResistorValue1 > 550)
  {
    digitalWrite(lampe1,LOW);
  }
  else
  {
    Serial.println(photoResistorValue1);
    digitalWrite(lampe1,HIGH);
  }  
  
  if(photoResistorValue2 > 550)
  {
    digitalWrite(lampe2,LOW);
  }
  else
  {
    Serial.println(photoResistorValue2);
    digitalWrite(lampe2,HIGH);
  }  

  if(photoResistorValue3 > 550)
  {
    digitalWrite(lampe3,LOW);
  }
  else
  {
    Serial.println(photoResistorValue3);
    digitalWrite(lampe3,HIGH);
  }  

  if(photoResistorValue4 > 550)
  {
    digitalWrite(lampe4,LOW);
  }
  else
  {
    Serial.println(photoResistorValue4);
    digitalWrite(lampe4,HIGH);
  }   
  delay(500); 
}

// write my ip adr to seriel monitor
void printIPAddress()
{
  Serial.print("My IP address: ");
  for (byte thisByte = 0; thisByte < 4; thisByte++) {
    // print the value of each byte of the IP address:
    Serial.print(Ethernet.localIP()[thisByte], DEC);
    Serial.print(".");
  }

  Serial.println();
}

// turn all lamps off
void turnAllOff()
{
    digitalWrite(lampe1,LOW);
    digitalWrite(lampe2,LOW);
    digitalWrite(lampe3,LOW);
    digitalWrite(lampe4,LOW);    
}

// turn all lamps on
void turnAllOn()
{
    digitalWrite(lampe1,HIGH);
    digitalWrite(lampe2,HIGH);
    digitalWrite(lampe3,HIGH);
    digitalWrite(lampe4,HIGH);    
}

// turn 1 lamp on
void turnOneOn(int tal)
{
  switch(tal)
  {
    case 1:
    digitalWrite(lampe1,HIGH);
    break;
    case 2:
    digitalWrite(lampe2,HIGH);
    break;
    case 3:
    digitalWrite(lampe3,HIGH);
    break;
    case 4:
    digitalWrite(lampe4,HIGH);
    break;    
  }
  delay(500);
}




