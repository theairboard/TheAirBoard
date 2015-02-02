/*
3-axis accelerometer test program
- download the Android app: http://remotexy.com/en/download/
- plug ADXL345 breakout board: https://www.sparkfun.com/products/9836

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
#define CS    10
#define VCC   11        // trick: VCC signal mapped to pin 11
#define GND   12        // trick: GND signal mapped to pin 12

#include <TheAirBoard_pinout.h>
#include <SoftwareSerial.h>
#include <RemoteXY.h>
#include <Wire.h>
#include <ADXL345.h>

ADXL345 adxl;
  
unsigned char RemoteXY_CONF[] =
  { 0,3,23,0,1,5,66,0,33,22
  ,7,20,2,66,0,45,22,7,20,2
  ,66,0,57,22,7,20,2 }; 

struct {
  unsigned char x; // 0..100
  unsigned char y; // 0..100  
  unsigned char z; // 0..100  
  unsigned char connect_flag;
} RemoteXY;

void setup() {
  pinMode(CS, OUTPUT); digitalWrite(CS, 1);
  pinMode(VCC, OUTPUT); digitalWrite(VCC, 1);
  pinMode(GND, OUTPUT); digitalWrite(GND, 0);
  setup_accel();
  RemoteXY_Init();  
}

void loop() {
  int x,y,z;
  RemoteXY_Handler();
  adxl.readAccel(&x, &y, &z);
  RemoteXY.x = map(abs(x), 0, 512, 0, 100);
  RemoteXY.y = map(abs(y), 0, 512, 0, 100);
  RemoteXY.z = map(abs(z), 0, 512, 0, 100);
  delay(10);
}

void setup_accel(){
  adxl.powerOn();

  //set activity/ inactivity thresholds (0-255)
  adxl.setActivityThreshold(75); //62.5mg per increment
  adxl.setInactivityThreshold(75); //62.5mg per increment
  adxl.setTimeInactivity(10); // how many seconds of no activity is inactive?
 
  //look of activity movement on this axes - 1 == on; 0 == off 
  adxl.setActivityX(1);
  adxl.setActivityY(1);
  adxl.setActivityZ(1);
 
  //look of inactivity movement on this axes - 1 == on; 0 == off
  adxl.setInactivityX(1);
  adxl.setInactivityY(1);
  adxl.setInactivityZ(1);
 
  //look of tap movement on this axes - 1 == on; 0 == off
  adxl.setTapDetectionOnX(0);
  adxl.setTapDetectionOnY(0);
  adxl.setTapDetectionOnZ(1);
 
  //set values for what is a tap, and what is a double tap (0-255)
  adxl.setTapThreshold(50); //62.5mg per increment
  adxl.setTapDuration(15); //625μs per increment
  adxl.setDoubleTapLatency(80); //1.25ms per increment
  adxl.setDoubleTapWindow(200); //1.25ms per increment
 
  //set values for what is considered freefall (0-255)
  adxl.setFreeFallThreshold(7); //(5 - 9) recommended - 62.5mg per increment
  adxl.setFreeFallDuration(45); //(20 - 70) recommended - 5ms per increment
 
  //setting all interupts to take place on int pin 1
  //I had issues with int pin 2, was unable to reset it
  adxl.setInterruptMapping( ADXL345_INT_SINGLE_TAP_BIT,   ADXL345_INT1_PIN );
  adxl.setInterruptMapping( ADXL345_INT_DOUBLE_TAP_BIT,   ADXL345_INT1_PIN );
  adxl.setInterruptMapping( ADXL345_INT_FREE_FALL_BIT,    ADXL345_INT1_PIN );
  adxl.setInterruptMapping( ADXL345_INT_ACTIVITY_BIT,     ADXL345_INT1_PIN );
  adxl.setInterruptMapping( ADXL345_INT_INACTIVITY_BIT,   ADXL345_INT1_PIN );
 
  //register interupt actions - 1 == on; 0 == off  
  adxl.setInterrupt( ADXL345_INT_SINGLE_TAP_BIT, 1);
  adxl.setInterrupt( ADXL345_INT_DOUBLE_TAP_BIT, 1);
  adxl.setInterrupt( ADXL345_INT_FREE_FALL_BIT,  1);
  adxl.setInterrupt( ADXL345_INT_ACTIVITY_BIT,   1);
  adxl.setInterrupt( ADXL345_INT_INACTIVITY_BIT, 1);
}

