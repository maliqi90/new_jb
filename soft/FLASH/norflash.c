#include "norflash.h"

 extern uint8_t event_write_flag;
 extern uint8_t wav_buf[2048];
 static uint8_t flash_buff[4096];
 //读取通信设置
 void norflash_read_comm_set(uint8_t *set)
 {
	sFLASH_NOR_Read((uint8_t*)set,NORFLASH_COMM_SET_BASE,4);
 }
 //写入通信配置
 void norflash_write_comm_set(uint8_t *set)
 {
	uint32_t address = 0;
	uint16_t sector_id = 0;
	uint16_t j = 0,i = 0;
	//先从flash读到缓存区
	address = NORFLASH_COMM_SET_BASE;
	
	sector_id = address/4096;
	sFLASH_NOR_Read(flash_buff,sector_id*4096,4096);
	
	//擦除扇区
	sFLASH_NOR_SectorErase(sector_id*4096);
	//修改缓存数据
	j = address%4096;
	for(i = 0;i < 4;i++)
	{
		flash_buff[j+i] = set[i];
	}
	//把缓存写入flash
    for(i = 0;i < 16;i ++)
    {
        sFLASH_NOR_Pageprogram(&flash_buff[i*256],sector_id*4096+i*256,256);
    }	 
 }
 //读取背光值
 void norflash_read_LightSet(uint16_t *led)
 {
     sFLASH_NOR_Read((uint8_t*)led,NORFLASH_LIGHT_SET,2);
 }
 //存储背光值
 void norflash_write_LightSet(uint16_t led)
 {
 	uint32_t address = 0;
	uint16_t sector_id = 0;
	uint16_t j = 0,i = 0;
	//先从flash读到缓存区
	address = NORFLASH_LIGHT_SET ;
	
	sector_id = address/4096;
	sFLASH_NOR_Read(flash_buff,sector_id*4096,4096);
	
	//擦除扇区
	sFLASH_NOR_SectorErase(sector_id*4096);
	//修改缓存数据
	j = address%4096;

     flash_buff[j] = (uint8_t)led;
     flash_buff[j+1] = (uint8_t)(led>>8);

	//把缓存写入flash
    for(i = 0;i < 16;i ++)
    {
        sFLASH_NOR_Pageprogram(&flash_buff[i*256],sector_id*4096+i*256,256);
    }    
 }
/********************************************************
函数名称：norflash_read_font()
函数功能：读取flash字库

**********************************************************/
void norflash_read_font(uint32_t fontaddr,uint8_t *buff,uint8_t len)
{
    sFLASH_NOR_Read((uint8_t*)buff,fontaddr,len);
}
/******************************************************
  函数名称：norflash_read_device
	函数功能：读取现场设备配置
	参数    ：U8 addr 设备编号；SYSTEM_DEVICE *device 设备参数结构体
	
*******************************************************/
void norflash_read_device(uint8_t addr, DEVICE_CONFIG *pdevice)
{
	uint32_t address = 0;
	
	address = NORFLASH_DEVICE_BASE+(addr-1)*8;
	sFLASH_NOR_Read((uint8_t*)pdevice,address,8);
}
/***************************************************************
     函数名称：norflash_write_deivce
     函数功能：写入flash现场设备配置
     参数    ：U8 addr 设备编号；SYSTEM_DEVICE *device 设备参数结构体
****************************************************************/
void norflash_write_device(uint8_t addr,uint8_t *pdevice)
{
	uint32_t address = 0;
	uint16_t sector_id = 0;
	uint16_t j = 0,i = 0;
	//先从flash读到缓存区
	address = NORFLASH_DEVICE_BASE + (addr-1)*8;
	
	sector_id = address/4096;
	sFLASH_NOR_Read(flash_buff,sector_id*4096,4096);
	
	//擦除扇区
	sFLASH_NOR_SectorErase(sector_id*4096);
	//修改缓存数据
	j = address%4096;
	for(i = 0;i < 8;i++)
	{
		flash_buff[j+i] = pdevice[i];
	}
	//把缓存写入flash
    for(i = 0;i < 16;i ++)
    {
        sFLASH_NOR_Pageprogram(&flash_buff[i*256],sector_id*4096+i*256,256);
    }
	//sFLASH_NOR_Pageprogram(buff,sector_id*4096,4096);
	
}

