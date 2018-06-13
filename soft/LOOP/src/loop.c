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
extern  LOOPS LOOP[2]; 
//u16 dev_addr ; 
u8	 send_buff[8];
u8	 loop_num 		= 0;
u8   poweron_flag   = 0;
BOOL safe_mode 		= FALSE;
BOOL low_power_mode = FALSE;
#define MAX_LDCMD_NUM  1    	// 最大的联动命令数量
static u8 LDCmdNum   = 0; 		// 保存未执行的联动命令数量
static u8 IdxLDCmdRd = 0; 		// 联动命令读指针
static u8 IdxLDCmdWr = 0; 		// 联动命令写指针
static u8 LDCmdBuf[MAX_LDCMD_NUM][6]; // 可存储MAX_LDCMD_NUM条联动命令
uc8 stop_model[6] = {0x03, 0x00, 0x00, 0x00, 0x18, 0x40};
uint8_t loop_login_flag = 0;//回路登录完成标志位
extern uint8_t device_total;
//BOOL send_buff_full = FALSE;
//BOOL is_send_buff_full(void)
//{
//	return send_buff_full;
//}

/*
********************************************************************************
* 函数名称: void LoopLDCmdBufAdd(u8 buff[6]);
* 函数功能: 将多条联动命令放入联动命令缓冲区，等待逐条执行。
* 参    数: 3个对回路有效的联动字节
* 返 回 值: 无
* 注    意: 
********************************************************************************
*/
void LoopLDCmdBufAdd(u8 buff[6])
{
    if (LDCmdNum >= MAX_LDCMD_NUM) // 联动命令数量太多，后续命令被抛弃
    {
        LDCmdNum = MAX_LDCMD_NUM;
        return;
    }
    
     // 相邻两条命令不相同才认为是有效的命令
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
* 函数名称: u8 GetLoopLDCmdAndExe( );
* 函数功能: 从联动命令缓冲区取出联动命令执行。
* 参    数: 无
* 返 回 值: 1：设置执行成功。 0：无联动命令
* 注    意: 
********************************************************************************
*/
u8 GetLoopLDCmdAndExe(void)
{
    if (LDCmdNum == 0) // 无联动命令
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
	case 0x00:		//电平打开
		LDCmdBuf[IdxLDCmdRd][5] = 0x00;	
		break;
	case 0x01:		//脉冲打开
		LDCmdBuf[IdxLDCmdRd][5] = 0x80;	
		break;
	case 0x02:		//电平关闭
		LDCmdBuf[IdxLDCmdRd][5] = 0x40;	
		break;
	case 0x03:		//脉冲关闭
		LDCmdBuf[IdxLDCmdRd][5] = 0xC0;	
		break;
	default:		//电平关闭
//		LDCmdBuf[IdxLDCmdRd][5] = 0x40;	
		break;
    }
    
    SetLDList (LDCmdBuf[IdxLDCmdRd][0],
               ((u16)LDCmdBuf[IdxLDCmdRd][2] << 8) | LDCmdBuf[IdxLDCmdRd][1],
               LDCmdBuf[IdxLDCmdRd][3], 
               LDCmdBuf[IdxLDCmdRd][4],
               LDCmdBuf[IdxLDCmdRd][5]);
               
    LDCmdNum--; // 命令被执行，数量减少一条
    
    return 1;
}

