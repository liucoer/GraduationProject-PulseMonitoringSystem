#include "pulsecoll.h"
#include "adc.h"
#include "stm32f4xx_conf.h"

void pulsecoll_init()
{
    GPIO_InitTypeDef GPIO_InitStruct;
    /* 时钟配置 */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
    /* GPIO配置 */
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStruct.GPIO_Speed = GPIO_Medium_Speed;
    GPIO_Init(GPIOC, &GPIO_InitStruct);
}

float get_pulse_val()
{
    float adc_value = (float)get_adc_value(ADC_Channel_11);
    adc_value = (adc_value > 4095.0f) ? 9999.9f : adc_value;
    float val = adc_value / 4095.0f * REF_VOLTAGE;
    return val;
}
