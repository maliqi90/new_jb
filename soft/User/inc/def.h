#ifndef _DEF_H
#define _DEF_H
#include "stdint.h"
#include "GUI_ConfDefaults.h"
#ifndef BOOL
#define BOOL  uint8_t
#endif
#ifndef bool
#define bool uint8_t 
#endif
#ifndef TRUE  
#define TRUE   1
#endif
#ifndef FALSE
#define FALSE   0
#endif
extern volatile GUI_TIMER_TIME OS_TimeMS;
#
static __inline uint32_t GetTickCount(void)
{
	return(OS_TimeMS);
}	
void feed_dog(void);
#endif


