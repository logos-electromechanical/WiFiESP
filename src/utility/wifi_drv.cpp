/*
  wifi_drv.cpp - Library for Arduino Wifi shield.
  Copyright (c) 2011-2014 Arduino.  All right reserved.

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

#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "Arduino.h"
#include "utility/wifi_drv.h"
#include "utility/at_drv.h"

#define _DEBUG_

extern "C" {
#include "utility/wl_types.h"
#include "utility/debug.h"
}

// Array of data to cache the information related to the networks discovered
char 	WiFiESPDrv::_networkSsid[][WL_SSID_MAX_LENGTH] = {{"1"},{"2"},{"3"},{"4"},{"5"}};
int32_t WiFiESPDrv::_networkRssi[WL_NETWORKS_LIST_MAXNUM] = { 0 };
uint8_t WiFiESPDrv::_networkEncr[WL_NETWORKS_LIST_MAXNUM] = { 0 };

// Cached values of retrieved data
char 	WiFiESPDrv::_ssid[] = {0};
uint8_t	WiFiESPDrv::_bssid[] = {0};
uint8_t WiFiESPDrv::_mac[] = {0};
uint8_t WiFiESPDrv::_localIp[] = {0};
uint8_t WiFiESPDrv::_subnetMask[] = {0};
uint8_t WiFiESPDrv::_gatewayIp[] = {0};

// Firmware version
char    WiFiESPDrv::fwVersion[] = {0};

// AT driver
ATDrvClass WiFiESPDrv::_esp = ATDrvClass(&WL_AT_DEVICE, WL_AT_BAUD);

// Public Methods

void WiFiESPDrv::wifiDriverInit()
{
	// Check that the ESP chip is active & reboot if not
	// Put ESP into station mode & activate DHCP
	// Turn on the mux for multiple connections
}

int8_t WiFiESPDrv::wifiSetNetwork(char* ssid, uint8_t ssid_len)
{
	// Set target network
}

int8_t WiFiESPDrv::wifiSetPassphrase(char* ssid, uint8_t ssid_len, const char *passphrase, const uint8_t len)
{
	// connect to named AP with AT+CWJAP_CUR and the given passphrase
}

int8_t WiFiESPDrv::wifiSetKey(char* ssid, uint8_t ssid_len, uint8_t key_idx, const void *key, const uint8_t len)
{
	// Since the ESP doesn't support WEP, this always fails
	return WL_FAILURE;
}

void WiFiESPDrv::config(uint8_t validParams, uint32_t local_ip, uint32_t gateway, uint32_t subnet)
{
	// use AT+CIPSTA_CUR to set local IP, gateway, and subnet, as requested
}

void WiFiESPDrv::setDNS(uint8_t validParams, uint32_t dns_server1, uint32_t dns_server2)
{
	// The DNS servers are set in firmware, so this is a no-op
}

int8_t WiFiESPDrv::disconnect()
{
	// Close connection with AT+CIPCLOSE
}

uint8_t WiFiESPDrv::getConnectionStatus()
{
	// get connection status with AT+CIPSTATUS
}

uint8_t* WiFiESPDrv::getMacAddress()
{
	// get local MAC address with AT+CIPSTAMAC?
}

void WiFiESPDrv::getIpAddress(IPAddress& ip)
{
	// get local IP address with AT+CIPSTA?
}

 void WiFiESPDrv::getSubnetMask(IPAddress& mask)
 {
	// No way I know of to get this, so do nothing
 }

 void WiFiESPDrv::getGatewayIP(IPAddress& ip)
 {
	// No way I know of to get this, so do nothing
 }

char* WiFiESPDrv::getCurrentSSID()
{
	// grab SSID with AT+CWJAP_CUR?
}

uint8_t* WiFiESPDrv::getCurrentBSSID()
{
	// grab BSSID with AT+CWJAP_CUR?
}

int32_t WiFiESPDrv::getCurrentRSSI()
{
	// grab RSSI with AT+CWJAP_CUR?
}

uint8_t WiFiESPDrv::getCurrentEncryptionType()
{
	// Not sure how to grab this, so returns 0 for now.
	return 0;
}

int8_t WiFiESPDrv::startScanNetworks()
{
	// Start network scan with AT+CWLAP
	// Note that this causes disconnect from any current AP
}

uint8_t WiFiESPDrv::getScanNetworks()
{
	// Returns number of entries in the list from AT+CWLAP
}

char* WiFiESPDrv::getSSIDNetoworks(uint8_t networkItem)
{
	if (networkItem >= WL_NETWORKS_LIST_MAXNUM)
		return NULL;

	return _networkSsid[networkItem];
}

uint8_t WiFiESPDrv::getEncTypeNetowrks(uint8_t networkItem)
{
	if (networkItem >= WL_NETWORKS_LIST_MAXNUM)
		return NULL;

	return _networkEncr[networkItem];

}

int32_t WiFiESPDrv::getRSSINetoworks(uint8_t networkItem)
{
	if (networkItem >= WL_NETWORKS_LIST_MAXNUM)
		return NULL;
	
	return _networkRssi[networkItem];
}

uint8_t WiFiESPDrv::reqHostByName(const char* aHostname)
{
	// deprecated because no higher level code uses it
}

int WiFiESPDrv::getHostByName(IPAddress& aResult)
{
	// deprecated because no higher level code uses it
}

int WiFiESPDrv::getHostByName(const char* aHostname, IPAddress& aResult)
{
	// there's no proper way to do this, so we connect to port 80 and check status for the IP
	// this is amazingly lame and weak -- need to fork the AT command set to fix this
	// However, since I'm also controlling the higher level things and the ESP takes a string,
	// I should be able to just deprecate this entirely
}

char*  WiFiESPDrv::getFwVersion()
{
	// returns output of AT+GMR
}

WiFiESPDrv wifiESPDrv;
