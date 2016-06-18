/*******************************************************************************
This is The AirBoard utility library.
Revision 2.2, June 18, 2016
- added getTemp function to return internal MCU temperature in °C

The AirBoard is a thumb-size, Arduino-compatible, wireless, low-power,
ubiquitous computer designed to sketch Internet-of-Things, fast!
Visit http://www.theairboard.cc
Upload your first sketch in seconds from https://codebender.cc?referral_code=Ub56L825Qb
Check README.txt and license.txt for more information.
All text above must be included in any redistribution.
*******************************************************************************/
 
#ifndef TheAirBoard_h
#define TheAirBoard_h
 
#include <Arduino.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>
#include <math.h>

// The AirBoard pin mapping
#define RX		 0 // UART serial receive port
#define TX		 1 // UART serial transmit port
#define CTS		 2 // Bluetooth/WiFi UART CTS flow control
#define WFF		 4 // WiFi GPIO6 TCP connection status
#define GREEN	 5 // GREEN LED (PWM)
#define BLUE	 6 // BLUE LED (PWM)
#define BTF		 7 // Bluetooth connection status/WiFi GPIO4 association/authentification status
#define RTS		 8 // Bluetooth/WiFi UART RTS flow control
#define RED		 9 // RED LED (PWM)
#define RF		14 // RF power control pin
#define RSSI	15 // XBee RSSI RX Strength Signal Indicator/WiFi GPIO5 TCP connection trigger
#define GPIO8	16 // Bluetooth PIO10 UART DTR flow control/WiFi GPIO8 sleep trigger/XBee DO8* output/GPRSbee wakeup pin
#define NCHG	17 // battery charge status (0 = charging)
#define VUSB	A6 // USB voltage indicator
#define VBAT	A7 // battery voltage indicator

// XBee configuration roles
#define MASTER 1
#define SLAVE  0

class TheAirBoard
{
  public:
    TheAirBoard(void);
	void powerDown();
	void setWatchdog(int period);
	float batteryChk(void);
	void configXBee(boolean role);
	void configRN42();
	void configWiFly();
	double getTemp(void);
};

#endif