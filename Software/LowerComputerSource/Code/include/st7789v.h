#ifndef __ST7789V_H
#define __ST7789V_H

#include <stdint.h>

#define ST7789_MAX_H 320
#define ST7789_MAX_W 240

#define ST7789V_CMD()  GPIO_ResetBits(GPIOA, GPIO_Pin_1)
#define ST7789V_DAT()  GPIO_SetBits(GPIOA, GPIO_Pin_1)
#define ST7789V_RST(x) x ? GPIO_SetBits(GPIOB, GPIO_Pin_9) : GPIO_ResetBits(GPIOB, GPIO_Pin_9)

void st7789v_init();
void st7789v_wcmd(uint8_t cmd);
void st7789v_wbyte(uint8_t data);
void st7789v_w2byte(uint16_t data);

#endif
