/*
********************************************************************************
*                             ������̩�����������޹�˾
* ��    ַ �� �����з�̨���żҿ���·8��
* ��    �� �� 100072
* ��    ַ �� Http://www.guotaiyian.com
*
* �� �� �� :  LoopSendOpt.c
* ��    �� :  �� �
* ����ʱ�� :  2006/11/16
* ��    �� :  ��·�������������������ǰ�����й���������ļ��д���
*             ��Ҫ��Ϊ�˼���ԭ��LoopOpt.c�ļ�����������ԭ�ļ�һ��Ϊ����
*             ��Ϊ���������ǰ�ͽ�����󣬹�ͬ����LoopOpt.h��
*             ����������ԭ mainC.c �� linechk.c �ļ��е����ݡ�
*
* ע    �� �� �»�·������ΪӲ���Ĳ�ͬ������ʱ�ȵȵ��½��أ�Ȼ���������ء�
*             ��24Vʱ�ڼ��ܽ��ϵ�1��ƽ��ʾ������0��ƽ��ʾ��·��
*             �� 5Vʱ�ڼ��ܽ��ϵ�1��ƽ��ʾ������0��ƽ��ʾ���롣
* ��    �� �� 2008/02/25 ��ֲ���»�·���ϵ��޸ġ�
*             ��Ҫ�Ķ����������º����У�SendVolt();LoopShortChk()
*             LoopCardISR_Handler(); _DuplAddrChk()->del
********************************************************************************
*/
#include "loop.h"
//#include "can.h"
#include "loop_hw.h"
#include "LoopDataStruct.h"
#include "LoopOpt.h"
//#include "SysTick.h"
#include "LoopDevTables.h"
//#include "communication.h"
#include "DeviceTypeDefine.h"
#include "def.h"
#include "event.h"
/******************************************************************************/
extern u16 dev_addr; 
extern const u8 DEVICE_IO_TYPE[256];

 LOOPS LOOP[2];               			// �����·�ṹ�ͻ�·������
LOOPDevice ALLDevices[2][256]; 			// ÿ����·��256����·�����ͽṹ�ռ�
u8  OptingLoopNO;                   	// ��ǰ���ڲ����Ļ�·��0��1
u32 CheckTypeTimeTicks;             	// ��·�������������ͼ�飬�˱�����������������ʱ�������
u8  LDList[2][256];            			// ����������
u8	auto_res_ln;						//�Զ���¼��·��

int LDNode[2];						  	//��ǰ�������ֳ��������

BOOL   AutoRegister_flag = 0;         	// �Զ���½ʱ��λ����½�����0
BOOL   LoopCanScan_flag  = 0;         	// ���ƻ�·�Ƿ����ɨ��

/******************************************************************************/
static u8  AnsCodeCount;     // ��ͬ���͵������ڲ�ͬ����������Ӧ�û���ĸ���

/*
********************************************************************************
* ��������: void LoopListInit( u8 LN );
* ��������: ��·����ĳ�ʼ����ϵͳ���յ�����������û��޸�������ú����
* ��    ��: u8 LN :��·�� 0,1
* �� �� ֵ: ��
* ע    ��:
********************************************************************************
*/
static u8 LoopListFirstInit_flag[2] ={1,1}; // 20071012���ӣ��޸��������ú��ʼ����LDlist�����º��߼��ظ�����

