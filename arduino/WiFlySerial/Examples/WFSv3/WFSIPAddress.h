/*
 * WFSIPAddress.h
 * Arduino IP Address class for wifi devices
 * Based on Arduino 1.0 IP Address class
 * 
 * Credits:
 * First to the Arduino Ethernet team for their model upon which this is based
 * and Adrian McEwen
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
/*
 *
 * MIT License:
 * Copyright (c) 2011 Adrian McEwen
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * adrianm@mcqn.com 1/1/2011
 */

#ifndef WFSIPAddress_h
#define WFSIPAddress_h

#include <Printable.h>
#include <IPAddress.h>

// A class to make it easier to handle and pass around IP addresses

class WFSIPAddress : public IPAddress {
private:
    uint8_t _address[4];  // IPv4 address


public:
    // Access the raw byte array containing the address.  Because this returns a pointer
    // to the internal structure rather than a copy of the address this function should only
    // be used when you know that the usage of the returned uint8_t* will be transient and not
    // stored.
    uint8_t* raw_address() { return _address; };

// public:
    // Constructors
    WFSIPAddress();
    WFSIPAddress(uint8_t first_octet, uint8_t second_octet, uint8_t third_octet, uint8_t fourth_octet);
    WFSIPAddress(uint32_t address);
    WFSIPAddress(const uint8_t *address);

    // Overloaded cast operator to allow IPAddress objects to be used where a pointer
    // to a four-byte uint8_t array is expected
    operator uint32_t() { return *((uint32_t*)_address); };
    bool operator==(const WFSIPAddress& addr) { return (*((uint32_t*)_address)) == (*((uint32_t*)addr._address)); };
    bool operator==(const uint8_t* addr);

    // Overloaded index operator to allow getting and setting individual octets of the address
    uint8_t operator[](int index) const { return _address[index]; };
    uint8_t& operator[](int index) { return _address[index]; };

    // Overloaded copy operators to allow initialisation of IPAddress objects from other types
    WFSIPAddress& operator=(const uint8_t *address);
    WFSIPAddress& operator=(uint32_t address);



    virtual size_t printTo(Print& p) const;

    friend class WFSEthernetClass;
    friend class WFSEthernetClient;
    friend class WFSEthernetServer;
};

//const WFSIPAddress INADDR_NONE(0,0,0,0);


#endif
