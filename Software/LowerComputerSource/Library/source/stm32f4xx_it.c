/**
 ******************************************************************************
 * @file    Project/STM32F4xx_StdPeriph_Templates/stm32f4xx_it.c
 * @author  MCD Application Team
 * @version V1.8.1
 * @date    27-January-2022
 * @brief   Main Interrupt Service Routines.
 *          This file provides template for all exceptions handler and
 *          peripherals interrupt service routine.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2016 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"
#include "batmana.h"
#include "cst816t.h"
#include "font.h"
#include "global.h"
#include "gui.h"
#include "lcd.h"
#include "pulsecoll.h"
#include "rtc.h"
#include "swmode.h"
#include "upcom.h"
#include "usart1.h"

/** @addtogroup Template_Project
 * @{
 */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
 * @brief  This function handles NMI exception.
 * @param  None
 * @retval None
 */
void NMI_Handler(void) {}

/**
 * @brief  This function handles Hard Fault exception.
 * @param  None
 * @retval None
 */
void HardFault_Handler(void)
{
    /* Go to infinite loop when Hard Fault exception occurs */
    while(1) {}
}

/**
 * @brief  This function handles Memory Manage exception.
 * @param  None
 * @retval None
 */
void MemManage_Handler(void)
{
    /* Go to infinite loop when Memory Manage exception occurs */
    while(1) {}
}

/**
 * @brief  This function handles Bus Fault exception.
 * @param  None
 * @retval None
 */
void BusFault_Handler(void)
{
    /* Go to infinite loop when Bus Fault exception occurs */
    while(1) {}
}

/**
 * @brief  This function handles Usage Fault exception.
 * @param  None
 * @retval None
 */
void UsageFault_Handler(void)
{
    /* Go to infinite loop when Usage Fault exception occurs */
    while(1) {}
}

/**
 * @brief  This function handles SVCall exception.
 * @param  None
 * @retval None
 */
void SVC_Handler(void) {}

/**
 * @brief  This function handles Debug Monitor exception.
 * @param  None
 * @retval None
 */
void DebugMon_Handler(void) {}

/**
 * @brief  This function handles PendSVC exception.
 * @param  None
 * @retval None
 */
void PendSV_Handler(void) {}

/**
 * @brief  This function handles SysTick Handler.
 * @param  None
 * @retval None
 */
void SysTick_Handler(void) {}

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/
void EXTI0_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line0) != RESET) enter_standby();
    EXTI_ClearITPendingBit(EXTI_Line0);
}

void EXTI2_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line2) != RESET) { button_ctrl(); }
    EXTI_ClearITPendingBit(EXTI_Line2);
}

void EXTI15_10_IRQHandler()
{
    if(EXTI_GetITStatus(EXTI_Line12) != RESET)
    {
        if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_12))
        {
            gui_draw_pic(10, 10, 15, 15, gImage_bllink);
            is_btconnected = 1;
        }
        else
        {
            gui_draw_pic(10, 10, 15, 15, gImage_bloff);
            is_btconnected = 0;
            TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);
        }
        EXTI_ClearITPendingBit(EXTI_Line12);
    }
    if(EXTI_GetITStatus(EXTI_Line13) != RESET)
    {
        if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13) == 0)
            gui_draw_pic(181, 7, 8, 15, gImage_charge);
        else lcd_fill(181, 7, 189, 25, BLACK);
        EXTI_ClearITPendingBit(EXTI_Line13);
    }
}

void TIM3_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
    {
        guanpv = get_pulse_val() / 1000.0f;
        send_data_to_uppc((float)(times) / 1000.0f, guanpv);
        times += 5;
    }
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
}

void TIM4_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
    {
        DateTime dt = rtc_rdatetime();
        gui_update_power();
        gui_show_dt(10, -50, dt);
    }
    TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
}

uint8_t dt[10];
uint8_t dt_flag = 0;

void USART1_IRQHandler(void)
{
    uint8_t ucTemp;
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
        ucTemp = USART_ReceiveData(USART1);
        if(ucTemp == 0x74 || dt_flag > 0)
        {
            dt[dt_flag++] = ucTemp;
            if(dt_flag == 10)
            {
                if(dt[8] == 0x0d && dt[9] == 0x0a)
                {
                    // 使用串口调整时间日期
                    rtc_wdatetime(dt[1], dt[2], dt[3], dt[4], dt[5], dt[6], dt[7]);
                    USART_SendData(USART1, 0xAA);
                }
                else USART_SendData(USART1, 0xFF);
                dt_flag = 0;
            }
        }
        else
        {
            dt_flag = 0;
            USART_SendData(USART1, ucTemp);
        }
    }
}

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
