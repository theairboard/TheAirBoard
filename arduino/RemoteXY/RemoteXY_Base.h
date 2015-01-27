/* RemoteXY_Base.h 
   A RemoteXY Library - Remote device control
   version 1.0.4   
   
   ===========================================================
   For use RemoteXY library visit website http://remotexy.com
   This website will help you use the library for configuring 
   a remote control from a smartphone or tablet.
   ===========================================================
     
*/

#ifndef _REMOTEXY_BASE_H_
#define _REMOTEXY_BASE_H_

#include <inttypes.h>


class RemoteXY_Base {
protected:
  uint8_t output_length;
  uint8_t input_length;
  uint16_t conf_length;
  uint8_t *conf;
  uint8_t var_length;  
  uint8_t *var;
  uint8_t *connect_flag;
  uint8_t *var_buffer;
  
public:
  RemoteXY_Base (void * _conf, void * _var);
};  
  
  
  
#endif //_REMOTEXY_BASE_H_

