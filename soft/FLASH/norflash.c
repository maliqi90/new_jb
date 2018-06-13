#include "norflash.h"

 extern uint8_t event_write_flag;
 extern uint8_t wav_buf[2048];
 static uint8_t flash_buff[4096];
 //��ȡͨ������
 void norflash_read_comm_set(uint8_t *set)
 {
	sFLASH_NOR_Read((uint8_t*)set,NORFLASH_COMM_SET_BASE,4);
 }
 //д��ͨ������
 void norflash_write_comm_set(uint8_t *set)
 {
	uint32_t address = 0;
	uint16_t sector_id = 0;
	uint16_t j = 0,i = 0;
	//�ȴ�flash����������
	address = NORFLASH_COMM_SET_BASE;
	
	sector_id = address/4096;
	sFLASH_NOR_Read(flash_buff,sector_id*4096,4096);
	
	//��������
	sFLASH_NOR_SectorErase(sector_id*4096);
	//�޸Ļ�������
	j = address%4096;
	for(i = 0;i < 4;i++)
	{
		flash_buff[j+i] = set[i];
	}
	//�ѻ���д��flash
    for(i = 0;i < 16;i ++)
    {
        sFLASH_NOR_Pageprogram(&flash_buff[i*256],sector_id*4096+i*256,256);
    }	 
 }
 //��ȡ����ֵ
 void norflash_read_LightSet(uint16_t *led)
 {
     sFLASH_NOR_Read((uint8_t*)led,NORFLASH_LIGHT_SET,2);
 }
 //�洢����ֵ
 void norflash_write_LightSet(uint16_t led)
 {
 	uint32_t address = 0;
	uint16_t sector_id = 0;
	uint16_t j = 0,i = 0;
	//�ȴ�flash����������
	address = NORFLASH_LIGHT_SET ;
	
	sector_id = address/4096;
	sFLASH_NOR_Read(flash_buff,sector_id*4096,4096);
	
	//��������
	sFLASH_NOR_SectorErase(sector_id*4096);
	//�޸Ļ�������
	j = address%4096;

     flash_buff[j] = (uint8_t)led;
     flash_buff[j+1] = (uint8_t)(led>>8);

	//�ѻ���д��flash
    for(i = 0;i < 16;i ++)
    {
        sFLASH_NOR_Pageprogram(&flash_buff[i*256],sector_id*4096+i*256,256);
    }    
 }
/********************************************************
�������ƣ�norflash_read_font()
�������ܣ���ȡflash�ֿ�

**********************************************************/
void norflash_read_font(uint32_t fontaddr,uint8_t *buff,uint8_t len)
{
    sFLASH_NOR_Read((uint8_t*)buff,fontaddr,len);
}
/******************************************************
  �������ƣ�norflash_read_device
	�������ܣ���ȡ�ֳ��豸����
	����    ��U8 addr �豸��ţ�SYSTEM_DEVICE *device �豸�����ṹ��
	
*******************************************************/
void norflash_read_device(uint8_t addr, DEVICE_CONFIG *pdevice)
{
	uint32_t address = 0;
	
	address = NORFLASH_DEVICE_BASE+(addr-1)*8;
	sFLASH_NOR_Read((uint8_t*)pdevice,address,8);
}
/***************************************************************
     �������ƣ�norflash_write_deivce
     �������ܣ�д��flash�ֳ��豸����
     ����    ��U8 addr �豸��ţ�SYSTEM_DEVICE *device �豸�����ṹ��
****************************************************************/
void norflash_write_device(uint8_t addr,uint8_t *pdevice)
{
	uint32_t address = 0;
	uint16_t sector_id = 0;
	uint16_t j = 0,i = 0;
	//�ȴ�flash����������
	address = NORFLASH_DEVICE_BASE + (addr-1)*8;
	
	sector_id = address/4096;
	sFLASH_NOR_Read(flash_buff,sector_id*4096,4096);
	
	//��������
	sFLASH_NOR_SectorErase(sector_id*4096);
	//�޸Ļ�������
	j = address%4096;
	for(i = 0;i < 8;i++)
	{
		flash_buff[j+i] = pdevice[i];
	}
	//�ѻ���д��flash
    for(i = 0;i < 16;i ++)
    {
        sFLASH_NOR_Pageprogram(&flash_buff[i*256],sector_id*4096+i*256,256);
    }
	//sFLASH_NOR_Pageprogram(buff,sector_id*4096,4096);
	
}

