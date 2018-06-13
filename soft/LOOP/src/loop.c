#include <stdio.h>
#include <string.h>
#include "loop_hw.h"
#include "loop.h"
#include "LoopDataStruct.h"
#include "LoopOpt.h"
//#include "SysTick.h"
#include "stm32_flash.h"
//#include "can.h"
//#include "crc.h"
#include "LoopDevTables.h"
//#include "EventDefine.h"
//#include "SystemCommand.h"
//#include "DeviceTypeDefine.h"
#include "init.h"
#include "def.h"
#include "datastruct.h"
#include "norflash.h"
//#include "communication.h"

extern  const DEVICE_TYPE device_type1[];
//extern u16   dev_addr; 
extern volatile LOOPS LOOP[2]; 
//u16 dev_addr ; 
u8	 send_buff[8];
u8	 loop_num 		= 0;
u8   poweron_flag   = 0;
BOOL safe_mode 		= FALSE;
BOOL low_power_mode = FALSE;
#define MAX_LDCMD_NUM  1    	// ����������������
static u8 LDCmdNum   = 0; 		// ����δִ�е�������������
static u8 IdxLDCmdRd = 0; 		// ���������ָ��
static u8 IdxLDCmdWr = 0; 		// ��������дָ��
static u8 LDCmdBuf[MAX_LDCMD_NUM][6]; // �ɴ洢MAX_LDCMD_NUM����������
uc8 stop_model[6] = {0x03, 0x00, 0x00, 0x00, 0x18, 0x40};
uint8_t loop_login_flag = 0;//��·��¼��ɱ�־λ
extern uint8_t device_total;
//BOOL send_buff_full = FALSE;
//BOOL is_send_buff_full(void)
//{
//	return send_buff_full;
//}

/*
********************************************************************************
* ��������: void LoopLDCmdBufAdd(u8 buff[6]);
* ��������: �������������������������������ȴ�����ִ�С�
* ��    ��: 3���Ի�·��Ч�������ֽ�
* �� �� ֵ: ��
* ע    ��: 
********************************************************************************
*/
void LoopLDCmdBufAdd(u8 buff[6])
{
    if (LDCmdNum >= MAX_LDCMD_NUM) // ������������̫�࣬�����������
    {
        LDCmdNum = MAX_LDCMD_NUM;
        return;
    }
    
     // �������������ͬ����Ϊ����Ч������
    if (memcmp(buff, LDCmdBuf[IdxLDCmdWr], 6))
    {
        IdxLDCmdWr++;
        
        if (IdxLDCmdWr >= MAX_LDCMD_NUM)
        {
            IdxLDCmdWr = 0;
        }
        memcpy(LDCmdBuf[IdxLDCmdWr], buff, 6);
        LDCmdNum++;
    }
}

/*
********************************************************************************
* ��������: u8 GetLoopLDCmdAndExe( );
* ��������: �������������ȡ����������ִ�С�
* ��    ��: ��
* �� �� ֵ: 1������ִ�гɹ��� 0������������
* ע    ��: 
********************************************************************************
*/
u8 GetLoopLDCmdAndExe(void)
{
    if (LDCmdNum == 0) // ����������
    {
        return 0;
    }
    
    IdxLDCmdRd++;
    
    if (IdxLDCmdRd >= MAX_LDCMD_NUM)
    {
        IdxLDCmdRd = 0;
    }

    switch (LDCmdBuf[IdxLDCmdRd][5])
    {
	case 0x00:		//��ƽ��
		LDCmdBuf[IdxLDCmdRd][5] = 0x00;	
		break;
	case 0x01:		//�����
		LDCmdBuf[IdxLDCmdRd][5] = 0x80;	
		break;
	case 0x02:		//��ƽ�ر�
		LDCmdBuf[IdxLDCmdRd][5] = 0x40;	
		break;
	case 0x03:		//����ر�
		LDCmdBuf[IdxLDCmdRd][5] = 0xC0;	
		break;
	default:		//��ƽ�ر�
//		LDCmdBuf[IdxLDCmdRd][5] = 0x40;	
		break;
    }
    
    SetLDList (LDCmdBuf[IdxLDCmdRd][0],
               ((u16)LDCmdBuf[IdxLDCmdRd][2] << 8) | LDCmdBuf[IdxLDCmdRd][1],
               LDCmdBuf[IdxLDCmdRd][3], 
               LDCmdBuf[IdxLDCmdRd][4],
               LDCmdBuf[IdxLDCmdRd][5]);
               
    LDCmdNum--; // ���ִ�У���������һ��
    
    return 1;
}