void LoopListInit( u8 LN )
{
    u32 i, j;
    //Ϊ1ʱ��ʾɨ�����ñ��ҵ����ǵ�һ����ͨ�����·�е�����
    BOOL firstFindLPAddr_flag = 1, firstFindHPAddr_flag = 1;
    u8   firstFindLPAddr = 0, 	   firstFindHPAddr;
    u8   LPAddrNum = 0, 		   HPAddrNum = 0;
    u8   temp;
    u8   InsIntv;

    // ��ʼ������·����ͨ����(ѭ���ṹ)������ɨ������(����ṹ)
    LOOP[LN].HPFirstAddr = 0;
    LOOP[LN].LPFirstAddr = 0;
    LOOP[LN].RNum 		 = 0;   
    LOOP[LN].TNum 		 = 0;

    for (i = 1; i <= 16; i++) 
    {
        if (ALLDevices[LN][i].DeviceCondition & (1 << 0)) 				// �������� 
        {
            if (!(ALLDevices[LN][i].DeviceCondition & (1 << 1)))  		// ����û�б�����
            {
                if (!(ALLDevices[LN][i].DeviceCondition & (1 << 2)))	//����ɨ��
                {
                    // ʹ�õ�ǰ����������ͨ����
                    if (firstFindLPAddr_flag) 
                    {
                        firstFindLPAddr_flag = 0;
                        firstFindLPAddr      = i;
                        LOOP[LN].LPFirstAddr = i;
                    } 
                    else 
                    {
                        ALLDevices[LN][i].NextAddr = LOOP[LN].LPFirstAddr;
                        LOOP[LN].LPFirstAddr = i;
                    }
                    LPAddrNum ++; // ��ͨ�����������ĸ�������
                } 
                else 
                {
                    // ʹ�õ�ǰ������������ȼ�ɨ������
                    if (firstFindHPAddr_flag) 
                    {
                        firstFindHPAddr_flag = 0;
                        firstFindHPAddr = i;
                    }
                    ALLDevices[LN][i].NextAddr = LOOP[LN].HPFirstAddr;
                    LOOP[LN].HPFirstAddr = i;
                    HPAddrNum ++; // ����ɨ�������������ĸ�������
                }
                // ��ǰ������һЩ�����б�Ҫ�ڴ˴���ʼ��
                //ALLDevices[LN][i].OtherFlags.Val = 0; ������2����־����������
                ALLDevices[LN][i].OtherFlags.FlagBit.Go2Prior_flag = 0;
                ALLDevices[LN][i].OtherFlags.FlagBit.IsOldDev_flag = 0;
                ALLDevices[LN][i].OtherFlags.FlagBit.IsDuAddr_flag = 0;
                // 0x0a����ֻ�и�λ��ſ�����0
                if (0x0a != (0x0f & ALLDevices[LN][i].Comd))
                {
                    ALLDevices[LN][i].Comd = XJCMD(LN, i);
                }// ���Ҫ�ָ�Ϊ00��0d�����Ѿ�ȷ�ϵ�Ϊ0A
            }// ����û�б�����
        }// ��������
    }// ��������255������

    /*******************���������ñ���ɨ���������****************************/
    // ������Ч��������
    LOOP[LN].TNum  = HPAddrNum + LPAddrNum;
    LOOP[LN].RNum  = LOOP[LN].TNum;
    LOOP[LN].HPNum = HPAddrNum;
    if (LOOP[LN].TNum < MIN_DEVICE_NUM) // ��������̫����Ҫ�����ɨ
    { 
        LOOP[LN].ScanedCount = 0;          // �˴���ʼ����ɨ������Ϊ0
    }

    /**************************************************************************/
    if (1 == LoopListFirstInit_flag[LN])// �����ϵ��һ�ε��ñ�����ʱ��ʼ��
    { 
        LoopListFirstInit_flag[LN] = 0;
        LOOP[LN].OptStatFlags.Val = 0;
        LDNode[LN] = -1;  // ����ɨ�������޸�����Ȼ������������ҲҪ����������Ҷ���
        
        for (i = 0; i <= 255; i++) 
        {
            LDList[LN][i] = 0;
        }
    }
    /**************************************************************************/
    if (LPAddrNum > 0)  // ��ͨ���������������Ϊ0��������
    {        
        // �պ���ͨ�����Ϊ����
        ALLDevices[LN][firstFindLPAddr].NextAddr = LOOP[LN].LPFirstAddr;
        LOOP[LN].OptingAddr    = LOOP[LN].LPFirstAddr;
        LOOP[LN].OldJmpPreAddr = LOOP[LN].LPFirstAddr;
    } 
    else if (HPAddrNum > 0) // ������������������������ɨ��
    {   //��ʱLOOP[LN].LPFirstAddr = 0; ��LOOP[LN].RNUM != 0;��������ɨ��ĵ���
        //��������NULL�ڵ��ֱ��ת�����Լ�����������ɨ�裬��ʵ�ǰ���Ҳ���պϳ�
        //һ��ѭ�������ˣ��˴�ֱ�ӱպϣ�ȥ���˵��������β��NULL��
        LOOP[LN].LPFirstAddr = LOOP[LN].HPFirstAddr;
        ALLDevices[LN][firstFindHPAddr].NextAddr = LOOP[LN].HPFirstAddr;
        LOOP[LN].OptingAddr    = LOOP[LN].LPFirstAddr;
        LOOP[LN].OldJmpPreAddr = LOOP[LN].LPFirstAddr;
        LOOP[LN].HPNum = 0;  //�Ѿ���Ϊ�ջ���������Ϊ������ͨ���ȼ�������

        if( LOOP[LN].Defi & (1 << 1))  // ��·������
        {
            LOOP[LN].RNum = 0;
            LOOP[LN].OptingAddr = 0;
        }
        return; // ��·��ȫ�Ǹ����ȼ������Ҵ˴������ɨ�������ʼ��
    } 
    else  
    	return; // ��·��û������������ֱ��ɨ0�룬��������

    /**************************************************************************/
    // ��������ɨ����������ͨ�����еĲ���㣬���ڲ���㴦������
    if( LPAddrNum <= PRIOR_SCAN_TIMES )// �����ͨ�����е���������������ɨ���������,��
    {                                  // ÿ������ɨ����ɺ󣬶�Ҫת������ɨ��������ȥ
        temp = LOOP[LN].LPFirstAddr;
        for( i = 0; i < LPAddrNum; i++ ) 
        {
            ALLDevices[LN][temp].OtherFlags.FlagBit.Go2Prior_flag = 1; // ��ת�����������־
            temp  = ALLDevices[LN][temp].NextAddr;             // ȡ������ͨ������һ��������ַ
        }
    } 
    else 
    {
        InsIntv = LPAddrNum / PRIOR_SCAN_TIMES;   // ��������ɨ������ļ��
        temp = LOOP[LN].LPFirstAddr;
        for( i = 0; i < PRIOR_SCAN_TIMES; i++) 
        {
            for( j = 0; j < InsIntv; j++) 
            {
                temp = ALLDevices[LN][temp].NextAddr;
            }
            ALLDevices[LN][temp].OtherFlags.FlagBit.Go2Prior_flag = 1;  // ��ת�����������־
        }
    }
	
//    if (LOOP[LN].Defi & (1 << 1)) 
//    {
//        LOOP[LN].RNum = 0;
//        LOOP[LN].OptingAddr = 0;
//        return;
//    } //��·������

    LOOP[LN].OptingAddr = LOOP[LN].LPFirstAddr;
}

