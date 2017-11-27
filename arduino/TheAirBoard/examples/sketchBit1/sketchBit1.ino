/*
sketchBit transparent mode
- directly copy inpiut bit to output bit
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

The AirBoard is a thumb-size, Arduino-compatible, wireless, low-power,
ubiquitous computer designed to sketch Internet-of-Things, fast!
Visit http://www.theairboard.cc
Upload your first sketch in seconds from https://codebender.cc?referral_code=Ub56L825Qb
Check README.txt and license.txt for more information.
All text above must be included in any redistribution.
*/
#define BITIN    A5                   // sketchBit input bit
#define BITOUT   10                   // sketchBit output bit
#define RED       9                   // The Airboard RED LED
#define _5V      A6                   // The Airboard 5V voltage indicator

// the setup routine runs once when you switch on the board:
void setup() {
  pinMode(RED, OUTPUT);               // initialize the digital pin as an output
  Serial.begin(9600);                 // initialize serial communication at 9600 bits per second
}

// the loop routine runs over and over again forever:
void loop() {
  digitalWrite(RED, analogRead(_5V) > 512 ? true:false); // set RED LED when POWER BIT is connected
  analogWrite(BITOUT, map(constrain(analogRead(BITIN), 0, 700), 0, 700, 0, 255)); // redirect the input bit to the output bit
  Serial.println(analogRead(BITIN));
  delay(500);                         // wait for 500 milliseconds
}

