#ifndef __SFLASH_SPI_H__
#define __SFLASH_SPI_H__

#define TIMEOUT 1000

/**
  * @brief  Select sFLASH: Chip Select pin low
  */
#define sFLASH_CS_LOW()       GPIO_ResetBits(sFLASH_CS_GPIO_PORT, sFLASH_CS_PIN)
/**
  * @brief  Deselect sFLASH: Chip Select pin high
  */
#define sFLASH_CS_HIGH()      GPIO_SetBits(sFLASH_CS_GPIO_PORT, sFLASH_CS_PIN)   
/**
  * @}
  */ 
#define sFLASH_SPI                       SPI3
#define sFLASH_SPI_CLK                   RCC_APB1Periph_SPI3
#define sFLASH_SPI_SCK_PIN               GPIO_Pin_3                 /* PB.3 */
#define sFLASH_SPI_SCK_GPIO_PORT         GPIOB                       /* GPIOB */
#define sFLASH_SPI_SCK_GPIO_CLK          RCC_APB2Periph_GPIOB
#define sFLASH_SPI_MISO_PIN              GPIO_Pin_4                 /* PB.4 */
#define sFLASH_SPI_MISO_GPIO_PORT        GPIOB                       /* GPIOB */
#define sFLASH_SPI_MISO_GPIO_CLK         RCC_APB2Periph_GPIOB
#define sFLASH_SPI_MOSI_PIN              GPIO_Pin_5                 /* PB.5 */
#define sFLASH_SPI_MOSI_GPIO_PORT        GPIOB                       /* GPIOB */
#define sFLASH_SPI_MOSI_GPIO_CLK         RCC_APB2Periph_GPIOB
#define sFLASH_CS_PIN                    GPIO_Pin_6                 /* PB.6 */
#define sFLASH_CS_GPIO_PORT              GPIOB                       /* GPIOB */
#define sFLASH_CS_GPIO_CLK               RCC_APB2Periph_GPIOB

#define sFLASH_DUMMY_BYTE             0x55
/**
  * @ SPI NOR CMD
  */ 

#define sFLASH_CMD_NOR_WRITE          0x02  /*!< Write to Memory instruction */
#define sFLASH_CMD_NOR_WRSR           0x01  /*!< Write Status Register instruction */
#define sFLASH_CMD_NOR_WREN           0x06  /*!< Write enable instruction */
#define sFLASH_CMD_NOR_WDIS           0x04   /*!< Write disable instruction */
#define sFLASH_CMD_NOR_READ           0x03  /*!< Read from Memory instruction */
#define sFLASH_CMD_NOR_SFDP           0x5A  /*!< Read from Memory instruction */
#define sFLASH_CMD_NOR_RDSR           0x05  /*!< Read Status Register instruction  */
#define sFLASH_CMD_NOR_MISR           0x35  /*!< Read Status Register instruction Midde Byte */
#define sFLASH_CMD_NOR_HISR           0x15  /*!< Read Status Register instruction Hige Byte */
#define sFLASH_CMD_NOR_RDID           0x9F  /*!< Read identification */
#define sFLASH_CMD_NOR_SE             0x20  /*!< Sector Erase instruction */
#define sFLASH_CMD_NOR_BE             0xD8  /*!< Block Erase instruction */
#define sFLASH_CMD_NOR_CE             0xC7  /*!< Chip Erase instruction */

#define sFLASH_WIP_FLAG           0x01  /*!< Write In Progress (WIP) flag */
/**
  * @ SPI NAND CMD
  */ 
#define sFLASH_CMD_NOR_RESET          0xFF  /*!< RESET TO NAND*/ 
#define sFLASH_CMD_Get_Feature        0x0F  /* GET Feature */
#define sFLASH_CMD_Set_Feature        0x1F  /* Set Feature */  
#define sFLASH_CMD_READ_TO_CACHE      0x13   /*Read to cache*/
#define sFLASH_CMD_READ_FROM_CACHE    0x03  /*!< Read from Memory instruction */
#define sFLASH_CMD_NAND_WRITE         0x84  /*!< Write to Memory instruction */
#define sFLASH_CMD_NAND_WREN          0x06
#define sFLASH_CMD_NAND_W_Execute     0x10  /*!< Write to Memory instruction */
#define sFLASH_CMD_NAND_BE            0xD8

#define TIME_OUT_COUNTER          50

//void  NandSetFeature(uint8_t Add, unsigned char uldata);
//uint8_t NandGetFeature(uint8_t Add);  //SPINaned GetFeatures

void sFLASH_WriteEnable(void);
void sFLASH_WriteDisable(void);
/**
  * @ SPI Nor function
  */ 
uint32_t sFLASH_NOR_ReadID( void );
void sFLASH_NOR_WaitForWriteEnd(void);
void sFLASH_NOR_ChipErase(void);
void sFLASH_NOR_SectorErase(uint32_t SectorAddr);
void sFLASH_NOR_BlockErase(uint32_t BlockAddr);
void sFLASH_NOR_Read(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead);
void sFLASH_NOR_Pageprogram(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
void sFLASH_NOR_SFDP_Read(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead);
void sFLASH_NOR_WriteLowSR( uint8_t uldata );
void sFLASH_NOR_WriteMidSR( uint8_t uldata );
void sFLASH_NOR_WriteHiSR( uint8_t uldata );
uint8_t sFLASH_NOR_ReadLowSR( void );
uint8_t sFLASH_NOR_ReadMidSR( void );
uint8_t sFLASH_NOR_ReadHiSR( void );
uint8_t NandGetFeature(uint8_t Add);
void sFLASH_NOR_WriteAllSR( uint32_t uldata);
/**
  * @ SPI Nand function
  */ 
//uint32_t sFLASH_NAND_ReadID( void );
//void sFLASH_NandReset(void);     //NandFlash Reset
//uint8_t NandGetFeature(uint8_t Add);  //SPINaned GetFeatures
//void  NandSetFeature(uint8_t Add, unsigned char uldata);  //SPINaned GetFeatures
//uint8_t Spi_Nand_Wait_Busy(void);     //Waiting 
//uint8_t Nand_read_to_cache(uint32_t ReadAddr);   //PageRead
//void Spi_Nand_Read_from_cache(uint8_t* pBuffer,uint32_t ReadAddr,uint16_t DataLen);  //ReadCache
//void Spi_Nand_Read_from_cache_c(uint8_t* pBuffer,uint32_t ReadAddr,uint16_t DataLen);   //ReadCache for C_version
//void sFLASH_NAND_Pageprogram(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
//uint8_t sFLASH_NAND_Execute(uint32_t WriteAddr);
//uint8_t sFLASH_NAND_BlockErase(uint32_t Blockaddr);
//uint8_t Spi_Nand_Read_Num( uint32_t block_num, uint32_t page_num, uint32_t page_add, uint8_t* pBuffer, uint16_t DataLen);
//uint8_t Spi_Nand_Read_Num_c( uint32_t block_num, uint32_t page_num, uint32_t page_add, uint8_t* pBuffer, uint16_t DataLen); //for C version
//uint8_t Spi_Nand_Write_Num( uint32_t block_num, uint32_t page_num, uint32_t page_add, uint8_t* pBuffer, uint16_t DataLen);



void sFLASH_Hardware_Init(void);
uint8_t sFLASH_ReadByte(void);
uint8_t sFLASH_SendByte(uint8_t byte);
#endif // __SFLASH_SPI_H__
