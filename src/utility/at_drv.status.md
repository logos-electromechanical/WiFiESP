| **Function** | **Status** | **Test Written?** | **Notes** |
|--------------|------------|-------------------|-----------|
| init() | Implemented | No | This may be causing crash on startup |
| rx_empty() | Implemented | Indirect | Inherited without change from ESP8266; no separate test required. |
| recvString(String, uint32_t) | Implemented | Indirect | Inherited without change from ESP8266; no separate test required. |
| recvString(String, String, uint32_t) | Implemented | Indirect | Inherited without change from ESP8266; no separate test required. |
| recvFind() | Implemented | Indirect | Inherited without change from ESP8266; no separate test required. |
| recvFindAndFilter() | Implemented | Indirect | Inherited without change from ESP8266; no separate test required. |
| recvPkg() | Implemented | Indirect | Inherited without change from ESP8266; no separate test required. |
| eAT() | Tested/Working | Yes | |
| sATCIPDINFO() | Implemented | Yes | May not exist with the firmware version on my current test mule |
| eATRST() | Tested/Working | Yes | | 
| eATGMR() | Tested/Working | Yes | | 
| qATCWMODE() | Tested/Provisional | Yes | My test mule is an older firmware version, so I can't test some of this | 
| sATCWMODE() | Tested/Provisional | Yes | My test mule is an older firmware version, so I can't test some of this | 
| qATCWJAP() | Tested/Provisional | Yes | This returns ERROR when not connected, so good for connection status. Must test with current firmware | 
| sATCWJAP() | Tested/Provisional | Yes | My test mule is an older firmware version, so I can't test some of this | 
| eATCWQAP() | Tested/Working | Yes | | 
| eATCWLAP() | Tested/Working | Yes | | 
| qATCWDHCP() | Implemented | Yes | Does not appear to work on older firmware | 
| sATCWDHCP() | Implemented | Yes | Does not appear to work on older firmware | 
| qATCIPSTAMAC() | Implemented | Yes | Does not appear to work on older firmware | 
| qATCIPSTAIP() | Implemented | Yes | Does not appear to work on older firmware | 
| sATCIPSTAIP() | Implemented | Yes | Does not appear to work on older firmware | 
| eATCIPSTATUS() | Tested/Working | Yes | | 
| sATCIPSTARTMultiple() | Implemented | Yes | | 
| sATCIPSTARTMultiple() | Implemented | Yes | | 
| sATCIPSENDMultiple() | Implemented | Yes | | 
| sATCIPSENDMultipleFromFlash() | Implemented | Yes | | 
| sATCIPCLOSEMultiple() | Implemented | Yes | | 
| eATCIFSR() | Implemented | Yes | | 
| sATCIPMUX() | Tested/Working | Yes | | 
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