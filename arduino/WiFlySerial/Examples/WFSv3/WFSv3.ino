/*
 * Web Server example for the WiFly showing the current time and some status items.
 * Client/Server demo 0.07
 * 
 * EARLY PREVIEW of CLIENT/SERVER model
 * Although it may appear to be stable this is an early preview subject to change.
 * 
 *
 * Remember to set credentials to your local values.
 
 Aim your browser at your WiFly address for a simple UTC time report.
 Add /status to the URL to get battery voltage and RSSI.
 or add /auto to get refreshes every 30 seconds.
 
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
 
 Copyright GPL 2.1 Tom Waldock 2011,2012
 */
#include <Arduino.h>
#include <Time.h>
#include <SoftwareSerial.h>
#include <Streaming.h>
#include <PString.h>
#include <WiFlySerial.h>
#include "MemoryFree.h"
#include "Credentials.h"
//#include "WFSEthernetServer.h"
#include "WFSEthernet.h"

// Connect the WiFly TX pin to the Arduino RX pin  (Transmit from WiFly-> Receive into Arduino)
// Connect the WiFly RX pin to the Arduino TX pin  (Transmit from Arduino-> Receive into WiFly)
// 
// Connect the WiFly GND pin to an Arduino GND pin
// Finally, connect the WiFly BATT pin to the 3.3V pin (NOT the 5v pin)

#define ARDUINO_RX_PIN  2
#define ARDUINO_TX_PIN  3

#define MY_TIMEZONE_OFFSET -7 

#define REQUEST_BUFFER_SIZE 80
#define HEADER_BUFFER_SIZE 120 
#define BODY_BUFFER_SIZE 180
#define TEMP_BUFFER_SIZE 80

char bufRequest[REQUEST_BUFFER_SIZE];
char bufTemp[TEMP_BUFFER_SIZE];
char chMisc;
int iRequest = 0;
int iTrack = 0;

extern WiFlySerial wifi;

WFSEthernetServer myServer(WIFLY_DEFAULT_LOCAL_PORT);


// Reconnects to a wifi network.
// DHCP is enabled explicitly.
// You may need to add the MAC address to your MAC filter list.
// Static IP settings available if needed.
boolean Connect() {

  // Start wifi Ethernet
  // Change these to fit your local situation
  int bOk_init = WFSEthernet.initDevice();
  int bOk_configure = WFSEthernet.configure(WIFLY_AUTH_WPA2_PSK, WIFLY_JOIN_AUTO, WIFLY_DHCP_ON);
  int bOk_cred = WFSEthernet.credentials(ssid, passphrase);
  Serial << F("init was:") << ( bOk_init ? F("true") : F("false") ) << endl
         << F("configure was:") << ( bOk_init ? F("true") : F("false") ) << endl
         << F("cred was:") << ( bOk_init ? F("true") : F("false") ) << endl;
  Serial << F("Initialization and configuration done ... beginning wifi association...") << endl;
  
  WFSEthernet.begin();
  Serial << F("WFSEthernet started.") << endl;

  // reboot if not working right yet.
  iTrack++;
  if ( iTrack > 5 ) {
    wifi.reboot();
    iTrack = 0;
  }

}

// Make Response Body
// Based on GET request string, generate a response.
int MakeReponseBody( char* pBody,  char* pRequest, const int sizeRequest ) {

  PString strBody( pBody, BODY_BUFFER_SIZE);

  if ( strstr(pRequest, "/status" ) ) {
    strBody << F("<html>WebServerTime Status:</br>Free RAM:") << freeMemory() << "</br>"
      << F("DateTime:") << year() << "-" << month() << "-" << day() << " " << _DEC(hour()) << ":" << minute() << ":" << second() << "</br>"
      << F("Battery: ") << wifi.getBattery(bufTemp,TEMP_BUFFER_SIZE) << "</br>"   
      << F("RSSI: ") << wifi.getRSSI(bufTemp,TEMP_BUFFER_SIZE) << "</br>"
      << F("</html>\r\n\r\n");

    // need to exit command mode to be able to send data
    wifi.exitCommandMode();

  } else {
    if ( strstr(pRequest,"/auto") ) {
      strBody << F("<META HTTP-EQUIV=\"REFRESH\" CONTENT=\"30\">");
    }
    strBody << F("<html>Current request:") << pRequest << F("</br>Millis:") << millis() << F(" Micros:") << micros()
      << F("</br>DateTime:") << year() << "-" << month() << "-" << day() << " " << hour() << ":" << minute() << ":" << second()
      << F("</html>\r\n\r\n");
    // No calls back to WiFly command mode; hence no need to exit Command mode that wasn't entered.
  }
  return strBody.length();
}

// MakeResponseHeader
// Form a HTML header, including length of body.
int MakeResponseHeader( char* pHeader, char* pBody ) {

  PString strHeader( pHeader, HEADER_BUFFER_SIZE);
  // send a standard http response header    

  strHeader << F("HTTP/1.1 200 OK \r ") 
    << F("Content-Type: text/html;charset=UTF-8\r ") 
    << F(" Content-Length: ") << (int) strlen(pBody) << " \r"
    << F("Connection: close \r\n\r\n");

  return strHeader.length() ;
}


// Arduino Setup routine.