/*******************************************************************************
* ��������: is_soft_reset
* ��������:�Ƿ�Ϊ�����λ
* ��    ��: ��
* ����ֵ:TRUE->Ϊ�����λFALSE->�������λ
*******************************************************************************/
//BOOL is_soft_reset(void)
//{
//	u32 temp = RCC->CSR & (1 << 28);
//	
//	RCC->CSR |= 1 << 24;
//	
//	if (temp != 0)
//		return TRUE;
//	else	
//		return FALSE;
//}

void loop_short(u8 channel)							//��·��·����
{
	LOOP[channel].OptStatFlags.StateBit.LPLineIsShort_flag = 1;  // ȷʵ��·
	LOOP[channel].OptStatFlags.StateBit.ShortPowerOff_flag = 1;
 
    LOOP[channel].LoopShortCntTick = GetTickCount(); // ����رյ�Դʱ��ϵͳ���

    if (!LOOP[channel].OptStatFlags.StateBit.ShortReported_flag)  // �ϱ���·�¼�
    {	
		send_buff[0] = 0;
		send_buff[1] = EVENT_TYPE_LOOP_FAULT;
		send_buff[2] = 230;
		send_buff[3] = 0xFE;
		send_buff[4] = 0;
		send_buff[5] = 0;
		send_buff[6] = 0;
		 Loop_Revice(send_buff);
//		can_send(1, dev_addr + channel, 0, 7, send_buff);

		LOOP[channel].OptStatFlags.StateBit.ShortReported_flag = 1;
    }
}

/*******************************************************************************
* ��������: void LoopRstInit(void );
* ��������: ��·ɨ��ǰ�ĳ�ʼ������
* ��    ��: ��
* �� �� ֵ: ��
* ע    ��: ���������ϵ��ʼ��ʱ���á�Ҫ��ϵͳ����������ʼ����Ϻ��ٳ�ʼ����·��
*           �ر���ϵͳ�δ�����ʼ����ɡ�
*******************************************************************************/
void LoopRstInit( void )
{    
//	uc8 stop_model[6] = {0x03, 0x00, 0x00, 0x00, 0x18, 0x40};
	memset(device_allindex,0,sizeof(device_allindex));
	LoopCfgInit(LOOP_CFGED_INIT_STATE);
	LOOP[0].OptStatFlags.StateBit.NeedDownloadCfg_flag = 0;
	LOOP[1].OptStatFlags.StateBit.NeedDownloadCfg_flag = 0;
	
	LOOP[0].OptStatFlags.StateBit.RstCloseModu_flag = 1; // �ϵ�ر��������ģ��
	LOOP[1].OptStatFlags.StateBit.RstCloseModu_flag = 1; // �ϵ�ر��������ģ��

#if 0
    // ��·���ó�ʼ��
    if (is_soft_reset()) // ����λ���Ź���λ  
    { 
		loop_send_0V(CHANNEL_1);
		loop_send_0V(CHANNEL_2);
		
		delay_nms(2000);
		
		feed_dog();
        // ����������,2����·�ϵ��������ģ��Ѷ�����ȶ��Ե�ƽ��ʽ�ر�
        //LoopLDCmdBufAdd(0xd8, 0, 0x40);
        LoopLDCmdBufAdd((u8*)stop_model);
    }
    
    // ��·���ϵ�
    loop_power_on(CHANNEL_1);
    loop_power_on(CHANNEL_2);
	
	delay_nms(1000);
		
	feed_dog();
#endif
    // ��·�������������ͼ�飬�˴���ʼ��������������ʱ�������
    CheckTypeTimeTicks = GetTickCount();
   
    // ׼����һ��ɨ��
    LoopCanScan_flag = 1;
    LOOP[0].OptStatFlags.StateBit.OptFinish_flag = 1;
}