/*******************************************************
��������:norflash_read_phone(uint8_t addr,uint8_t *data)
�������ܣ���ȡ�洢�ĵ绰����
����    ��uint8_t addr �绰�����ַ,uint8_t *data �绰��������
**********************************************************/
void norflash_read_phone(uint8_t addr,uint8_t *data)
{
	uint32_t address = 0;
	address  = NORFLASH_PHONE_BASE + (addr-1)*12;
	sFLASH_NOR_Read(data,address,11);
}
/********************************************************
�������ƣ�norflash_write_phone
�������ܣ�д��绰����
����    ��uint8_t addr �绰�����ַ,uint8_t *data �绰��������
*********************************************************/
void norflash_write_phone(uint8_t addr,uint8_t *data)
{
	uint32_t address = 0;
	uint16_t sector_id = 0;
	uint16_t j = 0,i = 0;
	//�ȴ�flash����������
	address = NORFLASH_PHONE_BASE + (addr-1)*12;
	
	sector_id = address/4096;
	sFLASH_NOR_Read(flash_buff,sector_id*4096,4096);
	
	//��������
	sFLASH_NOR_SectorErase(sector_id*4096);
	//�޸Ļ�������
	j = address%4096;
	for(i = 0;i < 12;i++)
	{
		flash_buff[j+i] = data[i];
	}
	//�ѻ���д��flash
	//sFLASH_NOR_Pageprogram(buff,sector_id*4096,4096);	
     for(i = 0;i < 16;i ++)
    {
//         sFLASH_NOR_Read(&buff[256],4096,256);
        sFLASH_NOR_Pageprogram(&flash_buff[i*256],sector_id*4096+i*256,256);
       	
    }
}

/*********************************************************
�������ƣ�norflash_read_Alarmevent(uint16_t AlarmId,SYSTEM_EVENT *pevent)
�������ܣ���norflash�ж�ȡ�����¼�
����    ��uint16_t AlamId �����¼�ID,SYSTEM_EVENT *pevent �����¼��ṹ��
***********************************************************/
void norflash_read_AlarmEvent(uint16_t AlamId,SYSTEM_EVENT *pevent)
{
	uint32_t address = 0;
	address  = NORFLASH_FIREEVENT_BASE + (AlamId-1)*32;
	sFLASH_NOR_Read((uint8_t*)pevent,address,32);	
}

