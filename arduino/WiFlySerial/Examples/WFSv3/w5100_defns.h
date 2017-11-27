/*
 * Copyright (c) 2010 by Cristian Maglie <c.maglie@bug.st>
 *
 * Credits:
 * Cristian Maglie as above, original author
 * The Arduino Ethernet team for their model upon which this is based.
 * Modifications from Arduino base ONLY are
 * Copyright GPL 2.1 Tom Waldock 2012
   Version 1.07
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 
 */

#ifndef	W5100_H_INCLUDED
#define	W5100_H_INCLUDED

#include <avr/pgmspace.h>

#define MAX_SOCK_NUM 4

typedef uint8_t SOCKET;

#define IDM_OR  0x8000
#define IDM_AR0 0x8001
#define IDM_AR1 0x8002
#define IDM_DR  0x8003
/*
class MR {
public:
  static const uint8_t RST   = 0x80;
  static const uint8_t PB    = 0x10;
  static const uint8_t PPPOE = 0x08;
  static const uint8_t LB    = 0x04;
  static const uint8_t AI    = 0x02;
  static const uint8_t IND   = 0x01;
};
*/
/*
class IR {
public:
  static const uint8_t CONFLICT = 0x80;
  static const uint8_t UNREACH  = 0x40;
  static const uint8_t PPPoE    = 0x20;
  static const uint8_t SOCK0    = 0x01;
  static const uint8_t SOCK1    = 0x02;
  static const uint8_t SOCK2    = 0x04;
  static const uint8_t SOCK3    = 0x08;
  static inline uint8_t SOCK(SOCKET ch) { return (0x01 << ch); };
};
*/

class SnMR {
public:
  static const uint8_t CLOSE  = 0x00;
  static const uint8_t TCP    = 0x01;
  static const uint8_t UDP    = 0x02;
  static const uint8_t IPRAW  = 0x03;
  static const uint8_t MACRAW = 0x04;
  static const uint8_t PPPOE  = 0x05;
  static const uint8_t ND     = 0x20;
  static const uint8_t MULTI  = 0x80;
};

enum SockCMD {
  Sock_OPEN      = 0x01,
  Sock_LISTEN    = 0x02,
  Sock_CONNECT   = 0x04,
  Sock_DISCON    = 0x08,
  Sock_CLOSE     = 0x10,
  Sock_SEND      = 0x20,
  Sock_SEND_MAC  = 0x21,
  Sock_SEND_KEEP = 0x22,
  Sock_RECV      = 0x40
};

/*class SnCmd {
public:
  static const uint8_t OPEN      = 0x01;
  static const uint8_t LISTEN    = 0x02;
  static const uint8_t CONNECT   = 0x04;
  static const uint8_t DISCON    = 0x08;
  static const uint8_t CLOSE     = 0x10;
  static const uint8_t SEND      = 0x20;
  static const uint8_t SEND_MAC  = 0x21;
  static const uint8_t SEND_KEEP = 0x22;
  static const uint8_t RECV      = 0x40;
};
*/

class SnIR {
public:
  static const uint8_t SEND_OK = 0x10;
  static const uint8_t TIMEOUT = 0x08;
  static const uint8_t RECV    = 0x04;
  static const uint8_t DISCON  = 0x02;
  static const uint8_t CON     = 0x01;
};

class SnSR {
public:
  static const uint8_t CLOSED      = 0x00;
  static const uint8_t INIT        = 0x13;
  static const uint8_t LISTEN      = 0x14;
  static const uint8_t SYNSENT     = 0x15;
  static const uint8_t SYNRECV     = 0x16;
  static const uint8_t ESTABLISHED = 0x17;
  static const uint8_t FIN_WAIT    = 0x18;
  static const uint8_t CLOSING     = 0x1A;
  static const uint8_t TIME_WAIT   = 0x1B;
  static const uint8_t CLOSE_WAIT  = 0x1C;
  static const uint8_t LAST_ACK    = 0x1D;
  static const uint8_t UDP         = 0x22;
  static const uint8_t IPRAW       = 0x32;
  static const uint8_t MACRAW      = 0x42;
  static const uint8_t PPPOE       = 0x5F;
};

class IPPROTO {
public:
  static const uint8_t IP   = 0;
  static const uint8_t ICMP = 1;
  static const uint8_t IGMP = 2;
  static const uint8_t GGP  = 3;
  static const uint8_t TCP  = 6;
  static const uint8_t PUP  = 12;
  static const uint8_t UDP  = 17;
  static const uint8_t IDP  = 22;
  static const uint8_t ND   = 77;
  static const uint8_t RAW  = 255;
};


#endif
