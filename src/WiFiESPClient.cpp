/*
  WiFiESPClient.cpp - Library for Arduino Wifi shield.
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

extern "C" {
  #include "utility/wl_definitions.h"
  #include "utility/wl_types.h"
  #include "string.h"
  #include "utility/debug.h"
}

#include "WiFiESP.h"
#include "WiFiESPClient.h"
#include "WiFiESPServer.h"
#include "utility/server_drv.h"


uint16_t WiFiESPClient::_srcport = 1024;

WiFiESPClient::WiFiESPClient() : _sock(MAX_SOCK_NUM) {
}

WiFiESPClient::WiFiESPClient(uint8_t sock) : _sock(sock) {
}

int WiFiESPClient::connect(const char* host, uint16_t port) {
	_sock = getFirstSocket();
    if (_sock != NO_SOCKET_AVAIL) {
    	serverESPDrv.startClient((char *)host, port, _sock);
    	WiFiESP._state[_sock] = _sock;
    	if (!connected()) {
    		return 0;
    	}
    } else {
    	Serial.println("No Socket available");
    	return 0;
    }
    return 1;
}

int WiFiESPClient::connect(IPAddress ip, uint16_t port) {
    _sock = getFirstSocket();
    if (_sock != NO_SOCKET_AVAIL) {
    	serverESPDrv.startClient(uint32_t(ip), port, _sock);
    	WiFiESP._state[_sock] = _sock;
    	if (!connected()) {
    		return 0;
    	}
    } else {
    	Serial.println("No Socket available");
    	return 0;
    }
    return 1;
}

size_t WiFiESPClient::write(uint8_t b) {
	  return write(&b, 1);
}

size_t WiFiESPClient::write(const uint8_t *buf, size_t size) {
  if (_sock >= MAX_SOCK_NUM)
  {
	  setWriteError();
	  return 0;
  }
  if (size==0)
  {
	  setWriteError();
      return 0;
  }


  if (!serverESPDrv.sendData(_sock, buf, size))
  {
	  setWriteError();
      return 0;
  }
  /*if (!ServerESPDrv::checkDataSent(_sock))
  {
	  setWriteError();
      return 0;
  }*/

  return size;
}

int WiFiESPClient::available() {
  if (_sock != 255)
  {
      return serverESPDrv.availData(_sock);
  }
   
  return 0;
}

int WiFiESPClient::read() {
  uint8_t b;
  if (!available())
    return -1;

  serverESPDrv.getData(_sock, &b);
  return b;
}


int WiFiESPClient::read(uint8_t* buf, size_t size) {
  // sizeof(size_t) is architecture dependent
  // but we need a 16 bit data type here
  uint16_t _size = size;
  if (!serverESPDrv.getDataBuf(_sock, buf, &_size))
      return -1;
  return _size;
}

int WiFiESPClient::peek() {
	  uint8_t b;
	  if (!available())
	    return -1;

	  serverESPDrv.getData(_sock, &b, 1);
	  return b;
}

void WiFiESPClient::flush() {
  while (available())
    read();
}

void WiFiESPClient::stop() {

  if (_sock == 255)
    return;

  serverESPDrv.stopClient(_sock);
  WiFiESP._state[_sock] = NA_STATE;

  int count = 0;
  // wait maximum 5 secs for the connection to close
  while (status() != CLOSED && ++count < 50)
    delay(100);

  _sock = 255;
}

uint8_t WiFiESPClient::connected() {

  if (_sock == 255) {
    return 0;
  } else {
    uint8_t s = status();

    return !(s == LISTEN || s == CLOSED || s == FIN_WAIT_1 ||
    		s == FIN_WAIT_2 || s == TIME_WAIT ||
    		s == SYN_SENT || s== SYN_RCVD ||
    		(s == CLOSE_WAIT));
  }
}

uint8_t WiFiESPClient::status() {
  if (_sock == 255) {
    return CLOSED;
  } else {
    return serverESPDrv.getClientState(_sock);
  }
}

WiFiESPClient::operator bool() {
  return _sock != 255;
}

// Private Methods
uint8_t WiFiESPClient::getFirstSocket()
{
    for (int i = 0; i < MAX_SOCK_NUM; i++) {
      if (WiFiESP._state[i] == NA_STATE)
      {
          return i;
      }
    }
    return SOCK_NOT_AVAIL;
}

