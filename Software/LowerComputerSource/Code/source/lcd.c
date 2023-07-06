#include "lcd.h"
#include "cst816t.h"
#include "delay.h"
#include "font.h"
#include "st7789v.h"
#include "stm32f4xx_conf.h"
/* 屏幕和驱动芯片之间的偏差 */
#define H_OFFSET ((ST7789_MAX_H - LCD_H) / 2)
#define W_OFFSET ((ST7789_MAX_W - LCD_W) / 2)

void _lcd_init()
{
    GPIO_InitTypeDef GPIO_InitStruct;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;
    TIM_OCInitTypeDef TIM_OCInitStruct;
    /* 时钟配置 */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    /* 复用配置 */
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_TIM2);
    /* GPIO配置 */
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_Speed = GPIO_Low_Speed;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
    /* 基本定时器配置 */
    TIM_TimeBaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStruct.TIM_Prescaler = 84 - 1;
    TIM_TimeBaseStruct.TIM_Period = 100 - 1;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStruct);
    /* 输出比较配置 */
    TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStruct.TIM_Pulse = 100;
    TIM_OC3Init(TIM2, &TIM_OCInitStruct);
    /* 使能 */
    TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);
    TIM_ARRPreloadConfig(TIM2, ENABLE);
    TIM_Cmd(TIM2, ENABLE);
}

void lcd_init()
{
    st7789v_init();
    _lcd_init();
    cst816t_init();
}

void lcd_set_windows(uint16_t bx, uint16_t by, uint16_t ex, uint16_t ey)
{
    /* 指定X方向操作区域 */
    st7789v_wcmd(0x2A);
    st7789v_w2byte(bx + W_OFFSET);
    st7789v_w2byte(ex + W_OFFSET);
    /* 指定Y方向操作区域 */
    st7789v_wcmd(0x2B);
    st7789v_w2byte(by + H_OFFSET);
    st7789v_w2byte(ey + H_OFFSET);

    st7789v_wcmd(0x2C); // 发送该命令，LCD开始等待接收显存数据
}

void lcd_fill(uint16_t bx, uint16_t by, uint16_t ex, uint16_t ey, uint16_t c)
{
    /* 使bx，by总大于ex，ey */
    if(ex < bx)
    {
        uint16_t temp = ex;
        ex = bx;
        bx = temp;
    }
    if(ey < by)
    {
        uint16_t temp = ey;
        ey = by;
        by = temp;
    }
    lcd_set_windows(bx, by, ex, ey);
    uint32_t area = (ex - bx + 1) * (ey - by + 1); // 计算填充的像素点数
    for(uint32_t i = 0; i != area; ++i)
        st7789v_w2byte(c);
}

void lcd_clear(uint16_t c) { lcd_fill(0, 0, LCD_W - 1, LCD_H - 1, c); }

void lcd_draw_point(uint16_t x, uint16_t y, uint16_t c)
{
    lcd_set_windows(x, y, x, y);
    st7789v_w2byte(c);
}

void lcd_show_char(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint8_t ch, uint8_t size,
                   uint8_t mode)
{
    lcd_set_windows(x, y, x + size / 2 - 1, y + size - 1);
    uint8_t index = ch - ' ';
    uint8_t arrsize = size * size / 16;
    arrsize = arrsize < size ? size : arrsize;
    if(mode)
    {
        uint64_t c = 0x0000;
        for(uint16_t pos = 0; pos != arrsize; ++pos)
        {
            if(size == 12) c = asc2_1206[index][pos];      // 调用1206字体
            else if(size == 16) c = asc2_1608[index][pos]; // 调用1608字体
            else if(size == 32)
            {
                c = asc2_3216[index][pos];
                c |= (asc2_3216[index][++pos] << 8); // 调用3216字体
            }
            else if(size == 60)
            {
                c = asc2_6030[index][pos];
                c |= (asc2_6030[index][++pos] << 8);  // 调用3216字体
                c |= (asc2_6030[index][++pos] << 16); // 调用3216字体
                c |= (asc2_6030[index][++pos] << 24); // 调用3216字体
            }
            for(uint16_t col = 0; col != (size / 2); ++col)
            {
                if(c & 0x01) lcd_draw_point(x + col, y + pos, fc); // 画一个点
                c >>= 1;
                // delay_ms(1);
            }
        }
    }
    else
    {
        uint64_t c = 0xff;
        for(uint16_t pos = 0; pos < arrsize; ++pos)
        {
            if(size == 12) c = asc2_1206[index][pos];      // 调用1206字体
            else if(size == 16) c = asc2_1608[index][pos]; // 调用1608字体
            else if(size == 32)
            {
                c = asc2_3216[index][pos];
                c |= (asc2_3216[index][++pos] << 8); // 调用3216字体
            }
            else if(size == 60)
            {
                c = asc2_6030[index][pos];
                c |= (asc2_6030[index][++pos] << 8);  // 调用3216字体
                c |= (asc2_6030[index][++pos] << 16); // 调用3216字体
                c |= (asc2_6030[index][++pos] << 24); // 调用3216字体
            }
            for(uint16_t col = 0; col != (size / 2); ++col)
            {
                if(c & 0x01) st7789v_w2byte(fc); // 画一个点
                else st7789v_w2byte(bc);
                c >>= 1;
            }
        }
    }
}

void lcd_show_string(uint16_t x, uint16_t y, char* p, uint16_t fc, uint16_t bc, uint8_t size,
                     uint8_t mode)
{
    while((*p <= '~') && (*p >= ' ')) // 判断是不是非法字符!
    {
        if((x > (LCD_W - 1)) && (y <= (LCD_H - 1)))
        {
            y += size;
            x = 0;
        }
        else if(y > (LCD_H - size - 1)) return;
        lcd_show_char(x, y, fc, bc, *p, size, mode);
        x += size / 2;
        ++p;
    }
}

uint32_t _pow(uint8_t m, uint8_t n)
{
    uint32_t result = 1;
    while(n--)
        result *= m;
    return result;
}

void lcd_show_uint(uint16_t x, uint16_t y, uint32_t num, uint16_t fc, uint16_t bc, uint8_t len,
                   uint8_t size, uint8_t mode)
{
    uint8_t t, temp;
    uint8_t enshow = 0;
    // lcd_fill(x, y, x + len * size / 2, y + size, bc);
    for(t = 0; t < len; t++)
    {
        temp = (num / _pow(10, len - t - 1)) % 10;
        if(enshow == 0 && t < (len - 1))
        {
            if(temp == 0)
            {
                lcd_show_char(x + (size / 2) * t, y, fc, bc, '0', size, mode);
                continue;
            }
            else enshow = 1;
        }
        lcd_show_char(x + (size / 2) * t, y, fc, bc, temp + '0', size, mode);
    }
}
