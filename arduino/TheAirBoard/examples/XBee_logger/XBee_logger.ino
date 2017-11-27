/*
XBee ultra low power logger
- plug the light sensor between the A4 and GND pins
- configure the XBee shields (MASTER+SLAVE) at 57,600 bauds, using the ConfigShields program
- turn on the micro switch just before uploading to make sure the wireless module power is on (5 seconds timeout after boot)
- repeatedly read the light sensor from analog pin A4
- send the raw data every minute
- enter deep sleep between transmit (56-second watchdog timeout)
- average power consumption: 33.3 µA
- over 6 months estimated autonomy from a 155 mAh battery @ 1 data transmit per minute

The AirBoard is a thumb-size, Arduino-compatible, wireless, low-power,
ubiquitous computer designed to sketch Internet-of-Things, fast!
Visit http://www.theairboard.cc
Upload your first sketch in seconds from https://codebender.cc?referral_code=Ub56L825Qb
Check README.txt and license.txt for more information.
All text above must be included in any redistribution.
*/

// Place /TheAirBoard folder in your Preferences>Sketchbook location /libraries folder.
// You may need to create the /libraries subfolder if it is your first library.
// Restart the Arduino IDE.
#include <TheAirBoard.h>

TheAirBoard board;
volatile boolean f_wdt = true;
int timeout = 0;

// interrupt service routine: minimize time spent here
ISR(WDT_vect) {
  if(f_wdt == false)
    f_wdt = true;
}

// the setup routine runs once when you switch on the board:
void setup() {
  pinMode(RF, OUTPUT);
  digitalWrite(RF, 1);             // turn on wireless module
  Serial.begin(57600);             // initialize serial communication at 57600 bits per second
  delay(5000);                     // allow time to launch programming, before a possible wireless module power down
  board.setWatchdog(8000);         // set watchdog timeout in milliseconds (max 8000)
}

// the loop routine runs over and over again forever:
void loop() {
  if(f_wdt == true) {            // on watchdog expire (every 8 seconds)
    timeout++;
    if(timeout == 7) {           // timeout every 7*8 = 56 seconds
      analogWrite(BLUE, 1);      // instruction can be removed to save even more power
      digitalWrite(RF, HIGH);    // turn on wireless module
      delay(25);                 // delay for XBee power to stabilize
      digitalWrite(A4, HIGH);    // set internal pull-up for light sensor
      Serial.println(analogRead(A4)); // acquire analog data
      digitalWrite(A4, LOW);     // reset internal pull-up for light sensor to save power
      delay(5);                  // delay for data to transmit
      digitalWrite(RF, LOW);     // turn off wireless module
      analogWrite(BLUE, 0);      // reset communication indicator
      timeout = 0;
    }
    f_wdt = false;
    board.powerDown();           // goto deep sleep
  }
}
