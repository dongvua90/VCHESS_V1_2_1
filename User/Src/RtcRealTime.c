/*
 * RtcRealTime.c
 *
 *  Created on: Sep 16, 2022
 *      Author: goku
 */
#include "RtcRealTime.h"

extern RTC_HandleTypeDef hrtc;


/* data=6byte /second/minute/hour/day/month/year */
void RTC_SetTime(uint8_t* datatime)
{
  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};
  sTime.Hours = datatime[2];
  sTime.Minutes = datatime[1];
  sTime.Seconds = datatime[0];
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }
  sDate.WeekDay = RTC_WEEKDAY_MONDAY;
  sDate.Month = datatime[4];
  sDate.Date = datatime[3];
  sDate.Year = datatime[5];

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }
}

void BackupError_write(uint8_t error)
{
	HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR11, (uint32_t)error);
}

uint8_t BackupError_read()
{
	uint32_t error = HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR11);
	return (uint8_t)error;
}


