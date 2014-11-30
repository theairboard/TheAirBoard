/*******************************************************************************
This is the Power Management System library for The AirBoard.

The AirBoard is a thumb-size, Arduino-compatible, wireless,
ubiquitous computer designed to sketch Internet-of-Things, fast!

Visit http://www.theairboard.cc

Check readme.txt and license.txt for more information.
All text above must be included in any redistribution.
*******************************************************************************/
 
#ifndef TheAirBoard_power_h
#define TheAirBoard_power_h
 
#include <Arduino.h>
#include <TheAirBoard_pinout.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>
#include <math.h>

class TheAirBoard_power
{
  public:
    TheAirBoard_power(void);
	void powerDown(int baud);
	void setWatchdog(int period);
	float batteryChk(void);
};

#endif