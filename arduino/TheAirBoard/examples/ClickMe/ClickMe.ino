/*
Simple button with RemoteXY
- switch ON the GREEN LED on "click me" button press
- watch: http://www.youtube.com/watch?v=UtD5kPPm1xQ

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

// Place /TheAirBoard and /RemoteXY folders in your Preferences>Sketchbook location /libraries folder.
// You may need to create the /libraries subfolder if it is your first library.
// Restart the Arduino IDE.
#include <TheAirBoard.h>
#include <RemoteXY.h>
#include <SoftwareSerial.h>

// RemoteXY graphical interface configuration
unsigned char RemoteXY_CONF[] =
  { 1,0,18,0,1,5,1,0,24,6
  ,51,51,4,99,108,105,99,107,32,109
  ,101,0 };

struct {
  unsigned char click_me;
  unsigned char connect_flag;
} RemoteXY;

// the setup routine runs once when you switch on the board:
void setup() {
  RemoteXY_Init(); 
  pinMode(GREEN, OUTPUT);                     // initialize the digital pin as an output
}

// the loop routine runs over and over again forever:
void loop() { 
  RemoteXY_Handler();
  digitalWrite(GREEN, RemoteXY.click_me?1:0); // turn on GREEN LED on button click
}
