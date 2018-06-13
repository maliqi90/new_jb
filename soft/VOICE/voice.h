#ifndef _VOICE_H
#define _VOICE_H
#include "include.h"

#define P_DATA_PIN  GPIO_Pin_3
#define P_DATA       GPIOC   

#define P_CLK_PIN  GPIO_Pin_2
#define P_CLK      GPIOC

//����Ƭ�ζ���
#define FIRE_VOICE         0 //����ʾ��
#define FAULT_VOICE        1 //������ʾ��
#define BAT_FAULT_VOICE    2 //��ص���������ʾ��
#define NORMAL_VOICE       3 //��������
#define POLL_VOICE         4 //Ѳ������
#define NETFIRE_VOICE      5 //������

#define SLIENCE_VOICE      32 //����

#define FIRE_VOICE1        0  //������
#define FAULT_VOICE1       1  //������ʾ��

#define DHR12R1_Offset             ((unsigned int)0x00000008)
#define DHR12R2_Offset             ((unsigned int)0x00000014)
#define DHR12RD_Offset             ((unsigned int)0x00000020)
#define CR_CLEAR_Mask              ((unsigned int)0x00000FFE)
#define SWTRIGR_SWTRIG_Set         ((unsigned int)0x00000001)
#define CR_EN_Set                  ((unsigned int)0x00000001)
void  Line_2A_WTN6(uint8_t SB_DATA);
void Line_1A_WTN6( uint8_t SB_DATA);
typedef struct 
{
	u8 chRIFF[4];
	u32 wavlen;
	u8 chWAV[4];
	u8 chFMT[4];
	u16 formart;
	u16 CHnum;
	u32 SampleRate;
	u32 speed;
	u16 ajust;
	u16 SampleBits;
    u16  Bitnum;
	u8 chDATA[4];
	u32 DATAlen;
}WAV_file;

typedef struct
{
   u8 palyflag;//���ű��λ��0 ֹͣ���ţ�1��ʼ����
   u8 Bitnum;  //λ��
    volatile u8 DACdone;//DACת�����
   volatile u16 DApc;  //DACת��ָ��  
   u8 CHanalnum;
   u32 DAPointer;
   u32 voice_address; //�����ļ���ʼ��ַ
   u32 voice_address1;
   u8 playtype;//����ģʽ  
   u8 playstop; 
    u8 playcount;//���ż���
     
    
}WAV_Paly;
extern void Voice_GpioInit(void);
extern void Voice(uint8_t voicenum, uint8_t count);
u8 Playwav(void);
void PaltWav_Task(void);
u8 Play_Voice(u32 addr,u8 type);
void slience(void);
#endif

