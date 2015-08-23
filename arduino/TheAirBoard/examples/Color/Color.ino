/*
Smartphone-controlled LED test program
- Test 7 colors of the RGB LED at full power

1. Install tAB-COLOR.apk on your Android smartphone
2. Search and connect "Bluno" device

The AirBoard is a thumb-size, Arduino-compatible, wireless, low-power,
ubiquitous computer designed to sketch Internet-of-Things, fast!
Visit http://www.theairboard.cc
Upload your first sketch in seconds from https://codebender.cc?referral_code=Ub56L825Qb
Check README.txt and license.txt for more information.
All text above must be included in any redistribution.
*/

#include <TheAirBoard.h>

// the setup routine runs once when you switch on the board:
void setup() {
  pinMode(RED, OUTPUT);         // initialize the RED LED pin as an output
  pinMode(GREEN, OUTPUT);       // initialize the GREEN LED pin as an output
  pinMode(BLUE, OUTPUT);        // initialize the BLUE LED pin as an output
  analogWrite(GREEN, 1);        // switch ON indicator at low power 
  Serial.begin(9600);           // initialize serial communication at 9600 bits per second
}

// the loop routine runs over and over again forever:
void loop() { 
  while(Serial.available()) {
    switch(Serial.read()) {
      case 'a': // white
        digitalWrite(RED, 1); digitalWrite(GREEN, 1); digitalWrite(BLUE, 1);
        break;
      case 'b': // yellow
        digitalWrite(RED, 1); digitalWrite(GREEN, 1); digitalWrite(BLUE, 0);
        break;
      case 'c': // magenta
        digitalWrite(RED, 1); digitalWrite(GREEN, 0); digitalWrite(BLUE, 1);
        break;
      case 'd': // red
        digitalWrite(RED, 1); digitalWrite(GREEN, 0); digitalWrite(BLUE, 0);
        break;
      case 'e': // light blue
        digitalWrite(RED, 0); digitalWrite(GREEN, 1); digitalWrite(BLUE, 1);
        break;
      case 'f': // green
        digitalWrite(RED, 0); digitalWrite(GREEN, 1); digitalWrite(BLUE, 0);
        break;
      case 'g': // blue
        digitalWrite(RED, 0); digitalWrite(GREEN, 0); digitalWrite(BLUE, 1);
        break;
      case 'h': //off
        digitalWrite(RED, 0); digitalWrite(GREEN, 0); digitalWrite(BLUE, 0);
        break;
      default:
        break;
    }
  }
}
