/* RemoteXY.h 
   A RemoteXY Library - Remote device control
   version 1.1.0   
   
   ===========================================================
   For use RemoteXY library visit website http://remotexy.com
   This website will help you use the library for configuring 
   a remote control from a smartphone or tablet.
   ===========================================================
     
*/

#ifndef _REMOTEXY_H_
#define _REMOTEXY_H_


#if defined(REMOTEXY_MODE__SOFTWARESERIAL)

#include "RemoteXY_Serial.h"
#define RemoteXY_Init() RemoteXY_SerialSoft_Object = new RemoteXY_SerialSoft (REMOTEXY_SERIAL_RX, REMOTEXY_SERIAL_TX, REMOTEXY_SERIAL_SPEED, RemoteXY_CONF, &RemoteXY)
#define RemoteXY_Handler() RemoteXY_SerialSoft_Object->Handler ()
RemoteXY_SerialSoft *RemoteXY_SerialSoft_Object;

#elif defined(REMOTEXY_MODE__SERIAL) 

#include "RemoteXY_Serial.h"
#define RemoteXY_Init() RemoteXY_SerialHard_Object = new RemoteXY_SerialHard (&REMOTEXY_SERIAL, REMOTEXY_SERIAL_SPEED, RemoteXY_CONF, &RemoteXY)
#define RemoteXY_Handler() RemoteXY_SerialHard_Object->Handler ()
RemoteXY_SerialHard *RemoteXY_SerialHard_Object;

#else

#error No define RemoteXY mode: REMOTEXY_MODE__XXX

#endif

#endif //_REMOTEXY_H_

