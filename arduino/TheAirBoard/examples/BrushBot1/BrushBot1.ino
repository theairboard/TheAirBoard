/*
BrushBot direct control
- use the right/left arrow keys to pilot the BrushBot/Airship
- order and populate the dual motor drive PCB for brushbot: http://oshpark.com/shared_projects/fyhMVHcY
  or directly connect micro motors to PINs 10, 11 and GND for airship
- watch: http://www.youtube.com/watch?v=gNQzRTcE6cM

1. Install tAB-Direction.apk on Bluetooth 4.0 BLE Android devices:
   http://github.com/theairboard/TheAirBoard/raw/master/arduino/TheAirBoard/examples/BrushBot1/tAB-Direction.apk
2. Search and connect "Bluno" device

The AirBoard is a thumb-size, Arduino-compatible, wireless, low-power,
ubiquitous computer designed to sketch Internet-of-Things, fast!
Visit http://www.theairboard.cc
Upload your first sketch in seconds from http://codebender.cc
Check readme.txt and license.txt for more information.
All text above must be included in any redistribution.
*/
#define RIGHT 11                // right motor
#define LEFT  10                // left motor
#define GREEN  5                // The Airboard GREEN LED

// the setup routine runs once when you switch on the board:
void setup() {
  pinMode(LEFT, OUTPUT);
  digitalWrite(LEFT, 0);        // reset motor
  pinMode(RIGHT, OUTPUT);
  digitalWrite(RIGHT, 0);       // reset motor
  analogWrite(GREEN, 1);        // ON indicator
  Serial.begin(115200);         // initialize serial communication at 115200 bits per second
}

// the loop routine runs over and over again forever:
void loop() {
 while (Serial.available()) {
    switch (Serial.read()) {
      case 'l':
        analogWrite(LEFT, 255);  // on left key press, full power = 255
      break;
      case 'm':
        digitalWrite(LEFT, 0);  // on left key release
      break;
      case 'r':
        analogWrite(RIGHT, 255); // on right key press, full power = 255
      break;
      case 't':
        digitalWrite(RIGHT, 0); // on right key release
      break;
      case '\n':
      case '\r':
      break;
      default:
        digitalWrite(LEFT, 0);
        digitalWrite(RIGHT, 0);
      break;
    }
  }
}


