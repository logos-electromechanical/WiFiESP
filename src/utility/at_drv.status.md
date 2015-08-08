| **Function** | **Status** | **Test Written?** | **Notes** |
|--------------|------------|-------------------|-----------|
| init() | Implemented | Yes | This may be causing crash on startup |
| rx_empty() | Implemented | Indirect | Inherited without change from ESP8266; no separate test required. |
| recvString(String, uint32_t) | Implemented | Indirect | Inherited without change from ESP8266; no separate test required. |
| recvString(String, String, uint32_t) | Implemented | Indirect | Inherited without change from ESP8266; no separate test required. |
| recvFind() | Implemented | Indirect | Inherited without change from ESP8266; no separate test required. |
| recvFindAndFilter() | Implemented | Indirect | Inherited without change from ESP8266; no separate test required. |
| recvPkg() | Implemented | Indirect | Inherited without change from ESP8266; no separate test required. |
| eAT() | Implemented | Yes | |
| sATCIPDINFO() | Implemented | Yes | |
| eATRST() | Implemented | Yes | | 
| eATGMR() | Implemented | Yes | | 
| qATCWMODE() | Implemented | Yes | | 
| sATCWMODE() | Implemented | Yes | | 
| qATCWJAP() | Implemented | Yes | | 
| sATCWJAP() | Implemented | Yes | | 
| eATCWQAP() | Implemented | Yes | | 
| eATCWLAP() | Implemented | Yes | | 
| qATCWDHCP() | Implemented | Yes | | 
| sATCWDHCP() | Implemented | Yes | | 
| qATCIPSTAMAC() | Implemented | Yes | | 
| qATCIPSTAIP() | Implemented | Yes | | 
| sATCIPSTAIP() | Implemented | Yes | | 
| eATCIPSTATUS() | Implemented | Yes | | 
| sATCIPSTARTMultiple() | Implemented | Yes | | 
| sATCIPSTARTMultiple() | Implemented | Yes | | 
| sATCIPSENDMultiple() | Implemented | Yes | | 
| sATCIPSENDMultipleFromFlash() | Implemented | Yes | | 
| sATCIPCLOSEMultiple() | Implemented | Yes | | 
| eATCIFSR() | Implemented | Yes | | 
| sATCIPMUX() | Implemented | Yes | | 
| sATCIPSERVER() | Implemented | Yes | | 
| sATCIPMODE() | Implemented | Yes | | 
| eATPING() | Implemented | Yes | | 
| sATCIPSTO() | Implemented | Yes | | 
| qCIPBUFSTATUS() | Implemented | Yes | | 
| available() | Implemented | No ||
| availableTX() | Implemented | No ||
| peekChar() | Implemented | No ||
| getChar() | Implemented | No ||
| getCharTX() | Implemented | No ||
| getBuf() | Implemented | No ||
| getBufTX() | Implemented | No ||
| putCharRX() | Implemented | No ||
| putCharTX() | Implemented | No ||
| putBufTX() | Implemented | No ||
| putBufRX() | Implemented | No ||
| printIP(IPAddress ip);