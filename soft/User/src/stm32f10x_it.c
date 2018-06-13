/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
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
#include "stm32f10x_it.h"
#include "usb_lib.h"
#include "usb_istr.h"
#include "include.h"
#include "can.h"
/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
extern __IO int32_t OS_TimeMS; 
extern volatile uint32_t systickcount;
extern volatile uint8_t faultled_flag;
extern volatile uint8_t faultled_on;
extern loop_hw loop_val;
volatile uint32_t temppp;
uint8_t USB_Flag = 0;//USB和CAN标志位

//struct com_node *SEND_HEAD = NULL;	//发送数据
//struct com_node *REC_HEAD  = NULL;	//接收数据
/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/
void ADC1_2_IRQHandler(void)

{
    ADC_ITConfig(ADC1,ADC_IT_AWD,DISABLE);
    if(SET == ADC_GetFlagStatus(ADC1,ADC_FLAG_AWD))
    {
       LED_BAT_ON;
        LED_ALARM_ON;
       TIMDelay_Nms(35);
       LED_BAT_OFF;
        power_flag = 1;
      // temppp = (uint32_t)ADC_GetConversionValue(ADC1);
       ADC_ClearFlag(ADC1,ADC_FLAG_AWD);
        //ADC1->SR |= 0x01;
      // ADC_ClearITPendingBit(ADC1,ADC_IT_AWD);
    }
    // ADC_ITConfig(ADC1,ADC_IT_AWD,ENABLE);

  /* Clear ADC1 AWD pending interrupt bit */
//  ADC_ClearITPendingBit(ADC1, ADC_IT_EOC);
//  ADC_ClearITPendingBit(ADC1, ADC_IT_AWD);
}
/**
  * @brief  This function handles External line 3 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI1_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line1) != RESET)
  {
    /* Toggle LED1 */
     loop_val.sht[1] = TRUE;

    /* Clear the  EXTI line 0 pending bit */
    EXTI_ClearITPendingBit(EXTI_Line1);
  }
}
void EXTI5_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line3) != RESET)
  {
    /* Toggle LED1 */
     loop_val.sht[1] = TRUE;

    /* Clear the  EXTI line 0 pending bit */
    EXTI_ClearITPendingBit(EXTI_Line3);
  }
}
//void EXTI4_IRQHandler(void)
//{
//  if(EXTI_GetITStatus(EXTI_Line4) != RESET)
//  {
//    /* Toggle LED1 */
//     loop_val.sht[1] = TRUE;

//    /* Clear the  EXTI line 0 pending bit */
//    EXTI_ClearITPendingBit(EXTI_Line4);
//  }
//}
/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
    
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
	 OS_TimeMS ++;
    //故障灯与屏蔽灯切换
    if(faultled_flag == 1)
    {
        systickcount++;
        if(systickcount >200)
        {
            systickcount = 0;
            LED_SHEILD_ON;
            
            
        }
    }


  
}
/*******************************************************************************
* Function Name  : USB_IRQHandler
* Description    : This function handles USB Low Priority interrupts
*                  requests.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USB_LP_CAN1_RX0_IRQHandler(void)
{
//    u8 cnt;
//	can_rx_msg rx_msg;
//	struct com_node *p;
//	struct com_node *q;
//    if(USB_Flag == 1)
//    {
      USB_Istr();
//    }
//    else
//    {
//	
//	cnt = CAN1->RF0R & 3;

//	while (cnt != 0)
//	{										//fifo挂起中断
//		can_rec(&rx_msg);
//		
//		CAN1->RF0R |= CAN_RF0R_RFOM0;
//		
//		cnt = CAN1->RF0R & 7;
//		
//		if (rx_msg.IDE == CAN_ID_STD)
//			continue;
//		
//		p = (struct com_node*)malloc(sizeof(struct com_node));
//		if (p == NULL)
//		{
//			//删除发送缓存的最后一帧数据
//			p = SEND_HEAD->pnext;
//			q = SEND_HEAD;

//			if (p != NULL)
//			{
//				while (p->pnext != NULL)
//				{
//					q = p;
//					p = p->pnext;					
//				}

//				q->pnext = NULL;
//				free(p);												//????? 增加frame_num暂未处理

//				p = (struct com_node*)malloc(sizeof(struct com_node));//????? 增加frame_num暂未处理
//				if (p == NULL)
//				{
//					continue;
//				}
//			}
//			else
//			{
//				continue;
//			}
//		}

//		p->pnext = NULL;
//		
//		p->id = rx_msg.ExtId;
//		memcpy(p->buff, rx_msg.Data, 8);

//		insert_list(REC_HEAD, p);
//	}       
//    }
}
//void USB_LP_CAN1_RX0_IRQHandler(void)
//{
//    //USB_Istr();  
//    uint8_t cnt = 0;
//    cnt = CAN1->RF0R & 3;
//}
/*******************************************************************************
* Function Name  : USB_FS_WKUP_IRQHandler
* Description    : This function handles USB WakeUp interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USBWakeUp_IRQHandler(void)

{
  EXTI_ClearITPendingBit(EXTI_Line18);
}
/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
