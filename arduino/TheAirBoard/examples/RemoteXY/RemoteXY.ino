/*
RemoteXY test program
- download Android app: http://remotexy.com/en/download/
- switch GREEN LED ON on "click me" button press

The AirBoard is a thumb-size, Arduino-compatible, wireless,
ubiquitous computer designed to sketch Internet-of-Things, fast!

Visit http://www.theairboard.cc

Check readme.txt and license.txt for more information.
All text above must be included in any redistribution.
*/

#define REMOTEXY_MODE__SOFTWARESERIAL
#define REMOTEXY_SERIAL_RX 0
#define REMOTEXY_SERIAL_TX 1
#define REMOTEXY_SERIAL_SPEED 9600

#include <TheAirBoard_pinout.h>
#include <SoftwareSerial.h>
#include <RemoteXY.h>

unsigned char RemoteXY_CONF[] =
  { 1,0,18,0,1,5,1,0,24,6
  ,51,51,4,99,108,105,99,107,32,109
  ,101,0 };

struct {
  unsigned char click_me;
  unsigned char connect_flag;
} RemoteXY;

void setup() {
  RemoteXY_Init (); 
  pinMode (GREEN, OUTPUT);
}

void loop() { 
  RemoteXY_Handler ();
  digitalWrite(GREEN, (RemoteXY.click_me==0)?LOW:HIGH);
}
