
/*
 * WFSEthernet.cpp
 * Arduino Ethernet class for wifi devices
 * Based on Arduino 1.0 Ethernet class
 * 
 * Credits:
 * First to the Arduino Ethernet team for their model upon which this is based.
 * Modifications are
 * Copyright GPL 2.1 Tom Waldock 2012
 Version 1.07
 
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
 
 */

#include <WiFlySerial.h>
#include "WFSEthernet.h"
#include <Time.h>
//#include "Dhcp.h"

// XXX: don't make assumptions about the value of MAX_SOCK_NUM.
uint8_t WFSEthernetClass::_state[MAX_SOCK_NUM] = { 
  0, 0, 0, 0 };
uint16_t WFSEthernetClass::_server_port[MAX_SOCK_NUM] = { 
  0, 0, 0, 0 };

// Function for setSyncProvider
time_t WFSGetSyncTime() {
  
  // wifly returns UTC time.
  time_t tCurrent = (time_t) wifi.getTime();
  // DebugPrint( "RESYNC TIME FROM WIFLY *****" );
  wifi.exitCommandMode();
  return tCurrent;
}



//init
//Allows device to prepare for operations.
//
// Parameters: none
// Returns true on success, false on failure.
boolean WFSEthernetClass::initDevice() {
  
  return wifi.begin();
}

//configure
// Sets up modes of operation.
//
//Parameters
// AuthMode  - authentication modes
// JoinMode  - Means of joining a network
// DHCPMode  - Options for DHCP
//
// Returns true on success, false on failure.
boolean WFSEthernetClass::configure(uint8_t  AuthMode, uint8_t JoinMode, uint8_t  DHCPMode) {
  
   return wifi.setAuthMode(AuthMode) | wifi.setJoinMode(JoinMode) | wifi.setDHCPMode(DHCPMode) ;
  
}

// credentials
// Sets wifi name and passphrase/key
//
// Parameters
// pSSID      SSID to join
// pPassphrase WPA/2 Passphrase or WEP key
//
// Returns true on success, false on failure.
boolean WFSEthernetClass::credentials( char* pSSID, char* pPassphrase) {
  
  boolean bReturn =  ( wifi.setPassphrase(pPassphrase) & wifi.setSSID(pSSID) );
  return bReturn;
}

// begin()s
// call init(), then configure(), then credentials() 
/// BEFORE calling begin().
// fewer-parameter begins cascade to more-parameter methods.
// work really happens at last method.

int WFSEthernetClass::begin()
{
  // Initialise the basic info
  WFSIPAddress ip_addr(0,0,0,0);
  
  return begin(ip_addr);
  
}

int WFSEthernetClass::begin( WFSIPAddress local_ip)
{
  // Assume the DNS server will be the machine on the same network as the local IP
  // but with last octet being '1'
  WFSIPAddress dns_server = local_ip;
  dns_server[3] = 1;
  return begin( local_ip, dns_server);
}

int WFSEthernetClass::begin( WFSIPAddress local_ip, WFSIPAddress dns_server)
{
  // Assume the gateway will be the machine on the same network as the local IP
  // but with last octet being '1'
  WFSIPAddress gateway = local_ip;
  gateway[3] = 1;
  return begin( local_ip, dns_server, gateway);
}

int WFSEthernetClass::begin( WFSIPAddress local_ip, WFSIPAddress dns_server, WFSIPAddress gateway)
{
  WFSIPAddress subnet(255, 255, 255, 0);
  return begin( local_ip, dns_server, gateway, subnet);
}

