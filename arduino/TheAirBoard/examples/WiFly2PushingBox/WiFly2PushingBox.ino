/*
PushingBox notification program
- join your local WiFi network
- push a notification to http://www.pushingbox.com every minute
- send the duration of the last transaction
- require the WiFly RN-XV (RN-171) WiFi shield
- IMPORTANT: WiFly baudrate must first be set to 9600

The AirBoard is a thumb-size, Arduino-compatible, wireless,
ubiquitous computer designed to sketch Internet-of-Things, fast!

Visit http://www.theairboard.cc

Check readme.txt and license.txt for more information.
All text above must be included in any redistribution.
*/

#include <TheAirBoard_pinout.h>
#include <Streaming.h>
#include <SoftwareSerial.h>
#include "WiFlySerial.h"

#define NETWORK   "Your_SSID_here"
#define PASSWORD  "Your_password_here"
#define DEVICEID  "Your_PushingBox_DeviceID_here"

int duration = 0;
WiFlySerial wifi(0,1); // 0 = WiFly to The AirBoard, 1 = The AirBoard to WiFly

void setup() {
  pinMode(RED, OUTPUT);
  wifi.begin();
  wifi.println("set comm open *OPEN*");   // set local UART string on TCP port open
  wifi.println("set comm remote 0");      // set remote TCP client string on TCP port open
  wifi.setPassphrase(PASSWORD);
  wifi.join(NETWORK);
}

void loop() {
  duration = sendData(duration);
  delay(60000);
}

/*******************************************************************************
 * sendData
 * - send data to PushingBox server using GET Method
 * - light green LED during transaction
 * - return transaction time in seconds
 *******************************************************************************/
int sendData(int data) {
  unsigned long start = millis();  
  digitalWrite(RED, 1);
  wifi.openConnection("api.pushingbox.com");
  wifi << "GET /pushingbox?devid=" << DEVICEID << "&var=" << data << " HTTP/1.1\nHost: api.pushingbox.com\nUser-Agent: Arduino\n\n";
  wifi.closeConnection();   
  digitalWrite(RED, 0);
  return((millis()-start)/1000);
}

