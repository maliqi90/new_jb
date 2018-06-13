#include "stm32f10x.h"
#include "sflash_spi.h"
//#include "timerx.h"
//#include "uart.h"
#include <string.h>

void sFLASH_Hardware_Init(void)
{
  SPI_InitTypeDef  SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

   /*!< sFLASH_SPI_CS_GPIO, sFLASH_SPI_MOSI_GPIO, sFLASH_SPI_MISO_GPIO 
       and sFLASH_SPI_SCK_GPIO Periph clock enable */
  RCC_APB2PeriphClockCmd(sFLASH_CS_GPIO_CLK | sFLASH_SPI_MOSI_GPIO_CLK | sFLASH_SPI_MISO_GPIO_CLK |
                         sFLASH_SPI_SCK_GPIO_CLK | RCC_APB2Periph_AFIO,ENABLE);
     GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);

  /*!< sFLASH_SPI Periph clock enable */
  RCC_APB1PeriphClockCmd(sFLASH_SPI_CLK, ENABLE);
//  GPIO_PinRemapConfig(GPIO_Remap_SPI3,ENABLE);
  /*!< Configure sFLASH_SPI pins: SCK */
  GPIO_InitStructure.GPIO_Pin = sFLASH_SPI_SCK_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(sFLASH_SPI_SCK_GPIO_PORT, &GPIO_InitStructure);

  /*!< Configure sFLASH_SPI pins: MOSI */
  GPIO_InitStructure.GPIO_Pin = sFLASH_SPI_MOSI_PIN;
  GPIO_Init(sFLASH_SPI_MOSI_GPIO_PORT, &GPIO_InitStructure);

  /*!< Configure sFLASH_SPI pins: MISO */
  GPIO_InitStructure.GPIO_Pin = sFLASH_SPI_MISO_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  
  GPIO_Init(sFLASH_SPI_MISO_GPIO_PORT, &GPIO_InitStructure);
  
  /*!< Configure sFLASH_CS_PIN pin: sFLASH Card CS pin */
  GPIO_InitStructure.GPIO_Pin = sFLASH_CS_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(sFLASH_CS_GPIO_PORT, &GPIO_InitStructure);
    
  /*!< Deselect the FLASH: Chip Select high */
  sFLASH_CS_HIGH();

  /*!< SPI configuration */
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
#if defined (STM32F10X_LD_VL) || defined (STM32F10X_MD_VL) || defined (STM32F10X_HD_VL)
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
#else
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
#endif

  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(sFLASH_SPI, &SPI_InitStructure);

  /*!< Enable the sFLASH_SPI  */
  SPI_Cmd(sFLASH_SPI, ENABLE);
}
uint8_t sFLASH_ReadByte(void)
{
  return (sFLASH_SendByte(sFLASH_DUMMY_BYTE));
}

/**
  * @brief  Sends a byte through the SPI interface and return the byte received
  *         from the SPI bus.
  * @param  byte: byte to send.
  * @retval The value of the received byte.
  */
