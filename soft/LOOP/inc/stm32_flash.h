#ifndef __STM32_FLASH_H__
#define __STM32_FLASH_H__

//typedef enum
//{ 
//  FLASH_BUSY = 1,
//  FLASH_ERROR_PG,
//  FLASH_ERROR_WRP,
//  FLASH_COMPLETE,
//  FLASH_TIMEOUT
//}FLASH_Status;
#define STM32_FLASH_SIZE 512 	 		//��ѡSTM32��FLASH������С(��λΪK)
#define STM32_FLASH_WREN 1              //ʹ��FLASHд��(0��������;1��ʹ��)
#define IAPSTART		0x8005000
#define STM32_FLASH_BASE 0x08000000 	//STM32 FLASH����ʼ��ַ
#define FLASH_KEY1 		((u32)0x45670123)
#define FLASH_KEY2		((u32)0xCDEF89AB)

/* Delay definition */   
#define EraseTimeout    ((u32)0x00000FFF)
#define ProgramTimeout  ((u32)0x0000000F)
#define CR_PG_Set                ((uint32_t)0x00000001)
#define CR_PG_Reset              ((uint32_t)0x00001FFE) 
#define CR_PER_Set               ((uint32_t)0x00000002)
#define CR_PER_Reset             ((uint32_t)0x00001FFD)
#define CR_MER_Set               ((uint32_t)0x00000004)
#define CR_MER_Reset             ((uint32_t)0x00001FFB)
#define CR_OPTPG_Set             ((uint32_t)0x00000010)
#define CR_OPTPG_Reset           ((uint32_t)0x00001FEF)
#define CR_OPTER_Set             ((uint32_t)0x00000020)
#define CR_OPTER_Reset           ((uint32_t)0x00001FDF)
#define CR_STRT_Set              ((uint32_t)0x00000040)
#define CR_LOCK_Set              ((uint32_t)0x00000080)
/*******************************************************************************
* Function Name  : Stm32FlashErasePage
* Description    : Erases a specified FLASH page.
* Input          : - Page_Address: The page address to be erased.
* Output         : None
* Return         : FLASH Status: The returned value can be: FLASH_BUSY, 
*                  FLASH_ERROR_PG, FLASH_ERROR_WRP, FLASH_COMPLETE or 
*                  FLASH_TIMEOUT.
*******************************************************************************/
void Stm32FlashErasePage(u8 Page_Address);
FLASH_Status Flash_ErasePage(u32 Page_Address);
FLASH_Status FLASH_ErasePage(uint32_t Page_Address);
/*******************************************************************************
 �� �� ����	Stm32FlashProgram
 ����������	��stm32��flash��д������
 ��ڲ�����	address:��ʼ��ַ pdata:����ָ�� len:д�볤��
 ���ڲ�����	��
********************************************************************************/
void Stm32FlashProgram(u32 address, u8 *pdata, u16 len);

/*******************************************************************************
 �� �� ����	Stm32FlashRead
 ����������	��ȡstm32 flash�е�����
 ��ڲ�����	addr:��ַ buff:���ݻ���ָ�� len:��ȡ���ֽ���Ŀ
 ���ڲ�����	��
********************************************************************************/
void Stm32FlashRead(u32 addr, u8 *buff, u16 len);
void STMFLASH_Write(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite);	
#endif

