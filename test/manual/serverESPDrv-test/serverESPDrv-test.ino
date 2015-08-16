#include <WiFiESP.h>
#include <WiFiESPClient.h>
#include <WiFiESPServer.h>
#include <WiFiESPUdp.h>
#include <IPAddress.h>
#include "utility/wifi_drv.h"
#include "utility/server_drv.h"

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
  wifiESPDrv.wifiDriverInit();
  Serial.println(F("Remember to set line endings to either \\n or \\r\\n"));
}

void loop() {
  // put your main code here, to run repeatedly:
  int8_t selection;
  char c;
  uint8_t mode, mode1, mode2, mux, item, sock;
  uint32_t timeout, port;
  String retString, inString1, inString2, inString3;
  IPAddress ip, gateway, subnet, dns;
  uint8_t * bytePtr;
  uint8_t macAddressPtr[WL_MAC_ADDR_LENGTH];
  char incomingData[1024];
  uint16_t dataLen;

  Serial.println(F("Enter 0 for list of commands"));
  while(Serial.available()) Serial.read();
  while (!Serial.available()) {
    if (serialEventRun) serialEventRun();
    if (Serial1.available()) Serial.print((char *)Serial1.read());
  }
  selection = Serial.parseInt();
  switch(selection) {
    case 0:
      Serial.println(F("Select the WiFiESPDrv function to run:"));
      Serial.println(F("1) wifiSetPassphrase()"));
      Serial.println(F("2) getConnectionStatus()"));
      Serial.println(F("3) startScanNetworks()"));
      Serial.println(F("4) getScanNetworks()"));
      Serial.println(F("5) getSSIDNetworks()"));
      Serial.println(F("6) getFwVersion()"));
      Serial.println(F("Select the ServerESPDrv function to run:"));
      Serial.println(F("7) startServer()"));
      Serial.println(F("8) startClient() with IP address"));
      Serial.println(F("9) startClient() with host name"));
      Serial.println(F("10) stopClient()"));
      Serial.println(F("11) getServerState()"));
      Serial.println(F("12) getClientState()"));
      Serial.println(F("13) getData()"));
      Serial.println(F("14) getDataBuf()"));
      Serial.println(F("15) insertDataBuf()"));
      Serial.println(F("16) sendData()"));
      Serial.println(F("17) sendUdpData()"));
      Serial.println(F("18) availData()"));
      Serial.println(F("19) checkDataSent()"));
      break;
    case 1:
      Serial.println(F("Connecting to a network..."));
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
      Serial.println(wifiESPDrv.wifiSetPassphrase((char *)inString1.c_str(), inString1.length(), inString2.c_str(), inString2.length()));
      break;
    case 2:
      Serial.println(F("Getting connection status"));
      Serial.println(wifiESPDrv.getConnectionStatus());
      break;
    case 3:
      Serial.println(F("Scanning for networks..."));
      Serial.println(wifiESPDrv.startScanNetworks());
      break;
    case 4:
      Serial.println(F("Get the number of networks found by scanning..."));
      Serial.println(wifiESPDrv.getScanNetworks());
      break;
    case 5:
      Serial.println(F("Enter the network item to get the SSID of"));
      while(Serial.available()) Serial.read();
      while (!Serial.available());
      item = Serial.parseInt();
      Serial.println(wifiESPDrv.getSSIDNetworks(item));
      break;
    case 6:
      Serial.println(F("Getting firmware version string"));
      Serial.println(wifiESPDrv.getFwVersion());
      break;
    case 7:
      Serial.println(F("Starting a server.."));
      Serial.println(F("Enter desired port"));
      while(Serial.available()) Serial.read();
      while (!Serial.available());
      port = Serial.parseInt();
      Serial.println(F("Enter desired socket (0-4)"));
      while(Serial.available()) Serial.read();
      while (!Serial.available());
      mux = Serial.parseInt();
      Serial.println(F("Enter desired mode -- 0 for TCP, 1 for UDP "));
      while(Serial.available()) Serial.read();
      while (!Serial.available());
      mode = Serial.parseInt();
      if (mode == 0) {
        serverESPDrv.startServer(port, mux, TCP_MODE);
      } else if (mode == 1) {
        serverESPDrv.startServer(port, mux, UDP_MODE);
      } else Serial.println(F("Invalid mode selection"));
      break;
    case 8:
      Serial.println(F("Starting a client with an IP address..."));
      Serial.println(F("Enter desired IP address"));
      ip = getAddress();
      Serial.println(F("Enter desired port"));
      while(Serial.available()) Serial.read();
      while (!Serial.available());
      port = Serial.parseInt();
      Serial.println(F("Enter desired socket (0-4)"));
      while(Serial.available()) Serial.read();
      while (!Serial.available());
      mux = Serial.parseInt();
      Serial.println(F("Enter desired mode -- 0 for TCP, 1 for UDP "));
      while(Serial.available()) Serial.read();
      while (!Serial.available());
      mode = Serial.parseInt();
      if (mode == 0) {
        serverESPDrv.startClient(uint32_t(ip), port, mux, TCP_MODE);
      } else if (mode == 1) {
        serverESPDrv.startClient(uint32_t(ip), port, mux, UDP_MODE);
      } else Serial.println(F("Invalid mode selection"));
      break;
    case 9:
      Serial.println(F("Starting a client with a host name..."));
      Serial.println(F("Enter the desired host or IP address"));
      while (Serial.available()) Serial.read();
      while (!Serial.available());
      inString1 = Serial.readString();
      inString1.trim();
      Serial.println(F("Enter desired port"));
      while(Serial.available()) Serial.read();
      while (!Serial.available());
      port = Serial.parseInt();
      Serial.println(F("Enter desired socket (0-4)"));
      while(Serial.available()) Serial.read();
      while (!Serial.available());
      mux = Serial.parseInt();
      Serial.println(F("Enter desired mode -- 0 for TCP, 1 for UDP "));
      while(Serial.available()) Serial.read();
      while (!Serial.available());
      mode = Serial.parseInt();
      if (mode == 0) {
        serverESPDrv.startClient((char *)inString1.c_str(), port, mux, TCP_MODE);
      } else if (mode == 1) {
        serverESPDrv.startClient((char *)inString1.c_str(), port, mux, UDP_MODE);
      } else Serial.println(F("Invalid mode selection"));
      break;
    case 10:  
      Serial.println(F("Stop client on a selected sock"));
      Serial.println(F("Enter desired socket (0-4)"));
      while(Serial.available()) Serial.read();
      while (!Serial.available());
      mux = Serial.parseInt();
      serverESPDrv.stopClient(mux);
      break;
    case 11:
      Serial.println(F("Getting state of a given sock (server)"));
      Serial.println(F("Enter desired socket (0-4)"));
      while(Serial.available()) Serial.read();
      while (!Serial.available());
      mux = Serial.parseInt();
      Serial.println(serverESPDrv.getServerState(mux));
      break;
    case 12:
      Serial.println(F("Getting state of a given sock (client)"));
      Serial.println(F("Enter desired socket (0-4)"));
      while(Serial.available()) Serial.read();
      while (!Serial.available());
      mux = Serial.parseInt();
      Serial.println(serverESPDrv.getClientState(mux));
      break;
    case 13:
      Serial.println(F("Get a byte from the buffer of a given sock"));
      Serial.println(F("Enter desired socket (0-4)"));
      while(Serial.available()) Serial.read();
      while (!Serial.available());
      mux = Serial.parseInt();
      Serial.println(F("Enter 0 to read, 1 to poke"));
      while(Serial.available()) Serial.read();
      while (!Serial.available());
      mode = Serial.parseInt();
      Serial.println(serverESPDrv.getData(mux, (uint8_t *)(&c), mode));
      Serial.print(F("Returned character: ")); Serial.println(c);
      break;
    case 14:
      Serial.println(F("Get a buffer of data from the buffer of a given sock"));
      Serial.println(F("Enter desired socket (0-4)"));
      while(Serial.available()) Serial.read();
      while (!Serial.available());
      mux = Serial.parseInt();
      Serial.println(F("Enter the number of bytes to read"));
      while(Serial.available()) Serial.read();
      while (!Serial.available());
      dataLen = Serial.parseInt();
      Serial.println(serverESPDrv.getDataBuf(mux, (uint8_t *)incomingData, &dataLen));
      Serial.print(F("Bytes of data read: ")); Serial.println(dataLen);
      Serial.print(F("Data buffer read: ")); Serial.println(incomingData);
      break;
    case 15:
      Serial.println(F("Insert data into transmit buffer for desired socket"));
      Serial.println(F("Enter desired socket (0-4)"));
      while(Serial.available()) Serial.read();
      while (!Serial.available());
      mux = Serial.parseInt();
      Serial.println(F("Enter data to insert"));
      while(Serial.available()) Serial.read();
      while (!Serial.available());
      inString1 = Serial.readString();
      Serial.println(serverESPDrv.insertDataBuf(mux, (const uint8_t *)inString1.c_str(), inString1.length()));
      break;
    case 16:
      Serial.println(F("Send data on a given socket"));
      Serial.println(F("Enter desired socket (0-4)"));
      while(Serial.available()) Serial.read();
      while (!Serial.available());
      mux = Serial.parseInt();
      Serial.println(F("Enter data to insert"));
      while(Serial.available()) Serial.read();
      while (!Serial.available());
      inString1 = Serial.readString();
      Serial.println(serverESPDrv.sendData(mux, (const uint8_t *)inString1.c_str(), inString1.length()));
      break;
    case 17:
      Serial.println(F("Send buffered UDP data on a given socket"));
      Serial.println(F("Enter desired socket (0-4)"));
      while(Serial.available()) Serial.read();
      while (!Serial.available());
      mux = Serial.parseInt();
      Serial.println(serverESPDrv.sendUdpData(mux));
      break;
    case 18:
      Serial.println(F("Get the number of bytes in the input buffer for a given socket"));
      Serial.println(F("Enter desired socket (0-4)"));
      while(Serial.available()) Serial.read();
      while (!Serial.available());
      mux = Serial.parseInt();
      Serial.println(serverESPDrv.availData(mux));
      break;
    case 19:
      Serial.println(F("Check the status of the data previously sent on given socket"));
      Serial.println(F("Enter desired socket (0-4)"));
      while(Serial.available()) Serial.read();
      while (!Serial.available());
      mux = Serial.parseInt();
      Serial.println(serverESPDrv.checkDataSent(mux));
      break;
    default:
      break;
  };

    
      

}
