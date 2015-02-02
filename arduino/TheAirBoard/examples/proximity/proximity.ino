/*
Proximity sensor test program
- order and populate the proximity sensor PCB: https://oshpark.com/shared_projects/O4ikJzGw
- on the breakout board, connect VCC to 5V if USB is plugged in, to 3V3 otherwise

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

#include <TheAirBoard_pinout.h>
#include <SoftwareSerial.h>
#include <RemoteXY.h>
  
unsigned char RemoteXY_CONF[] =
  { 0,1,9,0,1,5,66,129,0,0
  ,100,63,2 }; 

struct {
  unsigned char proximity; // 0..100
  unsigned char connect_flag;
} RemoteXY;

void setup() {
  RemoteXY_Init();  
}

void loop() {
  RemoteXY_Handler();
  RemoteXY.proximity = map(constrain(analogRead(BITIN), 275, 600), 275, 600, 100, 0);
  if(analogRead(BITIN) > 450)
    analogWrite(RED, 255);
  else digitalWrite(RED, 0);
}

