/*
 * HTTP Client examples for the WiFly.
 *
 * Provides http client examples for GET and POST.
 *
 * GET /userprog_get.php?DATA=abc
 * POST /userprog_post.php
 *
 *
 * Download Mikal Hart's NewSoftSerial, Streaming and PString libraries from http://arduiniana.org
 *
 * Remember to set:
 * MY_WIFI_SSID
 * MY_WIFI_PASSPHRASE to your local wifi values.
 * MY_SERVER_GET
 * MY_SERVER_GET_URL
 * MY_SERVER_POST
 * MY_SERVER_POST_URL to your local server values.
 * ...server-side php scripts are required for this example.
 *    userprog_get.php and userprog_post.php are included and
 *    are in the download package.
 
 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.
 
 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 
 Copyright GPL 2.1 Tom Waldock 2012
 */
#include <Arduino.h>
#include <Time.h>
#include <SoftwareSerial.h>
#include <Streaming.h>
#include <PString.h>
#include <WiFlySerial.h>
#include "Credentials.h"
#include "MemoryFree.h"

// initialize WiFly
// loop
// send a parameterized GET to a web server
//    - show connection and response
// send a parameterized POST to a web server
//    - show connection and response
// 

// Connect the WiFly TX pin to the Arduino RX pin  (Transmit from WiFly-> Receive into Arduino)
// Connect the WiFly RX pin to the Arduino TX pin  (Transmit from Arduino-> Receive into WiFly)
// 
// Connect the WiFly GND pin to an Arduino GND pin
// Finally, connect the WiFly BATT pin to the 3.3V pin (NOT the 5v pin)

#define ARDUINO_RX_PIN  2
#define ARDUINO_TX_PIN  3

// server hosting GET example php script
#define MY_SERVER_GET "192.168.1.11"
#define MY_SERVER_GET_URL "/cgi-bin/userprog_get.php"


// server hosting POST example php script
#define MY_SERVER_POST MY_SERVER_GET
#define MY_SERVER_POST_URL "/cgi-bin/userprog_post.php"

// prog_char s_WT_SETUP_00[] PROGMEM = "nist1-la.ustiming.org";  /* change to your favorite NTP server */
prog_char s_WT_SETUP_01[] PROGMEM = "set u m 0x1";
prog_char s_WT_SETUP_02[] PROGMEM = "set comm remote 0";
prog_char s_WT_SETUP_03[] PROGMEM = "set comm idle 30";
prog_char s_WT_SETUP_04[] PROGMEM = "set comm time 2000";
prog_char s_WT_SETUP_05[] PROGMEM = "set comm size 64";
prog_char s_WT_SETUP_06[] PROGMEM = "set comm match 0x9";
prog_char s_WT_SETUP_07[] PROGMEM = "time";
prog_char s_WT_STATUS_SENSORS[] PROGMEM = "show q 0x177 ";
prog_char s_WT_STATUS_TEMP[] PROGMEM = "show q t ";
prog_char s_WT_STATUS_RSSI[] PROGMEM = "show rssi ";
prog_char s_WT_STATUS_BATT[] PROGMEM = "show battery ";
prog_char s_WT_MSG_JOIN[] PROGMEM = "Credentials Set, Joining ";
prog_char s_WT_MSG_START_WEBCLIENT[] PROGMEM = "Starting WebClientGetPut - Please wait. ";
prog_char s_WT_MSG_RAM[] PROGMEM = "RAM :";
prog_char s_WT_MSG_START_WIFLY[] PROGMEM = "Started WiFly, RAM :";
prog_char s_WT_MSG_WIFI[] PROGMEM = "Initial WiFi Settings :";
prog_char s_WT_MSG_APP_SETTINGS[] PROGMEM = "Configure WebClientGetPost Settings...";
prog_char s_WT_MSG_WIRE_RX[] PROGMEM = "Arduino Rx Pin (connect to WiFly Tx):";
prog_char s_WT_MSG_WIRE_TX[] PROGMEM = "Arduino Tx Pin (connect to WiFly Rx):";
prog_char s_WT_MSG_FAIL_OPEN[] PROGMEM = "Failed on opening connection to:";
prog_char s_WT_HTML_HEAD_01[] PROGMEM = "HTTP/1.1 200 OK \r ";
prog_char s_WT_HTML_HEAD_02[] PROGMEM = "Content-Type: text/html;charset=UTF-8\r ";
prog_char s_WT_HTML_HEAD_03[] PROGMEM = " Content-Length: ";
prog_char s_WT_HTML_HEAD_04[] PROGMEM = "Connection: close \r\n\r\n";
prog_char s_WT_POST_HEAD_01[] PROGMEM = "HTTP/1.1\n";
prog_char s_WT_POST_HEAD_02[] PROGMEM = "Content-Type: application/x-www-form-urlencoded\n";
prog_char s_WT_POST_HEAD_03[] PROGMEM = "Content-Length: ";
prog_char s_WT_POST_HEAD_04[] PROGMEM = "Connection: close\n\n";


