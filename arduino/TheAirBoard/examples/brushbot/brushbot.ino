/*
Brushbot control program
- download the Android app: http://remotexy.com/en/download/
- order and populate the dual motor drive PCB for brushbot: https://oshpark.com/shared_projects/fyhMVHcY
- or directly connect micro motors to PINs 10/11 and GND for airship
- press right/left buttons

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
#define LEFT  10
#define RIGHT 11

#include <TheAirBoard_pinout.h>
#include <SoftwareSerial.h>
#include <RemoteXY.h>

unsigned char RemoteXY_CONF[] =
  { 2,0,20,0,1,5,1,0,6,12
  ,38,38,5,60,0,1,0,56,12,38
  ,38,5,62,0 }; 

struct {
  unsigned char right;
  unsigned char left;
  unsigned char connect_flag;
} RemoteXY;

void setup() {
  analogWrite(RIGHT, 0);
  analogWrite(LEFT, 0);
  RemoteXY_Init(); 
}

void loop() { 
  RemoteXY_Handler();
  analogWrite(RIGHT, RemoteXY.right?255:0);
  analogWrite(LEFT, RemoteXY.left?255:0);
}
