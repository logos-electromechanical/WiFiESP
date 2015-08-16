#include <WiFiESP.h>
#include <WiFiESPClient.h>
#include <WiFiESPServer.h>
#include <WiFiESPUdp.h>
#include <IPAddress.h>
#include "utility/at_drv.h"

IPAddress getAddress(void) {
  uint8_t o1, o2, o3, o4;
  Serial.println(F("enter first octet"));
  while (Serial.available()) Serial.read();
  while (!Serial.available());
  o1 = Serial.parseInt();
  Serial.println(F("enter second octet"));
  while (Serial.available()) Serial.read();
  while (!Serial.available());
  o2 = Serial.parseInt();
  Serial.println(F("enter third octet"));
  while (Serial.available()) Serial.read();
  while (!Serial.available());
  o3 = Serial.parseInt();
  Serial.println(F("enter fourth octet"));
  while (Serial.available()) Serial.read();
  while (!Serial.available());
  o4 = Serial.parseInt();
  return IPAddress(o1, o2, o3, o4);
}

/* delays on Serial until a number is seen */
void scrubInput (void) {
  char c;
  if (Serial.available()) {
    c = Serial.peek();
    if ((c == '0') || (c == '1') || (c == '2') || 
        (c == '3') || (c == '4') || (c == '5') || 
        (c == '6') || (c == '7') || (c == '8') || (c == '9')) {
          return;
        } else {
          Serial.read();
          scrubInput();
        }
  } else return;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Starting up...");
  Serial1.begin(9600);
  delay(100);
  atDrv.init(9600);
}