/*
********************************************************************************
* ��������: BOOL LoopShortChk(u8 LN);
* ��������: �����·��·������������·
* ��    ��: u8 LN   : ��·�� 0,1
* �� �� ֵ:��
* ע    ��: 
********************************************************************************
*/
void LoopShortChk(u8 LN)
{
    u32 timer;
    uint8_t send_buff[7];
    if (LOOP[LN].OptStatFlags.StateBit.ShortPowerOff_flag) // ��·LN�Ѿ����·�رյ�Դ
    { 
        timer = GetTickCount();
        
        // ��·ʱ�䳬���涨ֵ
        if ((timer - LOOP[LN].LoopShortCntTick) > (WAIT_RECOVER_TIMERS * 1000 / MSPERTICK)) 
        {
        	LOOP[LN].OptStatFlags.StateBit.ShortPowerOff_flag = 0;   // ��ʼ��·����ָ�����
//            LOOP[LN].OptStatFlags.StateBit.LPLineIsShort_flag = 0;
//            LOOP[LN].LoopShortCntTick = 0;
					//LOOP[1].OptStatFlags.StateBit.ShortReported_flag = 0;
			    LOOP[LN].LoopShortCntTick = timer;
          loop_power_on(LN);		//��·�ϵ�
        }
    }
    else
    {
		 if (!LOOP[LN].OptStatFlags.StateBit.ShortPowerOff_flag && LOOP[LN].OptStatFlags.StateBit.LPLineIsShort_flag)
		{
			timer = GetTickCount();

			if ((timer - LOOP[LN].LoopShortCntTick) > (2 * WAIT_RECOVER_TIMERS * 1000 / MSPERTICK))		//20s��
			{
				LOOP[LN].OptStatFlags.StateBit.LPLineIsShort_flag = 0;
			}
		}
    }
}

