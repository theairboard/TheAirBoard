/*******************************************************************************
This is the Power Management System library for The AirBoard.

The AirBoard is a thumb-size, Arduino-compatible, wireless,
ubiquitous computer designed to sketch Internet-of-Things, fast!

Visit http://www.theairboard.cc

Check readme.txt and license.txt for more information.
All text above must be included in any redistribution.
*******************************************************************************/

#include <TheAirBoard_power.h>

/*******************************************************************************
 * Constructor : initialize utility library
 *******************************************************************************/
TheAirBoard_power::TheAirBoard_power(void) {
  digitalWrite(NCHG, HIGH);  // set internal pull-up for charge status
}

/*******************************************************************************
 * Power down processor
 * Don't forget to switch off the LED and the RF module for lowest consumption.
 * Board quiescent current budget:
 * charge circuit = 180 nA
 * ATmega328P deep sleep = 120 nA
 * regulators quiescent currents = 30 nA
 *******************************************************************************/
void TheAirBoard_power::powerDown(int baud) {
  Serial.end();
  digitalWrite(RX, LOW);           // reset internal pull-up serial link
  ADCSRA &= B01111111;             // disable ADC
  power_all_disable();
  /****************DON'T CHANGE************************************************/
  MCUCR = B01100000;               // BOD disable timed sequence
  MCUCR = B01000000;
  SMCR = B00000101;                // power down mode + sleep enable
  asm("sleep\n");
  sleep_disable();                 // first thing after wake up
  /****************************************************************************/
  power_all_enable();
  ADCSRA |= B10000000;             // enable ADC
  Serial.begin(baud);
}

/*******************************************************************************
 * Set watchdog timeout to the nearest value among:
 * 0.016, 0.032, 0.064, 0.125, 0.25, 0.5, 1.0, 2.0, 4.0, 8.0 seconds
 *******************************************************************************/
void TheAirBoard_power::setWatchdog(int period) {
  byte wdp;
  wdp = round(log(period/16)/log(2));
  wdp = wdp & B00000111 | (wdp & B00001000)<<2;
  MCUSR &= ~(1<<WDRF);             // clear reset flag
  WDTCSR |= (1<<WDCE) | (1<<WDE);  // in order to change WDE or the prescaler, we need to set WDCE (This will allow updates for 4 clock cycles)
  WDTCSR = wdp;					   // set watchdog prescaler
  WDTCSR |= _BV(WDIE);             // enable the WD interrupt (note no reset)
}

/*******************************************************************************
 * Battery check utility
 * - check battery voltage, USB voltage and charge status
 * - blink red/green while charging
 * - solid green when charged
 * - solid green when discharging
 * - solid red when battery low
 * - return battery voltage indicator
 *******************************************************************************/
#define REFRESH    500 // battery check and status refresh period in ms
#define LOWBAT     3.1 // battery low voltage threshold in V
#define VF         1.2 // SML-P11VT forward voltage in V (under current adjusted)

float TheAirBoard_power::batteryChk(void) {
  static unsigned long time;
  static float vbat;
  static boolean flag;
  boolean usbplug, batlow;
  
  if(millis() - time > REFRESH) {
    time = millis();
  
    usbplug = analogRead(VUSB) ? true:false;
    batlow = (vbat = 3.3*analogRead(VBAT)/1024 + VF) < LOWBAT ? true:false;
    
    if(usbplug) { // USB plugged in
      if(digitalRead(NCHG)) { // charged
        analogWrite(RED, 0);
        analogWrite(GREEN, 1);
      }
      else { // charging
         analogWrite(GREEN, flag^=1);
         analogWrite(RED, !flag);
      }
    }
    else { // no USB
      if(batlow) {
        analogWrite(RED, 1);
        analogWrite(GREEN, 0);
      }
      else {
        analogWrite(RED, 0);
        analogWrite(GREEN, 1);
      }
    }
  }
  
  return vbat;
}