uint8_t sFLASH_SendByte(uint8_t byte)
{
  /*!< Loop while DR register in not emplty */
  while (SPI_I2S_GetFlagStatus(sFLASH_SPI, SPI_I2S_FLAG_TXE) == RESET);

  /*!< Send byte through the SPI1 peripheral */
  SPI_I2S_SendData(sFLASH_SPI, byte);

  /*!< Wait to receive a byte */
  while (SPI_I2S_GetFlagStatus(sFLASH_SPI, SPI_I2S_FLAG_RXNE) == RESET);

  /*!< Return the byte read from the SPI bus */
  return SPI_I2S_ReceiveData(sFLASH_SPI);
}
uint32_t sFLASH_NOR_ReadID( void )
{
   uint8_t data1;
	 uint8_t data2;
	 uint8_t data3;
	 uint32_t data;
	 sFLASH_CS_LOW();
	 sFLASH_SendByte(sFLASH_CMD_NOR_RDID);
	 data1 = sFLASH_SendByte(sFLASH_DUMMY_BYTE);
	 data2 = sFLASH_SendByte(sFLASH_DUMMY_BYTE);
	 data3 = sFLASH_SendByte(sFLASH_DUMMY_BYTE);
	 sFLASH_CS_HIGH();
	 data = (data1<<16)|(data2<<8)|data3;
	 return data;
}
void sFLASH_NOR_ChipErase()
{
  /*!< Send write enable instruction */
  sFLASH_WriteEnable();

  /*!< Sector Erase */
  /*!< Select the FLASH: Chip Select low */
  sFLASH_CS_LOW();
  /*!< Send Sector Erase instruction */
  sFLASH_SendByte(sFLASH_CMD_NOR_CE);
  
  /*!< Deselect the FLASH: Chip Select high */
  sFLASH_CS_HIGH();

  /*!< Wait the end of Flash writing */
  sFLASH_NOR_WaitForWriteEnd();
}
void sFLASH_NOR_SectorErase(uint32_t SectorAddr)
{
  /*!< Send write enable instruction */
  sFLASH_WriteEnable();

  /*!< Sector Erase */
  /*!< Select the FLASH: Chip Select low */
  sFLASH_CS_LOW();
  /*!< Send Sector Erase instruction */
  sFLASH_SendByte(sFLASH_CMD_NOR_SE);
  /*!< Send SectorAddr high nibble address byte */
  sFLASH_SendByte((SectorAddr & 0xFF0000) >> 16);
  /*!< Send SectorAddr medium nibble address byte */
  sFLASH_SendByte((SectorAddr & 0xFF00) >> 8);
  /*!< Send SectorAddr low nibble address byte */
  sFLASH_SendByte(SectorAddr & 0xFF);
  /*!< Deselect the FLASH: Chip Select high */
  sFLASH_CS_HIGH();

  /*!< Wait the end of Flash writing */
  sFLASH_NOR_WaitForWriteEnd();
}
void sFLASH_NOR_BlockErase(uint32_t SectorAddr)
{
  /*!< Send write enable instruction */
  sFLASH_WriteEnable();

  /*!< Sector Erase */
  /*!< Select the FLASH: Chip Select low */
  sFLASH_CS_LOW();
  /*!< Send Sector Erase instruction */
  sFLASH_SendByte(sFLASH_CMD_NOR_BE);
  /*!< Send SectorAddr high nibble address byte */
  sFLASH_SendByte((SectorAddr & 0xFF0000) >> 16);
  /*!< Send SectorAddr medium nibble address byte */
  sFLASH_SendByte((SectorAddr & 0xFF00) >> 8);
  /*!< Send SectorAddr low nibble address byte */
  sFLASH_SendByte(SectorAddr & 0xFF);
  /*!< Deselect the FLASH: Chip Select high */
  sFLASH_CS_HIGH();

  /*!< Wait the end of Flash writing */
  sFLASH_NOR_WaitForWriteEnd();
}
void sFLASH_WriteEnable(void)
{
  /*!< Select the FLASH: Chip Select low */
  sFLASH_CS_LOW();

  /*!< Send "Write Enable" instruction */
  sFLASH_SendByte(sFLASH_CMD_NOR_WREN);

  /*!< Deselect the FLASH: Chip Select high */
  sFLASH_CS_HIGH();
	
}
void sFLASH_WriteDisable(void)
{
  /*!< Select the FLASH: Chip Select low */
  sFLASH_CS_LOW();

  /*!< Send "Write Enable" instruction */
  sFLASH_SendByte(sFLASH_CMD_NOR_WDIS);

  /*!< Deselect the FLASH: Chip Select high */
  sFLASH_CS_HIGH();

}
/**
  * @brief  Polls the status of the Write In Progress (WIP) flag in the FLASH's
  *         status register and loop until write opertaion has completed.
  * @param  None
  * @retval None
  */
void sFLASH_WaitForWriteEnd(void)
{
  uint8_t flashstatus = 0;

  /*!< Select the FLASH: Chip Select low */
  sFLASH_CS_LOW();

  /*!< Send "Read Status Register" instruction */
  sFLASH_SendByte(sFLASH_CMD_NOR_RDSR);

  /*!< Loop as long as the memory is busy with a write cycle */
  do
  {
    /*!< Send a dummy byte to generate the clock needed by the FLASH
    and put the value of the status register in FLASH_Status variable */
    flashstatus = sFLASH_SendByte(sFLASH_DUMMY_BYTE);

  }
  while ((flashstatus & sFLASH_WIP_FLAG) == SET); /* Write in progress */

  /*!< Deselect the FLASH: Chip Select high */
  sFLASH_CS_HIGH();
}