#define IDX_WT_SETUP_00 0
#define IDX_WT_SETUP_01 IDX_WT_SETUP_00 
#define IDX_WT_SETUP_02 IDX_WT_SETUP_01 +1
#define IDX_WT_SETUP_03 IDX_WT_SETUP_02 +1
#define IDX_WT_SETUP_04 IDX_WT_SETUP_03 +1
#define IDX_WT_SETUP_05 IDX_WT_SETUP_04 +1
#define IDX_WT_SETUP_06 IDX_WT_SETUP_05 +1
#define IDX_WT_SETUP_07 IDX_WT_SETUP_06 +1

#define IDX_WT_STATUS_SENSORS    IDX_WT_SETUP_07 +1
#define IDX_WT_STATUS_TEMP       IDX_WT_STATUS_SENSORS +1
#define IDX_WT_STATUS_RSSI       IDX_WT_STATUS_TEMP +1
#define IDX_WT_STATUS_BATT       IDX_WT_STATUS_RSSI +1

#define IDX_WT_MSG_JOIN          IDX_WT_STATUS_BATT +1
#define IDX_WT_MSG_START_WEBCLIENT IDX_WT_MSG_JOIN +1
#define IDX_WT_MSG_RAM           IDX_WT_MSG_START_WEBCLIENT +1
#define IDX_WT_MSG_START_WIFLY   IDX_WT_MSG_RAM +1
#define IDX_WT_MSG_WIFI          IDX_WT_MSG_START_WIFLY +1
#define IDX_WT_MSG_APP_SETTINGS  IDX_WT_MSG_WIFI +1
#define IDX_WT_MSG_WIRE_RX       IDX_WT_MSG_APP_SETTINGS +1
#define IDX_WT_MSG_WIRE_TX       IDX_WT_MSG_WIRE_RX +1
#define IDX_WT_MSG_FAIL_OPEN     IDX_WT_MSG_WIRE_TX +1

#define IDX_WT_HTML_HEAD_01      IDX_WT_MSG_FAIL_OPEN + 1
#define IDX_WT_HTML_HEAD_02      IDX_WT_HTML_HEAD_01 + 1
#define IDX_WT_HTML_HEAD_03      IDX_WT_HTML_HEAD_02 + 1
#define IDX_WT_HTML_HEAD_04      IDX_WT_HTML_HEAD_03 + 1

#define IDX_WT_POST_HEAD_01      IDX_WT_HTML_HEAD_04 + 1
#define IDX_WT_POST_HEAD_02      IDX_WT_POST_HEAD_01 + 1
#define IDX_WT_POST_HEAD_03      IDX_WT_POST_HEAD_02 + 1
#define IDX_WT_POST_HEAD_04      IDX_WT_POST_HEAD_03 + 1


