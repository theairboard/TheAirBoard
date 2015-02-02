/*
The sketchBit is an input/output bit adapter that allows direct control
of your littleBits modules with your smartphone.
Combine the flexibility of The AirBoard broad range of wireless shields
and the diversity of littleBits power bits
to make your own devices, robots, etc...

- One input bit goes to The AirBoard analog pin #5
- One output bit comes from The AirBoard digital pin #10
- A micro switch selects digital vs analog output

- download the Android Bluetooth 2 app: http://remotexy.com/en/download/
- get the following littleBits modules:
http://littlebits.cc/accessories/battery-plus-cable
http://littlebits.cc/bits/littlebits-power
http://littlebits.cc/bits/dimmer
http://littlebits.cc/bits/bargraph

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
#define BITIN A5
#define BITOUT 10

#include <TheAirBoard_pinout.h>
#include <SoftwareSerial.h>
#include <RemoteXY.h>
  
unsigned char RemoteXY_CONF[] =
  { 1,1,16,0,1,5,4,128,2,40
  ,93,13,2,66,128,8,12,83,11,2
   }; 

struct {
  unsigned char bargraph; // 0..100
  unsigned char dimmer; // 0..100
  unsigned char connect_flag;
} RemoteXY;

void setup() {
  RemoteXY_Init();  
}

void loop() {
  analogWrite(GREEN, analogRead(VUSB) > 512 ? true:false); // on indicator
  RemoteXY_Handler();
  RemoteXY.dimmer = map(analogRead(BITIN), 0, 560, 0, 100); // 560->1023
  analogWrite(BITOUT, map(RemoteXY.bargraph, 0, 100, 0, 255));
}

