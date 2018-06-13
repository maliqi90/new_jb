#ifndef _DELAY_H
#define _DELAY_H
#include "include.h"

//#ifndef BOOL
//typedef uint8_t  BOOL;
//#endif
//#ifndef bool
// typedef enum {FALSE = 0, TRUE = !FALSE} bool;
//#endif 
void TIMER6_Init(void);
void TIMDelay_Nms(uint16_t Times);
void TIMDelay_N10us(uint16_t Times);
#endif

