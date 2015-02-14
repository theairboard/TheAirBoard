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
 
#ifndef TheAirBoard_dio_h
#define TheAirBoard_dio_h
 
#include <Arduino.h>

class TheAirBoard_dio
{
  private:
   	int _pin;
	void transmitData(boolean blnOn);
	void sendBit(boolean b);
	void sendPair(boolean b);
	void itob(unsigned long integer, int length, boolean bits[]);
	unsigned long power2(int power);

  public:
    TheAirBoard_dio(int pin);
	void transmit433(boolean blnOn, unsigned long senderID);
};

#endif