/*******************************************************
函数名称:norflash_read_phone(uint8_t addr,uint8_t *data)
函数功能：读取存储的电话号码
参数    ：uint8_t addr 电话号码地址,uint8_t *data 电话号码数组
**********************************************************/
void norflash_read_phone(uint8_t addr,uint8_t *data)
{
	uint32_t address = 0;
	address  = NORFLASH_PHONE_BASE + (addr-1)*12;
	sFLASH_NOR_Read(data,address,11);
}
/********************************************************
函数名称：norflash_write_phone
函数功能：写入电话号码
参数    ：uint8_t addr 电话号码地址,uint8_t *data 电话号码数组
*********************************************************/
void norflash_write_phone(uint8_t addr,uint8_t *data)
{
	uint32_t address = 0;
	uint16_t sector_id = 0;
	uint16_t j = 0,i = 0;
	//先从flash读到缓存区
	address = NORFLASH_PHONE_BASE + (addr-1)*12;
	
	sector_id = address/4096;
	sFLASH_NOR_Read(flash_buff,sector_id*4096,4096);
	
	//擦除扇区
	sFLASH_NOR_SectorErase(sector_id*4096);
	//修改缓存数据
	j = address%4096;
	for(i = 0;i < 12;i++)
	{
		flash_buff[j+i] = data[i];
	}
	//把缓存写入flash
	//sFLASH_NOR_Pageprogram(buff,sector_id*4096,4096);	
     for(i = 0;i < 16;i ++)
    {
//         sFLASH_NOR_Read(&buff[256],4096,256);
        sFLASH_NOR_Pageprogram(&flash_buff[i*256],sector_id*4096+i*256,256);
       	
    }
}

/*********************************************************
函数名称：norflash_read_Alarmevent(uint16_t AlarmId,SYSTEM_EVENT *pevent)
函数功能：从norflash中读取报警事件
参数    ：uint16_t AlamId 报警事件ID,SYSTEM_EVENT *pevent 报警事件结构体
***********************************************************/
void norflash_read_AlarmEvent(uint16_t AlamId,SYSTEM_EVENT *pevent)
{
	uint32_t address = 0;
	address  = NORFLASH_FIREEVENT_BASE + (AlamId-1)*32;
	sFLASH_NOR_Read((uint8_t*)pevent,address,32);	
}

