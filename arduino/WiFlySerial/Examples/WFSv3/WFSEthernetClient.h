/*
 * WFSEthernetClient.h
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
#ifndef WFSethernetclient_h
#define WFSethernetclient_h
#include "Arduino.h"	
#include "Print.h"
#include "Client.h"
#include "WFSIPAddress.h"

class WFSEthernetClient : public Client {

public:
  WFSEthernetClient();
  WFSEthernetClient(uint8_t sock);

  // Devices often appreciate a chance to initialize before commencing operations
  boolean initDevice();
  
  // set up wifi association settings
  boolean configure(uint8_t AuthMode, uint8_t JoinMode, uint8_t DCHPMode);
  boolean credentials( char* pSSID, char* pPassphrase);
  uint8_t devicestatus();

  uint8_t status();
  virtual int connect(IPAddress ip, uint16_t port);
  virtual int connect(const char *host, uint16_t port);
  virtual size_t write(uint8_t);
  virtual size_t write(const uint8_t *buf, size_t size);
  virtual int available();
  virtual int read();
  virtual int read(uint8_t *buf, size_t size);
  virtual int peek();
  virtual void flush();
  virtual void stop();
  virtual uint8_t connected();
  virtual operator bool();

  friend class WFSEthernetServer;
  
  using Print::write;

private:
  static uint16_t _srcport;
  uint8_t _sock;
};

#endif