PROGMEM const char *WT_string_table[] = 	   
{   
//  s_WT_SETUP_00,
  s_WT_SETUP_01,
  s_WT_SETUP_02,
  s_WT_SETUP_03,
  s_WT_SETUP_04,
  s_WT_SETUP_05,
  s_WT_SETUP_06,
  s_WT_SETUP_07,
  s_WT_STATUS_SENSORS,
  s_WT_STATUS_TEMP,
  s_WT_STATUS_RSSI,
  s_WT_STATUS_BATT,
  s_WT_MSG_JOIN,
  s_WT_MSG_START_WEBCLIENT,
  s_WT_MSG_RAM,
  s_WT_MSG_START_WIFLY,
  s_WT_MSG_WIFI,
  s_WT_MSG_APP_SETTINGS,
  s_WT_MSG_WIRE_RX,
  s_WT_MSG_WIRE_TX,
  s_WT_MSG_FAIL_OPEN,
  s_WT_HTML_HEAD_01,
  s_WT_HTML_HEAD_02,
  s_WT_HTML_HEAD_03,
  s_WT_HTML_HEAD_04,
  s_WT_POST_HEAD_01,
  s_WT_POST_HEAD_02,
  s_WT_POST_HEAD_03,
  s_WT_POST_HEAD_04
};

// various buffer sizes
#define REQUEST_BUFFER_SIZE 180
#define POST_BUFFER_SIZE 180
#define TEMP_BUFFER_SIZE 60

char chMisc;
int iRequest = 0;
int iTrack = 0;
int iLoopCounter = 0;

WiFlySerial wifi(ARDUINO_RX_PIN ,ARDUINO_TX_PIN);
char bufRequest[REQUEST_BUFFER_SIZE];
char bufTemp[TEMP_BUFFER_SIZE];


// Function for setSyncProvider
time_t GetSyncTime() {
  time_t tCurrent = (time_t) wifi.getTime();
  wifi.exitCommandMode();
  return tCurrent;
}

// GetBuffer_P
// Returns pointer to a supplied Buffer, from PROGMEM based on StringIndex provided.
// based on example from http://arduino.cc/en/Reference/PROGMEM

char* GetBuffer_P(const int StringIndex, char* pBuffer, int bufSize) { 
  strncpy_P(pBuffer, (char*)pgm_read_word(&(WT_string_table[StringIndex])), bufSize);  
  return pBuffer; 
}

// Reconnects to a wifi network.
// DHCP is enabled explicitly.
// You may need to add the MAC address to your MAC filter list.
// Static IP settings available if needed.
boolean Reconnect() {

  wifi.SendCommand(GetBuffer_P(IDX_WT_SETUP_01,bufTemp,TEMP_BUFFER_SIZE), ">",bufRequest, REQUEST_BUFFER_SIZE);
  wifi.setDHCPMode(WIFLY_DHCP_CACHE );
  wifi.SendCommand(GetBuffer_P(IDX_WT_SETUP_02,bufTemp,TEMP_BUFFER_SIZE),">",bufRequest, REQUEST_BUFFER_SIZE);
  Serial << F("Leaving current wifi ...") << wifi.leave() << endl;
  // join
  wifi.setPassphrase(passphrase);    
  Serial << GetBuffer_P(IDX_WT_MSG_JOIN,bufTemp,TEMP_BUFFER_SIZE) << ssid << endl;
  wifi.join(ssid);

  // Set NTP server, update frequency, 
  wifi.setNTP(ntp_server); 
  wifi.setNTP_Update_Frequency(" 15");
  // don't send *HELLO* on http traffic
  // close idle connections after n seconds
  // give enough time for packet data to arrive
  // make data packet size sufficiently large
  // send data packet when a \t appears in stream
  //  force time resync.

  // Configure application-specific settings

  Serial << GetBuffer_P(IDX_WT_MSG_APP_SETTINGS, bufTemp, TEMP_BUFFER_SIZE) << endl;
  for (int i = 0; i< 7 ; i++) {
    wifi.SendCommand(GetBuffer_P(IDX_WT_SETUP_01 + i,bufTemp,TEMP_BUFFER_SIZE),">",bufRequest, REQUEST_BUFFER_SIZE);
  }
  wifi.getDeviceStatus();
  setTime( wifi.getTime() );
  delay(1000);
  setSyncProvider( GetSyncTime );

  // reboot if not working right yet.
  iTrack++;
  if ( iTrack > 5 ) {
    wifi.reboot();
    iTrack = 0;
  }

}


// Arduino Setup routine.