/*
********************************************************************************
* ��������: void LoopCfgInit(u8 InitType);
* ��������: ��·���ó�ʼ��
* ��    ��:LOOP_CFGED_INIT_STATE : ��·��������ʱ��ʼ��״̬��
*                  LOOP_CFGED_KEEP_STATE :��·��������ʱ����ʼ��״̬��
* �� �� ֵ: ��
* ע    ��: ������Ϊ��̬�ģ�ֻ�����ļ��е���
********************************************************************************
*/
void LoopCfgInit(u8 InitType)
{
    u32 m, i,j;
   DEVICE_CONFIG device_;
   j = 0;
    memset(ALLDevices,0,sizeof(ALLDevices));
    memset(LOOP,0,sizeof(LOOP));
    for (m = 0; m < 2; m++)
    {
        if(m == 0)
            continue;
        for (i = 0; i < 16; i++)
        {
             norflash_read_device(i+1,&device_);
            if(device_.DeviceCondition !=0xff)
            {
             if((device_.DeviceCondition&0x01) == 1)
             {
                 //device_allindex[j] = i;
                 j++;
             }
         }
             ALLDevices[m][i].DeviceCondition = device_.DeviceCondition;
             //ALLDevices[m][i].DeviceCondition = *pCfg;  // ����
             //pCfg++;   
             ALLDevices[m][i].DeviceType      = device_type1[device_.DeviceType].type;      
             //ALLDevices[m][i].DeviceType      = *pCfg;  // ����
            // pCfg++;
            ALLDevices[m][i].DeviceZone       = device_.DeviceZone;
             //ALLDevices[m][i].DeviceZone      = *pCfg;  // ��
            // pCfg++;
            ALLDevices[m][i].DeviceFloor      = device_.DeviceFloor;
             //ALLDevices[m][i].DeviceFloor     = *pCfg;  // ��
             //pCfg++;
            ALLDevices[m][i].DeviceBuilding   = device_.DeviceBuilding;
            // memcpy(&ALLDevices[m][i].DeviceBuilding, pCfg, 2);// ¥
            // pCfg += 2;
             
             if (LOOP_CFGED_INIT_STATE == InitType) 
             {
                 //ALLDevices[m][i].OtherFlags.Val = 0; ������2����־����������
                 ALLDevices[m][i].OtherFlags.FlagBit.Go2Prior_flag = 0;
                 ALLDevices[m][i].OtherFlags.FlagBit.IsOldDev_flag = 0;
                 ALLDevices[m][i].OtherFlags.FlagBit.IsDuAddr_flag = 0;
                 ALLDevices[m][i].ReScanStat1    = 0;
                 ALLDevices[m][i].ReScanStat2    = 0;
                 ALLDevices[m][i].ReScanStat3    = 0;
                 ALLDevices[m][i].ReScanStat4    = 0;
                 ALLDevices[m][i].ReScanStat5    = 0;
             }
         }
        
        if (LOOP_CFGED_INIT_STATE == InitType) 
        {
        	ALLDevices[m][0].DeviceCondition |= 0x01;					//����㲥�л�ʱʹ��
            LOOP[m].OptStatFlags.Val = 0;
            LOOP[m].DevLedTestAddr   = 0;
            LOOP[m].Defi             = ALLDevices[m][0].DeviceCondition; // ��λ���ݴ��·�����ֽ�
        }
        
        LoopListInit(m);
      //�Ƿ�����Ѳ��
//        if (LOOP[m].Defi & (1 << 3)) 		//����
//			LOOP[m].OptStatFlags.StateBit.LoopLedFlashOff_flag = 1;
//        else                      
			LOOP[m].OptStatFlags.StateBit.LoopLedFlashOff_flag = 0;
        // �趨�����ֶ�ʧ��������Ӧ  // Ϊ1Ϊ����������Ӧ
//        if (LOOP[m].Defi & (1 << 2))   		
//	       LOOP[m].OptStatFlags.StateBit.LoopLoseReportType_flag = 1;
//        else   // �����ֶ�ʧ                    
			LOOP[m].OptStatFlags.StateBit.LoopLoseReportType_flag = 0;
        
//        if (LOOP[m].Defi & (1 << 1))  //��Ϊ��·���������Σ�ʵ�ʸ�������0
//			LOOP[m].RNum = 0;				
//        else                      
			LOOP[m].RNum = LOOP[m].TNum; //�����ʼ��ʱ�������ͱ���ֵ���
    }
     device_total = j;
    register_loop_short_callback(loop_short);
}

//u16 get_loop_check(u8 loop)
//{
//	u32 addr;

//	if (loop == 0)
//	{
//		addr = LOOP_DEV_CFG_ADDR + 6;
//	}
//	else
//	{
//		addr = LOOP_DEV_CFG_ADDR + 257 * 6;
//	}

