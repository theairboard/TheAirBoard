/*
Simple echo program
- blink green LED every second
- print and increment i every second
- echo characters sent by terminal

The AirBoard is a thumb-size, Arduino-compatible, wireless,
ubiquitous computer designed to sketch Internet-of-Things, fast!

Visit http://www.theairboard.cc

Check readme.txt and license.txt for more information.
All text above must be included in any redistribution.
*/

#include <TheAirBoard_pinout.h>

int i = 0;
unsigned long time;
boolean flag = true;

void setup() {
  pinMode(GREEN, OUTPUT);
  Serial.begin(57600);
}

void loop() {
  if(millis() - time > 1000) {
    time = millis();
    analogWrite(GREEN, flag);
    flag ^= 1;
    Serial.println(i++);
  }
  
  if(Serial.available())
    Serial.print((char)Serial.read());  
}
