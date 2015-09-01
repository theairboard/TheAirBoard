/*
Wireless shields initial configuration
1. upload this program to The AirBoard via the BLE-LINK shield
2. replace the BLE-LINK shield by any blank XB24, XBP24, RN-42 or RN-XV shield
3. run the program once to setup the working configuration

The AirBoard is a thumb-size, Arduino-compatible, wireless, low-power,
ubiquitous computer designed to sketch Internet-of-Things, fast!
Visit http://www.theairboard.cc
Upload your first sketch in seconds from https://codebender.cc?referral_code=Ub56L825Qb
Check README.txt and license.txt for more information.
All text above must be included in any redistribution.
*/

// Place /TheAirBoard folder in your Preferences>Sketchbook location /libraries folder.
// You may need to create the /libraries subfolder if it is your first library.
// Restart the Arduino IDE.
#include <TheAirBoard.h>

TheAirBoard board;

// the setup routine runs once when you switch on the board:
void setup() {                
  pinMode(GREEN, OUTPUT);               // initialize the digital pin as an output

  delay(2000);                          // let the wireless module boot
  // XBee MASTER is configured for the XBee Explorer Dongle http://www.sparkfun.com/products/11697
  // XBee SLAVE is configured for The AirBoard
  board.configXBee(SLAVE);              // configure XBee module MASTER or SLAVE
  board.configRN42();                   // configure RN42 Bluetooth 2 module
  board.configWiFly();                  // configure WiFly RN-XV (RN171) module

  // LED blinks once when setup is done
  analogWrite(GREEN, 1);
  delay(100);
  analogWrite(GREEN, 0);  
}

// the loop routine runs over and over again forever:
void loop() {
  // do nothing
}
