/*******************************************************************************
This is the library for the SHARP GP2Y0A02YK0F analog infrared proximity sensor.

The AirBoard is a thumb-size, Arduino-compatible, wireless, low-power,
ubiquitous computer designed to sketch Internet-of-Things, fast!
Visit http://www.theairboard.cc
Upload your first sketch in seconds from https://codebender.cc?referral_code=Ub56L825Qb
Check readme.txt and license.txt for more information.
All text above must be included in any redistribution.
*******************************************************************************/

#ifndef GP2Y0A02YK0F_h
#define GP2Y0A02YK0F_h

#include <Arduino.h>

#define _5V		0	// 5V sensor supply
#define _3V3	1	// 3V3 sensor supply
#define CM		0	// return distance in centimeters
#define INCH	1	// return distance in inches

class GP2Y0A02YK0F
{
  public:
    GP2Y0A02YK0F(int pin);
    byte Ranging(boolean supply, boolean unit);

  private:
    int _pin;
};

#endif