void sFLASH_NOR_WaitForWriteEnd(void)
{
  uint8_t flashstatus = 0;
//  uint32_t begin_time = 0;
//	uint32_t last_time = 0;
//	begin_time = MillisecondCounter;
  /*!< Select the FLASH: Chip Select low */
  sFLASH_CS_LOW();

  /*!< Send "Read Status Register" instruction */
  sFLASH_SendByte(sFLASH_CMD_NOR_RDSR);

  /*!< Loop as long as the memory is busy with a write cycle */
  do
  {
    /*!< Send a dummy byte to generate the clock needed by the FLASH
    and put the value of the status register in FLASH_Status variable */
    flashstatus = sFLASH_SendByte(sFLASH_CMD_NOR_RDSR);
//    last_time = MillisecondCounter;
//		if( begin_time > last_time)
//		{
//         if( last_time+0xffffffff > begin_time + TIMEOUT )
//				 {
////               UART_send(1, " BUSY BIT HAVE TIME OUT \r\n", strlen(" BUSY BIT HAVE TIME OUT \r\n") );
////				       USART1_push();
//         }
//     }
//		 else
//		 {
//         if( last_time > begin_time + TIMEOUT )
//				 {
////               UART_send(1, " BUSY BIT HAVE TIME OUT \r\n", strlen(" BUSY BIT HAVE TIME OUT \r\n") );
////				       USART1_push();
//          }
//     }
  }
  while ((flashstatus & sFLASH_WIP_FLAG) == SET); /* Write in progress */

  /*!< Deselect the FLASH: Chip Select high */
  sFLASH_CS_HIGH();
}

void sFLASH_NOR_Pageprogram(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
  /*!< Enable the write access to the FLASH */
  sFLASH_WriteEnable();

  /*!< Select the FLASH: Chip Select low */
  sFLASH_CS_LOW();
  /*!< Send "Write to Memory " instruction */
  sFLASH_SendByte(sFLASH_CMD_NOR_WRITE);
  /*!< Send WriteAddr high nibble address byte to write to */
  sFLASH_SendByte((WriteAddr & 0xFF0000) >> 16);
  /*!< Send WriteAddr medium nibble address byte to write to */
  sFLASH_SendByte((WriteAddr & 0xFF00) >> 8);
  /*!< Send WriteAddr low nibble address byte to write to */
  sFLASH_SendByte(WriteAddr & 0xFF);

  /*!< while there is data to be written on the FLASH */
  while (NumByteToWrite--)
  {
    /*!< Send the current byte */
    sFLASH_SendByte(*pBuffer);
    /*!< Point on the next byte to be written */
    pBuffer++;
  }

  /*!< Deselect the FLASH: Chip Select high */
  sFLASH_CS_HIGH();

  /*!< Wait the end of Flash writing */
  sFLASH_NOR_WaitForWriteEnd();
}

