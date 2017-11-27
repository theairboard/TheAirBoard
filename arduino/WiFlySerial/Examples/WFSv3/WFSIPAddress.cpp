/*
 * WFSIPAddress.cpp
 * Arduino IP Address class for wifi devices
 * Based on Arduino 1.0 IP Address class
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

#include <Arduino.h>
#include "WFSIPAddress.h"

WFSIPAddress::WFSIPAddress()
{
    memset(_address, 0, sizeof(_address));
}

WFSIPAddress::WFSIPAddress(uint8_t first_octet, uint8_t second_octet, uint8_t third_octet, uint8_t fourth_octet)
{
    _address[0] = first_octet;
    _address[1] = second_octet;
    _address[2] = third_octet;
    _address[3] = fourth_octet;
}

WFSIPAddress::WFSIPAddress(uint32_t address)
{
    memcpy(_address, &address, sizeof(_address));
}

WFSIPAddress::WFSIPAddress(const uint8_t *address)
{
    memcpy(_address, address, sizeof(_address));
}

WFSIPAddress& WFSIPAddress::operator=(const uint8_t *address)
{
    memcpy(_address, address, sizeof(_address));
    return *this;
}

WFSIPAddress& WFSIPAddress::operator=(uint32_t address)
{
    memcpy(_address, (const uint8_t *)&address, sizeof(_address));
    return *this;
}

bool WFSIPAddress::operator==(const uint8_t* addr)
{
    return memcmp(addr, _address, sizeof(_address)) == 0;
}

size_t WFSIPAddress::printTo(Print& p) const
{
    size_t n = 0;
    for (int i =0; i < 3; i++)
    {
        n += p.print(_address[i], DEC);
        n += p.print('.');
    }
    n += p.print(_address[3], DEC);
    return n;
}

