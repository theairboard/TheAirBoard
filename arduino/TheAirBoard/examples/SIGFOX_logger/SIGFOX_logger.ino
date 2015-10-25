/*
SIGFOX logger program based on the Telecom Design TD120x module
- plug the light sensor between the A4 and GND pins
- repeatedly read the light sensor from analog pin A4
- send the value to the SIGFOX network every 10 minutes
  (maximum 140 messages per day)
   
1. upload this program to The AirBoard via the BLE-LINK shield
2. replace the BLE-LINK shield by the SIGFOX shield
3. connect to the SIGFOX backend: http://backend.sigfox.com

The AirBoard is a thumb-size, Arduino-compatible, wireless, low-power,
ubiquitous computer designed to sketch Internet-of-Things, fast!
Visit http://www.theairboard.cc
Upload your first sketch in seconds from https://codebender.cc?referral_code=Ub56L825Qb
Check README.txt and license.txt for more information.
All text above must be included in any redistribution.
*/

#define BLUE	  6                                  // The Airboard BLUE LED

char buffer[9];

// the setup routine runs once when you switch on the board:
void setup() {
  pinMode(BLUE, OUTPUT);                             // initialize the BLUE LED pin as an output
  digitalWrite(A4, HIGH);                            // set internal pull-up for light sensor
  Serial.begin(9600);                                // initialize serial communication at 9600 bits per second
}

// the loop routine runs over and over again forever:
void loop() {
    analogWrite(BLUE, 1);                            // set communication indicator
    sprintf(buffer, "AT$SS=%.2X", analogRead(A4)/4); // acquire and format analog data
    Serial.println(buffer);                          // send message
    delay(6000);                                     // wait until end of transmission
    digitalWrite(BLUE, 0);                           // reset communication indicator
    delay(600000);                                   // wait for 10 minutes
}
