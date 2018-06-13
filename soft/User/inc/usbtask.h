#ifndef _USB_TASK_H
#define _USB_TASK_H

#ifdef USB_PRIVATE
	#define USB_PUBLIC     
#else
	#define USB_PUBLIC extern
#endif
#include "include.h"

    
//USB通讯协议
    
#define WRITEFLASH              0x70        //写flash命令
#define WRITEFINSH              0x71        //下载完成
//字库下载
#define DOWNFONTERASE           0x80      //准备下载字库
#define DOWNFONTERASE_OK        0x81      //准备完毕
#define DOWNFONT                0x82      //下载字库

#define DOWN_APP_START          0x90      //准备升级程序  
#define DOWN_APP_OK             0x91      // 准备完毕
#define DOWN_APP                0x92      //下载程序
#define DOWN_APP_               0x93      //程序下载成功
#define DOWN_APP_FINSH          0x94      //程序下载完成

    
#define DOWNERR                 0x78      //下载错误    
    
 
    
    
#define DOWN_OK     0x79      //数据接收成功
#define DOWN_FINSH   0x77      //接收完成
#define USB_RX_LENGHT    2048+12 //USB接收缓冲区大小
#define USB_Tx_LENGHT    64      //USB发送缓冲区大小
USB_PUBLIC uint8_t   USB_Rx_Buffer[USB_RX_LENGHT];//USB接收缓冲区
USB_PUBLIC uint8_t   USB_Tx_Buffer[USB_Tx_LENGHT];//USB接收缓冲区
USB_PUBLIC volatile uint16_t  USBRxPointer;                //USB接收指针             

USB_PUBLIC  volatile uint8_t txFlag;
USB_PUBLIC  volatile uint32_t FrameCount;
USB_PUBLIC uint16_t txlen;
USB_PUBLIC uint32_t usbtimecount;
    
void USB_DataInit(void);
void USB_Send(uint8_t *data, uint16_t len);    
void USB_Task(void);
#endif




