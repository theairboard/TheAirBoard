/*
3-axis ADXL345 accelerometer
- plug the ADXL345 breakout board: SDA -> A4, SCL -> A5, CS->D10~, VCC->D11~, GND->D12
- power off and on The AirBoard to initialize the I2C driver
- repeatedly read and display the 3-axis accelerations
- display triggered actions: inactivity, activity, single tap, double tap and free fall

The AirBoard is a thumb-size, Arduino-compatible, wireless, low-power,
ubiquitous computer designed to sketch Internet-of-Things, fast!
Visit http://www.theairboard.cc
Upload your first sketch in seconds from https://codebender.cc?referral_code=Ub56L825Qb
Check readme.txt and license.txt for more information.
All text above must be included in any redistribution.
*/

#define CS       10
#define VCC      11 // trick: VCC signal mapped to pin 11
#define GND      12 // trick: GND signal mapped to pin 12
#define BAUD  57600 // communication speed: 57600 for XBee, 9600 otherwise

// Place /ADXL345 folder in your Preferences>Sketchbook location /libraries folder.
// You may need to create the /libraries subfolder if it is your first library.
// Restart the Arduino IDE.
#include <ADXL345.h>
#include <Wire.h>

ADXL345 adxl;
char buffer[15];

// the setup routine runs once when you switch on the board:
void setup(){
  pinMode(CS, OUTPUT);
  digitalWrite(CS, 1);
  pinMode(VCC, OUTPUT);
  digitalWrite(VCC, 1);
  pinMode(GND, OUTPUT);
  digitalWrite(GND, 0);
  adxl.setupAccel();
  Serial.begin(BAUD); // initialize serial communication
}

// the loop routine runs over and over again forever:
void loop(){ 
  int x,y,z; 
  adxl.readAccel(&x, &y, &z); // read x, y, z accelerations
  snprintf(buffer, sizeof(buffer), "%4d %4d %4d", x, y, z);
  Serial.println(buffer);
  
  // Read interrupts source and look for triggered actions 
  // getInterruptSource clears all triggered actions after returning value
  // so do not call again until you need to recheck for triggered actions
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
