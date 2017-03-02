#include <Dhcp.h>
#include <Dns.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include <EthernetServer.h>
#include <EthernetUdp.h>

int connection;
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
EthernetClient client;
//char server[] = "http://citybuilder.azurewebsites.net/sample/hello";
//byte server[] = {93, 176, 82, 58};
IPAddress server(192, 168, 1, 111);
IPAddress ip(192, 168, 1, 15);
IPAddress gateway(192,168,1, 1);
IPAddress subnet(255, 255, 255, 0);
char methodArray[] = {};
String methodStr = "lampe#test123,0,0,1,0,,#test321,0,0,1,0,lampe,5";
int bufferLength;

void setup() {
  Serial.println("OKK");
  Ethernet.begin(mac, ip, gateway, subnet);
  Serial.begin(9600);
  bufferLength = sizeof(methodArray);
  methodStr.toCharArray(methodArray, bufferLength);
  Serial.println(connection);
  Serial.println(Ethernet.localIP());
  delay(5000);
  connectToServer();

}

void loop() {
  //returns state of connection 1 for succes 0 for failure






  //renews the IP lease - returning a byte with the following:
  //0: nothing happened
  //1: renew failed
  //2: renew success
  //3: rebind fail
  //4: rebind success

  byte result = Ethernet.maintain();
}

void connectToServer() {
  Serial.println("BLA BLA");
  if (client.connect(server, 1000)) {
    Serial.println("connected");
    int bytesSent = client.write(methodArray, bufferLength);
    Serial.println(bytesSent);
  } else {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
  }
}

