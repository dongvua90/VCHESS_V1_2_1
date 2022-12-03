/*
 * Bluetooth.c
 *
 *  Created on: Nov 16, 2022
 *      Author: goku
 */
#include "Bluetooth.h"

extern UART_HandleTypeDef huart2;

void BLE_PowerOn()
{
	HAL_GPIO_WritePin(BLE_PWRC_GPIO_Port, BLE_PWRC_Pin, GPIO_PIN_SET);
}
void BLE_PowerOff()
{
	HAL_GPIO_WritePin(BLE_PWRC_GPIO_Port, BLE_PWRC_Pin, GPIO_PIN_RESET);
}

void BLE_Send(uint8_t *data,uint8_t size)
{
	// send only when connected
	if(HAL_GPIO_ReadPin(BLE_STA_GPIO_Port, BLE_STA_Pin)==GPIO_PIN_SET)
	{
		HAL_UART_Transmit(&huart2, data, size, 1000);
	}
}











