/*
 * calibase.h
 *
 *  Created on: Aug 17, 2022
 *      Author: goku
 */

#ifndef INC_CALIBASE_H_
#define INC_CALIBASE_H_
#include "main.h"

// Địa chỉ bắt đầu của mảng 2 chiều trong flash
#define THRESHOLD_FLASH_ARRAY_START_ADDR 0x08020000
// Kích thước của mảng 2 chiều
#define THRESHOLD_ROW_SIZE 		18
#define THRESHOLD_COLUMN_SIZE 	64
/* trả lại ngưỡng so sánh pulseLength để xác định pieces*/
uint8_t getMarkSq(uint8_t type,uint8_t squamark);
void Thresholdinit();
void ThresholdWrite();
void ThresholdRead();
bool ThresholdCheckError();

#endif /* INC_CALIBASE_H_ */
