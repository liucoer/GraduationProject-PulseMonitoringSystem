#include "swmode.h"
#include "stm32f4xx_conf.h"

void swmode_init()
{
    GPIO_InitTypeDef GPIO_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;
    EXTI_InitTypeDef EXIT_InitStruct;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);

    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    EXIT_InitStruct.EXTI_Line = EXTI_Line0;
    EXIT_InitStruct.EXTI_LineCmd = ENABLE;
    EXIT_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
    EXIT_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_Init(&EXIT_InitStruct);
    Disable_EXTI(EXTI_Line0);
    NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x03;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x02;
    NVIC_Init(&NVIC_InitStruct);
}

void enter_standby()
{
    PWR_ClearFlag(PWR_FLAG_WU);
    PWR_WakeUpPinCmd(ENABLE);
    PWR_EnterSTANDBYMode();
}
