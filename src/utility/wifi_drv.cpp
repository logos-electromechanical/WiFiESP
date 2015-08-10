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
String 	WiFiESPDrv::_networkSsid[WL_NETWORKS_LIST_MAXNUM] = {{String("")}};
int32_t WiFiESPDrv::_networkRssi[WL_NETWORKS_LIST_MAXNUM] = { 0 };
uint8_t WiFiESPDrv::_networkEncr[WL_NETWORKS_LIST_MAXNUM] = { 0 };
uint8_t WiFiESPDrv::_networkCount = 0;

// Cached values of retrieved data
String 	WiFiESPDrv::_ssid = String("");
uint8_t	WiFiESPDrv::_bssid[] = {0};
uint8_t WiFiESPDrv::_mac[] = {0};
uint8_t WiFiESPDrv::_localIp[] = {0};
uint8_t WiFiESPDrv::_subnetMask[] = {0};
uint8_t WiFiESPDrv::_gatewayIp[] = {0};

// Firmware version
String	WiFiESPDrv::fwVersion = String("");

// Public Methods

void WiFiESPDrv::wifiDriverInit()
{
	// Check that the ESP chip is active & reboot if not
	// Put ESP into station mode & activate DHCP
	// Turn on the mux for multiple connections
	atDrv.init(9600);
	while(!atDrv.eAT()) {
		WARN("Unable to connect to ESP8266, attempting to restart");
		atDrv.eATRST(WL_CONNECT_TIMEOUT);
	}
	if(!atDrv.sATCWDHCP(WL_MODE_STA, WL_DHCP_ENB, ESP_AT_CUR)) {
		WARN("Unable to set DHCP & mode");
	}
	if(!atDrv.sATCIPMUX(WL_MUX_MULT)) {
		WARN("Unable to set mux to multiple");
	}
	atDrv.sATCIPSTO(WL_CONNECT_TIMEOUT);	// Set TCP timeout
	//sATCIPDINFO(1);					// Turn on IPD info
	atDrv.eATGMR(fwVersion);		// grab the firmware version so we don't have to do it again
}

int8_t WiFiESPDrv::wifiSetNetwork(char* ssid, uint8_t ssid_len)
{
	return wifiSetPassphrase(ssid, ssid_len, NULL, 0);
}

int8_t WiFiESPDrv::wifiSetPassphrase(char* ssid, uint8_t ssid_len, const char *passphrase, const uint8_t len)
{
	// connect to named AP with AT+CWJAP_CUR and the given passphrase
	if (ssid_len > WL_SSID_MAX_LENGTH) ssid_len = WL_SSID_MAX_LENGTH;
	_ssid = String((const char *)ssid);
	String local_pwd = String(passphrase);
	if (atDrv.sATCWJAP(_ssid, local_pwd, ESP_AT_CUR)) {
		return WL_SUCCESS;
	} else {
		return WL_FAILURE;
	}
}

void WiFiESPDrv::config(uint8_t validParams, uint32_t local_ip, uint32_t gateway, uint32_t subnet)
{
	// use AT+CIPSTA_CUR to set local IP, gateway, and subnet, as requested
	atDrv.sATCIPSTAIP(validParams, local_ip, gateway, subnet, ESP_AT_CUR);
}

void WiFiESPDrv::setDNS(uint8_t validParams, uint32_t dns_server1, uint32_t dns_server2)
{
	// The DNS servers are set in firmware, so this is a no-op
}

int8_t WiFiESPDrv::disconnect()
{
	return (uint8_t)atDrv.eATCWQAP();
}

uint8_t WiFiESPDrv::getConnectionStatus()
{
	// returns the results of AT+CWJAP_CUR?
	String cSSID;
	bool ret;
	ret = atDrv.qATCWJAP(cSSID, ESP_AT_CUR);
	if (ret) {
		return WL_SUCCESS;
	} else {
		return WL_FAILURE;
	}
}