void sFLASH_NOR_Read(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead)
{
  /*!< Select the FLASH: Chip Select low */
  sFLASH_CS_LOW();

  /*!< Send "Read from Memory " instruction */
  sFLASH_SendByte(sFLASH_CMD_NOR_READ);

  /*!< Send ReadAddr high nibble address byte to read from */
  sFLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
  /*!< Send ReadAddr medium nibble address byte to read from */
  sFLASH_SendByte((ReadAddr& 0xFF00) >> 8);
  /*!< Send ReadAddr low nibble address byte to read from */
  sFLASH_SendByte(ReadAddr & 0xFF);

  while (NumByteToRead--) /*!< while there is data to be read */
  {
    /*!< Read a byte from the FLASH */
    *pBuffer = sFLASH_SendByte(sFLASH_DUMMY_BYTE);
    /*!< Point to the next location where the byte read will be saved */
    pBuffer++;
  }

  /*!< Deselect the FLASH: Chip Select high */
  sFLASH_CS_HIGH();
}
void sFLASH_NOR_SFDP_Read(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead)
{
  /*!< Select the FLASH: Chip Select low */
  sFLASH_CS_LOW();

  /*!< Send "Read from Memory " instruction */
  sFLASH_SendByte(sFLASH_CMD_NOR_SFDP );

  /*!< Send ReadAddr high nibble address byte to read from */
  sFLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
  /*!< Send ReadAddr medium nibble address byte to read from */
  sFLASH_SendByte((ReadAddr& 0xFF00) >> 8);
  /*!< Send ReadAddr low nibble address byte to read from */
  sFLASH_SendByte(ReadAddr & 0xFF);
	//dummy clock
	sFLASH_SendByte(0x00);

  while (NumByteToRead--) /*!< while there is data to be read */
  {
    /*!< Read a byte from the FLASH */
    *pBuffer = sFLASH_SendByte(sFLASH_DUMMY_BYTE);
    /*!< Point to the next location where the byte read will be saved */
    pBuffer++;
  }

  /*!< Deselect the FLASH: Chip Select high */
  sFLASH_CS_HIGH();
}
uint8_t sFLASH_NOR_ReadLowSR( void )
{
	 uint8_t data = 0;
	 sFLASH_CS_LOW();
   sFLASH_SendByte(sFLASH_CMD_NOR_RDSR);
	 data = sFLASH_SendByte(sFLASH_DUMMY_BYTE );
	 sFLASH_CS_HIGH();
   return data;
}
uint8_t sFLASH_NOR_ReadMidSR( void )
{
	 uint8_t data = 0;
	 sFLASH_CS_LOW();
   sFLASH_SendByte(sFLASH_CMD_NOR_MISR);
	 data = sFLASH_SendByte(sFLASH_DUMMY_BYTE );
	 sFLASH_CS_HIGH();
   return data;
}
uint8_t sFLASH_NOR_ReadHiSR( void )
{
	 uint8_t data = 0;
	 sFLASH_CS_LOW();
   sFLASH_SendByte(sFLASH_CMD_NOR_HISR);
	 data = sFLASH_SendByte(sFLASH_DUMMY_BYTE );
	 sFLASH_CS_HIGH();
   return data;
}
void sFLASH_NOR_WriteLowSR( uint8_t uldata )
{
	
	 sFLASH_WriteEnable();
	 sFLASH_CS_LOW();
   sFLASH_SendByte(0x01);
	 sFLASH_SendByte(uldata);
	 sFLASH_CS_HIGH();
	 sFLASH_NOR_WaitForWriteEnd();
}
void sFLASH_NOR_WriteMidSR( uint8_t uldata )
{
	 
	 sFLASH_WriteEnable();
	 sFLASH_CS_LOW();
   sFLASH_SendByte(sFLASH_CMD_NOR_MISR);
	 sFLASH_SendByte(uldata);
	 sFLASH_CS_HIGH();
	 sFLASH_NOR_WaitForWriteEnd();
}
void sFLASH_NOR_WriteHiSR( uint8_t uldata )
{
	 sFLASH_WriteEnable();
	 sFLASH_CS_LOW();
   sFLASH_SendByte(sFLASH_CMD_NOR_HISR);
	 sFLASH_SendByte(uldata);
	 sFLASH_CS_HIGH();
   sFLASH_NOR_WaitForWriteEnd();
}
void sFLASH_NOR_WriteAllSR( uint32_t uldata)
{
    sFLASH_WriteEnable();   
	  sFLASH_CS_LOW();
    sFLASH_SendByte(sFLASH_CMD_NOR_HISR);
	  sFLASH_SendByte((uldata & 0xFF0000) >> 16);
	  sFLASH_CS_HIGH();
	  sFLASH_WriteEnable();
	  sFLASH_CS_LOW();
    sFLASH_SendByte(sFLASH_CMD_NOR_MISR);
	  sFLASH_SendByte((uldata & 0xFF00) >> 8);
	  sFLASH_CS_HIGH();
	  sFLASH_WriteEnable(); 
	  sFLASH_CS_LOW();
    sFLASH_SendByte(sFLASH_CMD_NOR_RDSR);
	  sFLASH_SendByte((uldata & 0xFF));
	  sFLASH_CS_HIGH();
	  sFLASH_WriteEnable(); 
	  sFLASH_CS_LOW();
    sFLASH_SendByte(sFLASH_CMD_NOR_RDSR);
	  sFLASH_SendByte((uldata & 0xFF));
	  sFLASH_SendByte((uldata & 0xFF00) >> 8);
	  sFLASH_CS_HIGH();
		sFLASH_WriteDisable();
	   
}
//uint32_t sFLASH_NAND_ReadID( void )
//{
////   uint8_t data1 = 0;
//	 uint8_t data2 = 0;
//	 uint8_t data3 = 0;
//	 //uint8_t data4 = 0;
//	 uint32_t data;
//	 sFLASH_CS_LOW();
//	 sFLASH_SendByte(sFLASH_CMD_NOR_RDID);
//	 sFLASH_SendByte(0);
//	 data2 = sFLASH_SendByte(sFLASH_DUMMY_BYTE);
//	 data3 = sFLASH_SendByte(sFLASH_DUMMY_BYTE);
////	 data4 = sFLASH_SendByte(sFLASH_DUMMY_BYTE);
//	 sFLASH_CS_HIGH();
//	 data = (data2<<8)|data3;
//	 return data;
//}
//void sFLASH_NandReset(void)     //NandFlash Reset
//{
//	sFLASH_CS_LOW();
//	sFLASH_SendByte(sFLASH_CMD_NOR_RESET);
//	sFLASH_CS_HIGH();
//}