void setup() {
  Serial.begin(9600);
  
  Serial << F("Initializing WebServerTime Demo ... RAM=") << freeMemory() << endl
    << F("Arduino Rx Pin (connect to WiFly Tx):") << ARDUINO_RX_PIN << endl << F("Arduino Tx Pin (connect to WiFly Rx):") << ARDUINO_TX_PIN  << endl;

  // Start wifi Ethernet
  Connect();
 
  Serial << F("WFSEthernet started ... beginning myServer.") << endl;
  // Startup server
  Serial << F("Starting WebServerTime - Please wait... ") <<  wifi.getLibraryVersion(bufTemp, TEMP_BUFFER_SIZE) << "  RAM:" << freeMemory() << endl;
  myServer.begin();
  myServer.setProfile(ES_HTTP_SERVER);
  int bOk_ntp = WFSEthernet.setNTPServer( ntp_server , TIMEZONE_GMT_OFFSET );
  Serial << F("Time configuration ") << ( bOk_ntp ? F("ok") : F("failed") ) << F(" ... Datetime:") << year() << "-" << month() << "-" << day() << " " << hour() << ":" << minute() << ":" << second()<< endl;

  // get MAC
  Serial << F("MAC: ") << wifi.getMAC(bufRequest, REQUEST_BUFFER_SIZE) << endl;   
  Serial << F("IP Settings:") << endl  
    << F("From wifi:") << endl
    << F("IP: ") << wifi.getIP(bufRequest, REQUEST_BUFFER_SIZE) << endl
    << F("Netmask: ") << wifi.getNetMask(bufRequest, REQUEST_BUFFER_SIZE) << endl
    << F("Gateway: ") << wifi.getGateway(bufRequest, REQUEST_BUFFER_SIZE) << endl
    << F("DNS: ") << wifi.getDNS(bufRequest, REQUEST_BUFFER_SIZE) << endl
    << F("RSSI: ") << wifi.getRSSI(bufRequest, REQUEST_BUFFER_SIZE) << endl
    << F("battery: ") <<  wifi.getBattery(bufRequest, REQUEST_BUFFER_SIZE) << endl;
  memset (bufRequest,'\0',REQUEST_BUFFER_SIZE);

  Serial << F("From Ethernet:") << endl  
    << F("IP: ") << WFSEthernet.localIP() << endl
    << F("Netmask: ") << WFSEthernet.subnetMask() << endl
    << F("Gateway: ") << WFSEthernet.gatewayIP() << endl
    << F("DNS: ") << WFSEthernet.dnsServerIP() << endl;


  // close any open connections
  wifi.closeConnection();
  Serial << F("After Setup RAM:") << freeMemory() << endl ;
  Serial << F("Aim your browser at ( /status and /auto optional): ") << WFSEthernet.localIP() << endl;
  
  //  wifi.setDebugChannel( (Print*) &Serial);
}


void loop() {

  memset (bufRequest,'\0',REQUEST_BUFFER_SIZE);
  Serial << F("Loop RAM:") << freeMemory() << endl ;

  //  wifi.setDebugChannel( (Print*) &Serial);
  //  wifi.clearDebugChannel();
  
  wifi.getDeviceStatus();
  // if not connected restart link
  while (! wifi.isAssociated() ) {
    Serial << F("Not associated - Rejoining") << endl;
    Connect();
  } // while not connected
//    wifi.clearDebugChannel();

  
  Serial << F("Ready for HTTP Requests. ") << freeMemory() << endl ;
    
  WFSEthernetClient myClient = myServer.available();
  // iRequest = wifi.serveConnection();
  if (  myClient  ) {
    memset (bufRequest,'\0',REQUEST_BUFFER_SIZE);
    //wifi.bWiFlyInCommandMode = false;
    Serial << F("Connected ") << endl;
    // analyse request for GET;
    if ( myClient.findUntil("GET "," HTTP/1.1") ) {
      
      // get interesting parts of HTTP request into a buffer for later use
      myClient.readBytesUntil(' ', bufRequest,REQUEST_BUFFER_SIZE);

      // wifi.ScanForPattern( bufRequest, REQUEST_BUFFER_SIZE, " HTTP/1.1", 1000 );
      Serial << F("GET request,  bytes: ") << strlen(bufRequest) << endl << bufRequest << endl;
  
      char* pHeader = (char*) malloc(HEADER_BUFFER_SIZE);
      char* pBody = (char*) malloc(BODY_BUFFER_SIZE);
  
      // Form header and body of response
      MakeReponseBody(pBody,  bufRequest, REQUEST_BUFFER_SIZE);
      MakeResponseHeader( pHeader,  pBody);
  
      // send reply.  note the \t, whch is the end-of-packet signal for wifly, as set in SET MATCH 0x09 above.
      
      myClient << pHeader << pBody <<  "\t";
  
      myClient.flush();      
  
      Serial << endl << F("Replying with:") << freeMemory() << endl << pHeader << pBody << endl;
      
      free(pHeader);
      free(pBody);
      pHeader = NULL;
      pBody = NULL;
    } else {
      // no GET found
      // read until buffer is dry
      while ( myClient.available() ) {      
        Serial << (char) myClient.read();
      }
  
    } // if GET found
  
    
    //wifi.closeConnection();
    myClient.stop();
    

  } else {
    // Timout - no clients.  Do stuff, then go back to waiting.
    // A larger timeout value would be good as well.
    Serial << F("Timed out waiting for a client...")
        << year() << "-" << month() << "-" << day() << " " << hour() << ":" << minute() << ":" << second() << endl;
  }  // if Open connection found.
}



