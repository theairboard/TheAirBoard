/*
Servo test program
- Test 2 servo positions
- Install TABdirection.apk on your Android smartphone
- Pair the Bluetooth module (password is 1234 if requested)
- Launch the app and Connect a device - RfComm

The AirBoard is a thumb-size, Arduino-compatible, wireless,
ubiquitous computer designed to sketch Internet-of-Things, fast!

Visit http://www.theairboard.cc

Check readme.txt and license.txt for more information.
All text above must be included in any redistribution.
*/

#include <TheAirBoard_pinout.h>
#include <Servo.h>

Servo myservo;  // create servo object to control a servo 

void setup() {
  myservo.attach(10);  // attaches the servo on pin 10 to the servo object   
  analogWrite(BLUE, 0);
  analogWrite(GREEN, 1);
  myservo.write(120); // default servo position
        
  Serial.begin(57600);
}

void loop() {
  while (Serial.available()) {
    switch (Serial.read()) {
      case 'l': // press left
        analogWrite(GREEN, 0);
        analogWrite(BLUE, 1);
        myservo.write(30);
        break;
      case 'm': // release left
        break;
      case 'r': // press right
        analogWrite(BLUE, 0);
        analogWrite(GREEN, 1);
        myservo.write(120);
        break;
      case 't': // release right
        break;
      case '\n':
      case '\r':
        break;
      default:
        break;
    }
  }
}