/*
********************************************************************************
* ��������: void SetLDList(u8 B5, u8 B6, u8 B7);
* ��������: ���û�·������������,����Ϊ�������Э����ֽ�5��6��7��
* ��    ��: �������ֽ�5��6��7���ֽ�4����
* �� �� ֵ: ��
* ע    ��:
********************************************************************************
*/
void SetLDList(u8 category, u16 controllerID, u8 loop, u8 address, u8 control_type)
{
    u8   temp, tempAddr, tempDevNum, HPorLP;
    int  i;
    u8   LN;
    BOOL BuildPass, TypePass, Zo_Fl_Ma_Pass;  //¥�ŷ��ϡ����ͷ��ϡ�����������ŷ���
/*
	1	����	LinkageOutCategory
	2	������/¥��/���	LinkageOutControllerID
	3		
	4	��·��/����/���	LinkageOutLoop
	5	��ַ��/���� LinkageOutAddress
	6	���Ʒ�ʽ	LinkageOutControlType
*/
	if (category == 0)			//��ַ
	{
		if (loop == 0 && address == 0)
		{
			LDList[0][0] = (control_type & 0xc0) | 0x01;  		  //������������û��ִ��
            LOOP[0].OptStatFlags.StateBit.HaveLDDev_flag = 1; 	  //����Ҫ����������

            LDList[1][0] = (control_type & 0xc0) | 0x01;  //������������û��ִ��
            LOOP[1].OptStatFlags.StateBit.HaveLDDev_flag = 1; 	  //����Ҫ����������
		}
		else
		{
			if ((dev_addr - loop) == 7)
			{
				LN = 0;
			}
	
			if ((dev_addr - loop) == 6)
			{
				LN = 1;
			}
			
			if (LN == 0 || LN == 1) 	//�Ǳ���·���Ļ�·
			{
				if (!(ALLDevices[LN][address].DeviceCondition & (1 << 1)))
				{
					//������Ԫ��ʽ��D7(��ƽ������)+D6(�򿪻�ر�)+D1(�����Ѿ�ִ��)+D0(������δִ��)
					temp = ALLDevices[LN][address].DeviceType;
	
					if (DEVICE_IO_TYPE[temp] == 0x01 || DEVICE_IO_TYPE[temp] == 0x11)
					{
						temp = LDList[LN][address] ^ control_type;
	
						//�������͸ı䣬���巽ʽ��ƽ��ʽ������һ�µ�
						if ((temp & 0x40) || (0 == LDList[LN][address]))
						{
							 LDList[LN][address] = (control_type & 0xc0) | 0x01;		// ������������û��ִ��
							 LOOP[LN].OptStatFlags.StateBit.HaveLDDev_flag = 1; 		// ����Ҫ������������ʶ
						}
					}
				}
			}
		}		
	}
    else if ((0x01 == category) || (0x02 == category) || (0x03 == category))//(����/���/������) + ģ�����ͣ����ַ���� ��
    { 
        for (LN = 0; LN < 2; LN++) //��������·��Ѱ��
        {
            for (HPorLP = 0; HPorLP < 2; HPorLP++) //�ӵ�ǰ��·�ߵ����ȼ�������Ѱ��
            {
                if (0 == HPorLP) //��ɨ������ȼ�����
                {
                    tempAddr   = LOOP[LN].HPFirstAddr; 
                    tempDevNum = LOOP[LN].HPNum;
                }
                else              //��ɨ����ͨ����
                {
                    tempAddr   = LOOP[LN].LPFirstAddr; 
                    tempDevNum = LOOP[LN].TNum - LOOP[LN].HPNum;
                }

                for (i = 0; i < tempDevNum; i++)//��ĳ���ȼ�������Ѱ��
                {
                    BuildPass = 0;   
                    TypePass = 0;   
                    Zo_Fl_Ma_Pass = 0;
                    
                    //   ¥��Ϊ0    ��  ¥�����
                    if ((controllerID == 0) || (controllerID == ALLDevices[LN][tempAddr].DeviceBuilding))
                    { 
                        BuildPass = 1;
                    }
                    //�����������Ҳһ����Ϊ0��������ʾ��������
                    if (0x01 == category)       //����
                    {
                        if ((loop == 0) || (loop == ALLDevices[LN][tempAddr].DeviceZone))
                        	Zo_Fl_Ma_Pass = 1;
                    }
                    else if (0x02 == category)  //���
                    {
                        if ((loop == 0) || (loop == ALLDevices[LN][tempAddr].DeviceFloor))
                        	Zo_Fl_Ma_Pass = 1;
                    }
                    else                     //��������
                    {
                        Zo_Fl_Ma_Pass = 1;
                    }
                    //Ȼ�������ͼ��
                    temp = ALLDevices[LN][tempAddr].DeviceType;
                    
                    if (is_detector(temp))		//����ģ�鲻���
                    { 
                    }                               
                    else if (ALLDevices[LN][tempAddr].DeviceCondition & (1 << 1)) //�����β����
                    {
                    }
                    else if (temp == address)   	//�������
                    {   
                    	TypePass = 1;   
                    }
                    else if (address == DEVICE_MODULE)	//���Ͳ�����������ָ���������ģ��
                    {
                        if (DEVICE_IO_TYPE[temp] == 0x01 || DEVICE_IO_TYPE[temp] == 0x11)
                        {   
                        	TypePass = 1;   
                    	}  
                    }
                   
                    if (BuildPass && TypePass && Zo_Fl_Ma_Pass) //��������������
                    {
                        //������Ԫ��ʽ��D7(��ƽ������)+D6(�򿪻�ر�)+D1(�����Ѿ�ִ��)+D0(������δִ��)
                        temp = LDList[LN][tempAddr] ^ control_type;

                        if ((temp & 0x40) || (0 == LDList[LN][tempAddr]))//�������͸ı䣬���巽ʽ��ƽ��ʽ������һ�µ�
                        {
                            LDList[LN][tempAddr] = (control_type & 0xc0) | 0x01;  	//������������û��ִ��
                            LOOP[LN].OptStatFlags.StateBit.HaveLDDev_flag = 1; 		//����Ҫ����������
                        }
                    }

                    tempAddr = ALLDevices[LN][tempAddr].NextAddr;
                }
            }
        }
    }
    else
    {   
    	return;   
    }

    /**************�������Ѿ������������ã��������һ��ͳ��*****************/
    //������ÿһά����ĵ�0��Ԫ�ش��������ָ�룬��255�ݼ���Ϊ0��ʾ��ǰ
    //��·û���������������2����·��û������������Ҫ��HaveLDDev_flag
    if (LOOP[0].OptStatFlags.StateBit.HaveLDDev_flag || LOOP[1].OptStatFlags.StateBit.HaveLDDev_flag)
    {
        for (LN = 0; LN < 2; LN++)
        {
            for (i = 255; i >= 0; i--) //�Ӹߵ�ַ��͵�ַ���һ�û��ִ�еĵ�һ����������
            {
                //D7(��ƽ������)+D6(�򿪻�ر�)+D1(�����Ѿ�ִ��)+D0(������δִ��)
                if (LDList[LN][i] & bit0)    //D0(������δִ��),
                {
                    LDNode[LN] = i;  
                    break;
                } 
            }

            if (i < 0)
            {
				LDNode[LN] = -1;
            }
        }
    }

    /*********������ĳ����·�����Σ���ô����ָ��Ϊ0����ָ���κ�����*********/
    for (LN = 0; LN < 2; LN++) 
    {
        if (LOOP[LN].Defi & (1 << 1)) 
        {
            LDNode[LN] = -1;
            
            for (i = 255; i >= 0; i--) 
            {
                LDList[LN][i] &= 0xc0;
            }
        }
    }
}

