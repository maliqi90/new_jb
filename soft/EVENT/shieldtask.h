#ifndef _SHIELDTASK_H
#define _SHIELDTASK_H
#include "include.h"

#ifdef SHIELD_TASK_PRIVATE
	#define SHIELD_TASK_PUBLIC     
#else
	#define SHIELD_TASK_PUBLIC extern
#endif
extern SYSTEM_EVENT EventShieldtRamBuffer[17];	
extern    uint16_t 			EventShieldRamCurrentPointer;
extern void LightShieldLed(void);
//extern static void WriteShieldDataToFlash(SYSTEM_EVENT *pEvent);
extern   uint8_t  ShieldSet(uint8_t DeviceID);
 extern uint8_t OpenSet(uint8_t DeviceID);
#endif
