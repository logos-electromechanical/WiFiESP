/**
 * @file at_drv.h
 * @brief The definition of class at_drv for the ESP8266. 
 * @author Pierce Nichols<pierce@logos-electro.com
 * @author Wu Pengfei<pengfei.wu@itead.cc> 
 * @date 2015.08
 * 
 * @par Copyright:
 * Copyright (c) 2015 ITEAD Intelligent Systems Co., Ltd. \n\n
 * Copyright (c) 2015 Logos Electromechanical LLC \n\n
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version. \n\n
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
 
#ifndef __AT_DRV_H__
#define __AT_DRV_H__

#include "Arduino.h"
#include "utility/wl_definitions.h"
#include <IPAddress.h>

// Size memory buffers for each channel
#define ESP_RX_BUFLEN 	1024
#define ESP_TX_BUFLEN	128

// Select among _CUR, _DEF, and deprecated modes for certain AT commands
#define ESP_AT_CUR		1
#define ESP_AT_DEF		2
#define ESP_AT_DEP		3

class ATDrvClass {
	public:
	
	void init(uint32_t baud = 9600);
	
	/* 
     * Empty the buffer or UART RX.
     */
    void rx_empty(void);
 
    /* 
     * Recvive data from uart. Return all received data if target found or timeout. 
     */
    String recvString(String target, uint32_t timeout = 1000);
    
    /* 
     * Recvive data from uart. Return all received data if one of target1 and target2 found or timeout. 
     */
    String recvString(String target1, String target2, uint32_t timeout = 1000);
    
    /* 
     * Recvive data from uart. Return all received data if one of target1, target2 and target3 found or timeout. 
     */
    String recvString(String target1, String target2, String target3, uint32_t timeout = 1000);
    
    /* 
     * Recvive data from uart and search first target. Return true if target found, false for timeout.
     */
    bool recvFind(String target, uint32_t timeout = 1000);
    
    /* 
     * Recvive data from uart and search first target and cut out the substring between begin and end(excluding begin and end self). 
     * Return true if target found, false for timeout.
     */
    bool recvFindAndFilter(String target, String begin, String end, String &data, uint32_t timeout = 1000);
    
    /*
     * Receive a package from uart. 
     *
     * @param buffer - the buffer storing data. 
     * @param buffer_size - guess what!
     * @param data_len - the length of data actually received(maybe more than buffer_size, the remained data will be abandoned).
     * @param timeout - the duration waitting data comming.
     * @param coming_mux_id - in single connection mode, should be NULL and not NULL in multiple. 
     */
    uint32_t recvPkg(uint8_t *buffer, uint32_t buffer_size, uint32_t *data_len, uint32_t timeout, uint8_t *coming_mux_id);
	
    bool 	eAT(void);
	bool 	sATCIPDINFO(uint8_t mode);
    bool 	eATRST(uint32_t timeout);
    bool 	eATGMR(String &version);
    bool 	qATCWMODE(uint8_t *mode, uint8_t pattern=3);
    bool 	sATCWMODE(uint8_t mode,uint8_t pattern=3);
    bool 	qATCWJAP(String &ssid,uint8_t pattern=3) ;
    bool 	sATCWJAP(String ssid, String pwd,uint8_t pattern=3);
	bool 	eATCWQAP(void);
    bool 	eATCWLAP(String &list);
    bool 	qATCWDHCP(uint8_t *mode, uint8_t *en, uint8_t pattern=3); 
    bool 	sATCWDHCP(uint8_t mode, uint8_t en, uint8_t pattern=3);
    bool 	qATCIPSTAMAC(String &mac, uint8_t pattern=3);
    bool 	qATCIPSTAIP(String &ip, uint8_t pattern=3);
	bool 	sATCIPSTAIP(uint8_t validParams, uint32_t local_ip, uint32_t gateway, uint32_t subnet, uint8_t pattern);
    bool 	eATCIPSTATUS(String &list);
    bool 	sATCIPSTARTMultiple(uint8_t mux_id, String type, String addr, uint32_t port);
	bool 	sATCIPSTARTMultiple(uint8_t mux_id, String type, IPAddress addr, uint32_t port);
    bool 	sATCIPSENDMultiple(uint8_t mux_id, const uint8_t *buffer, uint32_t len);
    bool 	sATCIPSENDMultipleFromFlash(uint8_t mux_id, const uint8_t *buffer, uint32_t len);
    bool 	sATCIPCLOSEMultiple(uint8_t mux_id);
    bool 	eATCIFSR(String &list);
    bool 	sATCIPMUX(uint8_t mode);
    bool 	sATCIPSERVER(uint8_t mode, uint32_t port = 333);
    bool 	sATCIPMODE(uint8_t mode);
    bool 	eATPING(String ip);
    bool 	sATCIPSTO(uint32_t timeout);
	bool 	qCIPBUFSTATUS(uint8_t mux_id);
	
	int16_t	available(uint8_t mux_id);
	int16_t	availableTX(uint8_t mux_id);
	char 	peekChar(uint8_t mux_id);
	char 	getChar(uint8_t mux_id);
	char 	getCharTX(uint8_t mux_id);
	int16_t getBuf(uint8_t mux_id, uint8_t* buf, uint16_t len);
	int16_t getBufTX(uint8_t mux_id, uint8_t* buf, uint16_t len);
	bool	putCharRX(uint8_t mux_id, uint8_t c);
	bool	putCharTX(uint8_t mux_id, uint8_t c);
	int16_t	putBufTX(uint8_t mux_id, const uint8_t* buf, uint16_t len);
	int16_t	putBufRX(uint8_t mux_id, const uint8_t* buf, uint16_t len);
	
	void printIP(IPAddress ip);
    
	private:
    static HardwareSerial *m_puart; /* The UART to communicate with ESP8266 */
	static uint16_t _rx_buffer_head[MAX_SOCK_NUM];
	static uint16_t _rx_buffer_tail[MAX_SOCK_NUM];
	static uint16_t _tx_buffer_head[MAX_SOCK_NUM];
	static uint16_t _tx_buffer_tail[MAX_SOCK_NUM];
	static uint8_t rxBufs[MAX_SOCK_NUM][ESP_RX_BUFLEN];
	static uint8_t txBufs[MAX_SOCK_NUM][ESP_TX_BUFLEN];
};

extern ATDrvClass atDrv;

#endif