#ifndef __LOOP_H__
#define __LOOP_H__
#define  MSPERTICK    			10 
#include "stm32f10x.h"
#include "def.h"
typedef struct __loop
{
    u8 Defi;          // ��·�������ϸ�����ݽṹ.DOC��
    u8 TNum;          // �û�·�ϵ��������� (Total number)
    u8 RNum;          // ʵ���ϸû�·�ϵ�������(Real number)
    u8 HPNum;         // Ҫ������ɨ�����������(High Priority device number)
    /********************/
    u8 HPFirstAddr;   // ��ǰ��·�����ȼ�����(����)����������ַ����ʼ����Ͳ��ٱ仯��ֱ���´����³�ʼ����
    u8 LPFirstAddr;   // ��ǰ��·�����ȼ�����(ѭ��)����������ַ��ת������ȼ�����ʱ������������޸ġ�
    u8 OldJmpNxdAddr; // ��������ת��ָ�����һ��Ҫɨ�������ĵ�ַ
    u8 OldJmpPreAddr; // ��������ת��ո�ɨ�����������ĵ�ַ
    /********************/
    u8 OptingAddr;    // ��ǰ��·�����ڲ�����������1~255����ֱ����һ���������Ͽ�ʼ����֮ǰ�����ֵ�ſɱ��޸ģ�
    u8 RealOptingAddr;// ʵ�ʷ��͵���·���ϵĵ�ַ
    u8 DevLedTestAddr;// ������Ʋ��Եĵ�ַ
    u8 ScanedCount;   // �Ѿ�ɨ��Ĵ���, ��Ҫ�����ɨʱ�����ã���������²���Ҫ
    /********************/
    u8 LoopAnsStat;   // ��·����״ָ̬ʾ
    u8 NeedCheckAddr; // �𾯹���Ԥ������Ҫ��ɨȷ�ϵ�������ַ 070413
    
    u8 BroadTimeHourVal;   // ��������ʾ�̵�ʱ
    u8 BroadTimeMinuteVal; // ��������ʾ�̵ķ�
    u8 BroadCmdSendToDisp; // ��������·����ʾ�̵Ĺ㲥�������ݣ�0x0f��ͷ�����ݵ�ͷ2λ��
    u8 CmdBeforeBroadCmd;  // �㲥����֮ǰ��������������¼���飬��Ϊģ�鶯������˳��������
    union {              // ��������������ʹ�õ�һЩ״̬��־
        u32 Val;
        struct {
        u32  RstCloseModu_flag  : 1;  // �ϵ�ʱ��Ҫ�ȸ�����ģ�鷢�ر�����,��ʱ��λΪ1
        u32  HaveLDDev_flag     : 1;  // ��ǰ��·��ģ����Ҫ����ʱΪ1

        u32  AutoRegFinish_flag : 1;  // �Զ���½�����λ
        u32  AutoRegInited_flag : 1;  // �Զ���½��ʼ�������λ

        u32  LoopLedFlashOff_flag   : 1; // ������·��������Ѳ�����λΪ1
        u32  OldJmpLedSolution_flag : 1; // ��ģ����С��128�ĵ�ַ��Ծ������̽���������128��
                                            // ��ַ��ԾʱΪ1������Ϊ0�����ڽ����ʱled����ʱ�䳤������
        u32  OldJmpLedGoNxt_flag: 1;     // ������� 1:������һ��Ҫ���ĵ�ַ; 0:���ǲ������

        u32  LPLineIsShort_flag : 1;  // ��·��·
        u32  ShortReported_flag : 1;  // ��·��·���ϱ�
        u32  ShortPowerOff_flag : 1;  // ��·���·�رյ�Դ

        u32  OptFinish_flag : 1;     // �շ������������־
        u32  SendAddr0_flag : 1;     // ��0��ʱ��1��ָʾ���������ô��������
        u32  AnsCodeDealOk_flag : 1; // ���봦�������־
        u32  BeginCharge_flag   : 1; // ϵͳ��ֻ��һ����·ʱ����ɨ��֮����Ҫ���10ms,��ʼ����־
        u32  ChargeOK_flag      : 1; // �����ɱ�־
                
        u32  SendBroadCastCmdToDisp_flag : 1; // ������·����ʾ�̷��͹㲥�������CPU
                                                 // ����·���������¸�λ��λ,����ִ����ɺ��·�Լ���λ
        u32  InterruptByBroadCastCmd_flag: 1; // �����·���ɨ����̣����ø�λ֪ͨɨ�����̿�����غ���
        u32  LoopLoseReportType_flag: 1;      // ��·����ʧ��ʽ��Ϊ1ʱ����������Ӧ��Ϊ0ʱȫ�����ֶ�
            
        u32  NeedDownloadCfg_flag : 1;   // ��·��Ҫ����������1���ϵ����1���������λ����0 
        }StateBit;
    }OptStatFlags;
    /********************/
    u32 PWn[5];           // �������
    u32 SendBitsStream;   // ����ʱ������
    u32 LoopShortCntTick; // ��·�����ָ�ʱ��ʹ�ã���ֱ�ӱ���ϵͳtickֵ��ͨ�������ж�ʱ��
    /********************/
} LOOPS;

