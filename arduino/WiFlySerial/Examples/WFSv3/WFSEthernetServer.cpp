/*
 * WFSEthernetServer.cpp
 * Arduino Ethernet Server class for wifi devices
 * Based on Arduino 1.0 EthernetServer class
 * 
 * Credits:
 * First to the Arduino Ethernet team for their model upon which this is based.
 * Modifications: 
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

#include "WiFlySerial.h"
#include "WFSsocket.h"
extern "C" {
#include "string.h"
}

#include "WFSEthernet.h"
#include "WFSEthernetClient.h"
#include "WFSEthernetServer.h"

WFSEthernetServer::WFSEthernetServer(uint16_t port)
{
  _port = port;
}


// begin
// Sets basics for device configuration
void WFSEthernetServer::begin()
{
   char bufRequest[COMMAND_BUFFER_SIZE];
   
   wifi.SendCommand( (char*) F("set u m 0x1") ,WiFlyFixedPrompts[WIFLY_MSG_PROMPT], bufRequest, COMMAND_BUFFER_SIZE);
   wifi.SendCommand( (char*) F("set comm idle 30") ,WiFlyFixedPrompts[WIFLY_MSG_PROMPT], bufRequest, COMMAND_BUFFER_SIZE);
   wifi.SendCommand( (char*) F("set comm time 1000") ,WiFlyFixedPrompts[WIFLY_MSG_PROMPT], bufRequest, COMMAND_BUFFER_SIZE);
   wifi.SendCommand( (char*) F("set comm size 255") ,WiFlyFixedPrompts[WIFLY_MSG_PROMPT], bufRequest, COMMAND_BUFFER_SIZE);
}
// setProfile
// Configures server to respond in manner specified
// Assumes wifi device is semi-automnous.
//
// Parameters:
// serverProfile      bit-mapped flag of profile options

long WFSEthernetServer::setProfile(long serverProfile) {
  
   char bufRequest[SMALL_COMMAND_BUFFER_SIZE];
  
    // Defaults as according to device.
    // Settings may be residual values from prior saved sessions.
    if ( serverProfile & ES_DEVICE_DEFAULT) {
      // Set to default somehow (factory reset?)
      
    }
            
    if ( serverProfile & ES_HTTP_SERVER ) {
      // No *HELLO* on connection - confuses browsers
      wifi.SendCommand( (char*) F("set comm remote 0") ,WiFlyFixedPrompts[WIFLY_MSG_PROMPT], bufRequest, SMALL_COMMAND_BUFFER_SIZE);
      
      // Send packet on each tab character issued
      wifi.SendCommand( (char*) F("set comm match 0x9") ,WiFlyFixedPrompts[WIFLY_MSG_PROMPT], bufRequest, SMALL_COMMAND_BUFFER_SIZE);
      
      
    }
    
    // Telnet response setup
    if ( serverProfile & ES_TELNET_SERVER)  {
      
    }
    
    // UDP not implemented yet on CS libraries
    if ( serverProfile & ES_UDP_SERVER ) {
      
      
    }

    return serverProfile;    
    
}

void WFSEthernetServer::accept()
{
  int listening = 0;

  wifi.serveConnection();

}


// returns a WFSEthernetClient using an available socket.
// WiFly has one socket so ... use it.
WFSEthernetClient WFSEthernetServer::available()
{
  accept();
  WFSEthernetClient client(_port);
  return client;
  
//  return WFSEthernetClient(MAX_SOCK_NUM);
}

size_t WFSEthernetServer::write(uint8_t b) 
{
  return write(&b, 1);
}

size_t WFSEthernetServer::write(const uint8_t *buffer, size_t size) 
{
  
  return wifi.write(buffer, size);
}
