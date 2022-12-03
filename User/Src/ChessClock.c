/*
 * ChessClock.c
 *
 *  Created on: Nov 9, 2022
 *      Author: goku
 */

#include "ChessClock.h"
#include "Status.h"

extern I2C_HandleTypeDef hi2c1;
extern osSemaphoreId binarySemI2C_StatusHandle;

bool chessclock_interrup = false;

static void MMX_I2C1_Init(void);

uint8_t data_chessclock[15];
uint32_t i2c_code_error;
uint8_t numerr=0;
uint8_t crcval=0;

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
		}
	}
}
// Handler I2C Error
void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c){
	if(hi2c->Instance==I2C1){
		i2c_code_error = hi2c->ErrorCode;
		Status_Set(STT_I2C_ERROR);
		numerr++;
		//HAL_NVIC_DisableIRQ(I2C1_ER_IRQn);
		HAL_I2C_DeInit(&hi2c1);
		MMX_I2C1_Init();
		HAL_I2C_EnableListen_IT(&hi2c1);
	}
}

void ChessClock_Init()
{
	HAL_I2C_EnableListen_IT(&hi2c1);   // bat ngat I2C, khi co thao tac read/write
}

static void MMX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0x50;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0x50;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */
}


