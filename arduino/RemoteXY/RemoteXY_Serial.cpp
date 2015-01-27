/* RemoteXY_Serial.cpp 
   A RemoteXY Library - Remote device control
   version 1.0.4   
   
   ===========================================================
   For use RemoteXY library visit website http://remotexy.com
   This website will help you use the library for configuring 
   a remote control from a smartphone or tablet.
   ===========================================================
     
*/


#include "RemoteXY_Serial.h"
#include <Arduino.h>
#include <inttypes.h>

#define REMOTEXY_TIMOUT  1000 
#define REMOTEXY_TIMOUT_DISCONNECTED  2000 

RemoteXY_Serial::RemoteXY_Serial (void * _conf, void * _var):RemoteXY_Base (_conf, _var) {  
  index = 0;  
  crc = 0;   
} //CRemoteXY

void RemoteXY_Serial::Handler () {
  uint8_t c;  
  uint32_t tim;  
  while (rxy_available () > 0) {    
    c = rxy_read ();  
    crc+=c;  
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
    if ((index == count) && (count >= 4)) {  
      if (crc == 0) {  
        switch (command) {  
          case 0x00:  
            Send (conf, conf_length);  
            *connect_flag=1; 
            break;   
          case 0x40:  
            Send (var, output_length+input_length);  
            break;   
          case 0x80:  
            Receive (var, index-4);   
            Send (0, 0);  
            break;   
          case 0xC0:  
            Send (var+output_length, input_length);  
            break;   
        }  
      }  
      index = 0;  
      crc = 0;         
    }  
  }  
  if (index>0) {  
    if (millis()-timeout>REMOTEXY_TIMOUT) {  
      index = 0;  
      crc = 0;               
    }  
  }  
  if (millis()-timeout>REMOTEXY_TIMOUT_DISCONNECTED) { 
    *connect_flag=0;  
  }  
}  //Handler

void RemoteXY_Serial::WriteByte (uint8_t c, uint8_t * crc) {  
  rxy_write (c);   
  *crc-=c;    
}  //WriteByte

void RemoteXY_Serial::Send (uint8_t *p, uint16_t len) {  
  uint8_t c;    
  uint8_t crc = 0;  
  uint16_t i = len+4;  
    
  WriteByte (i & 0xff, &crc);  
  WriteByte ((i & 0xff00)>>8, &crc);  
  WriteByte (command, &crc);    
  while (len--) {  
    WriteByte (*(p++), &crc);  
  }    
  rxy_write (crc);  
}  //Send

void RemoteXY_Serial::Receive (uint8_t *p, uint16_t len) {  
  uint8_t *pi = var_buffer;  
  while (len--) *p++=*pi++;  
}  //Receive



