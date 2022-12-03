/*
 * Status.c
 *
 *  Created on: Nov 21, 2022
 *      Author: goku
 */
#include "Status.h"

uint8_t Status=STT_ALL_OK;
uint16_t led_duty=100;
uint16_t led_period =200;
uint16_t led_frequency = 10;

void Status_Set(uint8_t stt)
{
	Status = stt;
}


void Led_Blink(uint8_t blink,uint16_t duty)
{
	for(int i=0;i<blink;i++){
		LED_OFF;
		osDelay(duty);
		LED_ON;
		osDelay(duty);
	}
}

void Status_Task()
{
	LED_ON;
	osDelay(1000);
	if(Status != STT_ALL_OK){
		if(Status==STT_I2C_ERROR){
			Led_Blink(1, 200);
		}else if(Status==STT_RS485_ERROR){
			Led_Blink(2, 200);
		}else if(Status==STT_BLE_ERROR){
			Led_Blink(3, 200);
		}
		Status = STT_ALL_OK;
	}
}

