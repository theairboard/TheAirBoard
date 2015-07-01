/*
Sharp GP2Y0A02YK0F infrared proximity sensor
- choose between 3V3 or 5V source to power the sensor
- repeatedly read the proximity sensor value from analog pin A4

Check README.txt and license.txt for more information.
All text above must be included in any redistribution.
*/

// Place /GP2Y0A02YK0F folders in your Preferences>Sketchbook location /libraries folder.
// You may need to create the /libraries subfolder if it is your first library.
// Restart the Arduino IDE.
#include <GP2Y0A02YK0F.h>

GP2Y0A02YK0F sensor(A4);                   // attach the proximity sensor to analog pin A4
char buffer[7];

// the setup routine runs once when you switch on the board:
void setup() {
  Serial.begin(9600);                      // initialize serial communication
}

// the loop routine runs over and over again forever:
void loop() {
    byte distance;
    distance = sensor.Ranging(_3V3, CM);   // compute distance (supply = _5V or _3V3, unit = CM or INCH)
    sprintf(buffer,"%3d cm", distance);    // display object distance
    Serial.println(buffer);
    delay(500);                            // wait for 500 milliseconds
}
