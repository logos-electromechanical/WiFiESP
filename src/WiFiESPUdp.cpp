/*
  WiFiESPUDP.cpp - Library for Arduino Wifi shield.
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
  #include "utility/debug.h"
}
#include <string.h>
#include "utility/server_drv.h"
#include "utility/wifi_drv.h"

#include "WiFiESP.h"
#include "WiFiESPUdp.h"
#include "WiFiESPClient.h"
#include "WiFiESPServer.h"


/* Constructor */
WiFiESPUDP::WiFiESPUDP() : _sock(NO_SOCKET_AVAIL) {}

/* Start WiFiESPUDP socket, listening at local port PORT */
uint8_t WiFiESPUDP::begin(uint16_t port) {

    uint8_t sock = WiFiESP.getSocket();
    if (sock != NO_SOCKET_AVAIL)
    {
        ServerESPDrv::startServer(port, sock, UDP_MODE);
        WiFiESP._server_port[sock] = port;
        _sock = sock;
        _port = port;
        return 1;
    }
    return 0;

}

/* return number of bytes available in the current packet,
   will return zero if parsePacket hasn't been called yet */
int WiFiESPUDP::available() {
	 if (_sock != NO_SOCKET_AVAIL)
	 {
	      return ServerESPDrv::availData(_sock);
	 }
	 return 0;
}

/* Release any resources being used by this WiFiESPUDP instance */
void WiFiESPUDP::stop()
{
	  if (_sock == NO_SOCKET_AVAIL)
	    return;

	  ServerESPDrv::stopClient(_sock);

	  _sock = NO_SOCKET_AVAIL;
}

int WiFiESPUDP::beginPacket(const char *host, uint16_t port)
{
	// Look up the host first
	int ret = 0;
	IPAddress remote_addr;
	if (WiFiESP.hostByName(host, remote_addr))
	{
		return beginPacket(remote_addr, port);
	}
	return ret;
}

int WiFiESPUDP::beginPacket(IPAddress ip, uint16_t port)
{
  if (_sock == NO_SOCKET_AVAIL)
	  _sock = WiFiESP.getSocket();
  if (_sock != NO_SOCKET_AVAIL)
  {
	  ServerESPDrv::startClient(uint32_t(ip), port, _sock, UDP_MODE);
	  WiFiESP._state[_sock] = _sock;
	  return 1;
  }
  return 0;
}

int WiFiESPUDP::endPacket()
{
	return ServerESPDrv::sendUdpData(_sock);
}

size_t WiFiESPUDP::write(uint8_t byte)
{
  return write(&byte, 1);
}

size_t WiFiESPUDP::write(const uint8_t *buffer, size_t size)
{
	ServerESPDrv::insertDataBuf(_sock, buffer, size);
	return size;
}

int WiFiESPUDP::parsePacket()
{
	return available();
}

int WiFiESPUDP::read()
{
  uint8_t b;
  if (available())
  {
	  ServerESPDrv::getData(_sock, &b);
  	  return b;
  }else{
	  return -1;
  }
}

int WiFiESPUDP::read(unsigned char* buffer, size_t len)
{
  if (available())
  {
	  uint16_t size = 0;
	  if (!ServerESPDrv::getDataBuf(_sock, buffer, &size))
		  return -1;
	  // TODO check if the buffer is too small respect to buffer size
	  return size;
  }else{
	  return -1;
  }
}

int WiFiESPUDP::peek()
{
  uint8_t b;
  if (!available())
    return -1;

  ServerESPDrv::getData(_sock, &b, 1);
  return b;
}

void WiFiESPUDP::flush()
{
  while (available())
    read();
}

IPAddress  WiFiESPUDP::remoteIP()
{
	uint8_t _remoteIp[4] = {0};
	uint8_t _remotePort[2] = {0};

	// TODO: make this work
	// WiFiESPDrv::getRemoteData(_sock, _remoteIp, _remotePort);
	IPAddress ip(_remoteIp);
	return ip;
}

uint16_t  WiFiESPUDP::remotePort()
{
	uint8_t _remoteIp[4] = {0};
	uint8_t _remotePort[2] = {0};

	// TODO: make this work
	// WiFiESPDrv::getRemoteData(_sock, _remoteIp, _remotePort);
	uint16_t port = (_remotePort[0]<<8)+_remotePort[1];
	return port;
}
