/*
GSM data logger based on the SODAQ GPRSbee SIM800H shield http://shop.sodaq.com/en/sodaq-gprsbee-rev6.html 
- plug the light sensor between the A4 and GND pins
- repeatedly read the light sensor from analog pin A4
- send an SMS every minute to a mobile phone with the light level

The GPRSbee can be powered by The AirBoard internal battery or by an external battery.
- the battery signal is the 11th header pin close to SJ2
- SJ2: short for internal AirBoard battery, open for external battery
WARNING! DO NOT CONNECT AN EXTERNAL BATTERY TO THE GPRSBEE IF SJ2 IS SHORTENED.

1. upload this program to The AirBoard via the BLE-LINK shield
2. connect the SMA antenna and insert a mini SIM card into the GPRSbee slot
3. check SJ1 and SJ2 connections on the back of the GPRSbee shield. SJ1: "ON" should be connected to "PIN 4", SJ2: see above
4. replace the BLE-LINK shield by the GPRSbee shield
5. place the device within a GSM network coverage area

The AirBoard is a thumb-size, Arduino-compatible, wireless, low-power,
ubiquitous computer designed to sketch Internet-of-Things, fast!
Visit http://www.theairboard.cc
Upload your first sketch in seconds from https://codebender.cc?referral_code=Ub56L825Qb
Check README.txt and license.txt for more information.
All text above must be included in any redistribution.
*/

#define CELL    "Your_cell_phone_number_here"
#define GPRSBEE  16                   // GPRSbee wakeup pin
#define BLUE	  6                   // The Airboard BLUE LED

// the setup routine runs once when you switch on the board:
void setup() {
  pinMode(BLUE, OUTPUT);              // initialize the BLUE LED pin as an output
  pinMode(GPRSBEE, OUTPUT);           // initialize the GPRSbee wakeup pin as an output
  digitalWrite(GPRSBEE, HIGH);        // turn ON the GPRSbee
  delay(5000);                        // wait 5 seconds for the GPRSbee to initialize
  digitalWrite(A4, HIGH);             // set internal pull-up for light sensor
  Serial.begin(9600);                 // initialize serial communication at 9600 bits per second
  Serial.println("AT+CMGF=1");        // set GPRSbee text mode
  delay(30);
}

// the loop routine runs over and over again forever:
void loop() {
  digitalWrite(BLUE, 1);            // set communication indicator
  Serial.print("AT+CMGS=\"");         // set destination cell number
  Serial.print(CELL);
  Serial.println("\"");
  delay(30);
  Serial.print((int)(100.0-100.0*analogRead(A4)/1024)); // acquire analog data
  Serial.println("\% luminosity\nSent from #theairboard");
  delay(30);
  Serial.write(0x1A);               // ctrl-z to terminate and send
  digitalWrite(BLUE, 0);            // reset communication indicator
  delay(60000);                     // wait for a minute
}