int WFSEthernetClass::begin( WFSIPAddress local_ip, WFSIPAddress dns_server, WFSIPAddress gateway, WFSIPAddress subnet)
{
  wifi.begin();
  
  // Arduino-WiFly communication-stability settings
  char bufRequest[COMMAND_BUFFER_SIZE];
  
  // Set echo off
  wifi.SendCommand("set u m 0x1",">",bufRequest, COMMAND_BUFFER_SIZE);
  
  // Auto-disconnect after 30 seconds of idle connection.
  wifi.SendCommand( "set comm idle 30",">"  ,bufRequest, COMMAND_BUFFER_SIZE)  ;
    
  // Auto-sends TCP packet if no additional bytes provided from Arduino (ms).
  wifi.SendCommand( "set comm time 1000",">",bufRequest, COMMAND_BUFFER_SIZE);
    
  // Auto-sends TCP packet once this number of bytes provided from Arduino.
  wifi.SendCommand("set comm size 256",">",bufRequest, COMMAND_BUFFER_SIZE);
  
  // if already joined then leave first
  wifi.getDeviceStatus();
  if (wifi.isAssociated() ) {
    Serial << "leaving" << endl;
    wifi.leave();
  }
  
  // Try to join network based on provided (?) settings.
  
  int iRetry = 0;
  boolean bJoined = false;
  while ( (!bJoined )  &&  (iRetry < COMMAND_RETRY_ATTEMPTS)  ) {
    Serial << F("Trying to join...") << ( ( bJoined = wifi.join() ) ? F(" ok ") : F(" failed ") ) ;
    iRetry++;
    
  }
  
  if ( bJoined ) {
    
    Serial << "Joined - setting addresses.";
    char bufAddr[IP_ADDR_WIDTH];
    
    if (local_ip != WFSIPAddress(0,0,0,0) ){
      Serial << "Set local IP";
      
      wifi.setIP(IP_ArrayToBuffer( local_ip._address, bufAddr, IP_ADDR_WIDTH));
    }
    if (subnet != WFSIPAddress(0,0,0,0) ){
      wifi.setNetMask(IP_ArrayToBuffer(subnet._address, bufAddr, IP_ADDR_WIDTH));
    }
    if (gateway != WFSIPAddress(0,0,0,1) ){
      wifi.setGateway(IP_ArrayToBuffer(gateway._address, bufAddr, IP_ADDR_WIDTH));
    }
    if (dns_server != WFSIPAddress(0,0,0,1) ){
      wifi.setDNS(IP_ArrayToBuffer(dns_server._address, bufAddr, IP_ADDR_WIDTH));
    }

    return true;

  } else {
 //   DebugPrint("Failed to join...");
    return false;
  }
}

boolean WFSEthernetClass::setNTPServer( char* pNTPServer, float fTimeZoneOffsetHrs ) {
  
    char bufRequest[ SMALL_COMMAND_BUFFER_SIZE ];
    
    // Set NTP server, update frequency, 
    wifi.setNTP(pNTPServer); 
    wifi.SendCommand((char*) F("set time enable 60 "), WiFlyFixedPrompts[WIFLY_MSG_PROMPT] ,bufRequest, SMALL_COMMAND_BUFFER_SIZE);
    
    // set WiFly's time sync frequency with NTP server.
    wifi.setNTP_Update_Frequency(WIFLY_NTP_SYNC_INTERVAL);
    // Forces update of time.
    wifi.SendCommand((char*) F("time"), WiFlyFixedPrompts[WIFLY_MSG_PROMPT] ,bufRequest, SMALL_COMMAND_BUFFER_SIZE);
    
    wifi.setNTP_UTC_Offset ( fTimeZoneOffsetHrs );
    
    setTime( wifi.getTime() );
   // Set timezone adjustment: PST is -8h.  Adjust to your local timezone.
    
    delay(1000);
    setSyncProvider( WFSGetSyncTime );
    
    // Set Arduino's time sync frequency with WiFly
    setSyncInterval( WIFLY_TIME_SYNC_INTERVAL );
    return true;
  
}



WFSIPAddress WFSEthernetClass::localIP()
{
  WFSIPAddress ret;
  char bufAddr[COMMAND_BUFFER_SIZE];
  
  wifi.getIP(   bufAddr, COMMAND_BUFFER_SIZE );
  
  BufferToIP_Array(bufAddr,  ret._address);
  
  return ret;
}

WFSIPAddress WFSEthernetClass::subnetMask()
{
  WFSIPAddress ret;
  char bufAddr[SMALL_COMMAND_BUFFER_SIZE];
  
  wifi.getNetMask(   bufAddr, SMALL_COMMAND_BUFFER_SIZE );
  
  BufferToIP_Array(bufAddr,  ret._address);
  
  return ret;
}

WFSIPAddress WFSEthernetClass::gatewayIP()
{
  WFSIPAddress ret;
  char bufAddr[SMALL_COMMAND_BUFFER_SIZE];
  
  wifi.getGateway(   bufAddr, SMALL_COMMAND_BUFFER_SIZE );
  
  BufferToIP_Array(bufAddr,  ret._address);
  
  return ret;
}

WFSIPAddress WFSEthernetClass::dnsServerIP()
{
  WFSIPAddress ret;
  char bufAddr[SMALL_COMMAND_BUFFER_SIZE];
  
  wifi.getDNS(   bufAddr, SMALL_COMMAND_BUFFER_SIZE );
  
  BufferToIP_Array(bufAddr,  ret._address);
  
  return ret;
}

WFSEthernetClass WFSEthernet;
 WiFlySerial wifi(ARDUINO_RX_PIN,ARDUINO_TX_PIN);