uint8_t* WiFiESPDrv::getMacAddress()
{
	// get local MAC address with AT+CIPSTAMAC?
	String cMAC, procS;
	uint8_t i;
	bool ret;
	ret = atDrv.qATCIPSTAMAC(cMAC, ESP_AT_CUR);
	if (ret && (cMAC.indexOf(':') != -1)) {
		procS = cMAC.substring(cMAC.indexOf('"') + 1);	// grab the start of the MAC address
		for (i = 0; i < WL_MAC_ADDR_LENGTH; i++) {
			_mac[i] = (uint8_t)strtol(procS.c_str(), NULL, 16);
			procS = procS.substring(procS.indexOf(':') + 1);
		}
	}
	return _mac;
}

void WiFiESPDrv::getIpAddress(IPAddress& ip)
{
	// get local IP address with AT+CIPSTA?
	String cIP, procS;
	uint8_t i;
	bool ret;
	ret = atDrv.qATCIPSTAIP(cIP, ESP_AT_CUR);
	if (ret && (cIP.indexOf(':') != -1)) {
		procS = cIP.substring(cIP.indexOf('"') + 1);	// grab the start of the IP address
		for (i = 0; i < WL_MAC_ADDR_LENGTH; i++) {
			_localIp[i] = (uint8_t)procS.toInt();
			procS = procS.substring(procS.indexOf('.') + 1);
		}
	}
	ip = _localIp;
}

 void WiFiESPDrv::getSubnetMask(IPAddress& mask)
 {
	// No way I know of to get this, so do nothing
 }

 void WiFiESPDrv::getGatewayIP(IPAddress& ip)
 {
	// No way I know of to get this, so do nothing
 }

const char* WiFiESPDrv::getCurrentSSID()
{
	// grab SSID with AT+CWJAP_CUR?
	String cSSID;
	bool ret;
	ret = atDrv.qATCWJAP(cSSID, ESP_AT_CUR);
	if (ret && (cSSID.indexOf(':') != -1)) {
		_ssid = cSSID.substring(cSSID.indexOf(':'), cSSID.indexOf(','));
		// trim off the leading and trailing quotation marks
		_ssid.remove(_ssid.indexOf('"'), 1);
		_ssid.remove(_ssid.indexOf('"'), 1);
	} 
	return _ssid.c_str();
}

uint8_t* WiFiESPDrv::getCurrentBSSID()
{
	// grab BSSID with AT+CWJAP_CUR?
	String retS, procS, byteS;
	uint8_t i;
	bool ret;
	ret = atDrv.qATCWJAP(retS, ESP_AT_CUR);
	if (ret && (retS.indexOf(':') != -1)) {
		procS = retS.substring(retS.indexOf(',') + 1);
		for (i = 0; i < WL_MAC_ADDR_LENGTH; i++) {
			_bssid[i] = (uint8_t)strtol(procS.c_str(), NULL, 16);
			procS = procS.substring(procS.indexOf(':') + 1);
		}
	} 
	return _bssid;
}

int32_t WiFiESPDrv::getCurrentRSSI()
{
	// grab RSSI with AT+CWJAP_CUR?
	String retS;
	bool ret;
	ret = atDrv.qATCWJAP(retS, ESP_AT_CUR);
	if (ret && (retS.indexOf(':') != -1)) {
		retS = retS.substring(retS.indexOf(',') + 1);	// skip over the SSID
		retS = retS.substring(retS.indexOf(',') + 1);	// skip over the BSSID
		retS = retS.substring(retS.indexOf(',') + 1);	// skip over the channel
		return (int32_t)retS.toInt();
	} else {
		return WL_FAILURE;
	}
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
	String list;
	uint8_t count;
	if (atDrv.eATCWLAP(list) && (count < WL_NETWORKS_LIST_MAXNUM)) {
		while ((list.indexOf('+') != -1) && (count < WL_NETWORKS_LIST_MAXNUM)) {
			list = list.substring(list.indexOf('+'));
			if (list.startsWith(F("+CWLAP:"))) {
				list = list.substring(list.indexOf(':') + 1);	// go to the start of the encryption field
				_networkEncr[count] = (uint8_t)list.toInt();
				list = list.substring(list.indexOf(',') + 2);	// go to the start of the SSID field and chop off the leading quotation mark 
				_networkSsid[count] = list.substring(0, list.indexOf('"'));	// grab the SSID field
				list = list.substring(list.indexOf(',') + 1);	// go to the start of the RSSI field
				_networkRssi[count] = (int32_t)list.toInt();
			} else break;
			count++;
		}
		_networkCount = count;
		if (_networkCount == 0) {
			return WL_FAILURE;
		} else {
			return WL_SUCCESS;
		}
	} else {
		return WL_FAILURE;
	}
}

