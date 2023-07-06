#include "delay.h"
#include "stm32f4xx_conf.h"

static float fac_us = 0;    // us延时倍乘数
static uint16_t fac_ms = 0; // ms延时倍乘数

void delay_init()
{
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
    fac_us = SystemCoreClock / 8000000.f; // HCLK的8分频
    fac_ms = 10500;
}

/**
 * @note nus<1597830
 */
void delay_us(uint32_t nus)
{
    uint32_t temp;
    SysTick->LOAD = (uint32_t)(nus * fac_us); // 时间加载
    SysTick->VAL = 0x00;                      // 清空计数器
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; // 开始倒数
    do
    {
        temp = SysTick->CTRL;
    }
    while((temp & 0x01) && !(temp & (1 << 16))); // 等待时间到达
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;   // 关闭计数器
}

/**
 * @note nms<=1597
 */
void _delay_ms(uint32_t nms)
{
    uint32_t temp;
    SysTick->LOAD = (uint32_t)nms * fac_ms;   // 时间加载
    SysTick->VAL = 0x00;                      // 清空计数器
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; // 开始倒数
    do
    {
        temp = SysTick->CTRL;
    }
    while((temp & 0x01) && !(temp & (1 << 16))); // 等待时间到达
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;   // 关闭计数器
}

/**
 * @note nms<=65535
 */
void delay_ms(uint16_t nms)
{
    uint8_t repeat = nms / 1024;
    uint16_t remain = nms % 1024;
    while(repeat)
    {
        _delay_ms(1024);
        repeat--;
    }
    if(remain) _delay_ms(remain);
}
