#include "stm32f10x.h"
#include "stm32_flash.h"

FLASH_Status FLASH_ProgramHalfWord(uint32_t Address, uint16_t Data)
{
  FLASH_Status status = FLASH_COMPLETE;
  /* Check the parameters */
  assert_param(IS_FLASH_ADDRESS(Address));

#ifdef STM32F10X_XL
  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation(ProgramTimeout);
  
  if(Address < FLASH_BANK1_END_ADDRESS)
  {
    if(status == FLASH_COMPLETE)
    {
      /* if the previous operation is completed, proceed to program the new data */
      FLASH->CR |= CR_PG_Set;
  
      *(__IO uint16_t*)Address = Data;
      /* Wait for last operation to be completed */
      status = FLASH_WaitForLastBank1Operation(ProgramTimeout);

      /* Disable the PG Bit */
      FLASH->CR &= CR_PG_Reset;
    }
  }
  else
  {
    if(status == FLASH_COMPLETE)
    {
      /* if the previous operation is completed, proceed to program the new data */
      FLASH->CR2 |= CR_PG_Set;
  
      *(__IO uint16_t*)Address = Data;
      /* Wait for last operation to be completed */
      status = FLASH_WaitForLastBank2Operation(ProgramTimeout);

      /* Disable the PG Bit */
      FLASH->CR2 &= CR_PG_Reset;
    }
  }
#else
  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation(ProgramTimeout);
  
  if(status == FLASH_COMPLETE)
  {
    /* if the previous operation is completed, proceed to program the new data */
    FLASH->CR |= CR_PG_Set;
  
    *(__IO uint16_t*)Address = Data;
    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation(ProgramTimeout);
    
    /* Disable the PG Bit */
    FLASH->CR &= CR_PG_Reset;
  } 
#endif  /* STM32F10X_XL */
  
  /* Return the Program Status */
  return status;
}
/*******************************************************************************
* Function Name  : delay
* Description    : Inserts a time delay.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void delay(void)
{
  vu32 i = 0;

  for(i = 0xFF; i != 0; i--)
  {
  }
}

/*******************************************************************************
* Function Name  : FLASH_Unlock
* Description    : Unlocks the FLASH Program Erase Controller.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void FLASH_Unlock(void)
{
  /* Authorize the FPEC Access */
  FLASH->KEYR = FLASH_KEY1;
  FLASH->KEYR = FLASH_KEY2;
}

/*******************************************************************************
* Function Name  : FLASH_Lock
* Description    : Locks the FLASH Program Erase Controller.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void FLASH_Lock(void)
{
  /* Set the Lock Bit to lock the FPEC and the FCR */
  FLASH->CR |= 0x00000080;
}

/*******************************************************************************
* Function Name  : FLASH_GetStatus
* Description    : Returns the FLASH Status.
* Input          : None
* Output         : None
* Return         : FLASH Status: The returned value can be: FLASH_BUSY, 
*                  FLASH_ERROR_PG, FLASH_ERROR_WRP or FLASH_COMPLETE
*******************************************************************************/
FLASH_Status FLASH_GetStatus(void)
{
  FLASH_Status flashstatus = FLASH_COMPLETE;
  
  if((FLASH->SR & 0x00000001) == 0x00000001) 
  {
    flashstatus = FLASH_BUSY;
  }
  else 
  {  
    if(FLASH->SR & 0x00000004)
    { 
      flashstatus = FLASH_ERROR_PG;
    }
    else 
    {
      if(FLASH->SR & 0x00000010)
      {
        flashstatus = FLASH_ERROR_WRP;
      }
      else
      {
        flashstatus = FLASH_COMPLETE;
      }
    }
  }
  /* Return the Flash Status */
  return flashstatus;
}

