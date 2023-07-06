#ifndef __DELAY_H
#define __DELAY_H

#include <stdint.h>

void delay_init();
void delay_us(uint32_t nus);
void delay_ms(uint16_t nms);

#endif
