#include "st7789v.h"
#include "delay.h"
#include "spi1.h"
#include "stm32f4xx_conf.h"

void _st7789v_init()
{
    GPIO_InitTypeDef GPIO_InitStruct;
    /* 时钟配置 */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB, ENABLE);
    /* GPIO配置 */
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_High_Speed;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStruct.GPIO_Speed = GPIO_Low_Speed;
    GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void _st7789v_reset(void)
{
    ST7789V_RST(0);
    delay_ms(120);
    ST7789V_RST(1);
    delay_ms(120);
}

void _st7789v_config()
{
    _st7789v_reset();    // 硬件复位
    st7789v_wcmd(0x21);  // 反转显示开启
    st7789v_wcmd(0x26);  // Gamma曲线选择
    st7789v_wbyte(0x04); // G2.5
    st7789v_wcmd(0x3A);  // 设置颜色和控制接口格式
    st7789v_wbyte(0x55); // 16位RGB
    st7789v_wcmd(0x11);  // 退出睡眠模式
    delay_ms(5);
    st7789v_wcmd(0x29);  // 打开显示
}

void st7789v_init()
{
    spi1_init();
    _st7789v_init();
    _st7789v_config();
}

void st7789v_wcmd(uint8_t cmd)
{
    ST7789V_CMD();
    spi1_write_byte(cmd);
}

void st7789v_wbyte(uint8_t data)
{
    ST7789V_DAT();
    spi1_write_byte(data);
}

void st7789v_w2byte(uint16_t data)
{
    ST7789V_DAT();
    spi1_write_byte(data >> 8);
    spi1_write_byte((uint8_t)data);
}