//uint8_t NandGetFeature(uint8_t Add)  //SPINaned GetFeatures
//{
//	uint8_t temp;
//	sFLASH_CS_LOW();
//	sFLASH_SendByte(sFLASH_CMD_Get_Feature);
//	sFLASH_SendByte(Add);
//	temp = sFLASH_SendByte(sFLASH_DUMMY_BYTE);
//	sFLASH_CS_HIGH(); 
//	return temp;
//}

//void  NandSetFeature(uint8_t Add, unsigned char uldata)  //SPINaned GetFeatures
//{
//	sFLASH_CS_LOW();
//	sFLASH_SendByte(sFLASH_CMD_Set_Feature);
//	sFLASH_SendByte(Add);
//	sFLASH_SendByte(uldata);
//	sFLASH_CS_HIGH(); 
//}

//uint8_t Spi_Nand_Wait_Busy(void)     //Waiting 
//{   
////	unsigned long last_time = MillisecondCounter;
//	unsigned char timeout = 0;
////	while((NandGetFeature(0xC0)&0x01)==0x01)
////	{
////    if( MillisecondCounter >= last_time)
////		{
////        if( MillisecondCounter-last_time >= TIME_OUT_COUNTER )	timeout = 1;
////    }
////		else 
////		{
////        if( 0xffffffff + MillisecondCounter - last_time >=TIME_OUT_COUNTER )    timeout = 1;
////    }
////		if(timeout == 1)  break;
////   }
//	return timeout;
//}  

//uint8_t Nand_read_to_cache(uint32_t ReadAddr)   //PageRead
//{ 											    
//   sFLASH_CS_LOW();
//   sFLASH_SendByte(sFLASH_CMD_READ_TO_CACHE);      //PageRead to cache 
//   sFLASH_SendByte((uint8_t)((ReadAddr)>>16));  //send the Page/Block Add    
//   sFLASH_SendByte((uint8_t)((ReadAddr)>>8));   
//   sFLASH_SendByte((uint8_t)ReadAddr);   
//   sFLASH_CS_HIGH();   
//   Spi_Nand_Wait_Busy();
//	 return NandGetFeature(0xC0);
//}  
///* Read From Cache*/
//void Spi_Nand_Read_from_cache(uint8_t* pBuffer,uint32_t ReadAddr,uint16_t DataLen)  //ReadCache
//{
//	sFLASH_CS_LOW();
//  sFLASH_SendByte(sFLASH_CMD_READ_FROM_CACHE); //PageRead to cache 
//  sFLASH_SendByte((ReadAddr & 0xFF00) >> 8);   //send the Page/Block Add    
//  sFLASH_SendByte(ReadAddr & 0xFF);   
//  sFLASH_SendByte(sFLASH_DUMMY_BYTE); 
//	
//	while(DataLen--) /*!< while there is data to be read */
//  {
//    /*!< Read a byte from the FLASH */
//    *pBuffer = sFLASH_SendByte(sFLASH_DUMMY_BYTE);
//    /*!< Point to the next location where the byte read will be saved */
//    pBuffer++;
//  }
//	sFLASH_CS_HIGH();  
//}
///* Read From Cache for C version*/
//void Spi_Nand_Read_from_cache_c(uint8_t* pBuffer,uint32_t ReadAddr,uint16_t DataLen)  //ReadCache
//{
//	sFLASH_CS_LOW();
//  sFLASH_SendByte(sFLASH_CMD_READ_FROM_CACHE); //PageRead to cache 
//  sFLASH_SendByte((ReadAddr & 0xFF00) >> 8);   //send the Page/Block Add    
//  sFLASH_SendByte(ReadAddr & 0xFF);   
//  sFLASH_SendByte(sFLASH_DUMMY_BYTE); 
//	
//	while(DataLen--) /*!< while there is data to be read */
//  {
//    /*!< Read a byte from the FLASH */
//    *pBuffer = sFLASH_SendByte(sFLASH_DUMMY_BYTE);
//    /*!< Point to the next location where the byte read will be saved */
//    pBuffer++;
//  }
//	sFLASH_CS_HIGH();  
//}

