/*
SigFox logger program based on SigFox proto shield
- send a data every 10 minutes to SigFox network

The AirBoard is a thumb-size, Arduino-compatible, wireless,
ubiquitous computer designed to sketch Internet-of-Things, fast!

Visit http://www.theairboard.cc

Check readme.txt and license.txt for more information.
All text above must be included in any redistribution.
*/

#include <TheAirBoard_pinout.h>

unsigned char i = 0;
char buffer[2];

void setup() {
  pinMode(BLUE, OUTPUT);
  Serial.begin(9600);
}

void loop() {
    digitalWrite(BLUE, 1);
    Serial.print("at$ss=");
    sprintf(buffer, "%.2X", i++);
    Serial.println(buffer);
    delay(6000);
    digitalWrite(BLUE, 0);
    delay(600000);
}
