/*
 * ChessClock.c
 *
 *  Created on: Nov 9, 2022
 *      Author: goku
 */

#include "ChessClock.h"
#include "RtcRealTime.h"
extern I2C_HandleTypeDef hi2c1;

bool chessclock_interrup = false;


uint8_t data_chessclock[15];
uint32_t i2c_code_error;
uint8_t numerr=0;
uint8_t crcval=0;
/* Ping : kiểm tra trạng thái kết nối với đồng hồ.
 * 		- giá trị Ping sẽ countdown từ 7 đến 1 (bởi TaskDefault )
 * 		  nếu ping==0 thì sẽ set chessclock là mất kết nối
 * 		- set ping=7 khi có data từ chessclock gửi về để reset biến đếm
 * 	chu kỳ chessclock gửi tín hiệu về là 5s */
uint8_t ping_chessclock=7;
extern uint8_t sys_error;

uint8_t Checksum(uint8_t *data, uint8_t length)
{
	uint8_t count;
	uint8_t Sum = 0;
	for(count = 0; count < length; count++)
		Sum = Sum + data[count];
	Sum = -Sum;
	return (Sum & 0xFF);
}

// I2C-Interface , callback khi co  read/write i2c
void HAL_I2C_AddrCallback(I2C_HandleTypeDef *hi2c, uint8_t TransferDirection, uint16_t AddrMatchCode)
{
	if(hi2c->Instance==I2C1)
	{
		if(TransferDirection == I2C_DIRECTION_TRANSMIT)  // neu i2c master write to slave
		{
			HAL_I2C_Slave_Seq_Receive_DMA(&hi2c1,data_chessclock,15,I2C_FIRST_AND_LAST_FRAME); // thi luu data bang dma
		}
	}
}
// 	I2C2 for interface
void HAL_I2C_ListenCpltCallback(I2C_HandleTypeDef *hi2c)
{
	HAL_I2C_EnableListen_IT(&hi2c1);  // bat laij ngat
}
void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
	if(hi2c->Instance==I2C1)
	{
		crcval = Checksum(data_chessclock, 14);
		if(crcval == data_chessclock[14]){
			chessclock_interrup = true; // bat flag khi co data clock
			ping_chessclock = 7;
		}
	}
}
// Handler I2C Error
void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c){
	if(hi2c->Instance==I2C1){
		//HAL_NVIC_SystemReset();
		uint32_t i2c_error = HAL_I2C_GetError(&hi2c1);
		switch(i2c_error){
		case HAL_I2C_ERROR_BERR: sys_error = BOARD_I2C_ERROR_BERR;	break;
		case HAL_I2C_ERROR_ARLO: sys_error = BOARD_I2C_ERROR_ARLO;	break;
		case HAL_I2C_ERROR_AF:	 sys_error = BOARD_I2C_ERROR_AF;	break;
		case HAL_I2C_ERROR_OVR:	 sys_error = BOARD_I2C_ERROR_OVR;	break;
		case HAL_I2C_ERROR_DMA:	 sys_error = BOARD_I2C_ERROR_DMA;	break;
		case HAL_I2C_ERROR_TIMEOUT: sys_error = BOARD_I2C_ERROR_TIMEOUT;	break;
		case HAL_I2C_ERROR_SIZE:	sys_error = BOARD_I2C_ERROR_SIZE;	break;
		case HAL_I2C_ERROR_DMA_PARAM: sys_error = BOARD_I2C_ERROR_DMA_PARAM; break;
		case HAL_I2C_WRONG_START:	sys_error = BOARD_I2C_WRONG_START; break;
		default: sys_error = BOARD_SYS_ERROR; break;
		}
		BackupError_write(sys_error);
		HAL_Delay(100);
		HAL_NVIC_SystemReset();
	}
}

void ChessClock_Init()
{
	HAL_I2C_EnableListen_IT(&hi2c1);   // bat ngat I2C, khi co thao tac read/write
}
