/*
 * WFSEthernetClient.cpp
 * Arduino Ethernet Client class for wifi devices
 * Based on Arduino 1.0 EthernetClient class
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
#include "WFSsocket.h"
#include <WiFlySerial.h>

extern "C" {
  #include "string.h"
}

#include "Arduino.h"

#include "WFSEthernet.h"
#include "WFSEthernetClient.h"
#include "WFSEthernetServer.h"


uint16_t WFSEthernetClient::_srcport = 1024;

WFSEthernetClient::WFSEthernetClient() : _sock(MAX_SOCK_NUM) {
}

WFSEthernetClient::WFSEthernetClient(uint8_t sock) : _sock(sock) {
}

int WFSEthernetClient::connect(const char* host, uint16_t port) {
  return connect( host, port);
}

int WFSEthernetClient::connect(IPAddress ip, uint16_t port) {
  if (_sock != MAX_SOCK_NUM)
    return 0;

  return connect(ip, port);

}

size_t WFSEthernetClient::write(uint8_t b) {
  return write(&b, 1);
}

size_t WFSEthernetClient::write(const uint8_t *buf, size_t size) {
  if (_sock == MAX_SOCK_NUM) {
    setWriteError();
    return 0;
  }
  if (!send(_sock, buf, size)) {
    setWriteError();
    return 0;
  }
  return size;
}

int WFSEthernetClient::available() {
  if (_sock != MAX_SOCK_NUM) {
    
    // WiFly supports single connection at present so only one socket.
    // TODO: support multiple sockets, or at least appear to do so.
//    return wifi.available(_sock);
    return wifi.available();
  }
  return 0;
}

int WFSEthernetClient::read() {
  uint8_t b;
  if ( recv(_sock, &b, 1) > 0 )
  {
    // recv worked
    return b;
  }
  else
  {
    // No data available
    return -1;
  }
}

int WFSEthernetClient::read(uint8_t *buf, size_t size) {
  return recv(_sock, buf, size);
}

int WFSEthernetClient::peek() {
  uint8_t b;
  if (!available())
    return -1;
  ::peek(_sock, &b);
  return b;
}


// flush
// permits outgoing characters to complete sending, per Arduino 1.0 Serial implementation.
// Parameters: None
// 

void WFSEthernetClient::flush() {
  wifi.flush();

// drain incoming characters  
//  while (available())
//    read();
}

void WFSEthernetClient::stop() {
  if (_sock == MAX_SOCK_NUM)
    return;

  // attempt to close the connection gracefully (send a FIN to other side)
  disconnect(_sock);
  WFSEthernetClass::_server_port[_sock] = 0;
  _sock = MAX_SOCK_NUM;
}

uint8_t WFSEthernetClient::connected() {
  if (_sock == MAX_SOCK_NUM) return 0;
  
//  uint8_t s = status();
//  return !(s == SnSR::LISTEN || s == SnSR::CLOSED || s == SnSR::FIN_WAIT ||
//    (s == SnSR::CLOSE_WAIT && !available()));
  // 
  return wifi.isConnectionOpen();
}


// status
// Provides Client status
//
// Parameters: none
// Returns:
// one of:
// 0      CLOSED
// 0x17      Connected
//
uint8_t WFSEthernetClient::status() {
  if (_sock == MAX_SOCK_NUM) return SnSR::CLOSED;
  return (wifi.isConnectionOpen() ? SnSR::ESTABLISHED : SnSR::CLOSED );
}

// the next function allows us to use the client returned by
// EthernetServer::available() as the condition in an if-statement.

WFSEthernetClient::operator bool() {
//  return _sock != MAX_SOCK_NUM;
   return wifi.isConnectionOpen();
}


uint8_t WFSEthernetClient::devicestatus() {
  return wifi.getDeviceStatus();
}

