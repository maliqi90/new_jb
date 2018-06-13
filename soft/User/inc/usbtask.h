#ifndef _USB_TASK_H
#define _USB_TASK_H

#ifdef USB_PRIVATE
	#define USB_PUBLIC     
#else
	#define USB_PUBLIC extern
#endif
#include "include.h"

    
//USBͨѶЭ��
    
#define WRITEFLASH              0x70        //дflash����
#define WRITEFINSH              0x71        //�������
//�ֿ�����
#define DOWNFONTERASE           0x80      //׼�������ֿ�
#define DOWNFONTERASE_OK        0x81      //׼�����
#define DOWNFONT                0x82      //�����ֿ�

#define DOWN_APP_START          0x90      //׼����������  
#define DOWN_APP_OK             0x91      // ׼�����
#define DOWN_APP                0x92      //���س���
#define DOWN_APP_               0x93      //�������سɹ�
#define DOWN_APP_FINSH          0x94      //�����������

    
#define DOWNERR                 0x78      //���ش���    
    
 
    
    
#define DOWN_OK     0x79      //���ݽ��ճɹ�
#define DOWN_FINSH   0x77      //�������
#define USB_RX_LENGHT    2048+12 //USB���ջ�������С
#define USB_Tx_LENGHT    64      //USB���ͻ�������С
USB_PUBLIC uint8_t   USB_Rx_Buffer[USB_RX_LENGHT];//USB���ջ�����
USB_PUBLIC uint8_t   USB_Tx_Buffer[USB_Tx_LENGHT];//USB���ջ�����
USB_PUBLIC volatile uint16_t  USBRxPointer;                //USB����ָ��             

USB_PUBLIC  volatile uint8_t txFlag;
USB_PUBLIC  volatile uint32_t FrameCount;
USB_PUBLIC uint16_t txlen;
USB_PUBLIC uint32_t usbtimecount;
    
void USB_DataInit(void);
void USB_Send(uint8_t *data, uint16_t len);    
void USB_Task(void);
#endif




