/*
Battery indicator
- check battery voltage, USB voltage and charge status periodically
- blink red/green while charging
- solid green when charged or when discharging
- solid red when battery low

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
  Serial.begin(9600);                 // initialize serial communication at 9600 bits per second
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
