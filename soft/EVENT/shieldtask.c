//#define SHIELD_TASK_PRIVATE
#include "include.h"
SYSTEM_EVENT EventShieldtRamBuffer[17];	
    uint16_t 			EventShieldRamCurrentPointer;
/********************************************
�������ƣ��WWriteShieldDataToFlash
�������ܣ�  д���ε�FLASH
********************************************/
static void WriteShieldDataToFlash(SYSTEM_EVENT *pEvent)
{
	uint16_t i;
    SYSTEM_EVENT Event;
	SYSTEM_EVENT rEvent;
	DEVICE_CONFIG pDeviceConfig;
	memset(&pDeviceConfig.DeviceCondition, 0, 32);
	Event.EventYear         = Timer_value.year-2000;
    Event.EventMonth        = Timer_value.month;
    Event.EventDay          = Timer_value.date;
    Event.EventHour         = Timer_value.hour;
    Event.EventMinute       = Timer_value.min;
    Event.EventSecond       = Timer_value.sec;
	Event.EventAddress      = pEvent->EventAddress;
	Event.EventVerion = 1;
    Event.EventCaption      = pEvent->EventCaption;
    Event.EventDeviceType   = pEvent->EventDeviceType;
    Event.Zone              = pEvent->Zone;
    Event.EventCode   	  = pEvent->EventCode;
	for(i = 1;i < 17;i++)
	{
		norflash_read_SheildEvent(i,&rEvent);
		if(rEvent.EventCode == 0xff)
		{
			break;
		}
	}
	norflash_write_SheildEvent(i,(uint8_t*)&Event);
	
}
/******************************************
  �������ƣ�LightShieldLed
  �������ܣ������ε�
*******************************************/
void LightShieldLed(void)
{
	U16  i;
	SYSTEM_EVENT  	 Event;
	bool ShieldFlag 	 = FALSE;
    EventShieldRamCurrentPointer = 0;
    memset(EventShieldtRamBuffer,0,sizeof(EventShieldtRamBuffer));
	for(i = 1;i < 17;i++)
	{
		memset(&Event.EventVerion, 0, 32);
		norflash_read_SheildEvent(i,&Event);
		if (Event.EventCode != 0xff)
		{
			ShieldFlag = TRUE;
			memcpy(&EventShieldtRamBuffer[EventShieldRamCurrentPointer].EventVerion, &Event.EventVerion, 32);
			EventShieldRamCurrentPointer++;	
		}
		else
		{
			break;
		}
	}
	if(ShieldFlag) //�����ε�
	{
        if(GetEventFaultCount()==0)
        {
            W6_ShieldWindowDisplay();
          Led_Ctrl(LEDSHIELD_ON);
        }
		//�����εƺ���
	}
    else
    {
        Led_Ctrl(LEDSHIELD_OFF);
    }
	//������״̬����
		
}
/*****************************************************************************
       �������ƣ�ShieldSet
       �������ܣ������ֳ��豸����״̬
******************************************************************************/
uint8_t  ShieldSet(uint8_t DeviceID)
{
	DEVICE_CONFIG pDeviceConfig;
	SYSTEM_EVENT 		 Event;	
	norflash_read_device(DeviceID,&pDeviceConfig);
	if((pDeviceConfig.DeviceCondition & 0x03) == 1) //���ڲ���û�б�����
	{
		pDeviceConfig.DeviceCondition |= 0x02;
		norflash_write_device(DeviceID,&pDeviceConfig.DeviceCondition);
		
		Event.EventCode = EVENT_TYPE_DISABLE;
		Event.EventAddress 		= DeviceID-1;
        Event.EventCaption      = pDeviceConfig.DeviceCaption;
        Event.EventDeviceType   = pDeviceConfig.DeviceType;
        Event.Zone              = pDeviceConfig.DeviceZone;
		WriteEventToFlash(&Event);//д���¼���FLASH
		
		WriteShieldDataToFlash(&Event);//д���ε�FLASH
		LightShieldLed();
        return 1;
	}
    else
    {
       return 0;
    }
}
/********************************************************************************
           �������ƣ�OpenSet
           �������ܣ�������β���
*********************************************************************************/
uint8_t OpenSet(uint8_t DeviceID)
{
	DEVICE_CONFIG pDeviceConfig;
    SYSTEM_EVENT 		 Event;
//    uint8_t SectorID;
    int 				 i;
    norflash_read_device(DeviceID,&pDeviceConfig);
    
    if(pDeviceConfig.DeviceCondition & 0x02)
    {
        pDeviceConfig.DeviceCondition &= ~0x02;
        norflash_write_device(DeviceID,&pDeviceConfig.DeviceCondition );
        LightShieldLed();
        
        memcpy(&Event.EventVerion,&pDeviceConfig.DeviceCondition, 32);
        Event.EventAddress 		= DeviceID;
        Event.EventCode 		= EVENT_TYPE_DISABLE_CANCLE;
        Event.EventCaption      = pDeviceConfig.DeviceCaption;
        Event.EventDeviceType   = pDeviceConfig.DeviceType;
        Event.Zone              = pDeviceConfig.DeviceZone;
        WriteEventToFlash(&Event);
        for (i = 0; i < EventShieldRamCurrentPointer; i++)
        {
            if (EventShieldtRamBuffer[i].EventAddress == (DeviceID-1))

            {
                break;
            }
        }
        
        if (i >= EventShieldRamCurrentPointer)
        {
              return 0;
        }
          
        do
        {
            memcpy(&EventShieldtRamBuffer[i].EventVerion, &EventShieldtRamBuffer[i+1].EventVerion, 32);
            i++;
        }while ((i+1)<EventShieldRamCurrentPointer);
        
        EventShieldRamCurrentPointer--;
        //��Ӳ�����������
//        SectorID = (NORFLASH_SHIELD_BASE/4096);
        sFLASH_NOR_SectorErase(NORFLASH_SHIELD_BASE);
        for (i = 0; i < EventShieldRamCurrentPointer; i++)
        {
            WriteShieldDataToFlash(&EventShieldtRamBuffer[i]);
            
        }
        LightShieldLed();
        return 1;
        
    }
    else
    {
        return 0;
        //����ʧ��
    }
}





















