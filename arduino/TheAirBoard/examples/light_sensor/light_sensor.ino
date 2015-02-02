/*
Light sensor test program
- display a light indicator on the smartphone

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
  { 0,3,9,0,1,5,65,7,22,3
  ,58,58,0 }; 

struct {
  unsigned char led_r; // 0..255
  unsigned char led_g; // 0..255
  unsigned char led_b; // 0..255
  unsigned char connect_flag;
} RemoteXY;

void setup() {
  RemoteXY_Init();
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  
  digitalWrite(19, HIGH);  // set internal pull-up on A5
}

void loop() {
  RemoteXY_Handler();
  if(analogRead(BITIN) > 30) {
    RemoteXY.led_r = RemoteXY.led_g = RemoteXY.led_b = 0;
    digitalWrite(RED, 0);
    digitalWrite(GREEN, 0);
    digitalWrite(BLUE, 0);
  
  }
  else {
    RemoteXY.led_r = RemoteXY.led_g = RemoteXY.led_b = 255;
    digitalWrite(RED, 1);
    digitalWrite(GREEN, 1);
    digitalWrite(BLUE, 1);
  }

}

