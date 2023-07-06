#include "gui.h"
#include "batmana.h"
#include "font.h"
#include "global.h"
#include "lcd.h"
#include "st7789v.h"
#include <math.h>

void gui_init()
{
    lcd_init();
    lcd_clear(BLACK);
}

/****
 * x=rcos(b_ang)+bx;
 * y=rsin(b_ang)+by;
 */
void gui_draw_arc(uint16_t bx, uint16_t by, uint16_t b_ang, uint16_t e_ang, uint16_t r, uint16_t c)
{
    for(float i = b_ang; i <= e_ang; i += 0.5f)
    {
        uint16_t x = (uint16_t)(r * cosf(i / 180.0f * 3.1415926f) + bx + 0.5f);
        uint16_t y = (uint16_t)(r * sinf(i / 180.0f * 3.1415926f) + by + 0.5f);
        lcd_draw_point(x, y, c);
    }
}

void gui_draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t c)
{
    uint16_t t;
    int xerr = 0, yerr = 0, delta_x, delta_y, distance;
    int incx, incy, uRow, uCol;

    delta_x = x2 - x1; // 计算坐标增量
    delta_y = y2 - y1;
    uRow = x1;
    uCol = y1;
    if(delta_x > 0) incx = 1;       // 设置单步方向
    else if(delta_x == 0) incx = 0; // 垂直线
    else
    {
        incx = -1;
        delta_x = -delta_x;
    }
    if(delta_y > 0) incy = 1;
    else if(delta_y == 0) incy = 0; // 水平线
    else
    {
        incy = -1;
        delta_y = -delta_y;
    }
    if(delta_x > delta_y) distance = delta_x; // 选取基本增量坐标轴
    else distance = delta_y;
    for(t = 0; t <= distance + 1; t++)        // 画线输出
    {
        lcd_draw_point(uRow, uCol, c);        // 画点
        xerr += delta_x;
        yerr += delta_y;
        if(xerr > distance)
        {
            xerr -= distance;
            uRow += incx;
        }
        if(yerr > distance)
        {
            yerr -= distance;
            uCol += incy;
        }
    }
}

void gui_draw_rect(uint16_t bx, uint16_t by, uint16_t ex, uint16_t ey, uint16_t r, uint16_t c)
{
    gui_draw_line(bx + r, by, ex - r, by, c);
    gui_draw_line(bx, by + r, bx, ey - r, c);
    gui_draw_line(ex, by + r, ex, ey - r, c);
    gui_draw_line(bx + r, ey, ex - r, ey, c);
    gui_draw_arc(bx + r, by + r, 180, 270, r, c);
    gui_draw_arc(bx + r, ey - r, 90, 180, r, c);
    gui_draw_arc(ex - r, by + r, 270, 360, r, c);
    gui_draw_arc(ex - r, ey - r, 0, 90, r, c);
}

void gui_show_power(uint16_t x, uint16_t y, uint8_t size)
{
    lcd_show_char(170, 7, GREEN, BLACK, '%', 16, 1);
    gui_draw_rect(x, y, x + size, y + size / 2, 0, WHITE);
    lcd_fill(x + size, y + size / 8.0f, x + size + size / 8.0f, y + size * 3 / 8.0f, WHITE);
}

void gui_draw_pic(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const unsigned char* p)
{
    int i;
    unsigned char picH, picL;
    lcd_set_windows(x, y, x + w - 1, y + h - 1);
    for(i = 0; i < w * h; i++)
    {
        picL = *(p + i * 2);
        picH = *(p + i * 2 + 1);
        st7789v_w2byte(picH << 8 | picL);
    }
}

void gui_show_dt(int16_t x, int16_t y, DateTime dt)
{
    lcd_show_uint(32 + x, 88 + y, dt.date.RTC_Month, WHITE, BLACK, 2, 32, 0);
    lcd_show_char(64 + x, 88 + y, WHITE, BLACK, '/', 32, 0);
    lcd_show_uint(80 + x, 88 + y, dt.date.RTC_Date, WHITE, BLACK, 2, 32, 0);
    lcd_show_string(128 + x, 88 + y, week[dt.date.RTC_WeekDay - 1], WHITE, BLACK, 32, 0);
    lcd_show_uint(35 + x, 120 + y, dt.time.RTC_Hours, WHITE, BLACK, 2, 60, 0);
    if(dt.time.RTC_Seconds % 2 == 0) lcd_show_char(95 + x, 120 + y, WHITE, BLACK, ':', 60, 0);
    else lcd_show_char(95 + x, 120 + y, WHITE, BLACK, ' ', 60, 0);
    lcd_show_uint(125 + x, 120 + y, dt.time.RTC_Minutes, WHITE, BLACK, 2, 60, 0);
}

void gui_show_button(uint8_t state)
{
    if(state == 0)
    {
        lcd_fill(10, 150, 230, 250, BLACK);
        gui_draw_pic(70, 150, 100, 100, gImage_stop);
    }
    else if(state == 1)
    {
        if(pre_send_state == 0)
        {
            lcd_fill(70, 150, 170, 250, BLACK);
            gui_draw_pic(130, 150, 100, 100, gImage_end);
        }
        else lcd_fill(10, 150, 110, 250, BLACK);
        gui_draw_pic(10, 150, 100, 100, gImage_start);
    }
    else if(state == 2)
    {
        if(pre_send_state == 0)
        {
            lcd_fill(70, 150, 170, 250, BLACK);
            gui_draw_pic(130, 150, 100, 100, gImage_end);
        }
        else lcd_fill(10, 150, 110, 250, BLACK);
        gui_draw_pic(10, 150, 100, 100, gImage_stop);
    }
}

void gui_update_power()
{
    float vbat = get_bat_val() - 3500;
    float batpre = (vbat < 0 ? 0 : vbat) / 7.0f + 0.5f;
    float power = batpre * 0.3f;
    uint16_t c = batpre < 20 ? RED : GREEN;
    lcd_fill(191, 6, 191 + power, 19, c);
    lcd_fill(190 + power + 1, 6, 219, 19, BLACK);
    lcd_show_uint(154, 7, batpre, GREEN, BLACK, 2, 16, 0);
}