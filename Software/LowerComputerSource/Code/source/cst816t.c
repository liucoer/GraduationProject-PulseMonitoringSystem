#include "cst816t.h"
#include "delay.h"
#include "global.h"
#include "gui.h"
#include "lcd.h"
#include "siic.h"
#include "stm32f4xx_conf.h"
#include "upcom.h"

void cst816t_init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;
    EXTI_InitTypeDef EXIT_InitStruct;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource2);

    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Fast_Speed;
    GPIO_Init(GPIOD, &GPIO_InitStruct);

    EXIT_InitStruct.EXTI_Line = EXTI_Line2;
    EXIT_InitStruct.EXTI_LineCmd = ENABLE;
    EXIT_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
    EXIT_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_Init(&EXIT_InitStruct);

    NVIC_InitStruct.NVIC_IRQChannel = EXTI2_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x01;
    NVIC_Init(&NVIC_InitStruct);
    siic_init();
}

CST816T_XY cst816t_get_xy()
{
    CST816T_XY cur_xy;
    uint8_t temp[4];
    siic_start();
    siic_write_byte(0x2A);
    siic_wait_ack();
    siic_write_byte(0x03);
    siic_wait_ack();
    siic_start();
    siic_write_byte(0x2B);
    siic_wait_ack();
    temp[0] = siic_read_byte(Ack);
    temp[1] = siic_read_byte(Ack);
    temp[2] = siic_read_byte(Ack);
    temp[3] = siic_read_byte(Ack);
    siic_stop();
    uint16_t x = (uint16_t)((temp[0] & 0x0F) << 8) | temp[1];
    uint16_t y = (uint16_t)((temp[2] & 0x0F) << 8) | temp[3];
    if(x < LCD_W && y < LCD_H)
    {
        cur_xy.cur_x = x;
        cur_xy.cur_y = y;
    }
    else
    {
        cur_xy.cur_x = -1;
        cur_xy.cur_y = -1;
    }
    return cur_xy;
}

void button_ctrl()
{
    xy = cst816t_get_xy();
    if(send_state == 0)
    {
        if(xy.cur_x >= 70 && xy.cur_x <= 170 && xy.cur_y >= 150 && xy.cur_x <= 250)
        {
            pre_send_state = send_state;
            send_state = 1;
            gui_show_button(send_state);
        }
    }
    else if(send_state == 1)
    {
        if(xy.cur_x >= 10 && xy.cur_x <= 110 && xy.cur_y >= 150 && xy.cur_x <= 250)
        {
            pre_send_state = send_state;
            send_state = 2;
            gui_show_button(send_state);
        }
        if(xy.cur_x >= 130 && xy.cur_x <= 230 && xy.cur_y >= 150 && xy.cur_x <= 250)
        {
            pre_send_state = send_state;
            send_state = 0;
            gui_show_button(send_state);
        }
    }
    else if(send_state == 2)
    {
        if(xy.cur_x >= 10 && xy.cur_x <= 110 && xy.cur_y >= 150 && xy.cur_x <= 250)
        {
            pre_send_state = send_state;
            send_state = 1;
            gui_show_button(send_state);
        }
        if(xy.cur_x >= 130 && xy.cur_x <= 230 && xy.cur_y >= 150 && xy.cur_x <= 250)
        {
            pre_send_state = send_state;
            send_state = 0;
            gui_show_button(send_state);
        }
    }
    set_send_state(send_state);
    delay_ms(100);
}