uint8_t WiFiESPDrv::getScanNetworks()
{
	return _networkCount;
}

const char* WiFiESPDrv::getSSIDNetworks(uint8_t networkItem)
{
	if (networkItem >= WL_NETWORKS_LIST_MAXNUM)
		return NULL;

	return _networkSsid[networkItem].c_str();
}

uint8_t WiFiESPDrv::getEncTypeNetworks(uint8_t networkItem)
{
	if (networkItem >= WL_NETWORKS_LIST_MAXNUM)
		return NULL;

	return _networkEncr[networkItem];

}

int32_t WiFiESPDrv::getRSSINetworks(uint8_t networkItem)
{
	if (networkItem >= WL_NETWORKS_LIST_MAXNUM)
		return NULL;
	
	return _networkRssi[networkItem];
}

int WiFiESPDrv::getHostByName(const char* aHostname, IPAddress& aResult)
{
	// there's no proper way to do this, so we connect to port 80 and check status for the IP
	// this is amazingly lame and weak -- need to fork the AT command set to fix this
	// However, since I'm also controlling the higher level things and the ESP takes a string,
	// Therefore, I'm just going to deprecate this PoS and move on.
}

const char*  WiFiESPDrv::getFwVersion()
{
	// returns output of AT+GMR
	return fwVersion.c_str();
}

bool WiFiESPDrv::isAccessPoint() {
	uint8_t mode = 0;
	atDrv.qATCWMODE(&mode, ESP_AT_CUR);
	if (mode >= 2) return true;
	return false;
}

bool WiFiESPDrv::setAPMode() {
	return atDrv.sATCWMODE(2, ESP_AT_CUR);
}

bool WiFiESPDrv::setStationMode() {
	return atDrv.sATCWMODE(1, ESP_AT_CUR);
}

bool WiFiESPDrv::setAPconfig(const char* SSID, uint8_t SSIDLen, const char* password, uint8_t passlen, uint8_t channel, uint8_t encryption) {
	String _SSID = String(SSID);
	String _PWD = String(password);
	return atDrv.sATCWSAP(_SSID, _PWD, channel, encryption, ESP_AT_CUR);
}

bool WiFiESPDrv::getAPconfig(char* SSID, char* passwd, uint8_t *channel, uint8_t *encryption) {
	String list; 
	String _SSID, _PWD;
	uint8_t len;
	if (atDrv.qATCWSAP(list, ESP_AT_CUR) && (list.indexOf('+') != -1)) {
		list = list.substring(list.indexOf('+'));
		if (list.startsWith(String(F("+CWSAP:")))) {
			list = list.substring(list.indexOf(':') + 1);
			_SSID = list.substring(0, list.indexOf(','));
			len = _SSID.length();
			if (len > WL_SSID_MAX_LENGTH) len = WL_SSID_MAX_LENGTH;
			memcpy(SSID, _SSID.c_str(), len);
			list = list.substring(list.indexOf(',') + 1);
			_PWD = list.substring(0, list.indexOf(','));
			len = _PWD.length();
			if (len > WL_WPA_KEY_MAX_LENGTH) len = WL_WPA_KEY_MAX_LENGTH;
			memcpy(passwd, _PWD.c_str(), len);
			list = list.substring(list.indexOf(',') + 1);
			*channel = (uint8_t)list.toInt();
			list = list.substring(list.indexOf(',') + 1);
			*encryption = (uint8_t)list.toInt();
			return true;
		} else return false;
	} else return false;
}


WiFiESPDrv wifiESPDrv;
