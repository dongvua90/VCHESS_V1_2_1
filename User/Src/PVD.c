/*
 * PVD.c
 *
 *  Created on: Jun 7, 2023
 *      Author: Pika
 */
#include "PVD.h"
#include "RtcRealTime.h"

extern uint8_t sys_error;

// hàm được gọi khi điện áp xuống dưới 2.8V sau đó lên trên 2.8V
void HAL_PWR_PVDCallback()
{
	sys_error = BOARD_PVD_ERROR;
	BackupError_write(sys_error);
}

void PVD_Config(void) {
	PWR_PVDTypeDef pvdConfig;
	pvdConfig.PVDLevel = PWR_PVDLEVEL_6; // ngưỡng 6 = 2.8V
	pvdConfig.Mode = PWR_PVD_MODE_IT_FALLING; // chế độ ngắt là thời điểm dưới 2.8V lên trên 2.8V
	HAL_PWR_ConfigPVD(&pvdConfig);
	/* Configure the NVIC for PVD */
	HAL_NVIC_SetPriority(PVD_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(PVD_IRQn);
	/* Enable the PVD Output */
	HAL_PWR_EnablePVD();
}