/*
********************************************************************************
* ��������: static u8 _FindLDListNext( u8 LN );
* ��������: ����ĳ��·����������һ��Ҫ����������
* ��    ��:LN ��·��0��1   addr:�����������Ĳ�����ַ
* �� �� ֵ:0��ʾû������Ҫ����1��ʾ������Ҫ����
* ע    ��:
********************************************************************************
*/
static bool FindLDListNext(u8 LN, u8 *addr)
{
    int temp;
    
    if (LDNode[LN] != -1) 
    {
		temp = LDNode[LN];
		
        for ( ; temp >= 0; temp--) 
        {
            LDNode[LN] = temp - 1;   //�´ν����������ַ��ǰһ����ַ��ʼ�ж�
            if (temp == -1)
            	LDNode[LN] = 255;
            	
            //D7(��ƽ������)+D6(�򿪻�ر�)+D1(�����Ѿ�ִ��)+D0(������δִ��)
            if (LDList[LN][temp] & bit0)  //D0(������δִ��)
            {
                //Ϊ���ݴ��ٴ��жϻ�ȡ�ĵ�ַ����û����������û�б�����
                if ((ALLDevices[LN][temp].DeviceCondition & (1 << 0)) && (!(ALLDevices[LN][temp].DeviceCondition & (1 << 1))))
                {
                    if (LDList[LN][temp] & bit7)  //�������
                    {
                        if (LDList[LN][temp] & bit6) //�ر�
                        {
                            ALLDevices[LN][temp].Comd = 0x37; // ���ξ���һ�Σ����Բ�ӦΪ3��
                        } 
                        else             //��
                        {           
                            ALLDevices[LN][temp].Comd = 0x38;
                        }
                    } 
                    else 
                    {                        //�����ƽ
                        if (LDList[LN][temp] & bit6) //�ر�
                        {                        
                            ALLDevices[LN][temp].Comd = 0x37;
                        } 
                        else                     //��
                        {   
                            ALLDevices[LN][temp].Comd = 0x36;
                        }
                    }
                    
                    ClrBit(LDList[LN][temp], bit0);  //תȥִ��֮ǰ��������ִ�б�־
                    SetBit(LDList[LN][temp], bit1);  //��ʹû��ִ�гɹ�Ҳ������ִ����

                    *addr = temp;
                    
                    return TRUE;
                }
            }
        }
        return FALSE;
    }
    else 
    {
        LOOP[LN].OptStatFlags.StateBit.HaveLDDev_flag = 0;

        return FALSE;
    }
}

