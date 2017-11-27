/*
3-axis ADXL345 accelerometer
- plug the ADXL345 breakout board: SDA -> A4, SCL -> A5, CS->D10~, VCC->D11~, GND->D12
- power OFF and ON The AirBoard to initialize the I2C driver
- repeatedly read the 3-axis accelerations in g
- look for triggered actions: inactivity, activity, single tap, double tap and free fall

The AirBoard is a thumb-size, Arduino-compatible, wireless, low-power,
ubiquitous computer designed to sketch Internet-of-Things, fast!
Visit http://www.theairboard.cc
Upload your first sketch in seconds from https://codebender.cc?referral_code=Ub56L825Qb
Check README.txt and license.txt for more information.
All text above must be included in any redistribution.
*/

#define CS       10
#define VCC      11 // trick: VCC signal mapped to pin 11
#define GND      12 // trick: GND signal mapped to pin 12

// Place /ADXL345 folder in your Preferences>Sketchbook location /libraries folder.
// You may need to create the /libraries subfolder if it is your first library.
// Restart the Arduino IDE.
#include <ADXL345.h>
#include <Wire.h>

ADXL345 adxl;

// the setup routine runs once when you switch on the board:
void setup(){
  pinMode(CS, OUTPUT);
  digitalWrite(CS, 1);
  pinMode(VCC, OUTPUT);
  digitalWrite(VCC, 1);
  pinMode(GND, OUTPUT);
  digitalWrite(GND, 0);
  Serial.begin(9600); // initialize serial communication at 9600 bits per second
  
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

// the loop routine runs over and over again forever:
void loop(){ 
  double xyz[3];
  
  // read the x, y, z accelerations in g
  adxl.getAcceleration(xyz);
  Serial.print(xyz[0]);
  Serial.print("g ");
  Serial.print(xyz[1]);
  Serial.print("g ");
  Serial.print(xyz[2]);
  Serial.println('g');
  
  // read interrupts source and look for triggered actions 
  byte interrupts = adxl.getInterruptSource();
  if(adxl.triggered(interrupts, ADXL345_INACTIVITY))
    Serial.println("inactivity");
  if(adxl.triggered(interrupts, ADXL345_ACTIVITY))
    Serial.println("activity");
  if(adxl.triggered(interrupts, ADXL345_SINGLE_TAP))
    Serial.println("tap"); 
  if(adxl.triggered(interrupts, ADXL345_DOUBLE_TAP))
    Serial.println("double tap");
  if(adxl.triggered(interrupts, ADXL345_FREE_FALL))
    Serial.println("freefall");
  
  delay(500); // wait for 500 milliseconds
}
