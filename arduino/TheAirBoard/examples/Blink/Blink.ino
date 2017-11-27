/*
Blink LED
- turn an LED on for one second, then off for one second, repeatedly.

The AirBoard is a thumb-size, Arduino-compatible, wireless, low-power,
ubiquitous computer designed to sketch Internet-of-Things, fast!
Visit http://www.theairboard.cc
Upload your first sketch in seconds from https://codebender.cc?referral_code=Ub56L825Qb
Check README.txt and license.txt for more information.
All text above must be included in any redistribution.
*/

#define GREEN	 5               // GREEN dimmable LED
#define BLUE	 6               // BLUE dimmable LED
#define RED      9               // RED dimmable LED

// the setup routine runs once when you switch on the board:
void setup() {                
  pinMode(GREEN, OUTPUT);       // initialize the digital pin as an output
}

// the loop routine runs over and over again forever:
void loop() {
  analogWrite(GREEN, 255);     // turn the LED on and adjust the LED brightness between 1 and 255
  delay(1000);                 // wait for a second
  digitalWrite(GREEN, 0);      // turn the LED off
  delay(1000);                 // wait for a second
}
