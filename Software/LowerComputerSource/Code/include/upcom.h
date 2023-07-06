#ifndef __UPCOM_H
#define __UPCOM_H

#include <stdint.h>

void upcom_init();
void send_data_to_uppc(float ival, float fval);
void set_send_state(uint8_t state);

#endif
