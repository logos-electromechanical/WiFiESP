/*
  WiFi.cpp - Library for Arduino Wifi shield.
  Copyright (c) 2011-2014 Arduino LLC.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "utility/wifi_drv.h"
#include "WiFiESP.h"

extern "C" {
  #include "utility/wl_definitions.h"
  #include "utility/wl_types.h"
  #include "utility/debug.h"
}

// XXX: don't make assumptions about the value of MAX_SOCK_NUM.
int16_t 	WiFiESPClass::_state[MAX_SOCK_NUM] = { NA_STATE, NA_STATE, NA_STATE, NA_STATE };
uint16_t 	WiFiESPClass::_server_port[MAX_SOCK_NUM] = { 0, 0, 0, 0 };

WiFiESPClass::WiFiESPClass()
{
	// Driver initialization
	init();
}

void WiFiESPClass::init()
{
    wifiESPDrv::wifiDriverInit();
}

uint8_t WiFiESPClass::getSocket()
{
    for (uint8_t i = 0; i < MAX_SOCK_NUM; ++i)
    {
        if (WiFiESPClass::_server_port[i] == 0)
        {
             return i;
        }
    }
    return NO_SOCKET_AVAIL;
}

char* WiFiESPClass::firmwareVersion()
{
	return wifiESPDrv::getFwVersion();
}

int WiFiESPClass::begin(char* ssid)
{
	uint8_t status = WL_IDLE_STATUS;
	uint8_t attempts = WL_MAX_ATTEMPT_CONNECTION;

   if (wifiESPDrv::wifiSetNetwork(ssid, strlen(ssid)) != WL_FAILURE)
   {
	   do
	   {
		   delay(WL_DELAY_START_CONNECTION);
		   status = wifiESPDrv::getConnectionStatus();
	   }
	   while ((( status == WL_IDLE_STATUS)||(status == WL_SCAN_COMPLETED))&&(--attempts>0));
   }else
   {
	   status = WL_CONNECT_FAILED;
   }
   return status;
}

int WiFiESPClass::begin(char* ssid, const char *passphrase)
{
	uint8_t status = WL_IDLE_STATUS;
	uint8_t attempts = WL_MAX_ATTEMPT_CONNECTION;

    // set passphrase
    if (wifiESPDrv::wifiSetPassphrase(ssid, strlen(ssid), passphrase, strlen(passphrase))!= WL_FAILURE)
    {
 	   do
 	   {
 		   delay(WL_DELAY_START_CONNECTION);
 		   status = wifiESPDrv::getConnectionStatus();
 	   }
	   while ((( status == WL_IDLE_STATUS)||(status == WL_SCAN_COMPLETED))&&(--attempts>0));
    }else{
    	status = WL_CONNECT_FAILED;
    }
    return status;
}

void WiFiESPClass::config(IPAddress local_ip)
{
	wifiESPDrv::config(1, (uint32_t)local_ip, 0, 0);
}

void WiFiESPClass::config(IPAddress local_ip, IPAddress dns_server)
{
	wifiESPDrv::config(1, (uint32_t)local_ip, 0, 0);
	wifiESPDrv::setDNS(1, (uint32_t)dns_server, 0);
}

void WiFiESPClass::config(IPAddress local_ip, IPAddress dns_server, IPAddress gateway)
{
	wifiESPDrv::config(2, (uint32_t)local_ip, (uint32_t)gateway, 0);
	wifiESPDrv::setDNS(1, (uint32_t)dns_server, 0);
}

void WiFiESPClass::config(IPAddress local_ip, IPAddress dns_server, IPAddress gateway, IPAddress subnet)
{
	wifiESPDrv::config(3, (uint32_t)local_ip, (uint32_t)gateway, (uint32_t)subnet);
	wifiESPDrv::setDNS(1, (uint32_t)dns_server, 0);
}

void WiFiESPClass::setDNS(IPAddress dns_server1)
{
	wifiESPDrv::setDNS(1, (uint32_t)dns_server1, 0);
}

void WiFiESPClass::setDNS(IPAddress dns_server1, IPAddress dns_server2)
{
	wifiESPDrv::setDNS(2, (uint32_t)dns_server1, (uint32_t)dns_server2);
}

int WiFiESPClass::disconnect()
{
    return wifiESPDrv::disconnect();
}

uint8_t* WiFiESPClass::macAddress(uint8_t* mac)
{
	uint8_t* _mac = wifiESPDrv::getMacAddress();
	memcpy(mac, _mac, WL_MAC_ADDR_LENGTH);
    return mac;
}
   
IPAddress WiFiESPClass::localIP()
{
	IPAddress ret;
	wifiESPDrv::getIpAddress(ret);
	return ret;
}

IPAddress WiFiESPClass::subnetMask()
{
	IPAddress ret;
	wifiESPDrv::getSubnetMask(ret);
	return ret;
}

IPAddress WiFiESPClass::gatewayIP()
{
	IPAddress ret;
	wifiESPDrv::getGatewayIP(ret);
	return ret;
}

char* WiFiESPClass::SSID()
{
    return wifiESPDrv::getCurrentSSID();
}

uint8_t* WiFiESPClass::BSSID(uint8_t* bssid)
{
	uint8_t* _bssid = wifiESPDrv::getCurrentBSSID();
	memcpy(bssid, _bssid, WL_MAC_ADDR_LENGTH);
    return bssid;
}

int32_t WiFiESPClass::RSSI()
{
    return wifiESPDrv::getCurrentRSSI();
}

uint8_t WiFiESPClass::encryptionType()
{
    return wifiESPDrv::getCurrentEncryptionType();
}


int8_t WiFiESPClass::scanNetworks()
{
	uint8_t attempts = 10;
	uint8_t numOfNetworks = 0;

	if (wifiESPDrv::startScanNetworks() == WL_FAILURE)
		return WL_FAILURE;
 	do
 	{
 		delay(2000);
 		numOfNetworks = wifiESPDrv::getScanNetworks();
 	}
	while (( numOfNetworks == 0)&&(--attempts>0));
	return numOfNetworks;
}

char* WiFiESPClass::SSID(uint8_t networkItem)
{
	return wifiESPDrv::getSSIDNetoworks(networkItem);
}

int32_t WiFiESPClass::RSSI(uint8_t networkItem)
{
	return wifiESPDrv::getRSSINetoworks(networkItem);
}

uint8_t WiFiESPClass::encryptionType(uint8_t networkItem)
{
    return wifiESPDrv::getEncTypeNetowrks(networkItem);
}

uint8_t WiFiESPClass::status()
{
    return wifiESPDrv::getConnectionStatus();
}

int WiFiESPClass::hostByName(const char* aHostname, IPAddress& aResult)
{
	return wifiESPDrv::getHostByName(aHostname, aResult);
}

WiFiESPClass WiFi;
