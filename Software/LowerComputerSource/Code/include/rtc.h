#ifndef __RTC_H
#define __RTC_H

#include "stm32f4xx_conf.h"
#include <stdint.h>

typedef struct
{
    RTC_DateTypeDef date;
    RTC_TimeTypeDef time;
} DateTime;

extern char week[7][6];

void rtc_init();
DateTime rtc_rdatetime();
void rtc_wdatetime(uint8_t year, uint8_t mon, uint8_t day, uint8_t wd, uint8_t hour, uint8_t min,
                   uint8_t sec);
#endif