void setup() {
  Serial.begin(9600);

  Serial << GetBuffer_P(IDX_WT_MSG_START_WEBCLIENT,bufTemp,TEMP_BUFFER_SIZE) << endl << GetBuffer_P(IDX_WT_MSG_RAM,bufTemp,TEMP_BUFFER_SIZE) << freeMemory() << endl
    << GetBuffer_P(IDX_WT_MSG_WIRE_RX,bufTemp,TEMP_BUFFER_SIZE) << ARDUINO_RX_PIN << endl << GetBuffer_P(IDX_WT_MSG_WIRE_TX,bufTemp,TEMP_BUFFER_SIZE) << ARDUINO_TX_PIN << endl;

  wifi.begin();
  Serial << F("Starting WebClientGetPost...") <<  wifi.getLibraryVersion(bufRequest, REQUEST_BUFFER_SIZE) << "  RAM:" << freeMemory() << endl;

  // get MAC
  Serial << F("MAC: ") << wifi.getMAC(bufRequest, REQUEST_BUFFER_SIZE) << endl;

  Reconnect();
  // Set timezone adjustment: PST is -8h.  Adjust to your local timezone.
  adjustTime( (long) (-8 * 60 * 60) );
  Serial << F("DateTime:") << year() << "-" << month() << "-" << day() << " " << hour() << ":" << minute() << ":" << second() << endl;

  Serial << GetBuffer_P(IDX_WT_MSG_WIFI,bufTemp,TEMP_BUFFER_SIZE) << endl  
    << F("IP: ") << wifi.getIP(bufRequest, REQUEST_BUFFER_SIZE) << endl
    << F("Netmask: ") << wifi.getNetMask(bufRequest, REQUEST_BUFFER_SIZE) << endl
    << F("Gateway: ") << wifi.getGateway(bufRequest, REQUEST_BUFFER_SIZE) << endl
    << F("DNS: ") << wifi.getDNS(bufRequest, REQUEST_BUFFER_SIZE) << endl 
    << F("RSSI: ") << wifi.getRSSI(bufRequest, REQUEST_BUFFER_SIZE) << endl
    << F("battery: ") <<  wifi.getBattery(bufRequest, REQUEST_BUFFER_SIZE) << endl;
  
  memset (bufRequest,'\0',REQUEST_BUFFER_SIZE);

  // close any open connections
  wifi.closeConnection();
  Serial << F("After Setup RAM:") << freeMemory() << endl ;
  

}


void loop() {

  Serial << F("Beginning of Loop RAM:") << freeMemory() << endl ;
  
  // Perform GET example
  float fSampleValue = iLoopCounter + (iLoopCounter / 10 );

  Do_GET_Example(   iLoopCounter, fSampleValue );
  
  // Perform POST example
  Do_POST_Example ( iLoopCounter, fSampleValue );
  
  // Serial << "Clear leftovers" << endl;
  // flush the wifi buffer by reading and showing anything left around.


  while ((chMisc = wifi.read()) > -1) {
    Serial << chMisc;
  }
  iLoopCounter++;
  
  // wait a bit
  Serial << F("Waiting a moment..") << endl;
  delay(2000);
}


// Do_GET_Example
// 
// Performs a parameterized GET to communicate data to a server.
// Parameters: 
// iLoopCounter    A sample value.
// fValue          A sample float value
//
// Returns         0. Always. Play 'spot the demo code' and win!
//
// Feed model: http://www.myserver.example/get_example.php?LOOPCOUNTER=1&value=3.21
// Results displayed on console terminal.
//
//                    ...etc
int Do_GET_Example(  int iLoopCounter, float fValue ) {

  char bufRequest[REQUEST_BUFFER_SIZE];
  char bufTemp[TEMP_BUFFER_SIZE];
  
  PString strRequest(bufRequest, REQUEST_BUFFER_SIZE);
  
  // Build GET expression
  
  strRequest << F("GET ") << MY_SERVER_GET_URL << F("?counter=") << iLoopCounter << F("&value=") << fValue 
     << F(" HTTP/1.1") << "\n"
     << F("Host: ") << MY_SERVER_GET << "\n"
     << F("Connection: close") << "\n"
     << "\n\n";
  // send data via request
  // close connection
  
  Serial << F("GET request:")  << strRequest <<  endl << F("RAM: ") << freeMemory() << endl;

  // Open connection, then sent GET Request, and display response.
  if (wifi.openConnection( MY_SERVER_GET ) ) {
    
    wifi <<  (const char*) strRequest << endl; 
    
    // Show server response
    unsigned long TimeOut = millis() + 4000;

    while (  TimeOut > millis() && wifi.isConnectionOpen() ) {
      if (  wifi.available() > 0 ) {
        Serial << (char) wifi.read();
      }
    }
    
    // Force-close connection
    wifi.closeConnection();
 
  } else {
    // Failed to open connection
    Serial << GetBuffer_P(IDX_WT_MSG_FAIL_OPEN,bufTemp,TEMP_BUFFER_SIZE) << MY_SERVER_GET << endl;
  }
  
  wifi.setDebugChannel( NULL );
  return 0;
}