/*
********************************************************************************
* ��������: static u8 GetNextDevAddr( u8 LN );
* ��������: ���ĳ��·��������һ������ɨ��������ĵ�ַ
* ��    ��: u8 LN :��·�� 0,1
* �� �� ֵ: ������ַ����Ϊ0����0(��������ɨ���ɨ��
* ע    ��:
********************************************************************************
*/
static u8 GetNextDevAddr(u8 LN)
{
    u8 tempAddr;

    // ���ȴ������¼������������
    /************************************************/
    // A:��·��������������ʱ����255��汾��ɨ���ַ��Խ128ʱ��
    //   ��Ҫר��ɨ��0��128��ַ�������ĵƲ�Ϩ�𣬷���ʱ������
    if (LOOP[LN].OptStatFlags.StateBit.OldJmpLedSolution_flag && (!AutoRegister_flag))  
    {
    	  LOOP[LN].OptStatFlags.StateBit.OldJmpLedSolution_flag = 0;
    	  
    	  if( LOOP[LN].OptStatFlags.StateBit.OldJmpLedGoNxt_flag ) 
          {
    	  	  LOOP[LN].OptStatFlags.StateBit.OldJmpLedGoNxt_flag = 0;
    	  	  return( LOOP[LN].OldJmpNxdAddr );
    	  }
    	  LOOP[LN].OptStatFlags.StateBit.OldJmpLedGoNxt_flag = 0;
    }
    /************************************************/
    //B: ��·��û���������·�����κ����ֵ����0
    if (LOOP[LN].RNum == 0)    
    	return 0;   //��0���ɨ
    /************************************************/
    //C: ��·��������
    tempAddr = LOOP[LN].OptingAddr;
    //0���Ƿ�ɨ�豻������ʾ�̵Ĺ㲥�����Ϲ� 2007-8-28 add
    if (LOOP[LN].OptStatFlags.StateBit.InterruptByBroadCastCmd_flag)  
    {
        LOOP[LN].OptStatFlags.StateBit.InterruptByBroadCastCmd_flag = 0;
        return tempAddr;
    }
    
    //1��Ҫ���ж���û������Ҫ���ظ�ɨ��
    if (ALLDevices[LN][tempAddr].Comd & 0xf0)  
    {
        //Ҫ�������ɨ�������Ϊ0�����ȥ��ɨ��ǰ����
        ALLDevices[LN][tempAddr].Comd -=0x10;
        return tempAddr;
    }
    
    //2,�Ƿ����Զ���½״̬
    if (AutoRegister_flag) 
    {    	
        LOOP[LN].OptingAddr =  ALLDevices[LN][tempAddr].NextAddr;
        return( LOOP[LN].OptingAddr );
    }
    
    //3,�Ƿ���������Ҫ����
    if( 0 == LOOP[LN].OptStatFlags.StateBit.HaveLDDev_flag )
    {
        GetLoopLDCmdAndExe();
    }
    if ( LOOP[LN].OptStatFlags.StateBit.HaveLDDev_flag ) 
    {
        if (FindLDListNext(LN, &tempAddr))		//������ 
        {
            LOOP[LN].OptingAddr = tempAddr;
            return tempAddr;
        }
        else 
        {
        	LOOP[LN].OptStatFlags.StateBit.HaveLDDev_flag = 0;
        }
    }

    if( LOOP[LN].OptStatFlags.StateBit.RstCloseModu_flag ) 
    {
        if( !LOOP[LN].OptStatFlags.StateBit.HaveLDDev_flag ) 
        {
            LOOP[LN].OptStatFlags.StateBit.RstCloseModu_flag = 0;
        } // ���û��������Ҫ����˵����ǰ��·�ϵ�ر�ģ��Ķ��������
    }
    
    //4,ÿ��500ms�����û�д��ڻ𾯹�����ʱ�������������,����¼���ʱ�Ƿ񵽣����˾�Ҫתȥ����
    // û�еȴ�ȷ�ϻ𾯡����ϵ�������ת��ɨ���������ɨ��
    // ע�⣺�°������ȡ���˻𾯹�����ʱ��
    if( (0 != LOOP[LN].NeedCheckAddr) && (!AutoRegister_flag) ) 
    {  
        tempAddr = LOOP[LN].NeedCheckAddr;
        LOOP[LN].NeedCheckAddr = 0;
        return tempAddr;
    }
    
    //5,ÿ��һ��ʱ��ͽ������ͼ�⣬�����ߵ��˴�Ӧ�������ϸ���������������Ż����
    if ((GetTickCount() - CheckTypeTimeTicks) > (600000 / MSPERTICK)) // ÿ��10������һ�����ͼ��
    {
        CheckTypeTimeTicks = GetTickCount();
        
        for (tempAddr = 255; tempAddr > 0; tempAddr-- ) //���ͼ������
        {
            ALLDevices[0][tempAddr].Comd = 0x15 + (DA_TE_CNT_TIMES << 4);
            ALLDevices[1][tempAddr].Comd = 0x15 + (DA_TE_CNT_TIMES << 4);
        }
    }
    
    //6,�����·����������̫�ٵ��������ʱɨһȦʱ��̣ܶ�����������������
    //  �δ���Ϊ�Ĳ���0�룬����ʹɨһȦ��ʱ�䲻��̫�̡�
    if (LOOP[LN].RNum < MIN_DEVICE_NUM) 
    {
        // ������� = ��ɨ���� + ����ɨ��Ĵ���(��������¾�����ɨ�����������)��
        // ������MIN_DEVICE_NUM�󣬼���������
        LOOP[LN].ScanedCount ++;
        if(LOOP[LN].ScanedCount >= LOOP[LN].RNum)  // ɨ����ȫ����������ʼ�����ɨ
        {
            if( LOOP[LN].ScanedCount >= MIN_DEVICE_NUM ) 
            {
                LOOP[LN].ScanedCount = 0;
            } 
            else 
            {
                // �����������MIN_DEVICE_NUM��,�����һ��Ϊ��ģ��,������ʱ�䳤
                if (LOOP[LN].ScanedCount == LOOP[LN].RNum) 
                {
                    tempAddr = LOOP[LN].OptingAddr;
                    if ((0x08 <= ALLDevices[LN][tempAddr].DeviceType) && (tempAddr > 128)) 
                    { // ��ʱ�п�������ģ��
                        LOOP[LN].OptStatFlags.StateBit.OldJmpLedSolution_flag = 1;
                        LOOP[LN].OptStatFlags.StateBit.OldJmpLedGoNxt_flag    = 0;
                        return 128; //���ؾ�ģ���0��ַ
                    }
                } // ��ƽ�����
                return 0; //��ɨ
            }
        }
    } // ����Ӧ��ɨ���·�ϴ��ڵ�������������
    /**************************************************************************/
    /* ����Ϊ������ɨ�裬���޸�LOOP[LN].OptingAddr��ɨ����Ϻ󻹴�            */
    /* LOOP[LN].OptingAddr����ʼɨ��                                          */
    /*******************����Ϊ����ɨ���ַ��ȡ*********************************/
    tempAddr = LOOP[LN].OptingAddr;
    tempAddr = ALLDevices[LN][tempAddr].NextAddr;

    if (0 == tempAddr) // NULL,˵���ǵ�������Ľ�β
    { 
        LOOP[LN].OptingAddr = LOOP[LN].LPFirstAddr;  //ת��ѭ������ϵ㴦
        return ( LOOP[LN].OptingAddr );
    } 
    else //�����������β�����������ڵ㣬�ýڵ�ָ�����һ��������ַ��Ч 
    {
        tempAddr = LOOP[LN].OptingAddr;
        // �ж��Ƿ�Ӵ˴�ת�뵥������(�����ȼ���������)��ɨ��
        if (ALLDevices[LN][tempAddr].OtherFlags.FlagBit.Go2Prior_flag) 
        {
            if (LOOP[LN].HPNum > 0) // �����ȼ�����������
            { 
                LOOP[LN].LPFirstAddr = ALLDevices[LN][tempAddr].NextAddr;  //����ӵ��������з��ص�������ַ
                LOOP[LN].OptingAddr  = LOOP[LN].HPFirstAddr;
                return ( LOOP[LN].OptingAddr );
            }
        }

        LOOP[LN].OptingAddr  = ALLDevices[LN][tempAddr].NextAddr;
        
        return LOOP[LN].OptingAddr;
    }
}