void loop() {
  int8_t selection;
  char c;
  uint8_t mode, mode1, mode2, mux, linkStat, shellCnt = 0;
  uint32_t timeout, port;
  String retString, inString1, inString2, inString3;
  IPAddress ip, gateway, subnet;
  uint16_t localport, remoteport;
  String protocol, remotehost;

  while(Serial.available()) Serial.read();
  Serial.println(F("Enter 0 for list of commands"));
  while (!Serial.available()) {
    if (serialEventRun) serialEventRun();
    if (Serial1.available()) Serial.print((char)Serial1.read());
  }
  selection = Serial.parseInt();
  while(Serial.available()) Serial.read();
  switch(selection) {
    case 0:
      Serial.println(F("Select the AT command to run:"));
      Serial.println(F("1) execute AT"));
      Serial.println(F("2) set AT+CIPDINFO"));
      Serial.println(F("3) execute AT+RST"));
      Serial.println(F("4) execute AT+GMR"));
      Serial.println(F("5) query AT+CWMODE"));
      Serial.println(F("6) set AT+CWMODE"));
      Serial.println(F("7) query AT+CWJAP (get current access point)"));
      Serial.println(F("8) set AT+CWJAP (join access point)"));
      Serial.println(F("9) execute AT+QAP (quit access point)"));
      Serial.println(F("10) execute AT+LAP (list access points)"));
      Serial.println(F("11) query AT+DHCP"));
      Serial.println(F("12) set AT+DHCP"));
      Serial.println(F("13) query AT+CIPSTAMAC"));
      Serial.println(F("14) query AT+CIPSTAIP"));
      Serial.println(F("15) set AT+CIPSTAIP"));
      Serial.println(F("16) execute AT+CIPSTATUS"));
      Serial.println(F("17) set AT+CIPSTART (String version, start connection, only works in multiple mode)"));
      Serial.println(F("18) set AT+CIPSTART (IPAddres version, start connection, only works in multiple mode)"));
      Serial.println(F("19) set AT+CIPSEND (entered data version)"));
      Serial.println(F("20) set AT+CIPSEND (from Flash version)"));
      Serial.println(F("21) set AT+CIPCLOSE (close connection, only works in multiple mode)"));
      Serial.println(F("22) execute AT+CIFSR (returns local IP address)"));
      Serial.println(F("23) set AT+CIPMUX (set mux status)"));
      Serial.println(F("24) set AT+CIPSERVER (starts TCP server)"));
      Serial.println(F("25) set AT+CIPMODE (transition to passthrough mode)"));
      Serial.println(F("26) execute AT+PING"));
      Serial.println(F("27) set AT+CIPSTO (time to live)"));
      Serial.println(F("28) query AT+CIPBUFSTATUS"));
      Serial.println(F("29) parse the response from AT_CIPSTATUS"));
      Serial.println(F("30) get access point configuration"));
      Serial.println(F("31) set access point configuration"));
      Serial.println(F("32) open line until +++ is entered"));
      break;
    case 1:
      Serial.println(F("Executing AT command"));
      Serial.print(F("Start time: ")); Serial.println(millis());
      Serial.println(atDrv.eAT());
      Serial.print(F("Stop time: ")); Serial.println(millis());
      break;
    case 2:
      Serial.println(F("Enter desired mode to set for AT+CIPDINFO"));
      Serial.println(F("0 doesn't show the remote IP & port with +IPD, 1 does show it."));
      scrubInput();
      while (!Serial.available());
      mode = Serial.parseInt();
      while(Serial.available()) Serial.read();
      Serial.println(F("Setting CIPDINFO mode"));
      Serial.print(F("Start time: ")); Serial.println(millis());
      Serial.println(atDrv.sATCIPDINFO(mode));
      Serial.print(F("Stop time: ")); Serial.println(millis());
      break;
    case 3:
      Serial.println(F("Enter desired timeout for reset, in ms"));
      scrubInput();
      while (!Serial.available());
      timeout = Serial.parseInt();
      Serial.println(F("Executing AT+RST command"));
      Serial.print(F("Start time: ")); Serial.println(millis());
      Serial.println(atDrv.eATRST(timeout));
      Serial.print(F("Stop time: ")); Serial.println(millis());
      break;
    case 4:
      Serial.println(F("Fetching version string with AT+GMR command"));
      Serial.print(F("Start time: ")); Serial.println(millis());
      Serial.println(atDrv.eATGMR(retString));
      Serial.print(F("Stop time: ")); Serial.println(millis());
      Serial.println(retString);
      break;
    case 5:
      Serial.println(F("Get the AT+CWMODE"));
      Serial.println(F("Enter 1 for current value, 2 for default, or 3 for old-style"));
      scrubInput();
      while (!Serial.available());
      mode = Serial.parseInt();
      mode1 = 0;
      Serial.println(F("Querying AT+CWMODE"));
      Serial.print(F("Start time: ")); Serial.println(millis());
      Serial.println(atDrv.qATCWMODE(&mode1, mode));
      Serial.print(F("Stop time: ")); Serial.println(millis());
      Serial.print(F("Returned value: "));
      Serial.println(mode1);
      break;
    case 6:
      Serial.println(F("Set the AT+CWMODE"));
      Serial.println(F("Enter 1 for station mode, 2 for softAP mode, and 3 for both"));
      scrubInput();
      while (!Serial.available());
      mode = Serial.parseInt();
      Serial.println(F("Enter 1 for current value, 2 for default, or 3 for old-style"));
      scrubInput();
      while (!Serial.available());
      mode1 = Serial.parseInt();
      Serial.println(F("Setting AT+CWMODE"));
      Serial.print(F("Start time: ")); Serial.println(millis());
      Serial.println(atDrv.sATCWMODE(mode, mode1));
      Serial.print(F("Stop time: ")); Serial.println(millis());
      break;
    case 7:
      Serial.println(F("Get the current access point"));
      Serial.println(F("Enter 1 for current value, 2 for default, or 3 for old-style"));
      scrubInput();
      while (!Serial.available());
      mode = Serial.parseInt();
      Serial.println(F("Querying AT+CWJAP"));
      Serial.print(F("Start time: ")); Serial.println(millis());
      Serial.println(atDrv.qATCWJAP(retString, mode));
      Serial.print(F("Stop time: ")); Serial.println(millis());
      Serial.println(retString);
      break;
    case 8:
      Serial.println(F("Set the current access point"));
      Serial.println(F("Enter the SSID"));
      while (Serial.available()) Serial.read();
      while (!Serial.available());
      inString1 = Serial.readString();
      inString1.trim();
      Serial.println(F("Enter the password"));
      while (Serial.available()) Serial.read();
      while (!Serial.available());
      inString2 = Serial.readString();
      inString2.trim();
      Serial.println(F("Enter 1 for current value, 2 for default, or 3 for old-style"));
      while (!Serial.available());
      mode = Serial.parseInt();
      Serial.println(F("Setting AT+CWJAP"));
      Serial.print(F("Start time: ")); Serial.println(millis());
      Serial.println(atDrv.sATCWJAP(inString1, inString2, mode));
      Serial.print(F("Stop time: ")); Serial.println(millis());
      break;
    case 9:
      Serial.println(F("Quitting current access point"));
      Serial.print(F("Start time: ")); Serial.println(millis());
      Serial.println(atDrv.eATCWQAP());
      Serial.print(F("Stop time: ")); Serial.println(millis());
      break;
    case 10:
      Serial.println(F("Getting list of available access points"));
      Serial.print(F("Start time: ")); Serial.println(millis());
      Serial.println(atDrv.eATCWLAP(retString));
      Serial.print(F("Stop time: ")); Serial.println(millis());
      Serial.println(retString);
      break;
    case 11:
      Serial.println(F("Get the DHCP mode"));
      Serial.println(F("Enter 1 for current value, 2 for default, or 3 for old-style"));
      while (Serial.available()) Serial.read();
      while (!Serial.available());
      mode = Serial.parseInt();
      mode1 = 0;
      mode2 = 0;
      Serial.println(F("Querying AT+CWDHCP"));
      Serial.print(F("Start time: ")); Serial.println(millis());
      Serial.println(atDrv.qATCWDHCP(&mode1, &mode2, mode));
      Serial.print(F("Stop time: ")); Serial.println(millis());
      Serial.print(F("Mode:\t")); Serial.println(mode1);
      Serial.print(F("Enable:\t")); Serial.println(mode2);
      break;
    case 12:
      Serial.println(F("Set the DHCP mode"));
      Serial.println(F("Enter 0 for softAP mode, 1 for station mode, and 2 for both"));
      while (Serial.available()) Serial.read();
      while (!Serial.available());
      mode1 = Serial.parseInt();
      Serial.println(F("Enter 0 to disable DHCP and 1 to enable it"));
      while (Serial.available()) Serial.read();
      while (!Serial.available());
      mode2 = Serial.parseInt();
      Serial.println(F("Enter 1 for current value, 2 for default, or 3 for old-style"));
      while (Serial.available()) Serial.read();
      while (!Serial.available());
      mode = Serial.parseInt();
      Serial.println(F("Setting AT+CWDHCP"));
      Serial.print(F("Start time: ")); Serial.println(millis());
      Serial.println(atDrv.sATCWDHCP(mode1, mode2, mode));
      Serial.print(F("Stop time: ")); Serial.println(millis());
      break;
    case 13:
      Serial.println(F("Get the station MAC address"));
      Serial.println(F("Enter 1 for current value, 2 for default, or 3 for old-style"));
      while (Serial.available()) Serial.read();
      while (!Serial.available());
      mode = Serial.parseInt();
      Serial.println(F("Querying AT+CIPSTAMAC"));
      Serial.print(F("Start time: ")); Serial.println(millis());
      Serial.println(atDrv.qATCIPSTAMAC(retString, mode));
      Serial.print(F("Stop time: ")); Serial.println(millis());
      break;
    case 14:
      Serial.println(F("Get the station IP address"));
      Serial.println(F("Enter 1 for current value, 2 for default, or 3 for old-style"));
      while (Serial.available()) Serial.read();
      while (!Serial.available());
      mode = Serial.parseInt();
      Serial.println(F("Querying AT+CIPSTAMIP"));
      Serial.print(F("Start time: ")); Serial.println(millis());
      Serial.println(atDrv.qATCIPSTAIP(retString, mode));
      Serial.print(F("Stop time: ")); Serial.println(millis());
      break;
    case 15:
      Serial.println(F("Setting the station IP address"));
      Serial.println(F("Enter the number of parameters you want (1 for IP only, 2 for IP + gateway, 3 for IP + gateway + subnet mask"));
      while (Serial.available()) Serial.read();
      while (!Serial.available());
      mode = Serial.parseInt();
      if (mode >= 1) {
        Serial.println(F("Enter desired IP address"));
        ip = getAddress();
      }
      if (mode >= 2) {
        Serial.println(F("Entered the desired gateway"));
        gateway = getAddress();
      }
      if (mode >= 3) {
        mode = 3;
        Serial.println(F("Entered the desired subnet mask"));
        subnet = getAddress();
      }
      Serial.println(F("Enter 1 for current value, 2 for default, or 3 for old-style"));
      while (Serial.available()) Serial.read();
      while (!Serial.available());
      mode1 = Serial.parseInt();
      Serial.println(F("Setting AT+CIPSTAMIP"));
      Serial.print(F("Start time: ")); Serial.println(millis());
      Serial.println(atDrv.sATCIPSTAIP(mode, uint32_t(ip), uint32_t(gateway), uint32_t(subnet), mode1));
      Serial.print(F("Stop time: ")); Serial.println(millis());
      break;
    case 16:
      Serial.println(F("Get current IP Status"));
      Serial.print(F("Start time: ")); Serial.println(millis());
      Serial.println(atDrv.eATCIPSTATUS(retString));
      Serial.print(F("Stop time: ")); Serial.println(millis());
      Serial.println(retString);
      break;
    case 17:
      Serial.println(F("Open a connection to a given host"));
      Serial.println(F("Enter desired mux ID"));
      while (Serial.available()) Serial.read();
      while (!Serial.available());
      mux = Serial.parseInt();
      Serial.println(F("Enter the desired mode (UDP or TCP)"));
      while (Serial.available()) Serial.read();
      while (!Serial.available());
      inString1 = Serial.readString();
      inString1.trim();
      Serial.println(F("Enter the desired host or IP address"));
      while (Serial.available()) Serial.read();
      while (!Serial.available());
      inString2 = Serial.readString();
      inString2.trim();
      Serial.println(F("Enter the desired remote port"));
      while (Serial.available()) Serial.read();
      while (!Serial.available());
      port = Serial.parseInt();
      Serial.println(F("Opening connection"));
      Serial.print(F("Start time: ")); Serial.println(millis());
      Serial.println(atDrv.sATCIPSTARTMultiple(mux, inString1, inString2, port));
      Serial.print(F("Stop time: ")); Serial.println(millis());
      break;       
    case 18:
      Serial.println(F("Open a connection to a given IP address"));
      while (Serial.available()) Serial.read();
      Serial.println(F("Enter desired mux ID"));
      while (!Serial.available());
      mux = Serial.parseInt();
      Serial.println(F("Enter the desired mode (UDP or TCP)"));
      while (Serial.available()) Serial.read();
      while (!Serial.available());
      inString1 = Serial.readString();
      inString1.trim();
      Serial.println(F("Enter the desired IP address"));
      while (Serial.available()) Serial.read();
      ip = getAddress();
      Serial.println(F("Enter the desired remote port"));
      while (Serial.available()) Serial.read();
      while (!Serial.available());
      port = Serial.parseInt();
      Serial.println(F("Opening connection"));
      Serial.print(F("Start time: ")); Serial.println(millis());
      Serial.println(atDrv.sATCIPSTARTMultiple(mux, inString1, ip, port));
      Serial.print(F("Stop time: ")); Serial.println(millis());
      break;       
    case 19:
      Serial.println(F("Send some data out on a given mux"));
      Serial.println(F("Enter the desired mux"));
      while (Serial.available()) Serial.read();
      while (!Serial.available());
      mux = Serial.parseInt();
      Serial.println(F("Enter the data to send"));
      while (Serial.available()) Serial.read();
      while (!Serial.available());
      inString1 = Serial.readString();
      Serial.println(F("Sending data"));
      Serial.print(F("Start time: ")); Serial.println(millis());
      Serial.println(atDrv.sATCIPSENDMultiple(mux, (const uint8_t *)inString1.c_str(), inString1.length()));
      Serial.print(F("Stop time: ")); Serial.println(millis());
      break;
    case 20:
      Serial.println(F("Send some data from flash out on a given mux"));
      Serial.println(F("Enter the desired mux"));
      while (Serial.available()) Serial.read();
      while (!Serial.available());
      mux = Serial.parseInt();
      Serial.println(F("Sending data"));
      Serial.print(F("Start time: ")); Serial.println(millis());
      Serial.println(atDrv.sATCIPSENDMultiple(mux, (const uint8_t *)F("Here is some data going out"), 30));
      Serial.print(F("Stop time: ")); Serial.println(millis());
      break;
    case 21:
      Serial.println(F("Close the connection on a given mux"));
      Serial.println(F("Enter the desired mux"));
      while (Serial.available()) Serial.read();
      while (!Serial.available());
      mux = Serial.parseInt();
      Serial.println(F("Closing connection"));
      Serial.print(F("Start time: ")); Serial.println(millis());
      Serial.println(atDrv.sATCIPCLOSEMultiple(mux));
      Serial.print(F("Stop time: ")); Serial.println(millis());
      break;
    case 22:
      Serial.println(F("Geting the local IP address"));
      Serial.print(F("Start time: ")); Serial.println(millis());
      Serial.println(atDrv.eATCIFSR(retString));
      Serial.print(F("Stop time: ")); Serial.println(millis());
      Serial.println(retString);
      break;
    case 23:  
      Serial.println(F("Set the mux to on or off"));
      Serial.println(F("Enter the desired mux value; 0 for single mode, 1 to turn the mux on"));
      while (Serial.available()) Serial.read();
      while (!Serial.available());
      mode = Serial.parseInt();
      Serial.println(F("Setting mux"));
      Serial.print(F("Start time: ")); Serial.println(millis());
      Serial.println(atDrv.sATCIPMUX(mode));
      Serial.print(F("Stop time: ")); Serial.println(millis());
      break;
    case 24:
      Serial.println(F("Configure ESP8266 as a TCP server"));
      Serial.println(F("Enter 1 to create a server or 0 to delete it"));
      while (Serial.available()) Serial.read();
      while (!Serial.available());
      mode = Serial.parseInt();
      Serial.println(F("Enter the desired port to listen on"));
      while (Serial.available()) Serial.read();
      while (!Serial.available());
      port = Serial.parseInt();
      Serial.println(F("Configuring server"));
      Serial.print(F("Start time: ")); Serial.println(millis());
      Serial.println(atDrv.sATCIPSERVER(mode, port));
      Serial.print(F("Stop time: ")); Serial.println(millis());
      break;
    case 25:
      Serial.println(F("Choose transfer mode between standard (0) or UART-WiFI passthrough mode (1)"));
      while (Serial.available()) Serial.read();
      while (!Serial.available());
      mode = Serial.parseInt();
      Serial.println(F("Setting transfer mode"));
      Serial.print(F("Start time: ")); Serial.println(millis());
      Serial.println(atDrv.sATCIPMODE(mode));
      Serial.print(F("Stop time: ")); Serial.println(millis());
      break;
    case 26:
      Serial.println(F("Ping selected host"));
      Serial.println(F("Enter the name of the host to ping."));
      while (Serial.available()) Serial.read();
      while (!Serial.available());
      inString1 = Serial.readString();
      inString1.trim();
      Serial.print(F("Start time: ")); Serial.println(millis());
      Serial.println(atDrv.eATPING(inString1));
      Serial.print(F("Stop time: ")); Serial.println(millis());
      break;
    case 27:  
      Serial.println(F("Set the TCP time to live"));
      Serial.println(F("Enter the desired time to live, in ms"));
      while (Serial.available()) Serial.read();
      while (!Serial.available());
      timeout = Serial.parseInt();
      Serial.print(F("Start time: ")); Serial.println(millis());
      Serial.println(atDrv.sATCIPSTO(timeout));
      Serial.print(F("Stop time: ")); Serial.println(millis());
      break;
    case 28:
      Serial.println(F("Check the TCP output buffer status on a given mux id"));
      Serial.println(F("Enter the desired mux to check"));
      while (Serial.available()) Serial.read();
      while (!Serial.available());
      mux = Serial.parseInt();
      Serial.print(F("Start time: ")); Serial.println(millis());
      Serial.println(atDrv.qCIPBUFSTATUS(mux));
      Serial.print(F("Stop time: ")); Serial.println(millis());
      break;
    case 29:
      Serial.println(F("Fetching and parsing AT+CIPSTATUS output"));
      Serial.println(F("Enter the desired mux to check"));
      while (Serial.available()) Serial.read();
      while (!Serial.available());
      mux = Serial.parseInt();
      linkStat = 0;
      protocol = "";
      remotehost = "";
      remoteport = 0;
      localport = 0;
      mode = 0;
      Serial.print(F("Start time: ")); Serial.println(millis());
      Serial.println(atDrv.parseStatus(mux, &linkStat, protocol, remotehost, &remoteport, &localport, &mode));
      Serial.print(F("Stop time: ")); Serial.println(millis());
      Serial.print(F("Protocol: ")); Serial.println(protocol);
      Serial.print(F("Remote host: ")); Serial.println(remotehost);
      Serial.print(F("Remote port: ")); Serial.println(remoteport);
      Serial.print(F("Local port: ")); Serial.println(localport);
      Serial.print(F("Mode: ")); Serial.println(mode);
      break;
    case 30:
      Serial.println(F("Fetching access point configuration with AT+CWSAP?"));
      Serial.println(F("Enter 1 for current value, 2 for default, or 3 for old-style"));
      while (Serial.available()) Serial.read();
      while (!Serial.available());
      mode = Serial.parseInt();
      Serial.println(atDrv.qATCWSAP(retString, mode));
      Serial.println(retString);
      break;
    case 31:
      Serial.println(F("Setting access point configuration with AT+CWSAP?"));
      Serial.println(F("Enter the SSID"));
      while (Serial.available()) Serial.read();
      while (!Serial.available());
      inString1 = Serial.readString();
      inString1.trim();
      Serial.println(F("Enter the password"));
      while (Serial.available()) Serial.read();
      while (!Serial.available());
      inString2 = Serial.readString();
      inString2.trim();
      Serial.println(F("Enter the desired channel"));
      while (Serial.available()) Serial.read();
      while (!Serial.available());
      mode1 = Serial.parseInt();
      Serial.println(F("Enter the desired encryption"));
      while (Serial.available()) Serial.read();
      while (!Serial.available());
      mode2 = Serial.parseInt();
      Serial.println(F("Enter 1 for current value, 2 for default, or 3 for old-style"));
      while (Serial.available()) Serial.read();
      while (!Serial.available());
      mode = Serial.parseInt();
      Serial.println(atDrv.sATCWSAP(inString1, inString2, mode1, mode2, mode));
      break;
    case 32:
      Serial.println(F("Entering shell mode. Type +++ to exit"));
      while (shellCnt < 3) {
        if (Serial1.available()) Serial.print((char)Serial1.read());
        if (Serial.available()) {
          c = Serial.read();
          if (c == '+') {
            shellCnt++;
          } else shellCnt = 0;
          Serial1.print(c);
        }
      }
      break;
    default:
      break;
  };
}
