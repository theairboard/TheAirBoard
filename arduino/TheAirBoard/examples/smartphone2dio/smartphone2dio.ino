/*
Di.o by Chacon wireless power plug test program
- switch power plug remotely from a smartphone, using The AirBoard as a hub

Requires:
- The AirBoard with Bluetooth shield
- The free TABcolor.apk Android application
- A wireless Di.o 3500W power plug by Chacon
- QAM-TX1 433MHz ASK transmitter from Quasar with wire or chip antenna

Connect the QAM-TX1 module to The AirBoard proto board connector:
GND -> A5
DATA -> A4
VCC -> 3V3

Pair the plug with your Di.o power plug ID defined below, by:
- pressing the small switch on the plug
- sending the ON command

If necessary, unpair the plug with your Di.o power plug ID defined below, by:
- pressing the small switch on the plug
- sending the OFF command

The AirBoard is a thumb-size, Arduino-compatible, wireless,
ubiquitous computer designed to sketch Internet-of-Things, fast!

Visit http://www.theairboard.cc

Check readme.txt and license.txt for more information.
All text above must be included in any redistribution.
*/

#define GND   19        // GND signal mapped to pin 19 (labeled A5 on the proto board connector)
#define DATA  18        // DATA signal mapped to pin 18 (labeled A4 on the proto board connector)
#define ID    1234567   // Di.o power plug ID

#include <TheAirBoard_pinout.h>
#include <TheAirBoard_dio.h>

TheAirBoard_dio radio(DATA);

void setup() {
  analogWrite(RED, 1);  
  pinMode(GND, OUTPUT);
  digitalWrite(GND, 0); // trick : make the pin ground (RF module consumption = 11 mA) 
  Serial.begin(57600);
}

void loop() { 
  while(Serial.available()) {
    switch(Serial.read()) {
      case 'a': // ON
        analogWrite(RED, 0);
        analogWrite(GREEN, 1);
        radio.transmit433(1, ID);
      break;
      case 'h': // OFF
        analogWrite(RED, 1);
        analogWrite(GREEN, 0);
        radio.transmit433(0, ID);
      break;
      default:
      break;
    }
  }
}


