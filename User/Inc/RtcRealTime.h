/*
 * RtcRealTime.h
 *
 *  Created on: Sep 16, 2022
 *      Author: goku
 */

#ifndef INC_RTCREALTIME_H_
#define INC_RTCREALTIME_H_

#include "main.h"

/* RTC sử dụng để lấy thời gian thực trong lúc tạo 1 game mới
 * Sử dụng định dạng 32-bit Windowns time/date formats
 * Time
	Bit position:	0 1 2 3 4	5 6 7 8 9 A		B C D E F
	Length:				5			6				5
	Contents:		   hours      minutes	  2-second increments
	Value Range:	   0-23		   0-59		0-29 in 2-second intervals

   Date
	Bit position:	0 1 2 3 4 5 6	7 8 9 A		B C D E F
	Length:				7				4			5
	Contents:			year		   month		day
	Value Range:		0-119			1-12		1-31
					(relative to 1980)
 * */
void RTC_SetTime(uint8_t *dat);

#endif /* INC_RTCREALTIME_H_ */
