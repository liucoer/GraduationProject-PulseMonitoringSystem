#include "batmana.h"
#include "adc.h"
#include "stm32f4xx_conf.h"

void batmana_init()
{
    GPIO_InitTypeDef GPIO_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;
    EXTI_InitTypeDef EXIT_InitStruct;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource13);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStruct.GPIO_Speed = GPIO_Medium_Speed;
    GPIO_Init(GPIOB, &GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStruct.GPIO_Speed = GPIO_Low_Speed;
    GPIO_Init(GPIOB, &GPIO_InitStruct);

    EXIT_InitStruct.EXTI_Line = EXTI_Line13;
    EXIT_InitStruct.EXTI_LineCmd = ENABLE;
    EXIT_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
    EXIT_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    EXTI_Init(&EXIT_InitStruct);

    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannel = EXTI15_10_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&NVIC_InitStruct);
}

float get_bat_val()
{
    uint8_t sample_times = 16;
    uint16_t temp_val = 0;
    for(uint8_t i = 0; i != sample_times; ++i)
        temp_val += get_adc_value(ADC_Channel_9);
    float adc_value = (float)temp_val / (float)sample_times;
    float val = adc_value / 4095.0f * REF_VOLTAGE;
    val *= (1 + R0 / R1);
    return val;
}