// Do_POST_Example
// 
// Performs a parameterized POST to communicate data to a server.
// Parameters: 
// iLoopCounter    A sample value.
// fValue          A sample float value
//
// Returns         0. Always. Play 'spot the demo code' and win!
//                 Unacceptable for a code-review.
//
//
// Feed model: http://www.myserver.example/get_example.php?LOOPCOUNTER=1&value=3.21
//  
//                    ...etc
int Do_POST_Example(  int iLoopCounter, float fValue ) {
  Serial << F("POST Example RAM: ") << freeMemory() << endl;

  char bufRequest[REQUEST_BUFFER_SIZE];
  char bufTemp[TEMP_BUFFER_SIZE];
  char bufPayLoad[TEMP_BUFFER_SIZE];
  
  memset (bufPayLoad,'\0', TEMP_BUFFER_SIZE);
  
  PString strRequest(bufRequest, REQUEST_BUFFER_SIZE);
  PString strPayLoad(bufPayLoad, TEMP_BUFFER_SIZE);
  
  // Build POST expression
  
  strPayLoad << F("counter=") << iLoopCounter << F("&value=") << fValue;
   
//  strRequest << "POST /cgi-bin/userprog_post.php HTTP/1.1\n"
//    << "Host: 192.168.1.11\n"
//    << "Content-Type: application/x-www-form-urlencoded\n"
//    << "Content-Length: " << strPayLoad.length() << "\n"
//    << "Connection: close\n\n"
//    << strPayLoad << "\n\n";
  strRequest << F("POST ") << MY_SERVER_POST_URL << " " << GetBuffer_P(IDX_WT_POST_HEAD_01,bufTemp,TEMP_BUFFER_SIZE)
     << F("Host: ") << MY_SERVER_POST << "\n"
     << GetBuffer_P(IDX_WT_POST_HEAD_02,bufTemp,TEMP_BUFFER_SIZE)
     << GetBuffer_P(IDX_WT_POST_HEAD_03,bufTemp,TEMP_BUFFER_SIZE) << strPayLoad.length() << "\n"
     << GetBuffer_P(IDX_WT_POST_HEAD_04,bufTemp,TEMP_BUFFER_SIZE) 
     << strPayLoad << "\n\n" ;
  // send data via request
  // close connection
  
  Serial << F("POST Request:")  << endl << strRequest << endl << F("RAM:") << freeMemory() 
    << F(" Waiting for server:") << endl;

  // Open connection, then sent GET Request, and display response.
  if (wifi.openConnection( MY_SERVER_POST ) ) {
    
    wifi <<  (const char*) strRequest ; 
    
    // Show server response

    unsigned long TimeOut = millis() + 3000;

    while ( millis()  < TimeOut && wifi.isConnectionOpen() ) {
      if (  wifi.available() > 0 ) {
        Serial <<  (char) wifi.read();
      }
    }
    
    // Force-close connection

    wifi.closeConnection();
 
  } else {
    // Failed to open
    Serial << GetBuffer_P(IDX_WT_MSG_FAIL_OPEN,bufTemp,TEMP_BUFFER_SIZE) << MY_SERVER_POST << endl;
  }
  
  wifi.setDebugChannel( NULL );
  return 0;
}