/*
********************************************************************************
* ��������: void DevOptInitAndRun(u8 LN);
* ��������: ĳ��·����һ����������֮ǰ�Ĺ��̱����ȵĳ�ʼ������������
* ��    ��: u8 LN ����·�� 0,1
* �� �� ֵ: ��
* ע    ��:
********************************************************************************
*/
void DevOptInitAndRun(u8 LN)
{
    u8 j,tempAddr;
    u8 addr, cmd;

    /**********************************************/
    OptingLoopNO = LN;                 // Ҫ�����Ļ�·��
    tempAddr     = GetNextDevAddr(LN); // Ҫ�����ĵ�ַ��
    /**********************************************/
    if(tempAddr == 6)
    {
        j = 6;
    }
    // ��ʼ�����������������
    if ((LDNode[LN] != -1) || AutoRegister_flag)// ������������Ĵ������� 
    { 
        LOOP[LN].OldJmpPreAddr = tempAddr;
    } 
    else 
    {
        j = LOOP[LN].OldJmpPreAddr;
        if (((j < 128) && (tempAddr> 128)) || ((j > 128) && (tempAddr < 128))) // �ߵ͵�ַ��ת
        { 
            if (ALLDevices[LN][j].OtherFlags.FlagBit.IsOldDev_flag) // ��ǰһ��������������
            { 
                LOOP[LN].OptStatFlags.StateBit.OldJmpLedSolution_flag = 1;
                LOOP[LN].OptStatFlags.StateBit.OldJmpLedGoNxt_flag    = 1;
                LOOP[LN].OldJmpNxdAddr = tempAddr; // ��������Ҫ����ɨ���������ַ

                if (j > 128) 
                {
                    tempAddr = 128;                // ����ģ���ɨ
                    LOOP[LN].OldJmpPreAddr = 0;    // �´β��ٽ���
                } 
                else 
                {
                    tempAddr = 0;                  // ����̽������ɨ
                    LOOP[LN].OldJmpPreAddr = 129;  // �´β��ٽ���
                }
            }
        } 
        else  
        {
        	LOOP[LN].OldJmpPreAddr = tempAddr;
        }
    } // ��������ƽ�����

    /**********************************************/
    // ��Ʋ��Դ����ɳ���һ����·�������һ���ƣ�
    //�˴�����һ����·��һ����
    // �жϱ���Ʋ��Ե������Ƿ��ǵ�ǰ����
    if ((0 != LOOP[LN].DevLedTestAddr ) && (LOOP[LN].DevLedTestAddr == tempAddr)) 
    {
        ALLDevices[LN][tempAddr].Comd &= 0xf0;
        ALLDevices[LN][tempAddr].Comd += 0x0a; //���������
    }
    /**********************************************/
    LOOP[LN].RealOptingAddr = tempAddr;
    LOOP[LN].OptStatFlags.StateBit.OptFinish_flag = 0;
    LOOP[LN].LoopAnsStat = 0;  // �����״̬�ֽ�

    if (0 == tempAddr) 
    	LOOP[LN].OptStatFlags.StateBit.SendAddr0_flag = 1; // ָʾ������0��
    else                
    	LOOP[LN].OptStatFlags.StateBit.SendAddr0_flag = 0;

    addr = tempAddr;
    cmd  = ALLDevices[LN][tempAddr].Comd & 0x0f;

    // ����뷢�����߻��ԵĹ㲥����
    if( LOOP[LN].OptStatFlags.StateBit.SendBroadCastCmdToDisp_flag )
    {
        if ((LOOP[LN].CmdBeforeBroadCmd >= 0x09) ||     // �Ƕ��������һ�����������ִ����
            (LOOP[LN].CmdBeforeBroadCmd <  0x06)) 	// ��ʱ���Է��㲥����
        { 
            if ( 0 == ( LOOP[LN].BroadCmdSendToDisp >> 6 ) )
            { // Уʱ��Сʱ
                j = LOOP[LN].BroadTimeHourVal;
                LOOP[LN].BroadCmdSendToDisp = 1 << 6;
                LOOP[LN].OptStatFlags.StateBit.SendBroadCastCmdToDisp_flag = 1; // ׼���ͷ�
            } 
            else if( 1 == ( LOOP[LN].BroadCmdSendToDisp >> 6 ) ) 
            { // Уʱ������
                j = ( 1 << 6 ) + LOOP[LN].BroadTimeMinuteVal;
                LOOP[LN].OptStatFlags.StateBit.SendBroadCastCmdToDisp_flag  = 0;
            } 
            else 
            { // �����㲥��������Լ�
                j = LOOP[LN].BroadCmdSendToDisp;
                LOOP[LN].OptStatFlags.StateBit.SendBroadCastCmdToDisp_flag  = 0;                
            }
            //FillLoopBit( LN, j, 0x0f ); // �㲥=0x0f+val
            addr = j;
            cmd  = 0x0f;
            LOOP[LN].OptStatFlags.StateBit.InterruptByBroadCastCmd_flag = 1;
        } 
        else if(((ALLDevices[LN][tempAddr].Comd & 0x0f) >= 0x06)&&((ALLDevices[LN][tempAddr].Comd & 0x0f) <= 0x09))
        { // ����������ô��
            LOOP[LN].CmdBeforeBroadCmd = ALLDevices[LN][tempAddr].Comd & 0x0f;
        }
    } 
    else    
    	LOOP[LN].CmdBeforeBroadCmd = ALLDevices[LN][tempAddr].Comd & 0x0f;

    for (j = 0; j < 5; j++) 
    {
        LOOP[LN].PWn[j] = 0;
    }

    //��ò�ͬ���͵������ڲ�ͬ����������Ӧ�û���ĸ���
    if (is_detector(ALLDevices[LN][tempAddr].DeviceType))  					 // ̽����
    {
        AnsCodeCount = AnsCodeNUM[0][0x0f & ALLDevices[LN][tempAddr].Comd];  //�������
    }
    else 											// ģ�� 
    {
         AnsCodeCount = AnsCodeNUM[1][0x0f & ALLDevices[LN][tempAddr].Comd];  //�������
    }
//    if (LN == 1)
//    {
//        LN = 1;
//        addr = 40;
//        tempAddr = 40;
//        AnsCodeCount = 4;
//        ALLDevices[LN][tempAddr].Comd = 0x05;
//        loop_send(1,40,0x05,4);    
//    }
//    else
        loop_send(LN, addr, cmd, AnsCodeCount);
    
    return;
}

