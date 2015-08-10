#include <WiFiESP.h>
#include <WiFiESPClient.h>
#include <WiFiESPServer.h>
#include <WiFiESPUdp.h>
#include <IPAddress.h>

#define CLIENT_COUNT 6

IPAddress getAddress(void) {
  uint8_t o1, o2, o3, o4;
  Serial.println(F("enter first octet"));
  while(Serial.available()) Serial.read();
  while (!Serial.available());
  if ((Serial.peek() == '\r') || (Serial.peek() == '\n')) return IPAddress((uint32_t)0);
  o1 = Serial.parseInt();
  Serial.println(F("enter second octet"));
  while(Serial.available()) Serial.read();
  while (!Serial.available());
  o2 = Serial.parseInt();
  Serial.println(F("enter third octet"));
  while(Serial.available()) Serial.read();
  while (!Serial.available());
  o3 = Serial.parseInt();
  Serial.println(F("enter fourth octet"));
  while(Serial.available()) Serial.read();
  while (!Serial.available());
  o4 = Serial.parseInt();
  return IPAddress(o1, o2, o3, o4);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println(F("I live!"));
  WiFiESP.init();
  Serial.println(F("Remember to set line endings to either \\n or \\r\\n"));
}

void loop() {
  // put your main code here, to run repeatedly:
  int8_t selection;
  char c;
  uint8_t mode, mode1, mode2, mux, item;
  uint32_t timeout, port;
  String retString, inString1, inString2, inString3;
  IPAddress ip, gateway, subnet, dns;
  uint8_t * bytePtr;
  uint8_t macAddressPtr[WL_MAC_ADDR_LENGTH];
  WiFiESPClient clients[CLIENT_COUNT];
  uint8_t clientCtr = 0;
  char incomingData[1024];
  uint16_t dataLen;

  Serial.println(F("Enter 0 for list of commands"));
  while(Serial.available()) Serial.read();
  while (!Serial.available()) {
    if (Serial1.available()) Serial.print((char *)Serial1.read());
  }
  selection = Serial.parseInt();
  switch(selection) {
    case 0:
      Serial.println(F("Select the ESP Class function to run:"));
      Serial.println(F("1) begin()"));
      Serial.println(F("2) disconnect()"));
      Serial.println(F("3) SSID()"));
      Serial.println(F("4) scanNetworks()"));
      Serial.println(F("5) status()"));
      Serial.println(F("Select the ESP Client function to run"));
      Serial.println(F("6) Create new client"));
      Serial.println(F("7) Connect by IP address"));
      Serial.println(F("8) Connect by hostname"));
      Serial.println(F("9) Write one byte"));
      Serial.println(F("10) Write a buffer"));
      Serial.println(F("11) Return the number of bytes available in a given client's buffer"));
      Serial.println(F("12) Read one byte"));
      Serial.println(F("13) Read a buffer"));
      Serial.println(F("14) Peek at the next character"));
      Serial.println(F("15) Flush the input buffer"));
      Serial.println(F("16) Stop a client"));
      Serial.println(F("17) Check whether a client is connected"));
      Serial.println(F("18) boolean operator"));
      Serial.println(F("19) Return current number of clients"));
      break;
    case 1:
      Serial.println(F("Enter SSID to connect to"));
      while(Serial.available()) Serial.read();
      while (!Serial.available());
      inString1 = Serial.readString();
      inString1.trim();
      Serial.println(F("Enter passphrase, or enter for none"));
      while(Serial.available()) Serial.read();
      while (!Serial.available());
      inString2 = Serial.readString();
      inString2.trim();
      if (inString2.length() > 1) {
        Serial.println(WiFiESP.begin((char *)inString1.c_str(), inString2.c_str()));
      } else {
        Serial.println(WiFiESP.begin((char *)inString1.c_str()));
      }
      break;
    case 2:
      Serial.println(F("Disconnecting..."));
      Serial.println(WiFiESP.disconnect());
      break;
    case 3:
      Serial.println(F("Enter the network item to get the SSID of, or press enter for the current SSID"));
      while(Serial.available()) Serial.read();
      while (!Serial.available());
      if ((Serial.peek() == '\r') || (Serial.peek() == '\n')) {
        Serial.println(WiFiESP.SSID());
        break;
      }
      item = Serial.parseInt();
      Serial.println(WiFiESP.SSID(item));
      break;
    case 4:
      Serial.println(F("Scanning for networks..."));
      Serial.println(WiFiESP.scanNetworks());
      break;
    case 5:
      Serial.println(F("Getting connection status..."));
      Serial.println(WiFiESP.status());
      break;
    case 6:
      Serial.println(F("Creating a new client..."));
      if (clientCtr < CLIENT_COUNT) {
        clients[clientCtr] = WiFiESPClient();
        clientCtr++;
      } else {
        Serial.println(F("Maximum number of clients reached"));
      }
      break;
    case 7:
      Serial.println(F("Connect client by IP address"));
      Serial.print(F("Enter client to use (up to ")); Serial.print(clientCtr); Serial.println(')');
      while(Serial.available()) Serial.read();
      while (!Serial.available());
      mux = Serial.parseInt();
      Serial.println(F("Enter IP address"));
      ip = getAddress();
      Serial.println(F("Enter port"));
      while(Serial.available()) Serial.read();
      while (!Serial.available());
      port = Serial.parseInt();
      Serial.println(clients[mux].connect(ip, port));
      break;
    case 8:
      Serial.println(F("Connect client by host name"));
      Serial.print(F("Enter client to use (up to ")); Serial.print(clientCtr); Serial.println(')');
      while(Serial.available()) Serial.read();
      while (!Serial.available());
      mux = Serial.parseInt();
      Serial.println(F("Enter IP address"));
      while(Serial.available()) Serial.read();
      while (!Serial.available());
      inString1 = Serial.readString();
      Serial.println(F("Enter port"));
      while(Serial.available()) Serial.read();
      while (!Serial.available());
      port = Serial.parseInt();
      Serial.println(clients[mux].connect(inString1.c_str(), port));
      break;
    case 9:
      Serial.println(F("Send one byte over an established connection"));
      Serial.print(F("Enter client to use (up to ")); Serial.print(clientCtr); Serial.println(')');
      while(Serial.available()) Serial.read();
      while (!Serial.available());
      mux = Serial.parseInt();
      Serial.println(F("Enter the byte to be sent"));
      while(Serial.available()) Serial.read();
      while (!Serial.available());
      c = Serial.read();
      Serial.println(clients[mux].write(c));
      break;
    case 10:
      Serial.println(F("Send a buffer over an established connection"));
      Serial.print(F("Enter client to use (up to ")); Serial.print(clientCtr); Serial.println(')');
      while(Serial.available()) Serial.read();
      while (!Serial.available());
      mux = Serial.parseInt();
      Serial.println(F("Enter the buffer to be sent"));
      while(Serial.available()) Serial.read();
      while (!Serial.available());
      inString1 = Serial.readString();
      Serial.println(clients[mux].write((uint8_t *)inString1.c_str(), inString1.length()));
      break;
    case 11:
      Serial.println(F("Get the number of characters available in the buffer of a given client"));
      Serial.print(F("Enter client to use (up to ")); Serial.print(clientCtr); Serial.println(')');
      while(Serial.available()) Serial.read();
      while (!Serial.available());
      mux = Serial.parseInt();
      Serial.println(clients[mux].available());
      break;
    case 12:
      Serial.println(F("Read a single character from a given client"));
      Serial.print(F("Enter client to use (up to ")); Serial.print(clientCtr); Serial.println(')');
      while(Serial.available()) Serial.read();
      while (!Serial.available());
      mux = Serial.parseInt();
      Serial.println(clients[mux].read());
      break;
    case 13:
      Serial.println(F("Read a buffer of data from a given client"));
      Serial.print(F("Enter client to use (up to ")); Serial.print(clientCtr); Serial.println(')');
      while(Serial.available()) Serial.read();
      while (!Serial.available());
      mux = Serial.parseInt();
      Serial.print(F("Enter number of bytes to read"));
      while(Serial.available()) Serial.read();
      while (!Serial.available());
      dataLen = Serial.parseInt();
      Serial.println(clients[mux].read((uint8_t *)incomingData, dataLen));
      Serial.println(incomingData);
      break;
    case 14:
      Serial.println(F("Peek a single character from a given client"));
      Serial.print(F("Enter client to use (up to ")); Serial.print(clientCtr); Serial.println(')');
      while(Serial.available()) Serial.read();
      while (!Serial.available());
      mux = Serial.parseInt();
      Serial.println(clients[mux].peek());
      break;
    case 15:
      Serial.println(F("Flush the input buffer of a given client"));
      Serial.print(F("Enter client to use (up to ")); Serial.print(clientCtr); Serial.println(')');
      while(Serial.available()) Serial.read();
      while (!Serial.available());
      mux = Serial.parseInt();
      clients[mux].flush();
      break;
    case 16:
      Serial.println(F("Stop a given client"));
      Serial.print(F("Enter client to use (up to ")); Serial.print(clientCtr); Serial.println(')');
      while(Serial.available()) Serial.read();
      while (!Serial.available());
      mux = Serial.parseInt();
      clients[mux].stop();
      break;
    case 17:
      Serial.println(F("Check whether a given client is connected"));
      Serial.print(F("Enter client to use (up to ")); Serial.print(clientCtr); Serial.println(')');
      while(Serial.available()) Serial.read();
      while (!Serial.available());
      mux = Serial.parseInt();
      Serial.println(clients[mux].connected());
      break;
    case 18:
      Serial.println(F("Checking the boolean operator..."));
      Serial.print(F("Enter client to use (up to ")); Serial.print(clientCtr); Serial.println(')');
      while(Serial.available()) Serial.read();
      while (!Serial.available());
      mux = Serial.parseInt();
      Serial.println(bool(clients[mux]));
      break;
    case 19:
      Serial.print(F("There are ")); Serial.print(clientCtr); Serial.println(F(" clients"));
      break;
    default:
      break;
  };
}