/*******************************************************************************
* Function Name  : FLASH_WaitForLastOperation
* Description    : Waits for a Flash operation to complete or a TIMEOUT to occur.
* Input          : - Timeout: FLASH progamming Timeout
* Output         : None
* Return         : FLASH Status: The returned value can be: FLASH_BUSY, 
*                  FLASH_ERROR_PG, FLASH_ERROR_WRP, FLASH_COMPLETE or 
*                  FLASH_TIMEOUT.
*******************************************************************************/
FLASH_Status FLASH_WaitForLastOperation(u32 Timeout)
{ 
  FLASH_Status status = FLASH_COMPLETE;
   
  /* Check for the Flash Status */
  status = FLASH_GetStatus();

  /* Wait for a Flash operation to complete or a TIMEOUT to occur */
  while((status == FLASH_BUSY) && (Timeout != 0x00))
  {
    delay();
    status = FLASH_GetStatus();
    Timeout--;
  }

  if(Timeout == 0x00 )
  {
    status = FLASH_TIMEOUT;
  }

  /* Return the operation status */
  return status;
}
/**
  * @brief  Erases a specified FLASH page.
  * @note   This function can be used for all STM32F10x devices.
  * @param  Page_Address: The page address to be erased.
  * @retval FLASH Status: The returned value can be: FLASH_BUSY, FLASH_ERROR_PG,
  *         FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
  */
FLASH_Status FLASH_ErasePage(uint32_t Page_Address)
{
  FLASH_Status status = FLASH_COMPLETE;
  /* Check the parameters */
  assert_param(IS_FLASH_ADDRESS(Page_Address));

#ifdef STM32F10X_XL
  if(Page_Address < FLASH_BANK1_END_ADDRESS)  
  {
    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastBank1Operation(EraseTimeout);
    if(status == FLASH_COMPLETE)
    { 
      /* if the previous operation is completed, proceed to erase the page */
      FLASH->CR|= CR_PER_Set;
      FLASH->AR = Page_Address; 
      FLASH->CR|= CR_STRT_Set;
    
      /* Wait for last operation to be completed */
      status = FLASH_WaitForLastBank1Operation(EraseTimeout);

      /* Disable the PER Bit */
      FLASH->CR &= CR_PER_Reset;
    }
  }
  else
  {
    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastBank2Operation(EraseTimeout);
    if(status == FLASH_COMPLETE)
    { 
      /* if the previous operation is completed, proceed to erase the page */
      FLASH->CR2|= CR_PER_Set;
      FLASH->AR2 = Page_Address; 
      FLASH->CR2|= CR_STRT_Set;
    
      /* Wait for last operation to be completed */
      status = FLASH_WaitForLastBank2Operation(EraseTimeout);
      
      /* Disable the PER Bit */
      FLASH->CR2 &= CR_PER_Reset;
    }
  }
#else
  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation(EraseTimeout);
  
  if(status == FLASH_COMPLETE)
  { 
    /* if the previous operation is completed, proceed to erase the page */
    FLASH->CR|= CR_PER_Set;
    FLASH->AR = Page_Address; 
    FLASH->CR|= CR_STRT_Set;
    
    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation(EraseTimeout);
    
    /* Disable the PER Bit */
    FLASH->CR &= CR_PER_Reset;
  }
#endif /* STM32F10X_XL */

  /* Return the Erase Status */
  return status;
}
/*******************************************************************************
* Function Name  : Stm32FlashErasePage
* Description    : Erases a specified FLASH page.
* Input          : - Page_Address: The page address to be erased.
* Output         : None
* Return         : FLASH Status: The returned value can be: FLASH_BUSY, 
*                  FLASH_ERROR_PG, FLASH_ERROR_WRP, FLASH_COMPLETE or 
*                  FLASH_TIMEOUT.
*******************************************************************************/
FLASH_Status Flash_ErasePage(u32 Page_Address)
{
  FLASH_Status status = FLASH_COMPLETE;

  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation(EraseTimeout);
  
  if(status == FLASH_COMPLETE)
  { 
    /* if the previous operation is completed, proceed to erase the page */
    FLASH->CR|= 0x00000002;
    FLASH->AR = Page_Address; 
    FLASH->CR|= 0x00000040;
    
    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation(EraseTimeout);

    if(status != FLASH_BUSY)
    {
      /* if the erase operation is completed, disable the PER Bit */
      FLASH->CR &= 0x00001FFD;
    }
  }
  /* Return the Erase Status */
  return status;
}

