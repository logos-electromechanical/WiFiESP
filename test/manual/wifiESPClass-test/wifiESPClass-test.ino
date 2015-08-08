#include <WiFiESP.h>
#include <WiFiESPClient.h>
#include <WiFiESPServer.h>
#include <WiFiESPUdp.h>
#include <IPAddress.h>

IPAddress getAddress(void) {
  uint8_t o1, o2, o3, o4;
  Serial.println(F("enter first octet"));
  while (!Serial.available());
  if ((Serial.peek() == '\r') || (Serial.peek() == '\n')) return IPAddress((uint32_t)0);
  o1 = Serial.parseInt();
  Serial.println(F("enter second octet"));
  while (!Serial.available());
  o2 = Serial.parseInt();
  Serial.println(F("enter third octet"));
  while (!Serial.available());
  o3 = Serial.parseInt();
  Serial.println(F("enter fourth octet"));
  while (!Serial.available());
  o4 = Serial.parseInt();
  return IPAddress(o1, o2, o3, o4);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  //atDrv.init(9600);
  Serial.println(F("Remember to set line endings to either \n or \r\n"));
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
  while (!Serial.available()) {
    if (Serial1.available()) Serial.print(Serial1.read());
  }
  selection = Serial.parseInt();
  switch(selection) {
    case 0:
      Serial.println(F("Select the ESP function to run:"));
      Serial.println(F("1) getSocket()"));
      Serial.println(F("2) firmwareVersion()"));
      Serial.println(F("3) begin()"));
      Serial.println(F("4) config()"));
      Serial.println(F("5) disconnect()"));
      Serial.println(F("6) macAddress()"));
      Serial.println(F("7) localIP()"));
      Serial.println(F("8) subnetMask()"));
      Serial.println(F("9) gatewayIP()"));
      Serial.println(F("10) SSID()"));
      Serial.println(F("11) BSSID()"));
      Serial.println(F("12) RSSI()"));
      Serial.println(F("13) encryptionType()"));
      Serial.println(F("14) scanNetworks()"));
      Serial.println(F("15) status()"));
      break;
    case 1:
      Serial.println(F("Getting socket..."));
      Serial.println(WiFiESP.getSocket());
      break;
    case 2:
      Serial.println(F("Getting firmware version..."));
      Serial.println(WiFiESP.firmwareVersion());
      break;
    case 3:
      Serial.println(F("Enter SSID to connect to"));
      while (!Serial.available());
      inString1 = Serial.readString();
      inString1.trim();
      Serial.println(F("Enter passphrase, or enter for none"));
      while (!Serial.available());
      inString2 = Serial.readString();
      inString2.trim();
      if (inString2.length() > 1) {
        Serial.println(WiFiESP.begin((char *)inString1.c_str(), inString2.c_str()));
      } else {
        Serial.println(WiFiESP.begin((char *)inString1.c_str()));
      }
      break;
    case 4:
      Serial.println(F("Enter desired local IP"));
      ip = getAddress();
      Serial.println(F("Enter desired DNS server, or press enter to end entry of parameters. Note that DNS server cannot be set on the ESP8266"));
      dns = getAddress();
      if (uint32_t(dns) == 0) {
        WiFiESP.config(ip);
        break;
      }
      Serial.println(F("Enter desired gateway, or press enter to end entry of parameters"));
      gateway = getAddress();
      if (uint32_t(gateway) == 0) {
        WiFiESP.config(ip, dns);
        break;
      }
      Serial.println(F("Enter desired subnet mask, or press enter to end entry of parameters"));
      subnet = getAddress();
      if (uint32_t(subnet) == 0) {
        WiFiESP.config(ip, dns, gateway);
        break;
      }
      WiFiESP.config(ip, dns, gateway, subnet);
      break;
    case 5:
      Serial.println(F("Disconnecting..."));
      Serial.println(WiFiESP.disconnect());
      break;
    case 6:
      Serial.println(F("Getting local MAC address..."));
      WiFiESP.macAddress(macAddressPtr);
      for (uint8_t i = 0; i < WL_MAC_ADDR_LENGTH; i++) {
        Serial.print(macAddressPtr[i], HEX);
        Serial.print(':');
      }
      Serial.println(F(""));
      break;
    case 7:
      Serial.println(F("Getting local IP address..."));
      ip = WiFiESP.localIP();
      Serial.println(ip);
      break;
    case 8:
      Serial.println(F("Getting local subnet..."));
      ip = WiFiESP.subnetMask();
      Serial.println(ip);
      break;
    case 9:
      Serial.println(F("Getting local gateway address..."));
      ip = WiFiESP.gatewayIP();
      Serial.println(ip);
      break;
    case 10:
      Serial.println(F("Enter the network item to get the SSID of, or press enter for the current SSID"));
      while (!Serial.available());
      if ((Serial.peek() == '\r') || (Serial.peek() == '\n')) {
        Serial.println(WiFiESP.SSID());
        break;
      }
      item = Serial.parseInt();
      Serial.println(WiFiESP.SSID(item));
      break;
    case 11:
      Serial.println(F("Getting the current BSSID..."));
      WiFiESP.BSSID(macAddressPtr);
      for (uint8_t i = 0; i < WL_MAC_ADDR_LENGTH; i++) {
        Serial.print(macAddressPtr[i], HEX);
        Serial.print(':');
      }
      Serial.println(F(""));
      break;
    case 12:
      Serial.println(F("Enter the network item to get the RSSI of, or press enter for the current RSSI"));
      while (!Serial.available());
      if ((Serial.peek() == '\r') || (Serial.peek() == '\n')) {
        Serial.println(WiFiESP.RSSI());
        break;
      }
      item = Serial.parseInt();
      Serial.println(WiFiESP.RSSI(item));
      break;
    case 13:
      Serial.println(F("Enter the network item to get the encryption type of, or press enter for the current type"));
      while (!Serial.available());
      if ((Serial.peek() == '\r') || (Serial.peek() == '\n')) {
        Serial.println(WiFiESP.encryptionType());
        break;
      }
      item = Serial.parseInt();
      Serial.println(WiFiESP.encryptionType(item));
      break;
    case 14:
      Serial.println(F("Scanning for networks..."));
      Serial.println(WiFiESP.scanNetworks());
      break;
    case 15:
      Serial.println(F("Getting connection status..."));
      Serial.println(WiFiESP.status());
      break;
    default:
      break;
  };

    
      

}
