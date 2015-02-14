/*******************************************************************************
This is the Di.o wireless power plug library for The AirBoard.
This library uses the Home Easy protocol.
One pin is required to drive a QAM-TX1 433MHz ASK transmitter from Quasar.

The AirBoard is a thumb-size, Arduino-compatible, wireless,
ubiquitous computer designed to sketch Internet-of-Things, fast!

Visit http://www.theairboard.cc

Check readme.txt and license.txt for more information.
All text above must be included in any redistribution.
*******************************************************************************/
 
#include <TheAirBoard_dio.h>

boolean bitReceiver[4] = {0, 0, 1, 0};	// 4 bits to store the receiver identifier
boolean bitSender[26];					// 26 bit to store the HE device address

/*******************************************************************************
 * Constructor : initialize Dio driver
 *******************************************************************************/
TheAirBoard_dio::TheAirBoard_dio(int pin) {
  pinMode(pin, OUTPUT);
  _pin = pin;
}

/*******************************************************************************
 * Send command to 433MHz ASK module
 *******************************************************************************/
void TheAirBoard_dio::transmit433(boolean blnOn, unsigned long senderID) {
  for (int i=0; i < 3; i++) {
  	itob(senderID, 26, bitSender);	// convert our device code
  	// Transmit the data twice...
  	transmitData(blnOn);			// send ON or OFF
  	delay(10);                 		// wait (socket ignores us it appears unless we do this)
  	transmitData(blnOn);       		// send ON or OFF again
  	delay(90);
  	// Transmit the data twice...
  	transmitData(blnOn);       		// send ON or OFF
  	delay(10);                 		// wait (socket ignores us it appears unless we do this)
  	transmitData(blnOn);       		// send ON or OFF again
  	delay(10);                 		// wait (socket ignores us it appears unless we do this)
  }
}

/*******************************************************************************
 * Transmit data
 *******************************************************************************/
void TheAirBoard_dio::transmitData(boolean blnOn) {
  int i;

  // Do the latch sequence.. 
  digitalWrite(_pin, HIGH);
  delayMicroseconds(275);		// bit of radio shouting before we start. 
  digitalWrite(_pin, LOW);
  delayMicroseconds(9900);		// low for 9900 for latch 1
  digitalWrite(_pin, HIGH);		// high again 
  delayMicroseconds(275);		// wait a moment 275
  digitalWrite(_pin, LOW);		// low again for 2675 - latch 2.
  delayMicroseconds(2675);
  digitalWrite(_pin, HIGH);		// End on a high 

  // Send HE Device Address..
  // e.g. 1000010101000110010  272946 in binary.
  for(i=0; i<26; i++) {
    sendPair(bitSender[i]);
  }

  // Send 26th bit - group 1/0
  sendPair(false);

  // Send 27th bit - on/off 1/0
  sendPair(blnOn);

  // last 4 bits - recipient   -- button 1 on the HE300 set to 
  // slider position I in this example:

  // Send HE Device Address..
  // e.g. 1000010101000110010  272946 in binary.
  for(i=0; i<4; i++) {
    sendPair(bitReceiver[i]);
  }

  digitalWrite(_pin, HIGH);		// high again (shut up)
  delayMicroseconds(275);		// wait a moment
  digitalWrite(_pin, LOW);		// low again for 2675 - latch 2.
  delayMicroseconds(2675);		// wait a moment
}

/*******************************************************************************
 * Send bit
 *******************************************************************************/
void TheAirBoard_dio::sendBit(boolean b) {
  if (b) {
    digitalWrite(_pin, HIGH);
    delayMicroseconds(275);		//275 orinally, but tweaked to 310.
    digitalWrite(_pin, LOW);
    delayMicroseconds(1225);	//1225 orinally, but tweaked to 1340.
  }
  else {
    digitalWrite(_pin, HIGH);
    delayMicroseconds(275);		//275 orinally, but tweaked to 310.
    digitalWrite(_pin, LOW);
    delayMicroseconds(275);		//275 orinally, but tweaked to 310.
  }
}

/*******************************************************************************
 * Send the Manchester Encoded data 01 or 10, never 11 or 00
 *******************************************************************************/
void TheAirBoard_dio::sendPair(boolean b) {
  if (b) {
    sendBit(true);
    sendBit(false);
  }
  else {
    sendBit(false);
    sendBit(true);
  }
}

/*******************************************************************************
 * Convert long device code into binary (stores in global bits array)
 *******************************************************************************/
void TheAirBoard_dio::itob(unsigned long integer, int length, boolean bits[]) { // need bits[length]
  for (int i=0; i<length; i++) {
    if ((integer / power2(length-1-i)) >= 1) {
      integer-=power2(length-1-i);
      bits[i]=1;
    } else
      bits[i]=0;
  }
}

/*******************************************************************************
 * Return 2 to the (power)
 *******************************************************************************/
unsigned long TheAirBoard_dio::power2(int power) {
  unsigned long integer=1;          
  for (int i=0; i<power; i++) {      
    integer*=2;
  }
  return integer;
}
