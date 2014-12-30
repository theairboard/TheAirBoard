/*
Smartphone-controlled LED test program
- Test 7 colors of the RGB LED at maximum power
- Install TABcolor.apk on your Android smartphone
- Pair the Bluetooth module (password is 1234 if requested)
- Launch the app and Connect a device - RfComm

The AirBoard is a thumb-size, Arduino-compatible, wireless,
ubiquitous computer designed to sketch Internet-of-Things, fast!

Visit http://www.theairboard.cc

Check readme.txt and license.txt for more information.
All text above must be included in any redistribution.
*/

#include <TheAirBoard_pinout.h>

void setup() {
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  analogWrite(GREEN, 1);
  
  Serial.begin(57600);
}

void loop() { 
  while(Serial.available()) {
    switch(Serial.read()) {
      case 'a':
        digitalWrite(RED, 1); digitalWrite(GREEN, 1); digitalWrite(BLUE, 1);
        break;
      case 'b':
        digitalWrite(RED, 1); digitalWrite(GREEN, 1); digitalWrite(BLUE, 0);
        break;
      case 'c':
        digitalWrite(RED, 1); digitalWrite(GREEN, 0); digitalWrite(BLUE, 1);
        break;
      case 'd':
        digitalWrite(RED, 1); digitalWrite(GREEN, 0); digitalWrite(BLUE, 0);
        break;
      case 'e':
        digitalWrite(RED, 0); digitalWrite(GREEN, 1); digitalWrite(BLUE, 1);
        break;
      case 'f':
        digitalWrite(RED, 0); digitalWrite(GREEN, 1); digitalWrite(BLUE, 0);
        break;
      case 'g':
        digitalWrite(RED, 0); digitalWrite(GREEN, 0); digitalWrite(BLUE, 1);
        break;
      case 'h':
        digitalWrite(RED, 0); digitalWrite(GREEN, 0); digitalWrite(BLUE, 0);
        break;
      default:
        break;
    }
  }
}


