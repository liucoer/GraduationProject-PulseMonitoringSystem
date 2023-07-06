#ifndef __ADC_H
#define __ADC_H

#include <stdint.h>

#define REF_VOLTAGE 3270.0f

void adc_init();
uint16_t get_adc_value(uint8_t ADC_Channel);
#endif