/*******************************************************************************
* 函数名称: is_soft_reset
* 函数功能:是否为软件复位
* 参    数: 无
* 返回值:TRUE->为软件复位FALSE->非软件复位
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

void loop_short(u8 channel)							//回路短路处理
{
	LOOP[channel].OptStatFlags.StateBit.LPLineIsShort_flag = 1;  // 确实短路
	LOOP[channel].OptStatFlags.StateBit.ShortPowerOff_flag = 1;
 
    LOOP[channel].LoopShortCntTick = GetTickCount(); // 保存关闭电源时的系统滴嗒

    if (!LOOP[channel].OptStatFlags.StateBit.ShortReported_flag)  // 上报短路事件
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
			loop_short_flag = 1;
    }
}

/*******************************************************************************
* 函数名称: void LoopRstInit(void );
* 函数功能: 回路扫描前的初始化工作
* 参    数: 无
* 返 回 值: 无
* 注    意: 由主程序上电初始化时调用、要求系统其他部件初始化完毕后再初始化回路。
*           特别是系统滴答必须初始化完成。
*******************************************************************************/
void LoopRstInit( void )
{    
//	uc8 stop_model[6] = {0x03, 0x00, 0x00, 0x00, 0x18, 0x40};
	memset(device_allindex,0,sizeof(device_allindex));
	LoopCfgInit(LOOP_CFGED_INIT_STATE);
	LOOP[0].OptStatFlags.StateBit.NeedDownloadCfg_flag = 0;
	LOOP[1].OptStatFlags.StateBit.NeedDownloadCfg_flag = 0;
	
	LOOP[0].OptStatFlags.StateBit.RstCloseModu_flag = 1; // 上电关闭所有输出模块
	LOOP[1].OptStatFlags.StateBit.RstCloseModu_flag = 1; // 上电关闭所有输出模块

#if 0
    // 回路配置初始化
    if (is_soft_reset()) // 是软复位或看门狗复位  
    { 
		loop_send_0V(CHANNEL_1);
		loop_send_0V(CHANNEL_2);
		
		delay_nms(2000);
		
		feed_dog();
        // 设置联动表,2个回路上的所有输出模块讯响器等都以电平方式关闭
        //LoopLDCmdBufAdd(0xd8, 0, 0x40);
        LoopLDCmdBufAdd((u8*)stop_model);
    }
    
    // 回路线上电
    loop_power_on(CHANNEL_1);
    loop_power_on(CHANNEL_2);
	
	delay_nms(1000);
		
	feed_dog();
#endif
    // 回路定期做器件类型检查，此处初始化用于这个事情的时间计数器
    CheckTypeTimeTicks = GetTickCount();
   
    // 准备第一次扫描
    LoopCanScan_flag = 1;
    LOOP[0].OptStatFlags.StateBit.OptFinish_flag = 1;
}


/*
********************************************************************************
* 函数名称: void LoopCfgInit(u8 InitType);
* 函数功能: 回路配置初始化
* 参    数:LOOP_CFGED_INIT_STATE : 回路重新配置时初始化状态区
*                  LOOP_CFGED_KEEP_STATE :回路重新配置时不初始化状态区
* 返 回 值: 无
* 注    意: 本函数为静态的，只供本文件中调用
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
             //ALLDevices[m][i].DeviceCondition = *pCfg;  // 类型
             //pCfg++;   
             ALLDevices[m][i].DeviceType      = device_type1[device_.DeviceType].type;      
             //ALLDevices[m][i].DeviceType      = *pCfg;  // 类型
            // pCfg++;
            ALLDevices[m][i].DeviceZone       = device_.DeviceZone;
             //ALLDevices[m][i].DeviceZone      = *pCfg;  // 区
            // pCfg++;
            ALLDevices[m][i].DeviceFloor      = device_.DeviceFloor;
             //ALLDevices[m][i].DeviceFloor     = *pCfg;  // 层
             //pCfg++;
            ALLDevices[m][i].DeviceBuilding   = device_.DeviceBuilding;
            // memcpy(&ALLDevices[m][i].DeviceBuilding, pCfg, 2);// 楼
            // pCfg += 2;
             
             if (LOOP_CFGED_INIT_STATE == InitType) 
             {
                 //ALLDevices[m][i].OtherFlags.Val = 0; 这里有2个标志不能随便清除
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
        	ALLDevices[m][0].DeviceCondition |= 0x01;					//声光广播切换时使用
            LOOP[m].OptStatFlags.Val = 0;
            LOOP[m].DevLedTestAddr   = 0;
            LOOP[m].Defi             = ALLDevices[m][0].DeviceCondition; // 本位置暂存回路配置字节
        }
        
        LoopListInit(m);
      //是否亮灯巡检
//        if (LOOP[m].Defi & (1 << 3)) 		//闪灯
//			LOOP[m].OptStatFlags.StateBit.LoopLedFlashOff_flag = 1;
//        else                      
			LOOP[m].OptStatFlags.StateBit.LoopLedFlashOff_flag = 0;
        // 设定报部分丢失还是无响应  // 为1为单个报无响应
//        if (LOOP[m].Defi & (1 << 2))   		
//	       LOOP[m].OptStatFlags.StateBit.LoopLoseReportType_flag = 1;
//        else   // 报部分丢失                    
			LOOP[m].OptStatFlags.StateBit.LoopLoseReportType_flag = 0;
        
//        if (LOOP[m].Defi & (1 << 1))  //因为回路被整个屏蔽，实际个数被清0
//			LOOP[m].RNum = 0;				
//        else                      
			LOOP[m].RNum = LOOP[m].TNum; //链表初始化时这两个就被赋值相等
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
    	// 0器件的楼号处借用来存放该回路的配置字
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
* 函数名称: void LoopInSafeMode(u8 LN );
* 函数功能: 回路进入低功耗模式，送检用
* 参    数: 无
* 返 回 值: 无
* 注    意: main()死循环中当安全模式时执行
********************************************************************************
*/
void LoopInLowPwrMode(BOOL stat)
{
	low_power_mode = stat;
}

