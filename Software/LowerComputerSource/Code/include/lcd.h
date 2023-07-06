#ifndef __LCD_H
#define __LCD_H

#include <stdint.h>

#define LCD_H 280
#define LCD_W 240

void lcd_init();
void lcd_set_windows(uint16_t bx, uint16_t by, uint16_t ex, uint16_t ey);
void lcd_fill(uint16_t bx, uint16_t by, uint16_t ex, uint16_t ey, uint16_t c);
void lcd_clear(uint16_t c);
void lcd_draw_point(uint16_t x, uint16_t y, uint16_t c);
void lcd_show_char(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint8_t ch, uint8_t size,
                   uint8_t mode);
void lcd_show_string(uint16_t x, uint16_t y, char* p, uint16_t fc, uint16_t bc, uint8_t size,
                     uint8_t mode);
void lcd_show_uint(uint16_t x, uint16_t y, uint32_t num, uint16_t fc, uint16_t bc, uint8_t len,
                   uint8_t size, uint8_t mode);

#endif