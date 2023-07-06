#ifndef __GUI_H
#define __GUI_H

#include "rtc.h"
#include <stdint.h>

#define WHITE  0xFFFF // 白色
#define YELLOW 0xFFE0 // 黄色
#define BRRED  0XFC07 // 棕红色
#define PINK   0XF81F // 粉色
#define RED    0xF800 // 红色
#define BROWN  0XBC40 // 棕色
#define GRAY   0X8430 // 灰色
#define GBLUE  0X07FF // 兰色
#define GREEN  0x07E0 // 绿色
#define BLUE   0x001F // 蓝色
#define BLACK  0x0000 // 黑色

void gui_init();
void gui_draw_arc(uint16_t x0, uint16_t y0, uint16_t b_ang, uint16_t e_ang, uint16_t r, uint16_t c);
void gui_draw_line(uint16_t bx, uint16_t by, uint16_t ex, uint16_t ey, uint16_t c);
void gui_draw_rect(uint16_t bx, uint16_t by, uint16_t ex, uint16_t ey, uint16_t r, uint16_t c);
void gui_show_power(uint16_t x, uint16_t y, uint8_t size);
void gui_draw_pic(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const unsigned char* p);
void gui_show_dt(int16_t x, int16_t y, DateTime dt);
void gui_show_button(uint8_t state);

void gui_update_power();

// void gui_show_power();

#endif