/********************************************************
函数名称：norflash_write_AlarmEvent
函数功能：写入火警事件
参数    ：uint16_t AlamId 报警事件ID,SYSTEM_EVENT *pevent 报警事件结构体
*********************************************************/
void norflash_write_AlarmEvent(uint16_t AlamId,uint8_t *pevent)
{
	uint32_t address = 0;
	uint16_t sector_id = 0;
	uint16_t j = 0,i = 0;
	//先从flash读到缓存区
	address = NORFLASH_FIREEVENT_BASE + (AlamId-1)*32;
	
	sector_id = address/4096;
	sFLASH_NOR_Read(flash_buff,sector_id*4096,4096);
	
	//擦除扇区
	sFLASH_NOR_SectorErase(sector_id*4096);
    if((event_write_flag&0x80) != 0x80)
    {
        //修改缓存数据
        j = address%4096;
        for(i = 0;i < 32;i++)
        {
            flash_buff[j+i] = pevent[i];
        }
        //把缓存写入flash

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
  函数名称：norflash_read_SheildEvent
  函数功能：读取屏蔽事件
  参数    ：uint16_t SheildId 屏蔽事件ID,SYSTEM_EVENT *pevent 屏蔽事件结构体
*****************************************************/
void norflash_read_SheildEvent(uint16_t SheildId,SYSTEM_EVENT *pevent)
{
	uint32_t address = 0;
	address  = NORFLASH_SHIELD_BASE + (SheildId-1)*32;
	sFLASH_NOR_Read((uint8_t*)pevent,address,32);		
}
/******************************************************************************
  函数名称：norflash_write_SheildEvent
  函数功能：写入flash屏蔽事件
  参数    ：uint16_t SheildId 屏蔽事件ID,SYSTEM_EVENT *pevent 屏蔽事件结构体
******************************************************************************/
void norflash_write_SheildEvent(uint16_t SheildId,uint8_t *pevent)
{
	uint32_t address = 0;
	uint16_t sector_id = 0;
	uint16_t j = 0,i = 0;
	//先从flash读到缓存区
	address = NORFLASH_SHIELD_BASE + (SheildId-1)*32;
	
	sector_id = address/4096;
	sFLASH_NOR_Read(flash_buff,sector_id*4096,4096);
	
	//擦除扇区
	sFLASH_NOR_SectorErase(sector_id*4096);
	//修改缓存数据
	j = address%4096;
	for(i = 0;i < 32;i++)
	{
		flash_buff[j+i] = pevent[i];
	}
	//把缓存写入flash
     for(i = 0;i < 16;i ++)
    {
        sFLASH_NOR_Pageprogram(&flash_buff[i*256],sector_id*4096+i*256,256);
    }
	//sFLASH_NOR_Pageprogram(buff,sector_id*4096,4096);		
}
/*****************************************************************
   函数名称：norflash_read_otherevent
   函数功能：读取其他事件
   参数    ：uint16_t OtherEventId 事件ID; SYSTEM_EVENT *pevent事件结构体
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
函数名称：norflash_write_otherevent(uint16_t OtherEventId,SYSTEM_EVENT *pevent)
函数功能：写入其他事件
参数    ：uint16_t OtherEventId 事件ID; SYSTEM_EVENT *pevent事件结构体
***********************************************************************/
void norflash_write_otherevent(uint16_t OtherEventId,uint8_t *pevent)
{
	uint32_t address = 0;
	uint16_t sector_id = 0;
	uint16_t j = 0,i = 0;
	//先从flash读到缓存区
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
	
	//擦除扇区
	sFLASH_NOR_SectorErase(sector_id*4096);
	//修改缓存数据
	j = address%4096;
    if((event_write_flag&0x01) ==0x01)
	for(i = 0;i < 32;i++)
	{
		flash_buff[j+i] = pevent[i];
	}
	//把缓存写入flash
    for(i = 0;i < 16;i ++)
    {
        sFLASH_NOR_Pageprogram(&flash_buff[i*256],sector_id*4096+i*256,256);
    }
	//sFLASH_NOR_Pageprogram(buff,sector_id*4096,4096);	
}
/***********************************************************
函数名称：norflash_write_flash(uint32_t address,uint8_t *data,uint32_t len)
函数功能：写入flash数据
参    数: uint32_t address 写入的首地址,uint8_t *data 写入数据的地址，uint32_t len 写入数据的长度
返回值  ：false 写入失败  true 写入成功
备注：该函数最大写入数据长度4096字节
************************************************************/
void norflash_write_flash(uint32_t address,uint8_t *data,uint32_t len)
{
    uint32_t sector_address = 0;

    uint16_t j = 0,i = 0;
    uint32_t poniter = 0;
    
    //先从flash读取数据到缓冲区
    sector_address = (address/4096)*4096;
    sFLASH_NOR_Read(flash_buff,sector_address,4096);
    
    //擦除扇区
    sFLASH_NOR_SectorErase(sector_address);
    
    //修改缓存区数据
    j = address % 4096;
    for(i = 0;i < len;i ++)
    {
        flash_buff[j+i] = *data++;
        
    }
    poniter = sector_address;
   	//把缓存写入flash
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
	//先从flash读到缓存区

	
	sector_id = NORFLASH_EVENT_FLAG_BASE/4096;
	sFLASH_NOR_Read(flash_buff,sector_id*4096,4096);
	
	//擦除扇区
	sFLASH_NOR_SectorErase(sector_id*4096);
	//修改缓存数据
	j = NORFLASH_EVENT_FLAG_BASE%4096;


		flash_buff[j] = data;

	//把缓存写入flash
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
