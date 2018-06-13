#define USB_PRIVATE
#include "usbtask.h"
#include "stm32_flash.h"
static uint8_t fontframcnt = 0;//�����ֿ�֡����
static uint32_t fontaddr    = 0;//�ֿ��ַָ��
extern uint8_t wav_buf[1024];

/****************************************************
�������ƣ�USB_DataInit
�������ܣ�USB���ݳ�ʼ��
*****************************************************/
void USB_DataInit(void)
{
    memset(USB_Rx_Buffer,0,sizeof(USB_Rx_Buffer));
    memset(USB_Tx_Buffer,0,sizeof(USB_Tx_LENGHT));
    USBRxPointer = 0;
    FrameCount = 0;
    txFlag = 0;
    usbtimecount = 0;
}
/************************************************
�������ƣ�USB_Send(uint8_t *data, uint16_t len)
�������ܣ�USB���ݷ��ͺ���
����   �� uint8_t *data Ҫ���͵�����, �������ݵĳ��� uint16_t len
**************************************************/
void USB_Send(uint8_t *data, uint16_t len)
{
   memset(USB_Tx_Buffer,0,sizeof(USB_Tx_Buffer)); 
    memcpy(USB_Tx_Buffer,data,len);
    txlen = len;
    UserToPMABufferCopy(USB_Tx_Buffer, ENDP1_TXADDR, txlen);
    SetEPTxCount(ENDP1, txlen);
    SetEPTxValid(ENDP1);    
}

/******************************************************
��������:USB_Task
�������ܣ�USB��������
������

PC������      ֡ͷ            ����                 ����    DATA
              0x7e   lenght_L lenght_H             CMD    N
*******************************************************/
void USB_Task(void)
{
    uint16_t i = 0;
    uint8_t sendbuffer[20];
   // uint8_t databuff[2048];
    u16 temp[2] = {0xAA55,0};
    uint32_t address = 0,len = 0;
    if(USBRxPointer !=0)
    {
        		//�жϴ���֡��ʱ
		if(usbtimecount!=0)
		{
			if (GetTickCount() <  usbtimecount)
			{
				usbtimecount = GetTickCount();
			}
			else		
			{
				if ((GetTickCount() - usbtimecount) > 2000)//��ʱʱ��1s
				{
					USBRxPointer = 0;
					memset(USB_Rx_Buffer,0,sizeof(USB_Rx_Buffer));
				}	
			}
            if(USBRxPointer >= (USB_Rx_Buffer[1]+ USB_Rx_Buffer[2]*256))
            {
               usbtimecount = 0;
               USBRxPointer = 0;
                switch(USB_Rx_Buffer[3])
                {
                    case DOWNFONTERASE:
                        //�����ֿ�����
                        for(i = 0;i < NORFLASH_FONT_SECTORS;i++)
                        {
                           sFLASH_NOR_SectorErase(NORFLASH_FONT_BASE_SECTOR+i*4096); 
                            //ι��
                        }
//                    sFLASH_NOR_ChipErase();
                    sendbuffer[0] = 0x7e;
                    sendbuffer[1] = 0x04;
                    sendbuffer[2] = 0x00;
                    sendbuffer[3] = DOWNFONTERASE_OK;
                    fontframcnt = 0;//�����ֿ�֡��������
                    fontaddr = NORFLASH_FONT_BASE_SECTOR;
                    USB_Send(sendbuffer,4);
                        
                        break;
                    case DOWNFONT:
                        if(USB_Rx_Buffer[4] == fontframcnt)
                        {
                            
                            for(i = 0;i<8;i++ )
                            {
                                sFLASH_NOR_Pageprogram(&USB_Rx_Buffer[5+256*i],fontaddr,256); 
                                sFLASH_NOR_Read(&wav_buf[256*i],fontaddr,256);
                                 fontaddr+=256;
                            }
                            
                            
                           
                            sendbuffer[0] = 0x7e;
                            sendbuffer[1] = 0x04;
                            sendbuffer[2] = 0x00;
                            sendbuffer[3] = DOWN_OK;
                            //fontframcnt = 0;//�����ֿ�֡��������
                            //fontaddr = NORFLASH_FONT_BASE_SECTOR;
                            USB_Send(sendbuffer,4);
                        }
                        else
                        {
                            
                            sendbuffer[0] = 0x7e;
                            sendbuffer[1] = 0x04;
                            sendbuffer[2] = 0x00;
                            sendbuffer[3] = DOWNERR;
                            USB_Send(sendbuffer,4);
                            //����
                        }
                        fontframcnt++;
                        break;
                    case 0x77:
                        break;
                    case WRITEFLASH:  //дflash

                         address = (uint32_t)(USB_Rx_Buffer[4] + (uint32_t)(USB_Rx_Buffer[5]<<8) + (uint32_t)(USB_Rx_Buffer[6]<<16) + (uint32_t)(USB_Rx_Buffer[7]<<24));
                         len = (USB_Rx_Buffer[1] + (uint16_t)(USB_Rx_Buffer[2]<<8))-8;
//                         for(i = 0;i<8;i++ )
//                          {
//                             sFLASH_NOR_Pageprogram(&USB_Rx_Buffer[5+256*i],address,256); 
//                             //sFLASH_NOR_Read(&databuff[256*i],fontaddr,256);
//                              address+=256;
//                          }
                         norflash_write_flash(address,&USB_Rx_Buffer[8],len);
//                         sFLASH_NOR_Read(wav_buf,address,len);
//                         for(i = 0;i < len;i ++)
//                         {
//                             if(USB_Rx_Buffer[8+i]!=wav_buf[i])
//                             {
//                                 
//                                 break;
//                             }
//                         }
                         sendbuffer[0] = 0x7e;
                         sendbuffer[1] = 0x04;
                         sendbuffer[2] = 0x00;
                         sendbuffer[3] = WRITEFINSH;
                         USB_Send(sendbuffer,4);
                        break;
                    case DOWN_APP_START:
                        //flash���ñ��λ
                       //��λ
                        STMFLASH_Write(0x8005000-10,temp,2);
                    
                    	SCB->AIRCR	= ((0x5FA << 16) | (SCB->AIRCR & (0x700)) | (1 << 2)); 
			             while(1)
                         {};
                      
                    default:
                        break;
                }
            }
            else
            {
 
            }
		}
    }
}
















