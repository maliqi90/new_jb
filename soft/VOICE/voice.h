#ifndef _VOICE_H
#define _VOICE_H
#include "include.h"

#define P_DATA_PIN  GPIO_Pin_3
#define P_DATA       GPIOC   

#define P_CLK_PIN  GPIO_Pin_2
#define P_CLK      GPIOC

//语音片段定义
#define FIRE_VOICE         0 //火警提示音
#define FAULT_VOICE        1 //故障提示音
#define BAT_FAULT_VOICE    2 //电池电量不足提示音
#define NORMAL_VOICE       3 //正常声音
#define POLL_VOICE         4 //巡检声音
#define NETFIRE_VOICE      5 //联网火警

#define SLIENCE_VOICE      32 //静音

#define FIRE_VOICE1        0  //火警语音
#define FAULT_VOICE1       1  //故障提示音

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
   u8 palyflag;//播放标记位；0 停止播放，1开始播放
   u8 Bitnum;  //位数
    volatile u8 DACdone;//DAC转换完成
   volatile u16 DApc;  //DAC转换指针  
   u8 CHanalnum;
   u32 DAPointer;
   u32 voice_address; //声音文件起始地址
   u32 voice_address1;
   u8 playtype;//播放模式  
   u8 playstop; 
    u8 playcount;//播放计数
     
    
}WAV_Paly;
extern void Voice_GpioInit(void);
extern void Voice(uint8_t voicenum, uint8_t count);
u8 Playwav(void);
void PaltWav_Task(void);
u8 Play_Voice(u32 addr,u8 type);
void slience(void);
#endif

