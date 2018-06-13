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
#define STM32_FLASH_SIZE 512 	 		//所选STM32的FLASH容量大小(单位为K)
#define STM32_FLASH_WREN 1              //使能FLASH写入(0，不是能;1，使能)
#define IAPSTART		0x8005000
#define STM32_FLASH_BASE 0x08000000 	//STM32 FLASH的起始地址
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
 函 数 名：	Stm32FlashProgram
 功能描述：	向stm32的flash中写入数据
 入口参数：	address:起始地址 pdata:数据指针 len:写入长度
 出口参数：	无
********************************************************************************/
void Stm32FlashProgram(u32 address, u8 *pdata, u16 len);

/*******************************************************************************
 函 数 名：	Stm32FlashRead
 功能描述：	读取stm32 flash中的数据
 入口参数：	addr:地址 buff:数据缓冲指针 len:读取的字节数目
 出口参数：	无
********************************************************************************/
void Stm32FlashRead(u32 addr, u8 *buff, u16 len);
void STMFLASH_Write(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite);	
#endif