/*******************************************************************************
* Function Name  : FLASH_GetWriteProtectionOptionByte
* Description    : Returns the FLASH Write Protection Option Bytes Register value.
* Input          : None
* Output         : None
* Return         : The FLASH Write Protection  Option Bytes Register value
*******************************************************************************/
//u32 FLASH_GetWriteProtectionOptionByte(void)
//{
//  /* Return the Falsh write protection Register value */
//  return (u32)(FLASH->WRPR);
//}

/*******************************************************************************
 �� �� ����	Stm32FlashWrite
 ����������	��stm32��flash��д��1������
 ��ڲ�����	address:��ʼ��ַ data:����
 ���ڲ�����	��
********************************************************************************/
void Stm32FlashWrite(u32 address, u16 data)
{	
	FLASH_Unlock();
	
	FLASH->SR = 0x00000034;

	while (FLASH->SR & (1 << 0));
	FLASH->CR |= 0x00000001;
	*(vu16*)address = data;
	while (FLASH->SR & (1 << 0));
	FLASH->CR &= ~0x00000001;
	
	FLASH_Lock();
}
#if STM32_FLASH_SIZE<256
#define STM_SECTOR_SIZE 1024 //�ֽ�
#else 
#define STM_SECTOR_SIZE	2048
#endif		 
u16 STMFLASH_BUF[STM_SECTOR_SIZE/2];//�����2K�ֽ�
u16 STMFLASH_ReadHalfWord(u32 faddr)
{
	return *(vu16*)faddr; 
}
//��ָ����ַ��ʼ����ָ�����ȵ�����
//ReadAddr:��ʼ��ַ
//pBuffer:����ָ��
//NumToWrite:����(16λ)��
void STMFLASH_Read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead)   	
{
	u16 i;
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=STMFLASH_ReadHalfWord(ReadAddr);//��ȡ2���ֽ�.
		ReadAddr+=2;//ƫ��2���ֽ�.	
	}
}
void STMFLASH_Write_NoCheck(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)   
{ 			 		 
	u16 i;
	for(i=0;i<NumToWrite;i++)
	{
		FLASH_ProgramHalfWord(WriteAddr,pBuffer[i]);
	    WriteAddr+=2;//��ַ����2.
	}  
} 
void STMFLASH_Write(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)	
{
	u32 secpos;	   //������ַ
	u16 secoff;	   //������ƫ�Ƶ�ַ(16λ�ּ���)
	u16 secremain; //������ʣ���ַ(16λ�ּ���)	   
 	u16 i;    
	u32 offaddr;   //ȥ��0X08000000��ĵ�ַ
	if(WriteAddr<STM32_FLASH_BASE||(WriteAddr>=(STM32_FLASH_BASE+1024*STM32_FLASH_SIZE)))return;//�Ƿ���ַ
	FLASH_Unlock();						//����
	offaddr=WriteAddr-STM32_FLASH_BASE;		//ʵ��ƫ�Ƶ�ַ.
	secpos=offaddr/STM_SECTOR_SIZE;			//������ַ  0~127 for STM32F103RBT6
	secoff=(offaddr%STM_SECTOR_SIZE)/2;		//�������ڵ�ƫ��(2���ֽ�Ϊ������λ.)
	secremain=STM_SECTOR_SIZE/2-secoff;		//����ʣ��ռ��С   
	if(NumToWrite<=secremain)secremain=NumToWrite;//�����ڸ�������Χ
	while(1) 
	{	
		STMFLASH_Read(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE/2);//������������������
		for(i=0;i<secremain;i++)//У������
		{
			if(STMFLASH_BUF[secoff+i]!=0XFFFF)break;//��Ҫ����  	  
		}
		if(i<secremain)//��Ҫ����
		{
			FLASH_ErasePage(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE);//�����������
			for(i=0;i<secremain;i++)//����
			{
				STMFLASH_BUF[i+secoff]=pBuffer[i];	  
			}
			STMFLASH_Write_NoCheck(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE/2);//д����������  
		}else STMFLASH_Write_NoCheck(WriteAddr,pBuffer,secremain);//д�Ѿ������˵�,ֱ��д������ʣ������. 				   
		if(NumToWrite==secremain)break;//д�������
		else//д��δ����
		{
			secpos++;				//������ַ��1
			secoff=0;				//ƫ��λ��Ϊ0 	 
		   	pBuffer+=secremain;  	//ָ��ƫ��
			WriteAddr+=secremain;	//д��ַƫ��	   
		   	NumToWrite-=secremain;	//�ֽ�(16λ)���ݼ�
			if(NumToWrite>(STM_SECTOR_SIZE/2))secremain=STM_SECTOR_SIZE/2;//��һ����������д����
			else secremain=NumToWrite;//��һ����������д����
		}	 
	};	
	FLASH_Lock();//����
}
/*******************************************************************************
 �� �� ����	Stm32FlashProgram
 ����������	��stm32��flash��д������
 ��ڲ�����	address:��ʼ��ַ pdata:����ָ�� len:д�볤��
 ���ڲ�����	��
********************************************************************************/
void Stm32FlashProgram(u32 address, u8 *pdata, u16 len)
{
	u32 temp_addr = address;
	u16 temp_len  = len;
	u16 buff, temp;
	
	if ((address + len) & 1)
		temp_len = len - 1;
		
	if (temp_addr & 1)
	{
		temp_addr--;
		buff = *pdata++;
		buff <<= 8;
		buff |= 0x00ff;
		Stm32FlashWrite(temp_addr, buff);
		temp_addr += 2;
	}
		
	while (temp_addr < (address + temp_len))
	{
		buff = *pdata++;
		temp = *pdata++;
		temp <<= 8;
		buff = temp + buff;
		Stm32FlashWrite(temp_addr, buff);
		temp_addr += 2;
	}

	if ((address + len) & 1)
	{
		buff  = *pdata;
		buff |= 0xff00;
		Stm32FlashWrite(temp_addr, buff);
	}
}


