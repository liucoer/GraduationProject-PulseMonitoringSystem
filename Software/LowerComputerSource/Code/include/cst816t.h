#ifndef __CST816T_H
#define __CST816T_H

#include <stdint.h>

typedef struct
{
    uint16_t cur_x;
    uint16_t cur_y;
} CST816T_XY;

void cst816t_init(void);
CST816T_XY cst816t_get_xy();
void button_ctrl();
#endif
