#include <WiFiESP.h>
#include <WiFiESPClient.h>
#include <WiFiESPServer.h>
#include <WiFiESPUdp.h>
#include <IPAddress.h>
#include "utility/wifi_drv.h"

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
  //wifiESPDrv.init();
  Serial.println(F("Remember to set line endings to either \\n or \\r\\n"));
}

void loop() {
  // put your main code here, to run repeatedly:
  int8_t selection;
  char c;
  uint8_t mode, mode1, mode2, mux, item, shellCnt = 0;
  uint32_t timeout, port;
  String retString, inString1, inString2, inString3;
  IPAddress ip, gateway, subnet, dns;
  uint8_t * bytePtr;
  uint8_t macAddressPtr[WL_MAC_ADDR_LENGTH];

  Serial.println(F("Enter 0 for list of commands"));
  while(Serial.available()) Serial.read();
  while (!Serial.available()) {
    if (Serial1.available()) Serial.print((char *)Serial1.read());
  }
  selection = Serial.parseInt();
  switch(selection) {
    case 0:
      Serial.println(F("Select the WiFiESPDrv function to run:"));
      Serial.println(F("1) wifiDriverInit()"));
      Serial.println(F("2) wifiSetNetwork()"));
      Serial.println(F("3) wifiSetPassphrase()"));
      Serial.println(F("4) config()"));
      Serial.println(F("5) disconnect()"));
      Serial.println(F("6) getConnectionStatus()"));
      Serial.println(F("7) getMacAddress()"));
      Serial.println(F("8) getIpAddress()"));
      Serial.println(F("9) getSubnetMask()"));
      Serial.println(F("10) getGatewayIP()"));
      Serial.println(F("11) getCurrentSSID()"));
      Serial.println(F("12) getCurrentBSSID()"));
      Serial.println(F("13) getCurrentRSSI()"));
      Serial.println(F("14) getCurrentEncryptionType()"));
      Serial.println(F("15) startScanNetworks()"));
      Serial.println(F("16) getScanNetworks()"));
      Serial.println(F("17) getSSIDNetworks()"));
      Serial.println(F("18) getRSSINetworks()"));
      Serial.println(F("19) getEncTypeNetworks()"));
      Serial.println(F("20) getFwVersion()"));
      break;
      case 1:
        Serial.println(F("Initializing ESP..."));
        wifiESPDrv.wifiDriverInit();
        break;
      case 2:
        Serial.println(F("Connecting to a network with no passphrase..."));
        Serial.println(F("Enter SSID to connect to"));
        while(Serial.available()) Serial.read();
        while (!Serial.available());
        inString1 = Serial.readString();
        inString1.trim();
        Serial.println(wifiESPDrv.wifiSetNetwork((char *)inString1.c_str(), inString1.length()));
        break;
      case 3:
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
      case 4:
        Serial.println(F("Set IP configuration..."));
        Serial.println(F("Enter number of parameters"));
        while(Serial.available()) Serial.read();
        while (!Serial.available());
        mode = Serial.parseInt();
        if (mode > 3) mode = 3;
        if (mode < 1) mode = 1;
        if (mode >= 1) {
          Serial.println(F("Enter IP address"));
          ip = getAddress();
        } else ip = IPAddress((uint32_t)0);
        if (mode >= 2) {
          Serial.println(F("Enter gateway address"));
          gateway = getAddress();
        } else gateway = IPAddress((uint32_t)0);
        if (mode >= 3) {
          Serial.println(F("Enter gateway address"));
          subnet = getAddress();
        } else subnet = IPAddress((uint32_t)0);
        wifiESPDrv.config(mode, uint32_t(ip), uint32_t(gateway), uint32_t(subnet));
        break;
      case 5:
        Serial.println(F("Disconnection from current access point"));
        Serial.println(wifiESPDrv.disconnect());
        break;
      case 6:
        Serial.println(F("Getting connection status"));
        Serial.println(wifiESPDrv.getConnectionStatus());
        break;
      case 7:
        Serial.println(F("Getting local MAC address"));
        bytePtr = wifiESPDrv.getMacAddress();
        for (uint8_t i = 0; i < (WL_MAC_ADDR_LENGTH - 1); i++) {
          Serial.print(bytePtr[i], HEX);
          Serial.print(':');
        }
        Serial.println(bytePtr[(WL_MAC_ADDR_LENGTH - 1)]);
        break;
      case 8:
        Serial.println(F("Getting local IP address"));
        ip = IPAddress((uint32_t)0);
        wifiESPDrv.getIpAddress(ip);
        Serial.println(ip);
        break;
      case 9:
        Serial.println(F("Getting local subnet mask"));
        ip = IPAddress((uint32_t)0);
        wifiESPDrv.getSubnetMask(ip);
        Serial.println(ip);
        break;
      case 10:
        Serial.println(F("Getting gateway IP address"));
        ip = IPAddress((uint32_t)0);
        wifiESPDrv.getGatewayIP(ip);
        Serial.println(ip);
        break;
      case 11:
        Serial.println(F("Getting current SSID"));
        Serial.println(wifiESPDrv.getCurrentSSID());
        break;
      case 12:
        Serial.println(F("Getting current BSSID"));
        bytePtr = wifiESPDrv.getCurrentBSSID();
        for (uint8_t i = 0; i < (WL_MAC_ADDR_LENGTH - 1); i++) {
          Serial.print(bytePtr[i], HEX);
          Serial.print(':');
        }
        Serial.println(bytePtr[(WL_MAC_ADDR_LENGTH - 1)]);
        break;
      case 13:
        Serial.println(F("Getting current RSSI"));
        Serial.println(wifiESPDrv.getCurrentRSSI());
        break;
      case 14:
        Serial.println(F("Getting current encryption type"));
        Serial.println(wifiESPDrv.getCurrentEncryptionType());
        break;
      case 15:
        Serial.println(F("Scanning for networks..."));
        Serial.println(wifiESPDrv.startScanNetworks());
        break;
      case 16:
        Serial.println(F("Get the number of networks found by scanning..."));
        Serial.println(wifiESPDrv.getScanNetworks());
        break;
      case 17:
        Serial.println(F("Enter the network item to get the SSID of"));
        while(Serial.available()) Serial.read();
        while (!Serial.available());
        item = Serial.parseInt();
        Serial.println(wifiESPDrv.getSSIDNetworks(item));
        break;
      case 18:
        Serial.println(F("Enter the network item to get the encryption type of"));
        while(Serial.available()) Serial.read();
        while (!Serial.available());
        item = Serial.parseInt();
        Serial.println(wifiESPDrv.getEncTypeNetworks(item));
        break;
      case 19:
        Serial.println(F("Enter the network item to get the SSID of"));
        while(Serial.available()) Serial.read();
        while (!Serial.available());
        item = Serial.parseInt();
        Serial.println(wifiESPDrv.getSSIDNetworks(item));
        break;
      case 20:
        Serial.println(F("Getting firmware version string"));
        Serial.println(wifiESPDrv.getFwVersion());
        break;
    default:
      break;
  };

    
      

}
