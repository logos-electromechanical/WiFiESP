/*
  server_drv.cpp - Library for Arduino Wifi shield.
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

//#define _DEBUG_

#include "utility/server_drv.h"
#include "utility/at_drv.h"

#include "Arduino.h"

extern "C" {
#include "utility/wl_types.h"
#include "utility/debug.h"
}

// Start server TCP on port specified
void ServerESPDrv::startServer(uint16_t port, uint8_t sock, uint8_t protMode)
{
	// Start new server on port using AT+CIPSERVER for TCP
	// Uses AT+CIPSTART and sock for UDP
	// protMode is either TCP_MODE or UDP_MODE
	switch (protMode) {
		case TCP_MODE:
			atDrv.sATCIPSERVER(1, port);
			break;
		case UDP_MODE:
			atDrv.sATCIPSTARTMultiple(sock, "UDP", "", port);
			break;
		default:
			WARN("Invalid mode selection for startServer");
			break;
	};
}

// Start client TCP on port/IP specified
void ServerESPDrv::startClient(uint32_t ipAddress, uint16_t port, uint8_t sock, uint8_t protMode)
{
	// Open a client using AT+CIPSTART
	String type;
	IPAddress host = ipAddress;
	if (protMode == TCP_MODE) {
		type = "\"TCP\"";
	} else if (protMode == UDP_MODE) {
		type = "\"UDP\"";
	} else
	atDrv.sATCIPSTARTMultiple(sock, type, host, port);
}

void ServerESPDrv::startClient(char * host, uint16_t port, uint8_t sock, uint8_t protMode)
{
	// Open a client using AT+CIPSTART
	String type;
	String _host = host;
	if (protMode == TCP_MODE) {
		type = "\"TCP\"";
	} else if (protMode == UDP_MODE) {
		type = "\"UDP\"";
	} else
	atDrv.sATCIPSTARTMultiple(sock, type, _host, port);
}

// Stop server TCP on socket specified
void ServerESPDrv::stopClient(uint8_t sock)
{
	// Kill it with AT+CIPCLOSE. Works for UDP, TCP, and servers
	atDrv.sATCIPCLOSEMultiple(sock);
}


uint8_t ServerESPDrv::getServerState(uint8_t sock)
{
	// Grab the status of mux ID sock with AT+CIPSTATUS
	// Translate into the language of wl_tcp_state enum
	String list;
	String mode;
	String host;
	uint8_t dir;
	if (atDrv.eATCIPSTATUS(list)) {
		while(list.indexOf('+') != -1) {									// Is there another return line to check?
			list = list.substring(list.indexOf('+'));						// Chop off all preceding lines
			if ((uint8_t)((list.substring(list.indexOf(':') + 1)).toInt()) == sock) {	// Check if the current response line is the right one
				mode = list.substring((list.indexOf(',') + 1), (list.indexOf(',') + 6));
				list = list.substring(list.indexOf(',') + 1 );				// Chop after <link ID> (aka mux_id, sock, etc)
				list = list.substring(list.indexOf(',') + 1 );				// Chop after <type>
				host = list.substring(0, list.indexOf(','));				// grab remote IP address
				list = list.substring(list.indexOf(',') + 1 );				// Chop after <remote IP>
				list = list.substring(list.indexOf(',') + 1 );				// Chop after <remote port>
				list = list.substring(list.indexOf(',') + 1 );				// Chop after <local port>
				if (mode.equals(F("TCP"))) {
					dir = (uint8_t)list.toInt();
					if (dir) {
						if (host.length() > 8) {
							return ESTABLISHED;
						} else return LISTEN;
					} else {
						if (host.length() > 8) {
							return ESTABLISHED;
						} else return CLOSED;
					}
				} else if (mode.equals(F("UDP"))) {
					return LISTEN;
				}
			}
		}
		return CLOSED;		// If we get here, it means that there was no line for the given sock, so we assume closed
	} else return CLOSED;	// If we didn't get a good response from AT+CIPSTATUS, tell the world it's closed. 
	
}

uint8_t ServerESPDrv::getClientState(uint8_t sock)
{
	// Call getServerState (same difference)
	return getServerState(sock);
}

uint16_t ServerESPDrv::availData(uint8_t sock)
{
	atDrv.available(sock);
}

bool ServerESPDrv::getData(uint8_t sock, uint8_t *data, uint8_t peek)
{
	// grab a byte from the given socket (possibly as a peek)
	if (atDrv.available(sock)) {
		if (peek) {
			*data = atDrv.peekChar(sock);
		} else {
			*data = atDrv.getChar(sock);
		}
		return true;
	} else return false;
}

bool ServerESPDrv::getDataBuf(uint8_t sock, uint8_t *_data, uint16_t *_dataLen)
{
	// Grab all of the data waiting in the buffer for a given socket up to _datalen
	// The value of _datalen is modified with the amount of data returned. 
	if (atDrv.available(sock)) {
		*_dataLen = atDrv.getBuf(sock, _data, *_dataLen);
		return true;
	} else return false;
}

bool ServerESPDrv::insertDataBuf(uint8_t sock, const uint8_t *data, uint16_t _len)
{
	// Add transmit data to UDP write buffer
	if (atDrv.putBufTX(sock, data, _len)) return true;
	return false;
}

bool ServerESPDrv::sendUdpData(uint8_t sock)
{
	uint8_t _tmp[ESP_TX_BUFLEN];
	int16_t _len;
	if (atDrv.availableTX(sock)) {
		_len = atDrv.getBufTX(sock, _tmp, atDrv.availableTX(sock));
	} else return false;
	if (_len > 0) {
		return atDrv.sATCIPSENDMultiple(sock, _tmp, _len);
	} else return false;		
}

bool ServerESPDrv::sendData(uint8_t sock, const uint8_t *data, uint16_t len)
{
	return atDrv.sATCIPSENDMultiple(sock, data, len);
}


uint8_t ServerESPDrv::checkDataSent(uint8_t sock)
{
	// Check on how we're doing sending buffered data with AT+CIPBUFSTATUS
	return atDrv.qCIPBUFSTATUS(sock);
}

ServerESPDrv serverESPDrv;
