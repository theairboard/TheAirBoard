/*
ChargeBattery
- check battery voltage, USB voltage and charge status periodically
- blink red/green while charging
- solid green when discharging or when charged
- solid red when battery low

The AirBoard is a thumb-size, Arduino-compatible, wireless, low-power,
ubiquitous computer designed to sketch Internet-of-Things, fast!
Visit http://www.theairboard.cc
Upload your first sketch in seconds from https://codebender.cc?referral_code=Ub56L825Qb
Check readme.txt and license.txt for more information.
All text above must be included in any redistribution.
*/

#define BAUD   9600                   // communication speed: 57600 for XBee, 9600 otherwise

// Place /TheAirBoard folder in your Preferences>Sketchbook location /libraries folder.
// You may need to create the /libraries subfolder if it is your first library.
// Restart the Arduino IDE.
#include <TheAirBoard.h>

TheAirBoard board;

// the setup routine runs once when you switch on the board:
void setup() {
  Serial.begin(BAUD);                 // initialize serial communication
}

// the loop routine runs over and over again forever:
void loop() {
    static unsigned long time;
  
    if(millis() - time > 500) {       // every 500 milliseconds
    time = millis();
    Serial.print(board.batteryChk()); // display battery level
    Serial.println(" V");             // in volts
    }
}
