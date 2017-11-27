/*
sketchBit smartphone control with RemoteXY
- input bit is displayed on your smartphone and smartphone controls output bit
- the sketchBit http://littlebits.cc/bitlab/bits/sketchbit is an input/output bit adapter
  that allows to control your littleBits modules from The AirBoard.
- combine the flexibility of The AirBoard broad range of wireless shields
  and the diversity of littleBits power bits to make your own devices, robots, etc...
- watch: http://www.youtube.com/watch?v=M4LEDxZeDYQ
- one input bit http://littlebits.cc/bits/dimmer goes to The AirBoard analog pin 5
- one output bit http://littlebits.cc/bits/bargraph is driven by The AirBoard digital pin 10
- a micro switch selects digital vs analog output
- a 9V battery http://littlebits.cc/accessories/battery-plus-cable powers the littleBits 5V
  power bit http://littlebits.cc/bits/littlebits-power which drives the AirBoard 5VUSB pin.
  This allows littleBits to recharge continuously the AirBoard's internal battery.
  WARNING! DO NOT CONNECT THE AIRBOARD MICRO USB CONNECTOR AND THE POWER BIT AT THE SAME TIME.

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
#define BITIN   A5                    // sketchBit input bit
#define BITOUT  10                    // sketchBit output bit
#define _5V     A6                    // The Airboard 5V voltage indicator

// Place /TheAirBoard and /RemoteXY folders in your Preferences>Sketchbook location /libraries folder.
// You may need to create the /libraries subfolder if it is your first library.
// Restart the Arduino IDE.
#include <TheAirBoard.h>
#include <RemoteXY.h>
#include <SoftwareSerial.h>

// RemoteXY graphical interface configuration
unsigned char RemoteXY_CONF[] =
  { 1,1,16,0,1,5,4,128,2,40
  ,93,13,2,66,128,8,12,83,11,2
   }; 

// RemoteXY controls definition
struct {
  unsigned char slider;               // slider value = 0 to 100
  unsigned char bargraph;             // bargraph value = 0 to 100
  unsigned char connect_flag;
} RemoteXY;

// the setup routine runs once when you switch on the board:
void setup() {
  RemoteXY_Init();  
}

// the loop routine runs over and over again forever:
void loop() {
  analogWrite(RED, analogRead(_5V) > 512 ? true:false);        // set RED LED when POWER BIT is connected
  RemoteXY_Handler();
  RemoteXY.bargraph = map(constrain(analogRead(BITIN), 0, 820), 0, 820, 0, 100);  // display input bit value on smartphone
  analogWrite(BITOUT, map(RemoteXY.slider, 0, 100, 0, 255));   // direct the slider value to output bit
}