/*
********************************************************************************
* 函数名称: void LoopInSafeMode(u8 LN );
* 函数功能: 回路进入安全模式，即停止扫描，停止对回路供电
* 参    数: 无
* 返 回 值: 无
* 注    意: main()死循环中当安全模式时执行
********************************************************************************
*/
void LoopInSafeMode(BOOL stat)
{
	safe_mode = stat;
}

/*
********************************************************************************
* 函数名称:LoopIsSafeMode
* 函数功能:是否为安全模式
* 参    数: 无
* 返 回 值: TRUE->安全模式FALSE->非安全模式
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
	static u32 res_delay;	//自动登录前延时和上电延时
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
	
	if (AutoRegister_flag && res_delay)	//自动登录前res_delay应为2000 (延时大概2S)
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
		*  系统有两个回路,扫描完成一个回路上的一个器件后,转到另一个回路扫描,同时处 *
		*  理刚才那个器件的回码 												   *
		***************************************************************************/
		// 回路2扫描完成处理它的回码并启动回路1扫描
		if (allow_loop_receive())
		{
	 		LOOP[loop_num].OptStatFlags.StateBit.OptFinish_flag = 1;
			loop_answer(LOOP[loop_num].PWn);
		}	
			
		if (LoopCanScan_flag && LOOP[1].OptStatFlags.StateBit.OptFinish_flag) 
		{
			loop_num = 0;
			LOOP[1].OptStatFlags.StateBit.OptFinish_flag = 0;
//			LoopShortChk(0);		//短路后一段时间后重新上电
			DevOptInitAndRun(0);	// 启动回路1
			// 回路2不短路则处理它的回码，短路但是正在自动登陆也要处理它的回码
			if ((!LOOP[1].OptStatFlags.StateBit.LPLineIsShort_flag) || AutoRegister_flag)   
			{
				AnsCodeDeal(1,LOOP[1].RealOptingAddr); // 处理回路2回码信息
			}
		}
		// 回路1扫描完成处理它的回码并启动回路2扫描
		if (LoopCanScan_flag && LOOP[0].OptStatFlags.StateBit.OptFinish_flag)  
		{
			loop_num = 1;
			LOOP[0].OptStatFlags.StateBit.OptFinish_flag = 0;
			LoopShortChk(1);		//短路后一段时间后重新上电
			DevOptInitAndRun(1);		  // 启动回路2
			// 回路1不短路则处理它的回码，短路但是正在自动登陆也要处理它的回码
			if ((!LOOP[0].OptStatFlags.StateBit.LPLineIsShort_flag) || AutoRegister_flag) 
			{
				AnsCodeDeal(0,LOOP[0].RealOptingAddr); // 处理回路1回码信息
			}
		}
		// 检测自动登录标志
		if (AutoRegister_flag)  // 自动登陆标志,收到自动登陆命令后置位
		{ 
			if (LOOP[0].OptStatFlags.StateBit.AutoRegFinish_flag &&  // 回路已经自动登陆完成
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
		if (LoopIsSafeMode())		//安全模式
		{
			loop_clear_24V(CHANNEL_1);
			loop_clear_24V(CHANNEL_2);
		}

		if (is_send_buff_full() && allow_loop_receive())	//发送缓存满不进行回路上的操作
		{
			loop_send_24V(CHANNEL_1);
			loop_send_24V(CHANNEL_2);
		}
	}

	//送检降低功耗用
	if (LOOP[0].RNum == 0 && LOOP[1].RNum == 0)
	{
//		safe_time = (GetTickCount() + dev_addr / 2 * 100) % (13 * 100);

//		//5s 开8s关
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

