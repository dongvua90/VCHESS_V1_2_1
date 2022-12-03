/*
 * Status.h
 *
 *  Created on: Nov 21, 2022
 *      Author: goku
 */

#ifndef INC_STATUS_H_
#define INC_STATUS_H_
#include "main.h"
#include "cmsis_os.h"

#define LED_ON 	HAL_GPIO_WritePin(LED_STATUS_GPIO_Port, LED_STATUS_Pin, GPIO_PIN_SET)
#define LED_OFF HAL_GPIO_WritePin(LED_STATUS_GPIO_Port, LED_STATUS_Pin, GPIO_PIN_RESET);
enum STATUS{STT_ALL_OK=0,STT_I2C_ERROR,STT_RS485_ERROR,STT_BLE_ERROR};


void Status_Set(uint8_t stt);
void Status_Task();


#endif /* INC_STATUS_H_ */
