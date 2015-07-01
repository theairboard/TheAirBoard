/*
 * WiFly_Device Test Platform
 * A simple tester for communicating with the WiFly GSx RN-131b/g series.
 * LGPL 2.0
 * Tom Waldock, 2011 
 */

#include <Arduino.h>
#include <Streaming.h>
#include <SoftwareSerial.h>
#include "WiFlySerial.h"
#include "MemoryFree.h"
#include "Credentials.h"


// Pins are 3 for INCOMING TO Arduino, 5 for OUTGOING TO Wifly
// Arduino       WiFly
//  2 - receive  TX   (Send from Wifly, Receive to Arduino)
//  3 - send     RX   (Send from Arduino, Receive to WiFly) 
WiFlySerial WiFly(2,3); 

#define REQUEST_BUFFER_SIZE 120
#define HEADER_BUFFER_SIZE 150 
#define BODY_BUFFER_SIZE 100

char bufRequest[REQUEST_BUFFER_SIZE];
char bufHeader[HEADER_BUFFER_SIZE];
char bufBody[BODY_BUFFER_SIZE];


void setup() {
  
  Serial.begin(9600);
  Serial.println(F("Starting WiFly Tester." ) );
  Serial << F("Free memory:") << freeMemory() << endl;  

  WiFly.begin();
  Serial << F("Starting WiFly...") <<  WiFly.getLibraryVersion(bufRequest, REQUEST_BUFFER_SIZE)
   << F(" Free memory:") << freeMemory() << endl;
  
  // get MAC
  Serial << F("MAC: ") << WiFly.getMAC(bufRequest, REQUEST_BUFFER_SIZE) << endl;
  // is connected ?
  
  // WiFly.setDebugChannel( (Print*) &Serial);
  
  WiFly.setAuthMode( WIFLY_AUTH_WPA2_PSK);
  WiFly.setJoinMode(  WIFLY_JOIN_AUTO );
  WiFly.setDHCPMode( WIFLY_DHCP_ON );

   
  // if not connected restart link
  WiFly.getDeviceStatus();
  if (! WiFly.isifUp() ) {
    Serial << "Leave:" <<  ssid << WiFly.leave() << endl;
    // join
    if (WiFly.setSSID(ssid) ) {    
      Serial << "SSID Set :" << ssid << endl;
    }
    if (WiFly.setPassphrase(passphrase)) {
      Serial << "Passphrase Set :" << endl;
    }
    Serial << "Joining... :"<< ssid << endl;

    if ( WiFly.join() ) {
      Serial << F("Joined ") << ssid << F(" successfully.") << endl;
      WiFly.setNTP( ntp_server ); // use your favorite NTP server
    } else {
      Serial << F("Join to ") << ssid << F(" failed.") << endl;
    }
  } // if not connected

  Serial << F("IP: ") << WiFly.getIP(bufRequest, REQUEST_BUFFER_SIZE) << endl <<
    F("Netmask: ") << WiFly.getNetMask(bufRequest, REQUEST_BUFFER_SIZE) << endl <<
    F("Gateway: ") << WiFly.getGateway(bufRequest, REQUEST_BUFFER_SIZE) << endl <<
    F("DNS: ") << WiFly.getDNS(bufRequest, REQUEST_BUFFER_SIZE) << endl 
    << F("WiFly Sensors: ") << bufBody <<  WiFly.SendCommand("show q 0x177 ",">", bufBody, BODY_BUFFER_SIZE) << endl
    << F("WiFly Temp: ") <<  WiFly.SendCommand("show q t ",">", bufBody, BODY_BUFFER_SIZE)
    << F("WiFly battery: ") << WiFly.getBattery(bufBody, BODY_BUFFER_SIZE) << endl;

  WiFly.SendCommand("set comm remote 0",">", bufBody, BODY_BUFFER_SIZE);
  memset (bufBody,'\0',BODY_BUFFER_SIZE);

  WiFly.closeConnection();
  Serial << F("After Setup mem:") << freeMemory() << endl ;
  
  Serial << F("WiFly now listening for commands.  Type 'exit' to listen for wifi traffic.  $$$ (no CR) for command-mode.") << endl;


  // clear out prior requests.
  WiFly.flush();
  while (WiFly.available() )
    WiFly.read();
  
}

char chOut;
void loop() {
  // Terminal routine

  // Always display a response uninterrupted by typing
  // but note that this makes the terminal unresponsive
  // while a response is being received.
  
  while(WiFly.available() > 0) {
    Serial.write(WiFly.read());
  }
  
  if(Serial.available()) { // Outgoing data
    WiFly.write( (chOut = Serial.read()) );
    Serial.write (chOut);
  }

} //loop
