/* RemoteXY_Serial.cpp 
   A RemoteXY Library - Remote device control
   version 1.1.0   
   
   ===========================================================
   For use RemoteXY library visit website http://remotexy.com
   This website will help you use the library for configuring 
   a remote control from a smartphone or tablet.
   ===========================================================
     
*/

//#define REMOTEXY_MODE__DEBUGLOGS 

#include "RemoteXY_Serial.h"
#include <Arduino.h>
#include <inttypes.h>

#ifdef REMOTEXY_MODE__DEBUGLOGS
#include <HardwareSerial.h>
#endif

#define REMOTEXY_TIMOUT  300 
#define REMOTEXY_TIMOUT_TRY  10 
#define REMOTEXY_TIMOUT_DISCONNECTED  3000 

RemoteXY_Serial::RemoteXY_Serial (void * _conf, void * _var):RemoteXY_Base (_conf, _var) {  
  index = 0;  
  crc = 0;   
  timeout_try = REMOTEXY_TIMOUT_TRY;
#ifdef REMOTEXY_MODE__DEBUGLOGS
  Serial.begin (9600);
#endif
} //CRemoteXY

void RemoteXY_Serial::Handler () {
  uint8_t c;  
  uint32_t tim;  
  while (rxy_available () > 0) {    
    c = rxy_read ();  
    
#ifdef REMOTEXY_MODE__DEBUGLOGS
    if (index==0) Serial.write("\r\n< ");
    Serial.print(c, HEX);  
    Serial.write(" ");
#endif
    
    if (index==0) crc=c;
    else crc+=c;  
    switch (index) {  
      case 0:   
        count=c;  
        break;  
      case 1:   
        count+=c<<8;  
        break;  
      case 2:   
        command=c;  
        break;  
      default:   
        if (index-3<var_length)  
          *(var_buffer+index-3)=c;  
    }  
    index++;  
    timeout=millis();  
    timeout_try = 0;
    if ((index >= 4) && (index == count) && (count >= 4)) {  
      if (crc == 0) {  
        switch (command) {  
          case 0x00:  
            Send (conf, conf_length);  
            break;   
          case 0x40:  
            Send (var, output_length+input_length);  
            break;   
          case 0x80:  
            Receive (var, index-4);   
            Send (0, 0);  
            *connect_flag=1; 
            break;   
          case 0xC0:  
            Send (var+output_length, input_length);  
            *connect_flag=1; 
            break;   
        }  
      }  
      index = 0;  
    } 
  }  
  if (millis()-timeout>REMOTEXY_TIMOUT) {  
    index = 0;   
    if (timeout_try<REMOTEXY_TIMOUT_TRY) {
      timeout_try++;
#ifdef REMOTEXY_MODE__DEBUGLOGS
      Serial.print("\r\n0");
#endif
      rxy_write (0); 
      timeout=millis();
    }
    else *connect_flag=0;
  }  
}  //Handler

void RemoteXY_Serial::WriteByte (uint8_t c, uint8_t * crc) {  
#ifdef REMOTEXY_MODE__DEBUGLOGS
  Serial.print(c, HEX); 
  Serial.write(" ");
#endif
  rxy_write (c);   
  *crc-=c;    
}  //WriteByte

void RemoteXY_Serial::Send (uint8_t *p, uint16_t len) {  
  uint8_t c;    
  uint8_t crc = 0;  
  uint16_t i = len+4;  
#ifdef REMOTEXY_MODE__DEBUGLOGS
  Serial.write("\r\n> ");  
#endif
  WriteByte (i & 0xff, &crc);  
  WriteByte ((i & 0xff00)>>8, &crc);  
  WriteByte (command, &crc);    
  while (len--) {  
    WriteByte (*(p++), &crc);  
  }    
#ifdef REMOTEXY_MODE__DEBUGLOGS
  Serial.print(crc, HEX);  
#endif
  rxy_write (crc);  
}  //Send

void RemoteXY_Serial::Receive (uint8_t *p, uint16_t len) {  
  uint8_t *pi = var_buffer;  
  while (len--) *p++=*pi++;  
}  //Receive



