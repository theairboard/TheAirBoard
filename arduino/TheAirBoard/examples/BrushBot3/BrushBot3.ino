/*
BrushBot proportional control with RemoteXY
- use the smartphone internal G-sensor to pilot the BrushBot/Airship
- order and populate the dual motor drive PCB for brushbot: http://oshpark.com/shared_projects/fyhMVHcY
  or directly connect micro motors to PINs 10, 11 and GND for airship
- watch: http://www.youtube.com/watch?v=gNQzRTcE6cM

1. upload this program to The AirBoard via the BLE-LINK shield
2. replace the BLE-LINK shield by the RN-42 Bluetooth 2 shield
3. launch RemoteXY http://remotexy.com/en/download/ for Android

The AirBoard is a thumb-size, Arduino-compatible, wireless, low-power,
ubiquitous computer designed to sketch Internet-of-Things, fast!
Visit http://www.theairboard.cc
Upload your first sketch in seconds from https://codebender.cc?referral_code=Ub56L825Qb
Check README.txt and license.txt for more information.
All text above must be included in any redistribution.
*/

#define REMOTEXY_MODE__SOFTWARESERIAL // RemoteXY software serial select
#define REMOTEXY_SERIAL_RX 0          // RemoteXY serial receive pin
#define REMOTEXY_SERIAL_TX 1          // RemoteXY serial transmit pin
#define REMOTEXY_SERIAL_SPEED 9600    // RemoteXY serial communication at 9600 bits per second
#define RIGHT 11                      // right motor
#define LEFT  10                      // left motor

// Place /TheAirBoard and /RemoteXY folders in your Preferences>Sketchbook location /libraries folder.
// You may need to create the /libraries subfolder if it is your first library.
// Restart the Arduino IDE.
#include <TheAirBoard.h>
#include <RemoteXY.h>
#include <SoftwareSerial.h>

// RemoteXY graphical interface configuration
unsigned char RemoteXY_CONF[] =
  { 2,0,9,0,1,5,5,9,25,11
  ,49,49,2 }; 

struct {
  signed char joystick_1_x;           // -100..100 x-coordinate joystick position
  signed char joystick_1_y;           // -100..100 y-coordinate joystick position
  unsigned char connect_flag;
} RemoteXY;

// the setup routine runs once when you switch on the board:
void setup() {
  analogWrite(RIGHT, 0);              // reset motor
  analogWrite(LEFT, 0);               // reset motor
  analogWrite(GREEN, 1);              // ON indicator
  RemoteXY_Init();
}

// the loop routine runs over and over again forever:
void loop() { 
  RemoteXY_Handler();
  analogWrite(RIGHT, map(constrain(RemoteXY.joystick_1_x, 0, 100), 0, 100, 0, 255)); // drive right motor from 0 to 255
  analogWrite(LEFT, map(constrain(RemoteXY.joystick_1_y, 0, 100), 0, 100, 0, 255));  // drive left motor from 0 to 255
}