typedef struct __device
{   // ϵͳҪ������ݽṹ
	u8 DeviceCondition;		//״̬,��λ���壺BIT0,1-���ڣ�0-�����ڣ�BIT1,1-���룬0-������
							//BIT2:1-����ɨ��0-������ɨ��BIT3:1-���Ѳ��0-����Ѳ��   
	u8 DeviceType;         // ��������
	u8 DeviceZone;         // �������ڵ���
	u8 DeviceFloor;        // �������ڵĲ�
	u16 DeviceBuilding;      // �������ڵ�¥��    
	//	u16 DeviceAlarmValue;	//����ֵ,��չֵ 					//		   
    /*******************/
    // ����Ϊ�Զ���
    u8 Comd;         // ����������������ִ�д���(D7~D4ͨ��Ϊ0) + ��������(D3~D0)
    u8 NextAddr;     // ��һ��������������ַ��ͬһ��·�ģ���
    union {             // �����������õ���һЩ״̬��־
        u8 Val;
        struct {
        u8  Go2Prior_flag     : 1; // �¸�����ת����������
        u8  CircuitERR_flag   : 1; // ����������˵�·��������λ�����������Ļ���
        u8  IsOldDev_flag     : 1; // �������������������λ
        u8  IsDuAddr_flag     : 1; // ��ǰ��������
        u8  DisLoseErrAns_flag: 1; // ��ʧ�ָ���������0
        } FlagBit;
    }OtherFlags;

    u8 ReScanStat1;   //������ɨ״̬��������ɨ�쳣��
    u8 ReScanStat2;   //������ɨ״̬�������쳣ȷ�ϣ�
    u8 ReScanStat3;   //������ɨ״̬��������ɨ�쳣��
    u8 ReScanStat4;   //������ɨ״̬�������쳣ȷ�ϣ�
	u8 ReScanStat5;   //    �ָ���־
    u8 ReScanCnt;     //������ͬ�¼���ɨ����������ע���������Ϊģ�����Ϊ2����������4��Ϊ���ģ��
                      //�ļ�������4��Ϊ���ģ��ļ���������Ϊ�˱�����������Ҫ����ɨ�������ô���F

    u8  OtherFauCnt;  // ���롢���ʹ��������ÿ����ѭ��ɨ�������һ�Σ���������涨������ȷ�ϲ��ϱ�
    u32 LoseCntTicks; // ��ʧ����ʹ����ǰ�ļƴΣ�����ʹ�ü�ʱ�İ취������������ʱ��ܺÿ��ƣ�ֱ�ӱ���ϵͳ�δ�
}LOOPDevice;

void LoopRstInit( void );

u16 get_loop_check(u8 loop);

void LoopCfgSaveToFlash(void);

/*
********************************************************************************
* ��������: LoopInSafeMode
* ��������: ��·���밲ȫģʽ����ֹͣɨ�裬ֹͣ�Ի�·����
* ��    ��: ��
* �� �� ֵ: ��
********************************************************************************
*/
void LoopInSafeMode(BOOL stat);

/*
********************************************************************************
* ��������:LoopIsSafeMode
* ��������:�Ƿ�Ϊ��ȫģʽ
* ��    ��: ��
* �� �� ֵ: TRUE->��ȫģʽFALSE->�ǰ�ȫģʽ
********************************************************************************
*/
BOOL LoopIsSafeMode(void);
void LoopLDCmdBufAdd(u8 buff[6]);
u8 GetLoopLDCmdAndExe(void);
void loop_handle(void);
extern uint8_t loop_login_flag;
extern void light_dev(uint8_t dat);
#endif

