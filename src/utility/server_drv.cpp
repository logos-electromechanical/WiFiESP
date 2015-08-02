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
	// Start new server on port, using sock, using AT+CIPSERVER for TCP
	// Uses AT+CIPSTART for UDP
	// protMode is either TCP_MODE or UDP_MODE
}

// Start client TCP on port/IP specified
void ServerESPDrv::startClient(uint32_t ipAddress, uint16_t port, uint8_t sock, uint8_t protMode)
{
	// Calls the version of this that uses a host string
}

void ServerESPDrv::startClient(char * host, uint16_t port, uint8_t sock, uint8_t protMode)
{
	// Open a client using AT+CIPSTART
}

// Stop server TCP on socket specified
void ServerESPDrv::stopClient(uint8_t sock)
{
	// Kill it with AT+CIPCLOSE. Works for UDP, TCP, and servers
}


uint8_t ServerESPDrv::getServerState(uint8_t sock)
{
	// Grab the status of mux ID sock with AT+CIPSTATUS
	// Translate into the language of wl_tcp_state enum
}

uint8_t ServerESPDrv::getClientState(uint8_t sock)
{
	// Call getServerState (same difference)
}

uint16_t ServerESPDrv::availData(uint8_t sock)
{
	// return the available data a given socket
}

bool ServerESPDrv::getData(uint8_t sock, uint8_t *data, uint8_t peek)
{
	// grab a byte from the given socket (possibly as a peek)
}

bool ServerESPDrv::getDataBuf(uint8_t sock, uint8_t *_data, uint16_t *_dataLen)
{
	// Grab all of the data waiting in the buffer for a given socket
}

bool ServerESPDrv::insertDataBuf(uint8_t sock, const uint8_t *data, uint16_t _len)
{
	// Add transmit data to UDP write buffer
}

bool ServerESPDrv::sendUdpData(uint8_t sock)
{
	// Send the UDP write buffer using AT+CIPSEND
}


bool ServerESPDrv::sendData(uint8_t sock, const uint8_t *data, uint16_t len)
{
	// Send some data using AT+CIPSEND
}


uint8_t ServerESPDrv::checkDataSent(uint8_t sock)
{
	// Check on how we're doing sending buffered data with AT+CIPBUFSTATUS
}

ServerESPDrv serverESPDrv;
