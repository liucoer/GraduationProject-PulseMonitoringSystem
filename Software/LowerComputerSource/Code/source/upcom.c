#include "upcom.h"
#include "global.h"
#include "stm32f4xx_conf.h"
#include "usart1.h"

void upcom_init()
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    /* 时钟配置 */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    /* 基本定时器配置 */
    TIM_TimeBaseInitStructure.TIM_Period = 3359;
    TIM_TimeBaseInitStructure.TIM_Prescaler = 124;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
    /* NVIC 配置 */
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    /* 时钟配置 */
    TIM_Cmd(TIM3, ENABLE);
}

void beg_byte(void) { usart1_send_byte(0x0f); }

void end_byte(void) { usart1_send_byte(0xf0); }

void send_int(int32_t val)
{
    uint8_t value_h = 0, value_l = 0;
    uint8_t value_h1 = 0, value_l2 = 0;
    value_h1 = (val >> 24) & 0xff;
    value_l2 = (val >> 16) & 0xff;
    value_h = (val >> 8) & 0xff;
    value_l = val & 0xff;
    usart1_send_byte(value_l);
    usart1_send_byte(value_h);
    usart1_send_byte(value_l2);
    usart1_send_byte(value_h1);
}

void send_float(float val)
{
    union
    {
        uint8_t info[4];
        float f;
    } float_data;

    float_data.f = val;
    for(uint8_t i = 0; i != 4; ++i)
        usart1_send_byte(float_data.info[i]);
}

void send_data_to_uppc(float ival, float fval)
{
    beg_byte();
    send_float(ival);
    send_float(fval);
    end_byte();
}

void set_send_state(uint8_t state)
{
    if(state == 0)
    {
        TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);
        times = 0;
    }
    else if(state == 1) { TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE); }
    else if(state == 2) { TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE); }
}
