/*
LoRa logger program based on the Multitech MultiConnect® mDot™ LoRaWAN™ module
http://www.multitech.com/brands/multiconnect-mdot
- send a message to the LoRa network every 10 seconds
   
1. upload this program to The AirBoard via the BLE-LINK shield
2. replace the BLE-LINK shield by the LoRa shield

The AirBoard is a thumb-size, Arduino-compatible, wireless, low-power,
ubiquitous computer designed to sketch Internet-of-Things, fast!
Visit http://www.theairboard.cc
Upload your first sketch in seconds from https://codebender.cc?referral_code=Ub56L825Qb
Check README.txt and license.txt for more information.
All text above must be included in any redistribution.
*/

#define BLUE	  6                                  // The Airboard BLUE LED

// the setup routine runs once when you switch on the board:
void setup() {
  pinMode(BLUE, OUTPUT);                             // initialize the BLUE LED pin as an output
  Serial.begin(115200);                              // initialize serial communication at 115200 bits per second
  Serial.println("AT+JOIN");                         // send message  
}

// the loop routine runs over and over again forever:
void loop() {
    digitalWrite(BLUE, 1);                           // set communication indicator
    Serial.println("AT+SEND=hello airboard");        // send message
    delay(100);
    digitalWrite(BLUE, 0);                           // reset communication indicator
    delay(10000);
}
