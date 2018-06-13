#ifndef _NORFLASH_H
#define _NORFLASH_H
#include "include.h"
#include "sflash_spi.h"

#define NORFLASH_BLOCKS   128 //整块FLASH 总共128块
#define NORFLASH_SECTORS  2048//整块FLASH 总共2048扇区
#define NORFLASH_PAGES    32768//整块FLASH 总共32768页


#define NORFLASH_FONT_BASE_SECTOR    0x2000 //字库基地址
#define NORFLASH_FONT_SECTORS        200//扇区数量

#define NORFLASH_BASE    0x0 //FLASH 基地址
#define JIAZHONGFIRE     NORFLASH_BATFAULT_VOICE_BASE

#define NORFLASH_DEVICE_BASE         0          //现场设备存储基地址 16*8 //16个现场设备
#define NORFLASH_PHONE_BASE          0x1000       //电话号码存储基地址 2*12 两组电话号码

#define NORFLASH_COMM_SET_BASE       0x1018
#define NORFLASH_LIGHT_SET           0x102C      //背光设置存储
#define NORFLASH_EVENT_FLAG_BASE     0x102D      // 事件记录扇区写入标志存储
#define NORFLASH_FIREEVENT_BASE      0XCA000      //火警事件存储基地址 512*32 //512条火警记录
#define NORFLASH_SHIELD_BASE         (NORFLASH_EVENT_BASE + 32*4096)    //屏蔽记录存储基地址 16*32  //总共16条屏蔽记录
#define NORFLASH_EVENT_BASE          (NORFLASH_FIREEVENT_BASE + 32*512)       //事件记录存储基地址 4096*32 //总共10240条事件记录

//各种语音大小

#define NORFLASH_BATFAULT_VOICE_SECTORS   8  //备电故障语音存储扇区数
#define NORFLASH_FAULT_VOICE_SECTORS      19 //故障音存储扇区数
#define NORFLASH_FIRE_VOICE_SECTOES       20 //火警音存储扇区数
#define NORFLASH_HONMEFIRE_VOICE_SECTORS  10 //家中检测到火警音扇区数
#define NORFLASH_KAIJI_VOICE_SECTORS      16 //开机语音存储扇区数
#define NORFLASH_COMMFAULT_VOICE_SECTORS  8  //通信故障存储扇区数
#define NORFLASH_POWERFAULT_VOICE_SECTORS 8  //主电故障语音存储扇区数

//语音存储地址
#define NORFLASH_VOICE_BASE               0xF0000 //语音存储基地址
#define NORFLASH_BATFAULT_VOICE_BASE       NORFLASH_VOICE_BASE//备电故障语音存储基地址
#define NORFLASH_FAULT_VOICE_BASE         (NORFLASH_BATFAULT_VOICE_BASE  + NORFLASH_BATFAULT_VOICE_SECTORS*4096) //故障音存储基地址
#define NORFLASH_FIRE_VOICE_BASE          (NORFLASH_FAULT_VOICE_BASE + NORFLASH_FAULT_VOICE_SECTORS*4096) //火警音存储基地址
#define NORFLASH_HONMEFIRE_VOICE_BASE     (NORFLASH_FIRE_VOICE_BASE + NORFLASH_FIRE_VOICE_SECTOES*4096) //家中检测到火警存储基地址
#define NORFLASH_KAIJI_VOICE_BASE         (NORFLASH_HONMEFIRE_VOICE_BASE + NORFLASH_HONMEFIRE_VOICE_SECTORS*4096) //开机语音存储基地址
#define NORFLASH_COMMFAULT_VOICE_BASE     (NORFLASH_KAIJI_VOICE_BASE + NORFLASH_KAIJI_VOICE_SECTORS*4096) //通信故障语音存储基地址
#define NORFLASH_POWERFAULT_VOICE_BASE    (NORFLASH_COMMFAULT_VOICE_BASE + NORFLASH_COMMFAULT_VOICE_SECTORS*4096)//主电故障语音存储基地址

//图片存储地址

#define PIC_10K_ADDR_BASE                 (NORFLASH_POWERFAULT_VOICE_BASE +   NORFLASH_POWERFAULT_VOICE_SECTORS*4096) //10K图标存储地址
#define PIC_20K_ADDR_BASE                 (PIC_10K_ADDR_BASE + 50*4096)
#define PIC_50K_ADDR_BASE                 (PIC_20K_ADDR_BASE + 50*4096)
#define PIC_MAX_ADDR_BASE                 (PIC_50K_ADDR_BASE + 62*4096)

void norflash_read_device(uint8_t addr, DEVICE_CONFIG *device);
void norflash_write_device(uint8_t addr,uint8_t *pdevice);
void norflash_read_phone(uint8_t addr,uint8_t *data);
void norflash_write_phone(uint8_t addr,uint8_t *data);
void norflash_read_AlarmEvent(uint16_t AlamId,SYSTEM_EVENT *pevent);
void norflash_write_AlarmEvent(uint16_t AlamId,uint8_t *pevent);
void norflash_read_SheildEvent(uint16_t SheildId,SYSTEM_EVENT *pevent);
void norflash_write_SheildEvent(uint16_t SheildId,uint8_t *pevent);
void norflash_read_otherevent(uint16_t OtherEventId,SYSTEM_EVENT *pevent);
void norflash_write_otherevent(uint16_t OtherEventId,uint8_t *pevent);
void norflash_read_font(uint32_t fontaddr,uint8_t *buff,uint8_t len);
void norflash_write_flash(uint32_t address,uint8_t *data,uint32_t len);
void norflash_read_LightSet(uint16_t *led);
void norflash_write_LightSet(uint16_t led);
void norflash_read_comm_set(uint8_t *set);
void norflash_write_comm_set(uint8_t *set);
uint8_t norflash_read_eventflag(uint8_t  *data);
void norflash_write_eventflag(uint8_t data);
#endif

