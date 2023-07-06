#ifndef __GLOBAL_H
#define __GLOBAL_H

#include "cst816t.h"
#include <stdint.h>

extern uint8_t is_btconnected;
extern float cunpv;
extern float guanpv;
extern float chipv;
extern CST816T_XY xy;
extern uint32_t times;
extern uint8_t send_state;
extern uint8_t pre_send_state;
#endif
