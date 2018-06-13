#ifndef _DEVICE_H_
#define _DEVICE_H_

#include "device.h"
#define UART2_RECEIVE_TIMEOUT		280

#define UART2_BUFFER_COUNT  	10
#define UART2_BUFFER_LENGTH  	3

//struct Task *FirstTask;//头任务指针
//struct Task *LastTask;//尾任务指针
//struct Task  TaskBuf[TASKS];//任务寄存器组
extern struct UsartDate Usat2_data;//串口数据
void UART2_Init(void);
void uart2_send(uint8_t len, uint8_t *buff);
void Rev2_Istr(void);
void shell2(uint8_t *cmd,uint8_t length );
#endif