/*******************************************************************************
 �� �� ����	Stm32FlashRead
 ����������	��ȡstm32 flash�е�����
 ��ڲ�����	addr:��ַ buff:���ݻ���ָ�� len:��ȡ���ֽ���Ŀ
 ���ڲ�����	��
********************************************************************************/
void Stm32FlashRead(u32 addr, u8 *buff, u16 len)
{
	u16  temp_len  = len;
	u8 	*temp_buff = buff;
	u8  *temp_addr = (u8*)addr;
	u16  i = 0;

	while (i < temp_len)
	{
		*temp_buff++ = *temp_addr++;
		i++;
	}
}

/*******************************************************************************
* Function Name  : Stm32FlashErasePage
* Description    : Erases a specified FLASH page.
* Input          : - Page_Address: The page address to be erased.
* Output         : None
* Return         : FLASH Status: The returned value can be: FLASH_BUSY, 
*                  FLASH_ERROR_PG, FLASH_ERROR_WRP, FLASH_COMPLETE or 
*                  FLASH_TIMEOUT.
*******************************************************************************/
void Stm32FlashErasePage(u8 Page_Address)
{	
	FLASH_Unlock();
	
	FLASH->SR = 0x00000034;
	
	Flash_ErasePage(0x8000000 + (Page_Address * 1024));
	
	FLASH_Lock();
}

