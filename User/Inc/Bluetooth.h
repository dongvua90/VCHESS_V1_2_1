/*
 * Bluetooth.h
 *
 *  Created on: Nov 16, 2022
 *      Author: goku
 */

#ifndef INC_BLUETOOTH_H_
#define INC_BLUETOOTH_H_
/* Kết nối Bluetooth thông qua module JDY-23
 * JDY-23 : là module BLE 5.0
 * 		  : Kết nối thông qua UART
 * 		  : nhận trạng thái kết nối qua PIN STAT trên module (1:CONNECTED,0: DISCONNECT)
 *  */

#include "main.h"
#include "usart.h"

void BLE_PowerOn();
void BLE_PowerOff();
void BLE_Send(uint8_t *data,uint8_t size);


#endif /* INC_BLUETOOTH_H_ */