//	return CrcCheck((u8*)addr, 255 * 6);
//}

void LoopCfgSaveToFlash(void)
{
    u32 i, j,m;
    u8  buff[256],page;
    DEVICE_CONFIG device;
//	DstAddr = LOOP_DEV_CFG_ADDR / 1024;

//	Stm32FlashErasePage(DstAddr);
//    Stm32FlashErasePage(DstAddr + 1);
//    Stm32FlashErasePage(DstAddr + 2);
//    Stm32FlashErasePage(DstAddr + 3);
//    
//    DstAddr = LOOP_DEV_CFG_ADDR;
    sFLASH_NOR_SectorErase(0);//
    m= 0;
    for (j = 0; j < 2; j++)
    {
    	// 0������¥�Ŵ���������Ÿû�·��������
        if(j == 0)
            continue;
        for (i = 0; i < 256; i++) 
        {  
            if (0 == i)  
            {
            	ALLDevices[j][i].DeviceCondition = LOOP[j].Defi;
                //continue;
            }
            
            	
            device.DeviceCondition = ALLDevices[j][i].DeviceCondition;
            switch(ALLDevices[j][i].DeviceType)
            {
                case DEVICE_POINT_PY_SMOKE_FIRE_DETECTOR:
                    device.DeviceType = 0;
                    break;
                case DEVICE_FEEL_FIRE_DETECTOR:
                     device.DeviceType = 1;
                    break;
                case DEVICE_HAND_FIRE_DETECTOR:
                     device.DeviceType = 2;
                    break;
                default:
                    break;
            }
            device.DeviceZone = 1;
            device.DeviceCaption = 0;
            device.DeviceFloor = ALLDevices[j][i].DeviceFloor;
            device.DeviceBuilding = ALLDevices[j][i].DeviceBuilding;
            memcpy(&buff[m*8],&device.DeviceCondition,8);
            m++;
            if(m == 32)
            {
                m = 0;
               
               sFLASH_NOR_Pageprogram(buff,page*256,256); 
                 page++;
            }
            
                   

        }
    }
}

/*
********************************************************************************
* ��������: void LoopInSafeMode(u8 LN );
* ��������: ��·����͹���ģʽ���ͼ���
* ��    ��: ��
* �� �� ֵ: ��
* ע    ��: main()��ѭ���е���ȫģʽʱִ��
********************************************************************************
*/
void LoopInLowPwrMode(BOOL stat)
{
	low_power_mode = stat;
}

/*
********************************************************************************
* ��������: void LoopInSafeMode(u8 LN );
* ��������: ��·���밲ȫģʽ����ֹͣɨ�裬ֹͣ�Ի�·����
* ��    ��: ��
* �� �� ֵ: ��
* ע    ��: main()��ѭ���е���ȫģʽʱִ��
********************************************************************************
*/
void LoopInSafeMode(BOOL stat)
{
	safe_mode = stat;
}

/*
********************************************************************************
* ��������:LoopIsSafeMode
* ��������:�Ƿ�Ϊ��ȫģʽ
* ��    ��: ��
* �� �� ֵ: TRUE->��ȫģʽFALSE->�ǰ�ȫģʽ
********************************************************************************
*/
BOOL LoopIsSafeMode(void)
{
	if (low_power_mode || safe_mode)
		return TRUE;
	else
		return FALSE;
}

