/*******************************************************************************
This is The AirBoard utility library.
Revision 2.0, April 09, 2016
- in powerDown function, added 1ms delay to allow MCU to recover from sleep
Revision 2.1, April 25, 2016
- in configRN42 function, doubled delay between commands
Revision 2.2, June 18, 2016
- in powerDown function, changed baudrate restoration method upon recover from sleep
- added getTemp function to return internal MCU temperature in °C

The AirBoard is a thumb-size, Arduino-compatible, wireless, low-power,
ubiquitous computer designed to sketch Internet-of-Things, fast!
Visit http://www.theairboard.cc
Upload your first sketch in seconds from https://codebender.cc?referral_code=Ub56L825Qb
Check README.txt and license.txt for more information.
All text above must be included in any redistribution.
*******************************************************************************/

#include <TheAirBoard.h>

/*******************************************************************************
 * Constructor : initialize utility library
 *******************************************************************************/
TheAirBoard::TheAirBoard(void) {
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
void TheAirBoard::powerDown() {
  Serial.flush();				   // flush uart
  Serial.end();					   // end communication
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
  Serial.begin(U2X0?1000000/((UBRR0H<<8)+UBRR0L+1):500000/((UBRR0H<<8)+UBRR0L+1)); // restart uart with initial baudrate
}

/*******************************************************************************
 * Set watchdog timeout to the nearest value among:
 * 0.016, 0.032, 0.064, 0.125, 0.25, 0.5, 1.0, 2.0, 4.0, 8.0 seconds
 *******************************************************************************/
void TheAirBoard::setWatchdog(int period) {
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
 * - solid green when discharging or when charged
 * - solid red when battery low
 * - return battery voltage indicator
 *******************************************************************************/
#define REFRESH    500 // battery check and status refresh period in ms
#define LOWBAT     3.1 // battery low voltage threshold in V
#define VF         1.2 // SML-P11VT forward voltage in V (under current adjusted)

float TheAirBoard::batteryChk(void) {
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

/*******************************************************************************
 * Set XB24-API XBee reference configuration = all factory defaults, except:
 * BD=6 (Both: Interface Data Rate = 57600 bauds)
 * CH=F (Both: Channel = 0x0F)
 * D3= (Master: DIO3 = DI, Slave: DIO3 = DO high) 
 * D7=0 (Slave: DIO7 = Disabled) 
 * IA=FFFF (Slave: I/O Input Address = any received I/O packet can change outputs)
 * IC=8 (Master: DIO Change Detect = bit DIO3)
 *******************************************************************************/
 void TheAirBoard::configXBee(boolean role) {
  	Serial.begin(9600); 
    Serial.print("+++");
    delay(2000);
    if(role)
    	Serial.println("ATRE,BD6,CHF,D33,IC8,WR,CN");		 // MASTER
    else
    	Serial.println("ATRE,BD6,CHF,D35,D70,IAFFFF,WR,CN"); // SLAVE
    delay(50);
    Serial.end();
    delay(100);
}

/*******************************************************************************
 * Set RN42XVP Bluetooth2 reference configuration = all factory defaults, except:
 * Baudrt=9600 (57600 for Over-The-Air Programming) * InqWindw=0012 * PagWindw=0500 * SniffEna=30
 * HidFlags=0 * Profile= MDM
 *******************************************************************************/
 void TheAirBoard::configRN42() {
  	Serial.begin(115200); 
  	Serial.print("$$$"); delay(40);
  	Serial.println("su,96"); delay(40);
  	Serial.println("si,0012"); delay(40);
  	Serial.println("sj,0500"); delay(40);
  	Serial.println("sw,0030"); delay(40);
  	Serial.println("sh,0000"); delay(40);
  	Serial.println("s~,3"); delay(40);
  	Serial.println("r,1"); delay(40);
  	Serial.end();
  	delay(100);
}

/*******************************************************************************
 * Set RN-XV RN171 WiFly reference configuration = all factory defaults, except:
 * OPEN=
 * CLOSE=
 * REMOTE=
 * IP=169.254.1.1:2000
 * IoMask=0x21f2
 * Baudrate=9600 (57600 for Over-The-Air Programming)
 *******************************************************************************/
void TheAirBoard::configWiFly() {
  	Serial.begin(9600);
  	Serial.print("$$$");
  	delay(500);
  	Serial.println();
  	Serial.println("factory RESET");
  	delay(1000);
  	Serial.println("set comm open 0");
  	Serial.println("set comm close 0");
  	Serial.println("set comm remote 0");
  	Serial.println("set ip address 169.254.1.1");
  	Serial.println("set sys mask 0x21f2");
  	Serial.println("set uart baudrate 9600");  	
  	Serial.println("save");
  	delay(50);
  	Serial.end();
  	delay(1000);
}

/*******************************************************************************
 * Get temperature
 * - get internal temperature in °C
 *******************************************************************************/
double TheAirBoard::getTemp(void) {
  unsigned int wADC;
  double t;

  // The internal temperature has to be used
  // with the internal reference of 1.1V.
  // Channel 8 can not be selected with
  // the analogRead function yet.

  // Set the internal reference and mux.
  ADMUX = (_BV(REFS1) | _BV(REFS0) | _BV(MUX3));
  ADCSRA |= _BV(ADEN);  // enable the ADC

  delay(20);            // wait for voltages to become stable.

  ADCSRA |= _BV(ADSC);  // Start the ADC

  // Detect end-of-conversion
  while (bit_is_set(ADCSRA,ADSC));

  // Reading register "ADCW" takes care of how to read ADCL and ADCH.
  wADC = ADCW;

  // The offset of 324.31 could be wrong. It is just an indication.
  t = (wADC - 324.31 ) / 1.22;

  // The returned temperature is in degrees Celsius.
  return (t);
}
			