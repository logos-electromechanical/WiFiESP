| **Function** | **Status** | **Test Written?** | **Paramters** | **Returns** | **Notes** |
|--------------|------------|-------------------|---------------|-------------|-----------|
| init() | Implemented | Yes | uint32_t baud | void | This may be causing crash on startup |
| rx_empty() | Implemented | Indirect | void | void | Inherited without change from ESP8266; no separate test required. |
| recvString() | Implemented | Indirect | String target, uint32_t timeout (in ms) | String (all received data) | Inherited without change from ESP8266; no separate test required. |
| recvString() | Implemented | Indirect | String target1, target2, target 3, uint32_t timeout (in ms) | String (all received data) | Inherited without change from ESP8266; no separate test required. |
