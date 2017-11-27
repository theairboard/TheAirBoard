/*
 * WFSEthernetServer.h
 * Arduino Ethernet Server class for wifi devices
 * Based on Arduino 1.0 EthernetServer class
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
#ifndef WFSethernetserver_h
#define WFSethernetserver_h

#include "Server.h"
#include "Arduino.h"

#define ES_DEVICE_DEFAULT    0x00
#define ES_HTTP_SERVER       0x01
#define ES_TELNET_SERVER     0x02
#define ES_UDP_SERVER        0x04


class WFSEthernetClient;

class WFSEthernetServer : 
public Server {
private:
  uint16_t _port;
  void accept();
public:
  long setProfile(long ServerProfile = ES_HTTP_SERVER);
  WFSEthernetServer(uint16_t);
  WFSEthernetClient available();
  virtual void begin();
  virtual size_t write(uint8_t);
  virtual size_t write(const uint8_t *buf, size_t size);
  using Print::write;
};

#endif
