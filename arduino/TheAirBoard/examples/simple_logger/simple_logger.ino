/*
Simple logger program
- send a data every minute
- power consumption with unoptimized Bluetooth 2.1 shield: 18.6 mA
- tested: 9h30 autonomy with Bluetooth on a 155 mAh battery

The AirBoard is a thumb-size, Arduino-compatible, wireless,
ubiquitous computer designed to sketch Internet-of-Things, fast!

Visit http://www.theairboard.cc

Check readme.txt and license.txt for more information.
All text above must be included in any redistribution.
*/

unsigned int i = 0;

void setup() {
  Serial.begin(57600);
}

void loop() {
    Serial.println(i++);
    delay(60000);
}
