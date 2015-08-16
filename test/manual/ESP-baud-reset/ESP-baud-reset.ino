#include <WiFiESP.h>
#include <WiFiESPClient.h>
#include <WiFiESPServer.h>
#include <WiFiESPUdp.h>
#include <IPAddress.h>
#include "utility/at_drv.h"

void setup() {
  Serial.begin(115200);
  Serial.println(F("This is a utility for changing the speed of an ESP8266"));
}

void loop() {
  uint32_t oldSpeed, newSpeed;
  uint8_t shellCnt = 0;
  char c;
  Serial.println(F("Enter the starting speed"));
  while(Serial.available()) Serial.read();
  while (!Serial.available());
  oldSpeed = Serial.parseInt();
  atDrv.init(oldSpeed);
  delay(1000);
  Serial.println(F("Checking function at old speed"));
  if (atDrv.eAT()) {
    Serial.println(F("Old speed checks out... enter new speed."));
    while(Serial.available()) Serial.read();
    while (!Serial.available());
    newSpeed = Serial.parseInt();
    Serial.println(F("Transmitting UART command and then transitioning to shell mode."));
    Serial1.print(F("AT+UART="));
    Serial1.print(newSpeed);
    Serial1.print(F(",8,1,0,0\r\n"));
    delay(1000);
    if (atDrv.eAT()) {
      Serial.println(F("Reset failed... (speed not changed)"));
    } else {
      atDrv.init(newSpeed);
      delay(1000);
      if (atDrv.eAT()) {
        Serial.println(F("Entering shell mode. Type +++ to exit"));
        while (shellCnt < 3) {
          if (Serial1.available()) Serial.print((char)Serial1.read());
          if (Serial.available()) {
            c = Serial.read();
            if (c == '+') {
              shellCnt++;
            } else shellCnt = 0;
            Serial1.print(c);
          }
        }
      } else {
        Serial.println(F("Reset failed... (new speed did not take)"));
      }
    }
  } else {
    Serial.print(F("Old speed does not work -- enter a different speed. (Entered value: "));
    Serial.print(oldSpeed);
    Serial.println(')');
  }
  

}
