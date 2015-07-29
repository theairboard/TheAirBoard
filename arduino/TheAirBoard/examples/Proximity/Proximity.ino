/*
Sharp GP2Y0A02YK0F infrared proximity sensor
- choose between 3V3 or 5V source to power the sensor
- repeatedly read the proximity sensor value from analog pin A4
- light the RED LED when distance is under 50 cm

1. order and populate the proximity sensor PCB: http://oshpark.com/shared_projects/m8978EOZ
2. on the breakout board, connect VCC to 5V if USB is plugged in, to 3V3 otherwise

The AirBoard is a thumb-size, Arduino-compatible, wireless, low-power,
ubiquitous computer designed to sketch Internet-of-Things, fast!
Visit http://www.theairboard.cc
Upload your first sketch in seconds from https://codebender.cc?referral_code=Ub56L825Qb
Check README.txt and license.txt for more information.
All text above must be included in any redistribution.
*/

// Place /TheAirBoard and /GP2Y0A02YK0F folders in your Preferences>Sketchbook location /libraries folder.
// You may need to create the /libraries subfolder if it is your first library.
// Restart the Arduino IDE.
#include <TheAirBoard.h>
#include <GP2Y0A02YK0F.h>

GP2Y0A02YK0F board(A4);                    // attach the proximity sensor board to analog pin A4
char buffer[7];

// the setup routine runs once when you switch on the board:
void setup() {
  pinMode(RED, OUTPUT);                    // initialize the RED LED pin as an output
  Serial.begin(9600);                      // initialize serial communication at 9600 bits per second
}

// the loop routine runs over and over again forever:
void loop() {
    byte distance;
    distance = board.Ranging(_3V3, CM);    // compute distance (supply = _5V or _3V3, unit = CM or INCH)
    sprintf(buffer,"%3d cm", distance);    // display object distance
    Serial.println(buffer);
    digitalWrite(RED, distance<50?1:0);    // turn on RED LED when under 50 cm
    delay(500);                            // wait for 500 milliseconds
}
