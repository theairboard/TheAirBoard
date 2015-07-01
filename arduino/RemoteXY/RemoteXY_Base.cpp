/* RemoteXY_Serial.cpp 
   A RemoteXY Library - Remote device control
   version 1.1.0   
   
   ===========================================================
   For use RemoteXY library visit website http://remotexy.com
   This website will help you use the library for configuring 
   a remote control from a smartphone or tablet.
   ===========================================================
     
*/


#include "RemoteXY_Base.h"
#include <stdlib.h>
#include <inttypes.h>


#define REMOTEXY_OUTPUT_LENGTH_INDEX  0 
#define REMOTEXY_INPUT_LENGTH_INDEX  1
#define REMOTEXY_CONF_LENGTH_LO_INDEX 2 
#define REMOTEXY_CONF_LENGTH_HI_INDEX 3 
#define REMOTEXY_CONF_INDEX 4 

RemoteXY_Base::RemoteXY_Base (void * _conf, void * _var) {
  output_length = *((uint8_t*)_conf+REMOTEXY_OUTPUT_LENGTH_INDEX);
  input_length = *((uint8_t*)_conf+REMOTEXY_INPUT_LENGTH_INDEX);
  conf_length = *((uint8_t*)_conf+REMOTEXY_CONF_LENGTH_HI_INDEX);
  conf_length = (conf_length<<8) + *((uint8_t*)_conf+REMOTEXY_CONF_LENGTH_LO_INDEX);
  conf = (uint8_t*)_conf+REMOTEXY_CONF_INDEX;
  var_length = output_length+input_length+1;
  var = (uint8_t*)_var;
  var_buffer = (uint8_t*)malloc (var_length);
  connect_flag = var+var_length-1;
  
  uint8_t* p = var;
  for (uint8_t i=0;i<var_length;i++) *p++=0;       
} //RemoteXY_Base
