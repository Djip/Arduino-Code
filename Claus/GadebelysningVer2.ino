/* Gadebelysnings System
 *  Use Photoresistors to monitor lightstrength
 *  and turn LEDs On and Off 
 *  Dev: Claus Olsen // Date 16/2-2017 //
 */
// #include <Dhcp.h>
// #include <Dns.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include <EthernetServer.h>
#include <SPI.h>

int connection;

// Enter a MAC address for your controller below.
byte mac[] = {0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02};

// Initialize the Ethernet client
EthernetClient client;

IPAddress server(192, 168, 1, 100);
IPAddress ip(192, 168, 1, 15);
IPAddress gateway(192,168,1, 1);
IPAddress subnet(255, 255, 255, 0);

//Constants
const int photoResistor1 = A0;
const int photoResistor2 = A1;
const int photoResistor3 = A2;
const int photoResistor4 = A3;

// solskin = 1023 - værdi falder til 546 når hånden er over Photoresistor
const int defaultValue = 520;

const int lampe1 = 2;
const int lampe2 = 3;
const int lampe3 = 5;
const int lampe4 = 7;

int photoResistorValue1;
int photoResistorValue2;
int photoResistorValue3;
int photoResistorValue4;

long randomNumber;

String methodStr = "GadeLamper#Turn on/off lights,0,0,1,0,,";

// put your setup code here, to run once:
void setup() {
  

  // serial communications
  Serial.begin(9600);

  Ethernet.begin(mac);
 
  delay(1000);

  Serial.println("Trying to connect...");
  // Connecting to the server.
  int connectionStatus = connectToServer();
  if(connectionStatus == 1)     {
      Serial.println("connected");
    } 
  else if(connectionStatus == 0) 
    {      Serial.println("connection failed");
    }

  
  pinMode(lampe1, OUTPUT);
  pinMode(lampe2, OUTPUT);
  pinMode(lampe3, OUTPUT);
  pinMode(lampe4, OUTPUT);
  
  
  pinMode(photoResistor1, INPUT);
  pinMode(photoResistor2, INPUT);
  pinMode(photoResistor3, INPUT);
  pinMode(photoResistor4, INPUT);
 
 // randomSeed(42);
}

void loop() {
  // put your main code here, to run repeatedly:

  //renews the IP lease - returning a byte with the following:
  //0: nothing happened
  //1: renew failed
  //2: renew success
  //3: rebind fail
  //4: rebind success
  byte result = Ethernet.maintain(); 
  client.print("1\n");
  methodFromServer();
  
  // Close connecting if not connect    
  if (!client.connected()) 
   
    {
      Serial.println("disconnecting.");
      client.stop();
      for(;;)
     ;
    }
  
 // randomNumber = random(1,5);
 // turnOneOn(randomNumber);
 // digitalWrite(lampe2,HIGH);
  //sensorLightStrength();
 // delay(200);
 // turnAllOff();
 // delay(2000);
 // turnAllOn();  
}

// Method running on Arduino only
// photoResistor value determines if lights is On or Off
void sensorLightStrength()
{
  photoResistorValue1 = analogRead(photoResistor1);  
  photoResistorValue2 = analogRead(photoResistor2);
  photoResistorValue3 = analogRead(photoResistor3);
  photoResistorValue4 = analogRead(photoResistor4);
 
  // (Hvis sollys styrke) > (My defaultvalue) 
  // Så sluk for lamper
  // Ellers tænd lamper
  if(photoResistorValue1 > defaultValue)
  {
    digitalWrite(lampe1,LOW);
  }
  else
  {
    Serial.println(photoResistorValue1);
    digitalWrite(lampe1,HIGH);
  }  
  
  if(photoResistorValue2 > defaultValue)
  {
    digitalWrite(lampe2,LOW);
  }
  else
  {
    Serial.println(photoResistorValue2);
    digitalWrite(lampe2,HIGH);
  }  

  if(photoResistorValue3 > defaultValue)
  {
    digitalWrite(lampe3,LOW);
  }
  else
  {
    Serial.println(photoResistorValue3);
    digitalWrite(lampe3,HIGH);
  }  

  if(photoResistorValue4 > defaultValue)
  {
    digitalWrite(lampe4,LOW);
  }
  else
  {
    Serial.println(photoResistorValue4);
    digitalWrite(lampe4,HIGH);
  }     
  delay(1000);
}



// Method turn all lamps off
void turnAllOff()
{

  Serial.println("Turning off lights");
    digitalWrite(lampe1,LOW);
    digitalWrite(lampe2,LOW);
    digitalWrite(lampe3,LOW);
    digitalWrite(lampe4,LOW);   
}

// Method turn all lamps On
void turnAllOn()
{
  Serial.println("Turning on lights");
  
    digitalWrite(lampe1,HIGH);
    digitalWrite(lampe2,HIGH);
    digitalWrite(lampe3,HIGH);
    digitalWrite(lampe4,HIGH);   
}


// Method turn a single selected lamp On
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

void methodFromServer()
{
  String methodToCall;  

  //if (client.available())
  //{
    // Reads for 5 sec from server.
    client.setTimeout(5000);
    Serial.println("Reading");
    methodToCall = client.readString();

    // Removes first 2 char in string (Blanks from server)
    methodToCall.remove(0, 2);
    Serial.println("Method: " + methodToCall);
    
    //find the , and use it split the method name from the method data
    /*
    byte commaIndex = 0;
    String methodName = "";
    int methodData = 0;
    char m[20];
    methodToCall.toCharArray(m,20);

    for (int i = 0; i < 20; i++)
    {
      if (m[i] == ',')
      {
        commaIndex = i;
        break;
      }
    }

    methodName = methodToCall.substring(0, commaIndex - 1);
    methodData = methodToCall.substring(commaIndex).toInt();
*/
    if (methodToCall == "Turn on/off lights,0") // Check Method
    {
      turnAllOff();
    }
    else if (methodToCall == "Turn on/off lights,1") // Check Method
    {
      turnAllOn();
    }
    else if (methodToCall.equalsIgnoreCase("turnOneOn")) // Check Method
    {
      //turnOneOn(methodData);
    }
    methodToCall = "";
  //}
}


