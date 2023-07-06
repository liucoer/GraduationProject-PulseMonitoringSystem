#ifndef __MAIN_H
#define __MAIN_H

#include "adc.h"
#include "batmana.h"
#include "bluetooth.h"
#include "delay.h"
#include "font.h"
#include "global.h"
#include "gui.h"
#include "lcd.h"
#include "pulsecoll.h"
#include "rtc.h"
#include "swmode.h"
#include "timer4.h"
#include "upcom.h"
#include "usart1.h"

void pulsemonitor_init()
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    delay_init();
    adc_init();
    uart_init();
    timer4_init();
    gui_init();
    swmode_init();
    bluetooth_init();
    batmana_init();
    pulsecoll_init();
    upcom_init();
    gui_show_power(190, 5, 30);
    if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13) == 0) gui_draw_pic(181, 7, 8, 15, gImage_charge);
    else lcd_fill(181, 7, 189, 25, BLACK);
    if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_12))
    {
        gui_draw_pic(10, 10, 15, 15, gImage_bllink);
        is_btconnected = 1;
    }
    else
    {
        gui_draw_pic(10, 10, 15, 15, gImage_bloff);
        is_btconnected = 0;
    }
    gui_show_button(send_state);
    Enable_EXTI(EXTI_Line0);
}
#endif