/********************************************************
�������ƣ�norflash_write_AlarmEvent
�������ܣ�д����¼�
����    ��uint16_t AlamId �����¼�ID,SYSTEM_EVENT *pevent �����¼��ṹ��
*********************************************************/
void norflash_write_AlarmEvent(uint16_t AlamId,uint8_t *pevent)
{
	uint32_t address = 0;
	uint16_t sector_id = 0;
	uint16_t j = 0,i = 0;
	//�ȴ�flash����������
	address = NORFLASH_FIREEVENT_BASE + (AlamId-1)*32;
	
	sector_id = address/4096;
	sFLASH_NOR_Read(flash_buff,sector_id*4096,4096);
	
	//��������
	sFLASH_NOR_SectorErase(sector_id*4096);
    if((event_write_flag&0x80) != 0x80)
    {
        //�޸Ļ�������
        j = address%4096;
        for(i = 0;i < 32;i++)
        {
            flash_buff[j+i] = pevent[i];
        }
        //�ѻ���д��flash

    }
    else
    {
        for(i = 0;i < 4064;i++)
        {
            flash_buff[i] = flash_buff[i+32];
        }
         for(i = 0;i < 32;i++)
        {
            flash_buff[4064+i] = pevent[i];
        }
        
    }
    for(i = 0;i < 16;i ++)
    {
       sFLASH_NOR_Pageprogram(&flash_buff[i*256],sector_id*4096+i*256,256);
    }
	//sFLASH_NOR_Pageprogram(buff,sector_id*4096,4096);	
}
/****************************************************
  �������ƣ�norflash_read_SheildEvent
  �������ܣ���ȡ�����¼�
  ����    ��uint16_t SheildId �����¼�ID,SYSTEM_EVENT *pevent �����¼��ṹ��
*****************************************************/
void norflash_read_SheildEvent(uint16_t SheildId,SYSTEM_EVENT *pevent)
{
	uint32_t address = 0;
	address  = NORFLASH_SHIELD_BASE + (SheildId-1)*32;
	sFLASH_NOR_Read((uint8_t*)pevent,address,32);		
}
/******************************************************************************
  �������ƣ�norflash_write_SheildEvent
  �������ܣ�д��flash�����¼�
  ����    ��uint16_t SheildId �����¼�ID,SYSTEM_EVENT *pevent �����¼��ṹ��
******************************************************************************/
void norflash_write_SheildEvent(uint16_t SheildId,uint8_t *pevent)
{
	uint32_t address = 0;
	uint16_t sector_id = 0;
	uint16_t j = 0,i = 0;
	//�ȴ�flash����������
	address = NORFLASH_SHIELD_BASE + (SheildId-1)*32;
	
	sector_id = address/4096;
	sFLASH_NOR_Read(flash_buff,sector_id*4096,4096);
	
	//��������
	sFLASH_NOR_SectorErase(sector_id*4096);
	//�޸Ļ�������
	j = address%4096;
	for(i = 0;i < 32;i++)
	{
		flash_buff[j+i] = pevent[i];
	}
	//�ѻ���д��flash
     for(i = 0;i < 16;i ++)
    {
        sFLASH_NOR_Pageprogram(&flash_buff[i*256],sector_id*4096+i*256,256);
    }
	//sFLASH_NOR_Pageprogram(buff,sector_id*4096,4096);		
}
/*****************************************************************
   �������ƣ�norflash_read_otherevent
   �������ܣ���ȡ�����¼�
   ����    ��uint16_t OtherEventId �¼�ID; SYSTEM_EVENT *pevent�¼��ṹ��
******************************************************************/
void norflash_read_otherevent(uint16_t OtherEventId,SYSTEM_EVENT *pevent)
{
    uint8_t temp;
	uint32_t address = 0;
    temp = event_write_flag>>1;
	address  = NORFLASH_EVENT_BASE +temp*4096 + (OtherEventId-1)*32;
    if(address >= (NORFLASH_EVENT_BASE +32*4096))
    {
        address -=32*4096;
    }
	sFLASH_NOR_Read((uint8_t*)pevent,address,32);	
}
/*********************************************************************
�������ƣ�norflash_write_otherevent(uint16_t OtherEventId,SYSTEM_EVENT *pevent)
�������ܣ�д�������¼�
����    ��uint16_t OtherEventId �¼�ID; SYSTEM_EVENT *pevent�¼��ṹ��
***********************************************************************/
void norflash_write_otherevent(uint16_t OtherEventId,uint8_t *pevent)
{
	uint32_t address = 0;
	uint16_t sector_id = 0;
	uint16_t j = 0,i = 0;
	//�ȴ�flash����������
    if((event_write_flag&0x06) == 0x00)
    {
	  address = NORFLASH_EVENT_BASE + (OtherEventId-1)*32;
    }
    else if((event_write_flag&0x06) == 0x02)
    {
//        address =  NORFLASH_EVENT_BASE + (OtherEventId-1)*32 - ;
    }
    else if((event_write_flag&0x06) == 0x04)
    {
        
    }
	
	sector_id = address/4096;
	sFLASH_NOR_Read(flash_buff,sector_id*4096,4096);
	
	//��������
	sFLASH_NOR_SectorErase(sector_id*4096);
	//�޸Ļ�������
	j = address%4096;
    if((event_write_flag&0x01) ==0x01)
	for(i = 0;i < 32;i++)
	{
		flash_buff[j+i] = pevent[i];
	}
	//�ѻ���д��flash
    for(i = 0;i < 16;i ++)
    {
        sFLASH_NOR_Pageprogram(&flash_buff[i*256],sector_id*4096+i*256,256);
    }
	//sFLASH_NOR_Pageprogram(buff,sector_id*4096,4096);	
}
/***********************************************************
�������ƣ�norflash_write_flash(uint32_t address,uint8_t *data,uint32_t len)
�������ܣ�д��flash����
��    ��: uint32_t address д����׵�ַ,uint8_t *data д�����ݵĵ�ַ��uint32_t len д�����ݵĳ���
����ֵ  ��false д��ʧ��  true д��ɹ�
��ע���ú������д�����ݳ���4096�ֽ�
************************************************************/
void norflash_write_flash(uint32_t address,uint8_t *data,uint32_t len)
{
    uint32_t sector_address = 0;

    uint16_t j = 0,i = 0;
    uint32_t poniter = 0;
    
    //�ȴ�flash��ȡ���ݵ�������
    sector_address = (address/4096)*4096;
    sFLASH_NOR_Read(flash_buff,sector_address,4096);
    
    //��������
    sFLASH_NOR_SectorErase(sector_address);
    
    //�޸Ļ���������
    j = address % 4096;
    for(i = 0;i < len;i ++)
    {
        flash_buff[j+i] = *data++;
        
    }
    poniter = sector_address;
   	//�ѻ���д��flash
    for(i = 0;i < 16;i++)
    {
	   sFLASH_NOR_Pageprogram(&flash_buff[i*256],poniter,256);
//        sFLASH_NOR_Read(wav_buf,poniter,256); 
//        for(j = 0;j < 256;j++)
//        {
//            if(flash_buff[i*256+j]!=wav_buf[j])
//            {
//                break;
//            }
//        }        
        poniter+=256;
    }
   
}
void norflash_write_eventflag(uint8_t data)
{
	uint16_t sector_id = 0;
	uint16_t j = 0,i = 0;
	//�ȴ�flash����������

	
	sector_id = NORFLASH_EVENT_FLAG_BASE/4096;
	sFLASH_NOR_Read(flash_buff,sector_id*4096,4096);
	
	//��������
	sFLASH_NOR_SectorErase(sector_id*4096);
	//�޸Ļ�������
	j = NORFLASH_EVENT_FLAG_BASE%4096;


		flash_buff[j] = data;

	//�ѻ���д��flash
    for(i = 0;i < 16;i ++)
    {
        sFLASH_NOR_Pageprogram(&flash_buff[i*256],sector_id*4096+i*256,256);
    }    
}
uint8_t norflash_read_eventflag(uint8_t  *data)
{
	uint32_t address = 0;
	
	sFLASH_NOR_Read((uint8_t*)data,NORFLASH_EVENT_FLAG_BASE,1);	
  return 1;	
}
