/*
Optimized logger program
- send a data every minute
- 56-second watchdog timeout
- enter deep sleep between transmit

Bluetooth 2.1 operation:
- switch Bluetooth LED off (s%,2000)
- program 1-second Bluetooth sniff mode (sw,0640)
- average power consumption with optimized Bluetooth shield: 3.7 mA
- tested: 46 hours battery life with Bluetooth on a 155 mAh battery
- be sure to set sniff mode back to 30 ms to allow further uploads (sw,0030)

XBee operation:
- turn on the micro switch just before uploading to make sure the RF power is on (5 seconds timeout)
- average power consumption with XBee shield: 33.5 µA
- estimated: over 6 months battery life with XBee on a 155 mAh battery

The AirBoard is a thumb-size, Arduino-compatible, wireless,
ubiquitous computer designed to sketch Internet-of-Things, fast!

Visit http://www.theairboard.cc

Check readme.txt and license.txt for more information.
All text above must be included in any redistribution.
*/

#include <TheAirBoard_pinout.h>
#include <TheAirBoard_power.h>

#define XBEE                       // comment this line if using Bluetooth or WiFi shield
#define BAUD 57600

TheAirBoard_power board;
volatile boolean f_wdt = true;
unsigned int i = 0;
int timeout = 0;

ISR(WDT_vect) {
  if(f_wdt == false)
    f_wdt = true;
}

void setup() {
  pinMode(RF, OUTPUT);
  digitalWrite(RF, 1);             // switch RF on
  Serial.begin(BAUD);
  delay(5000);                     // allow time to launch programming, before a possible RF power down
  board.setWatchdog(8000);         // set watchdog timeout in milliseconds (max 8000)
}

void loop() {
  if(f_wdt == true) {
    timeout++;
    if(timeout == 7) {             // timeout every 7*8 = 56s
      #ifdef XBEE
        analogWrite(GREEN, 1);
        digitalWrite(RF, 1);       // turn on RF module
        delay(25);                 // delay for XBee to stabilize
        Serial.println(i++);
        delay(5);                  // delay for data to transmit
        digitalWrite(RF, 0);       // turn off RF module
        analogWrite(GREEN, 0);
      #else
        Serial.println(i++);
      #endif
      timeout = 0;
    }
    f_wdt = false;
    board.powerDown(BAUD);
  }
}
