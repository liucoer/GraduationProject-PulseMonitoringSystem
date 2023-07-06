#include "rtc.h"

char week[7][6] = {"Mon. ", "Tues.", "Wed. ", "Thur.", "Fri. ", "Sat. ", "Sun. "};

void rtc_wdatetime(uint8_t year, uint8_t mon, uint8_t day, uint8_t wd, uint8_t hour, uint8_t min,
                   uint8_t sec)
{
    RTC_TimeTypeDef RTC_TimeStruct;
    RTC_DateTypeDef RTC_DateStruct;
    // 初始化时间
    RTC_TimeStruct.RTC_H12 = RTC_H12_PM;
    RTC_TimeStruct.RTC_Hours = hour;
    RTC_TimeStruct.RTC_Minutes = min;
    RTC_TimeStruct.RTC_Seconds = sec;
    RTC_SetTime(RTC_Format_BCD, &RTC_TimeStruct);
    RTC_DateStruct.RTC_WeekDay = wd;
    RTC_DateStruct.RTC_Date = day;
    RTC_DateStruct.RTC_Month = mon;
    RTC_DateStruct.RTC_Year = year;
    RTC_SetDate(RTC_Format_BCD, &RTC_DateStruct);
}

void rtc_init()
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    PWR_BackupAccessCmd(ENABLE);
}

DateTime rtc_rdatetime()
{
    DateTime DT_struct;
    RTC_GetTime(RTC_Format_BIN, &DT_struct.time);
    RTC_GetDate(RTC_Format_BIN, &DT_struct.date);
    return DT_struct;
}