/*
 * Web Server example for the WiFly showing the current time and some status items.
 *
 * Download Mikal Hart's Streaming and PString libraries from http://arduiniana.org
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


// Connect the WiFly TX pin to the Arduino RX pin  (Transmit from WiFly-> Receive into Arduino)
// Connect the WiFly RX pin to the Arduino TX pin  (Transmit from Arduino-> Receive into WiFly)
// 
// Connect the WiFly GND pin to an Arduino GND pin
// Finally, connect the WiFly BATT pin to the 3.3V pin (NOT the 5v pin)

#define ARDUINO_RX_PIN  2
#define ARDUINO_TX_PIN  3


prog_char s_WT_SETUP_000[] PROGMEM = "Arduino Rx Pin (connect to WiFly Tx):";  
prog_char s_WT_SETUP_001[] PROGMEM = "Arduino Tx Pin (connect to WiFly Rx):";  
prog_char s_WT_SETUP_01[] PROGMEM = "set u m 0x1";
prog_char s_WT_SETUP_02[] PROGMEM = "set comm remote 0";
prog_char s_WT_SETUP_03[] PROGMEM = "set comm idle 30";
prog_char s_WT_SETUP_04[] PROGMEM = "set comm time 1000";
prog_char s_WT_SETUP_05[] PROGMEM = "set comm size 128";
prog_char s_WT_SETUP_06[] PROGMEM = "set comm match 0x9";
prog_char s_WT_SETUP_07[] PROGMEM = "time";
prog_char s_WT_STATUS_SENSORS[] PROGMEM = "show q 0x177 ";
prog_char s_WT_STATUS_TEMP[] PROGMEM = "show q t ";
prog_char s_WT_STATUS_RSSI[] PROGMEM = "show rssi ";
prog_char s_WT_STATUS_BATT[] PROGMEM = "show battery ";
prog_char s_WT_MSG_JOIN[] PROGMEM = "Credentials Set, Joining ";
prog_char s_WT_MSG_LEAVE[] PROGMEM = "Leaving current wifi ... ";
prog_char s_WT_MSG_START_WEBTIME[] PROGMEM = "Starting WebTime - Please wait. ";
prog_char s_WT_MSG_RAM[] PROGMEM = "RAM :";
prog_char s_WT_MSG_START_WIFLY[] PROGMEM = "Started WiFly, :";
prog_char s_WT_MSG_WIFI[] PROGMEM = "Initial WiFi Settings :";
prog_char s_WT_MSG_APP_SETTINGS[] PROGMEM = "Configure WebTime Settings...";
prog_char s_WT_MSG_AIM_BROWSER[] PROGMEM = "Aim your browser at ( /status and /auto optional): ";
prog_char s_WT_MSG_HTTP_READY[] PROGMEM = "Ready for HTTP Requests. ";
prog_char s_WT_MSG_TIMEOUT[] PROGMEM = "Timed out waiting for next customer. ";
prog_char s_WT_HTML_HEAD_01[] PROGMEM = "HTTP/1.1 200 OK \r ";
prog_char s_WT_HTML_HEAD_02[] PROGMEM = "Content-Type: text/html;charset=UTF-8\r ";
prog_char s_WT_HTML_HEAD_03[] PROGMEM = " Content-Length: ";
prog_char s_WT_HTML_HEAD_04[] PROGMEM = "Connection: close \r\n\r\n ";
prog_char s_WT_HTML_HEAD_05[] PROGMEM = "<META HTTP-EQUIV=\"REFRESH\" CONTENT=\"30\">";

#define IDX_WT_SETUP_000 0
#define IDX_WT_SETUP_001 1
#define IDX_WT_SETUP_01 IDX_WT_SETUP_001 +1
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
#define IDX_WT_MSG_LEAVE         IDX_WT_MSG_JOIN +1
#define IDX_WT_MSG_START_WEBTIME IDX_WT_MSG_JOIN +2
#define IDX_WT_MSG_RAM           IDX_WT_MSG_JOIN +3
#define IDX_WT_MSG_START_WIFLY   IDX_WT_MSG_JOIN +4
#define IDX_WT_MSG_WIFI          IDX_WT_MSG_JOIN +5
#define IDX_WT_MSG_AIM_BROWSER   IDX_WT_MSG_JOIN +6
#define IDX_WT_MSG_HTTP_READY    IDX_WT_MSG_JOIN +7
#define IDX_WT_MSG_TIMEOUT       IDX_WT_MSG_JOIN +8
#define IDX_WT_MSG_APP_SETTINGS  IDX_WT_MSG_JOIN +9

#define IDX_WT_HTML_HEAD_01      IDX_WT_MSG_APP_SETTINGS + 1
#define IDX_WT_HTML_HEAD_02      IDX_WT_HTML_HEAD_01 + 1
#define IDX_WT_HTML_HEAD_03      IDX_WT_HTML_HEAD_01 + 2
#define IDX_WT_HTML_HEAD_04      IDX_WT_HTML_HEAD_01 + 3
#define IDX_WT_HTML_HEAD_05      IDX_WT_HTML_HEAD_01 + 4



PROGMEM const char *WT_string_table[] = 	   
{   
  s_WT_SETUP_000,
  s_WT_SETUP_001,
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
  s_WT_MSG_LEAVE,
  s_WT_MSG_START_WEBTIME,
  s_WT_MSG_RAM,
  s_WT_MSG_START_WIFLY,
  s_WT_MSG_WIFI,
  s_WT_MSG_AIM_BROWSER,
  s_WT_MSG_HTTP_READY,
  s_WT_MSG_TIMEOUT,
  s_WT_MSG_APP_SETTINGS,
  s_WT_HTML_HEAD_01,
  s_WT_HTML_HEAD_02,
  s_WT_HTML_HEAD_03,
  s_WT_HTML_HEAD_04,
  s_WT_HTML_HEAD_05
};

#define REQUEST_BUFFER_SIZE 80
#define HEADER_BUFFER_SIZE 120 
#define BODY_BUFFER_SIZE 180
#define TEMP_BUFFER_SIZE 80

char bufRequest[REQUEST_BUFFER_SIZE];
char bufTemp[TEMP_BUFFER_SIZE];
char chMisc;
int iRequest = 0;
int iTrack = 0;

WiFlySerial wifi(ARDUINO_RX_PIN ,ARDUINO_TX_PIN);

// WFSServer server(80);

// Function for setSyncProvider
time_t GetSyncTime() {
  time_t tCurrent = (time_t) wifi.getTime();
  wifi.exitCommandMode();
  return tCurrent;
}

// GetBuffer_P
// Returns pointer to a supplied Buffer, from PROGMEM based on StringIndex provided.
// based on example from http://arduino.cc/en/Reference/PROGMEM
// Reduced need with Arduino's 1.0 F().  

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
  Serial << GetBuffer_P(IDX_WT_MSG_LEAVE,bufTemp,TEMP_BUFFER_SIZE) << wifi.leave() << endl;
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

// Make Response Body
// Based on GET request string, generate a response.
int MakeReponseBody( char* pBody,  char* pRequest, const int sizeRequest ) {

  PString strBody( pBody, BODY_BUFFER_SIZE);

  if ( strstr(pRequest, "/status" ) ) {
    strBody << F("<html>WebTime Status:</br>Free RAM:") << freeMemory() << "</br>"
      << F("DateTime:") << year() << "-" << month() << "-" << day() << " " << _DEC(hour()) << ":" << minute() << ":" << second() << "</br>";
    strBody << F("Battery: ") << wifi.getBattery(bufTemp,TEMP_BUFFER_SIZE) << "</br>";    
    strBody << F("RSSI: ") << wifi.getRSSI(bufTemp,TEMP_BUFFER_SIZE) << "</br>";
    strBody << F("</html>\r\n\r\n");

    // need to exit command mode to be able to send data
    wifi.exitCommandMode();

  } else {
    if ( strstr(pRequest,"/auto") ) {
      strBody << GetBuffer_P(IDX_WT_HTML_HEAD_05,bufTemp,TEMP_BUFFER_SIZE);
    }
    strBody << F("<html>Current request:") << pRequest << F("</br>Millis:") << millis() << F(" Micros:") << micros()
      << F("</br>DateTime:") << year() << "-" << month() << "-" << day() << " " << hour() << ":" << minute() << ":" << second()
        << F("</html>");
    // No calls back to WiFly command mode; hence no need to exit Command mode that wasn't entered.
  }
  return strBody.length();
}

// MakeResponseHeader
// Form a HTML header, including length of body.
int MakeResponseHeader( char* pHeader, char* pBody ) {

  PString strHeader( pHeader, HEADER_BUFFER_SIZE);
  // send a standard http response header    

  strHeader << GetBuffer_P(IDX_WT_HTML_HEAD_01,bufTemp,TEMP_BUFFER_SIZE)
    << GetBuffer_P(IDX_WT_HTML_HEAD_02,bufTemp,TEMP_BUFFER_SIZE)
      << GetBuffer_P(IDX_WT_HTML_HEAD_03,bufTemp,TEMP_BUFFER_SIZE) << (int) strlen(pBody) << " \r"
        << GetBuffer_P(IDX_WT_HTML_HEAD_04,bufTemp,TEMP_BUFFER_SIZE);

  return strHeader.length();
}


// Arduino Setup routine.

void setup() {
  Serial.begin(9600);
 
  Serial << GetBuffer_P(IDX_WT_MSG_START_WEBTIME,bufTemp,TEMP_BUFFER_SIZE) << endl << GetBuffer_P(IDX_WT_MSG_RAM,bufTemp,TEMP_BUFFER_SIZE) << freeMemory() << endl
    << GetBuffer_P(IDX_WT_SETUP_000,bufTemp,TEMP_BUFFER_SIZE) << ARDUINO_RX_PIN << endl << GetBuffer_P(IDX_WT_SETUP_001,bufTemp,TEMP_BUFFER_SIZE) << ARDUINO_TX_PIN << endl;

  wifi.begin();
  Serial << GetBuffer_P(IDX_WT_MSG_START_WIFLY,bufTemp,TEMP_BUFFER_SIZE) <<  wifi.getLibraryVersion(bufTemp, TEMP_BUFFER_SIZE) << "  RAM:" << freeMemory() << endl;

  // get MAC
  Serial << F("MAC: ") << wifi.getMAC(bufRequest, REQUEST_BUFFER_SIZE) << endl;
  
  // Change these to fit your local situation
  
  wifi.setAuthMode( WIFLY_AUTH_WPA2_PSK);
  wifi.setJoinMode(  WIFLY_JOIN_AUTO );
  wifi.setDHCPMode( WIFLY_DHCP_ON );

  Reconnect();
  
  // Set timezone adjustment: PST is -8h.  Adjust to your local timezone.
  adjustTime( (long) (-8 * 60 * 60) );
  Serial << F("DateTime:") << year() << "-" << month() << "-" << day() << " " << hour() << ":" << minute() << ":" << second() << F(" PST") << endl;

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
  Serial << GetBuffer_P(IDX_WT_MSG_AIM_BROWSER,bufTemp,TEMP_BUFFER_SIZE) << wifi.getIP(bufRequest, REQUEST_BUFFER_SIZE) << endl;
  
  //  wifi.setDebugChannel( (Print*) &Serial);

  // server.begin();
  

}


void loop() {

  memset (bufRequest,'\0',REQUEST_BUFFER_SIZE);
  Serial << F("Loop RAM:") << freeMemory() << endl ;

  // wifi.setDebugChannel( (Print*) &Serial);
  //  wifi.clearDebugChannel();
  
  wifi.getDeviceStatus();
  // if not connected restart link
  while (! wifi.isAssociated() ) {
    Reconnect();
  } // while not connected
//    wifi.clearDebugChannel();

//  Serial << "Clear leftovers" << endl;
  // read past the browser's header details
  while ((chMisc = wifi.read()) > -1) {
    Serial << chMisc;
  }
  
  Serial << GetBuffer_P(IDX_WT_MSG_HTTP_READY,bufTemp,TEMP_BUFFER_SIZE) << freeMemory() << endl ;
    
  iRequest = wifi.serveConnection();
  if (  iRequest  ) {
    memset (bufRequest,'\0',REQUEST_BUFFER_SIZE);
    //wifi.bWiFlyInCommandMode = false;
    Serial << F("Connected ") << endl;
    // analyse request for GET;
    wifi.ScanForPattern( bufRequest, REQUEST_BUFFER_SIZE, " HTTP/1.1", 1000 );
    Serial << F("GET request,  bytes: ") << strlen(bufRequest) << endl << bufRequest << endl;

    // read past the browser's header details
    while ((chMisc = wifi.read()) > -1)
      Serial << chMisc;

    char* pHeader = (char*) malloc(HEADER_BUFFER_SIZE);
    char* pBody = (char*) malloc(BODY_BUFFER_SIZE);

    // Form header and body of response
    MakeReponseBody(pBody,  bufRequest, REQUEST_BUFFER_SIZE);
    MakeResponseHeader( pHeader,  pBody);

    // send reply.  note the \t, whch is the end-of-packet signal for wifly, as set in SET MATCH 0x09 above.
    
    wifi << pHeader << pBody << "\r\n\r\n" << "\t";

    // give the web browser time to receive the data
    // NewSoftSerial will trickle data up to the WiFly in the background after print stmt completed.
    // settings are conservative ... more rapid responses possible by calcuguessing message length / (baudrate / bits/byte) x SomeFudgeFactor x milliseconds
    // baud bits/byte is >8 and < 10 by the time ecc figured in.
    // and subtract estimated time to send 'close' command (1000 ms?)
    // 
    int iDelay = ( ( (strlen(pHeader) + strlen(pBody))/ (9600/10) ) * 1000 ) - 1000;
    
    delay( ( iDelay < 0 ? 0 : iDelay) )  ;

    Serial << endl << F("Header:") << endl << pHeader << F("Body:") << pBody << endl;
    
    // close connection
    wifi.closeConnection();
    
    // release memory back to pool - even if freeMemory() doesn't recognize it.
    // Note: release AFTER connection closed and writing completed.

    free(pHeader);
    free(pBody);
    pHeader = NULL;
    pBody = NULL;

  } else {
    // Timout - no clients.  Do stuff, then go back to waiting.
    // A larger timeout value would be good as well.
    Serial << GetBuffer_P(IDX_WT_MSG_TIMEOUT,bufTemp,TEMP_BUFFER_SIZE)
        << year() << "-" << month() << "-" << day() << " " << hour() << ":" << minute() << ":" << second() << endl;
  }  // if Open connection found.
}



