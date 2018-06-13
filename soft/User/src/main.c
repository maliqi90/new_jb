/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/main.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */  

/* Includes ------------------------------------------------------------------*/

#include "include.h"

//#include "UI_APP.h"
extern void WinLoad(uint8_t index);
extern void Key_Task(void);
static char In_fire_flag = 0;
/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */
const char gprs_checksim[]    = "AT+CCID\n";
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
// USART_InitTypeDef USART_InitStructure;

/* Private function prototypes -----------------------------------------------*/
#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

    GPIO_InitTypeDef GPIO_InitStructure;
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
uint32_t NumberID = 0;
uint8_t i = 0;
//volatile uint8_t fire_flag = 0;
//volatile uint8_t fault_flag = 0;
volatile uint8_t selience_flag;
//static uint8_t buff3[2048];
volatile u8 Time_interr; //时间显示 1：时间刷新 0：时间不刷新
extern volatile u8 timer_interr_flag; //秒中断时间计数器
can_tx_msg data;
can_tx_msg tx;
//static uint8_t buff[4] ="AT/n";
static uint8_t send_buff[7];
extern uint8_t USB_Flag;
extern uint16_t light_set;
volatile uint8_t gprs_flag = 0;//设置gprs标志位
 uint8_t power_bat_flag = 0;//电量检测标志位
int main(void)
{
   // char i;
    USB_Flag = 1;
   SysTick_Config(SystemCoreClock /1000);
   RCC->AHBENR  |= (1<<6);
   sFLASH_Hardware_Init();
    	Read_Time_Bkp();
   TIMER6_Init();//延时定时器初始化
    TIMDelay_Nms(500);
    TIMDelay_Nms(500);
//    TIMDelay_Nms(500);
//    TIMDelay_Nms(500);



    GUI_Init();
    //WinLoad(W2_MAIN_WINDOW_WIN);
     PWM_Set(500);
	    EventInit();
	 //time_show();
      GPIO_Ctrl_Init();
  LOOP_CONFIG();

    TIM5_CONFIG();//按键扫描定时器初始化
    RTC_Init();
    
    norflash_read_comm_set((uint8_t*)&comm_set);//读取 设置
//    if(comm_set.Can_Set == 1)
//    {
//        //RCC_APB1PeriphClockCmd(RCC_APB1Periph_USB, DISABLE);
//        CAN1_CONFIG();
//        can_init();  
//        USB_Flag = 0;
//         
//    }
//    else
//    {
        Set_System();
        Set_USBClock();
        USB_Interrupts_Config();
        USB_Init();
         USB_Flag = 1;        
//    }

//     norflash_read_comm_set((uint8_t*)&comm_set);



       // WinLoad(W2_MAIN_WINDOW_WIN);
   
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = GPIO_IN_PIN;
    GPIO_Init(GPIO_IN_CTR, &GPIO_InitStructure);
    Voice_GpioInit();
   
//   //RELAY_ON;

// //   W1_MainWindowDisplay();
//   // W2_MainWindowDisplay();
    LoopRstInit();

    Time_interr = 1;
    key_flag = 0;
    loop_power_enable(1);
    //loop_send_24V(1);
// loop_send_0V(1);
// loop_send_5V(1);
//NumberID =  sFLASH_NOR_ReadID();

 	
//    Timer_value.year=2013;
//	  Timer_value.month=9;
//	  Timer_value.date=25;
//		Timer_value.hour=6;
//		Timer_value.min=31;
//		Timer_value.sec=00;
//		Write_Time_RTC();
//     Save_Time_Bkp();
       
    norflash_read_comm_set((uint8_t *)&comm_set);     
     UART2_Init();    
//    Led_Ctrl(LEDBAT_ON);
     gprs_init();

     data.Data[0] = 0x55;
     data.StdId = 1;
     data.DLC = 1;
     data.ExtId = 0;
    LightShieldLed();
     event_porwer_on_off();//记录开关机事件

//        GUI_Init();
       WinLoad(W2_MAIN_WINDOW_WIN);
        norflash_read_LightSet(&light_set);
       if(light_set > 500)
       {
          light_set = 500;
          norflash_write_LightSet(light_set);
       }
       PWM_Set(light_set);
       Play_Voice(NORFLASH_KAIJI_VOICE_BASE,0);
      phone_init();//电话号码初始化
        lcd_time = LCD_TIME;
        //adc_init();//ADC初始化
    while(1)
    {
      
        PaltWav_Task();
        loop_handle();
        GPRS_STATAS();
       
        Key_Task();    
        RTC_Dis();
        EventTask();

          
        if(GPIO_IN_STA == 0)
        {
             if(In_fire_flag == 0)
            {
                
                In_fire_flag = 1;
                send_buff[1] = EVENT_TYPE_ALARM;
                send_buff[2] = 230;
                send_buff[3] = 0xFD; //输入火警
                send_buff[4] = 0;
                send_buff[5] = 0;
                send_buff[6] = 0;
                Loop_Revice(send_buff);
            }

        }
        if( busaddr_flag == 2)
        {
            norflash_write_comm_set((uint8_t *)&comm_set);
        }
       
        UART3_Task();
         // LightShieldLed();
//        NumberID = ADC_Check();
        if(USB_Flag == 0)
        {
         // communication_handle();
            // can_transmit(&data);
        }
        else
        {
             USB_Task();
        }
        
    }
}




/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