/*
********************************************************************************
* ��������: AutoRegInit
* ��������: ��·�Զ���½ǰ��ʼ��������
* ��    ��: ln_no:BIT0 ������һ��·BIT1 �����ڶ���·
* �� �� ֵ: ��
********************************************************************************
*/
void AutoRegInit(u8 ln_no)
{
    u32 i, j;

    auto_res_ln = ln_no;

    for (j = 0; j < 2; j++) 
    {
        for (i = 255; i > 0; i--) 
        {
            ALLDevices[j][i].DeviceCondition = 0;  //���µ�½��,����,����,�����Ч
            ALLDevices[j][i].DeviceType      = 0xff;
            //ALLDevices[j][i].OtherFlags.Val = 0; ������2����־����������
            ALLDevices[j][i].OtherFlags.FlagBit.Go2Prior_flag = 0;
            ALLDevices[j][i].OtherFlags.FlagBit.IsOldDev_flag = 0;
            ALLDevices[j][i].OtherFlags.FlagBit.IsDuAddr_flag = 0;
            ALLDevices[j][i].Comd           = 0x05;
            ALLDevices[j][i - 1].NextAddr   = i;
            ALLDevices[j][i].ReScanStat3    = 0;
            ALLDevices[j][i].ReScanStat4    = 0;
            ALLDevices[j][i].ReScanStat5   &= 0xf3;
        }
        
        ALLDevices[j][255].NextAddr = 1;
        ALLDevices[j][0].Comd       = 0x05;

        LOOP[j].OptingAddr = 0;
        LOOP[j].RNum       = 255;
        LOOP[j].TNum       = 255;
    }
}

/*
********************************************************************************
* ��������: u8  XJCMD(u8 LN, u8 Addr);
* ��������: ��ȡ������ĳ����·��ĳ��������Ѳ�����
* ��    ��: u8  LN   : 0,1
*           u8  Addr : 1~255
* �� �� ֵ: 1���ֽڵ�Ѳ��������������   ����0x0a
*                              û�л������ ����0x0d
*                              �����������   ����0x00
********************************************************************************
*/
u8  XJCMD(u8 LN, u8 Addr)
{
    if (ALLDevices[LN][Addr].ReScanStat2 & (DevIsFireStat | DevIsOutFire))
        return 0x0a;
    else
        return ((LOOP[LN].OptStatFlags.StateBit.LoopLedFlashOff_flag ||
                (ALLDevices[LN][Addr].DeviceCondition & (1 << 3)))?0x0d:0x00);
}
void light_dev(uint8_t dat)
{
    LOOP[1].DevLedTestAddr = dat;
}