//void sFLASH_NAND_Pageprogram(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
//{

//  /*!< Select the FLASH: Chip Select low */
//  sFLASH_CS_LOW();
//  /*!< Send "Write to Memory " instruction */
//  sFLASH_SendByte(sFLASH_CMD_NAND_WRITE);
//  /*!< Send WriteAddr medium nibble address byte to write to */
//  sFLASH_SendByte((WriteAddr & 0xFF00) >> 8);
//  /*!< Send WriteAddr low nibble address byte to write to */
//  sFLASH_SendByte(WriteAddr & 0xFF);

//  /*!< while there is data to be written on the FLASH */
//  while (NumByteToWrite--)
//  {
//    /*!< Send the current byte */
//    sFLASH_SendByte(*pBuffer);
//    /*!< Point on the next byte to be written */
//    pBuffer++;
//  }

//  /*!< Deselect the FLASH: Chip Select high */
//  sFLASH_CS_HIGH();
//}
//uint8_t sFLASH_NAND_Execute(uint32_t WriteAddr)
//{
//  sFLASH_WriteEnable();
//  /*!< Select the FLASH: Chip Select low */
//  sFLASH_CS_LOW();
//  /*!< Send "Write to Memory " instruction */
//  sFLASH_SendByte(sFLASH_CMD_NAND_W_Execute);
//	 /*!< Send ReadAddr high nibble address byte to read from */
//  sFLASH_SendByte((WriteAddr & 0xFF0000) >> 16);
//  /*!< Send WriteAddr medium nibble address byte to write to */
//  sFLASH_SendByte((WriteAddr & 0xFF00) >> 8);
//  /*!< Send WriteAddr low nibble address byte to write to */
//  sFLASH_SendByte(WriteAddr & 0xFF);
//  /*!< Deselect the FLASH: Chip Select high */
//  sFLASH_CS_HIGH();
//	Spi_Nand_Wait_Busy();
//	 return NandGetFeature(0xC0);
//}

//uint8_t sFLASH_NAND_BlockErase(uint32_t Blockaddr)
//{
//  /*!< Send write enable instruction */
//  sFLASH_WriteEnable();


//  /*!< Select the FLASH: Chip Select low */
//  sFLASH_CS_LOW();
//  /*!< Send Block Erase instruction */
//  sFLASH_SendByte(sFLASH_CMD_NAND_BE );
//  /*!< Send SectorAddr high nibble address byte */
//  sFLASH_SendByte((Blockaddr & 0xFF0000) >> 16);
//  /*!< Send SectorAddr medium nibble address byte */
//  sFLASH_SendByte((Blockaddr & 0xFF00) >> 8);
//  /*!< Send SectorAddr low nibble address byte */
//  sFLASH_SendByte(Blockaddr & 0xFF);
//  /*!< Deselect the FLASH: Chip Select high */
//  sFLASH_CS_HIGH();

//  Spi_Nand_Wait_Busy();
//	return NandGetFeature(0xC0);
//}

//uint8_t Spi_Nand_Read_Num( uint32_t block_num, uint32_t page_num, uint32_t page_add, uint8_t* pBuffer, uint16_t DataLen)
//{
//	 unsigned long block_add;
//	 uint8_t status;
//	 block_add = ((block_num&0x3ff)<<6) | (page_num&0x3f);
//   status = Nand_read_to_cache(block_add);
//	 Spi_Nand_Read_from_cache(pBuffer, page_add, DataLen);
//	 return status;
//}
//uint8_t Spi_Nand_Read_Num_c( uint32_t block_num, uint32_t page_num, uint32_t page_add, uint8_t* pBuffer, uint16_t DataLen)
//{
//	 unsigned long block_add;
//	 uint8_t status;
//	 block_add = ((block_num&0x3ff)<<6) | (page_num&0x3f);
//   status = Nand_read_to_cache(block_add);
//	 Spi_Nand_Read_from_cache_c(pBuffer, page_add, DataLen);
//	 return status;
//}
//uint8_t Spi_Nand_Write_Num( uint32_t block_num, uint32_t page_num, uint32_t page_add, uint8_t* pBuffer, uint16_t DataLen)
//{
//	 unsigned long block_add;
//	 block_add = ((block_num&0x3ff)<<6) | (page_num&0x3f);
//	 sFLASH_NAND_Pageprogram(pBuffer, page_add, DataLen);
//	 return sFLASH_NAND_Execute(block_add);
//}