void loop_handle(void)
{
//	u32 safe_time = 0;
	static u32 res_delay;	//�Զ���¼ǰ��ʱ���ϵ���ʱ
    loopbar();
	if ((poweron_flag & 0xf0) == 0)
	{
		switch (poweron_flag)
		{
		case 0:
			res_delay = GetTickCount();
			
			if (is_soft_reset())			
				poweron_flag = 0x01;
			else
				poweron_flag = 0x02;
				
			break;
			
		case 1:
			loop_send_0V(CHANNEL_1);
			loop_send_0V(CHANNEL_2);

			if ((GetTickCount() - res_delay) > 200)
			{
				poweron_flag = 0x02;
				res_delay = GetTickCount();
				LoopLDCmdBufAdd((u8*)stop_model);
			}
			break;
			
		case 2:
			loop_power_on(CHANNEL_1);
    		loop_power_on(CHANNEL_2);

    		if ((GetTickCount() - res_delay) > 150)
			{
				poweron_flag = 0xff;	
				res_delay    = 2000;
			}
			
			break;

		default:
			poweron_flag = 0xff;	
			res_delay    = 2000;
			break;
		}
		return;
	}
	
	if (AutoRegister_flag && res_delay)	//�Զ���¼ǰres_delayӦΪ2000 (��ʱ���2S)
	{
		delay_1ms();
		LoopInLowPwrMode(FALSE);
		loop_send_24V(CHANNEL_1);
		loop_send_24V(CHANNEL_2);
		res_delay--;	

		return;
	}
	
	if (!LoopIsSafeMode() && !is_send_buff_full())
	{
		/***************************************************************************
		*  ϵͳ��������·,ɨ�����һ����·�ϵ�һ��������,ת����һ����·ɨ��,ͬʱ�� *
		*  ��ղ��Ǹ������Ļ��� 												   *
		***************************************************************************/
		// ��·2ɨ����ɴ������Ļ��벢������·1ɨ��
		if (allow_loop_receive())
		{
	 		LOOP[loop_num].OptStatFlags.StateBit.OptFinish_flag = 1;
			loop_answer(LOOP[loop_num].PWn);
		}	
			
		if (LoopCanScan_flag && LOOP[1].OptStatFlags.StateBit.OptFinish_flag) 
		{
			loop_num = 0;
			LOOP[1].OptStatFlags.StateBit.OptFinish_flag = 0;
//			LoopShortChk(0);		//��·��һ��ʱ��������ϵ�
			DevOptInitAndRun(0);	// ������·1
			// ��·2����·�������Ļ��룬��·���������Զ���½ҲҪ�������Ļ���
			if ((!LOOP[1].OptStatFlags.StateBit.LPLineIsShort_flag) || AutoRegister_flag)   
			{
				AnsCodeDeal(1,LOOP[1].RealOptingAddr); // �����·2������Ϣ
			}
		}
		// ��·1ɨ����ɴ������Ļ��벢������·2ɨ��
		if (LoopCanScan_flag && LOOP[0].OptStatFlags.StateBit.OptFinish_flag)  
		{
			loop_num = 1;
			LOOP[0].OptStatFlags.StateBit.OptFinish_flag = 0;
			LoopShortChk(1);		//��·��һ��ʱ��������ϵ�
			DevOptInitAndRun(1);		  // ������·2
			// ��·1����·�������Ļ��룬��·���������Զ���½ҲҪ�������Ļ���
			if ((!LOOP[0].OptStatFlags.StateBit.LPLineIsShort_flag) || AutoRegister_flag) 
			{
				AnsCodeDeal(0,LOOP[0].RealOptingAddr); // �����·1������Ϣ
			}
		}
		// ����Զ���¼��־
		if (AutoRegister_flag)  // �Զ���½��־,�յ��Զ���½�������λ
		{ 
			if (LOOP[0].OptStatFlags.StateBit.AutoRegFinish_flag &&  // ��·�Ѿ��Զ���½���
				LOOP[1].OptStatFlags.StateBit.AutoRegFinish_flag) 
			{
			   AutoRegFinished();
			   AutoRegister_flag = 0;
			   LOOP[0].OptStatFlags.StateBit.AutoRegFinish_flag = 0;
			   LOOP[1].OptStatFlags.StateBit.AutoRegFinish_flag = 0;
			   res_delay = 2000;
                loop_login_flag = 1;
            // LoopCfgSaveToFlash();
			}
		}
	}
	else		
	{
		if (LoopIsSafeMode())		//��ȫģʽ
		{
			loop_clear_24V(CHANNEL_1);
			loop_clear_24V(CHANNEL_2);
		}

		if (is_send_buff_full() && allow_loop_receive())	//���ͻ����������л�·�ϵĲ���
		{
			loop_send_24V(CHANNEL_1);
			loop_send_24V(CHANNEL_2);
		}
	}

	//�ͼ콵�͹�����
	if (LOOP[0].RNum == 0 && LOOP[1].RNum == 0)
	{
//		safe_time = (GetTickCount() + dev_addr / 2 * 100) % (13 * 100);

//		//5s ��8s��
//		if (safe_time < 8 * 100)	
//		{
//			LoopInLowPwrMode(TRUE);
//		}
//		else
//		{
//			LoopInLowPwrMode(FALSE);
//		}
	}
	else
	{
		LoopInLowPwrMode(FALSE);
	}
}

