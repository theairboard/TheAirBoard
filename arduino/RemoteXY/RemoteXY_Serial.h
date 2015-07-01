/* RemoteXY_Serial.h 
   A RemoteXY Library - Remote device control
   version 1.1.0   
   
   ===========================================================
   For use RemoteXY library visit website http://remotexy.com
   This website will help you use the library for configuring 
   a remote control from a smartphone or tablet.
   ===========================================================
     
*/

#ifndef _REMOTEXY_SERIAL_H_
#define _REMOTEXY_SERIAL_H_

#include "RemoteXY_Base.h"
#include <inttypes.h>
#include <SoftwareSerial.h>
#include <HardwareSerial.h>


class RemoteXY_Serial : public RemoteXY_Base {
private:  
  uint16_t count; 
  uint16_t index; 
  uint8_t command; 
  uint8_t crc; 
  uint32_t timeout; 
  uint8_t timeout_try; 
public:
  RemoteXY_Serial (void * _conf, void * _var);
  void Handler ();
  
private:
  virtual uint16_t rxy_available() = 0;
  virtual uint8_t rxy_read() = 0;
  virtual void rxy_write(uint8_t b) = 0;
  
  void WriteByte (uint8_t c, uint8_t * crc);
  void Send (uint8_t *p, uint16_t len);
  void Receive (uint8_t *p, uint16_t len);
  
};

#ifdef REMOTEXY_MODE__SOFTWARESERIAL

class RemoteXY_SerialSoft : public RemoteXY_Serial {
private:
  SoftwareSerial * serial;
    
public:
  RemoteXY_SerialSoft (uint8_t _rx, uint8_t _tx, long _serialspeed, void * _conf, void * _var): RemoteXY_Serial (_conf, _var) {serial = new SoftwareSerial (_rx, _tx);serial->begin (_serialspeed);};

public:
  uint16_t rxy_available() {return serial->available ();};
  uint8_t rxy_read() {return serial->read ();};
  void rxy_write(uint8_t b){serial->write (b);};
};


#endif

class RemoteXY_SerialHard : public RemoteXY_Serial {
private:
  HardwareSerial *serial;
    
public:
  RemoteXY_SerialHard (HardwareSerial *_serial, long _serialspeed, void * _conf, void * _var): RemoteXY_Serial (_conf, _var) {serial = _serial;serial->begin (_serialspeed);};

private:
  uint16_t rxy_available(){return serial->available ();};
  uint8_t rxy_read(){return serial->read ();};
  void rxy_write(uint8_t b){serial->write (b);};
};




#endif //_REMOTEXY_SERIAL_H_



