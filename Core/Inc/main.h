/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdbool.h>
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define RS485_TX_EN_Pin GPIO_PIN_0
#define RS485_TX_EN_GPIO_Port GPIOA
#define BLE_PWRC_Pin GPIO_PIN_1
#define BLE_PWRC_GPIO_Port GPIOA
#define SPI_CS_Pin GPIO_PIN_4
#define SPI_CS_GPIO_Port GPIOA
#define LED_STATUS_Pin GPIO_PIN_2
#define LED_STATUS_GPIO_Port GPIOB
#define CD4051_A_Pin GPIO_PIN_15
#define CD4051_A_GPIO_Port GPIOA
#define CD4051_B_Pin GPIO_PIN_5
#define CD4051_B_GPIO_Port GPIOB
#define CD4051_C_Pin GPIO_PIN_6
#define CD4051_C_GPIO_Port GPIOB
#define IC_IN_TIM4_CH2_Pin GPIO_PIN_7
#define IC_IN_TIM4_CH2_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */
#define LED_ON 	HAL_GPIO_WritePin(LED_STATUS_GPIO_Port, LED_STATUS_Pin, GPIO_PIN_SET)
#define LED_OFF HAL_GPIO_WritePin(LED_STATUS_GPIO_Port, LED_STATUS_Pin, GPIO_PIN_RESET);
#define RS485_TX_ENA HAL_GPIO_WritePin(RS485_TX_EN_GPIO_Port, RS485_TX_EN_Pin, GPIO_PIN_SET);
#define RS485_TX_DIS HAL_GPIO_WritePin(RS485_TX_EN_GPIO_Port, RS485_TX_EN_Pin, GPIO_PIN_RESET);
#define CALIBASE
#define ADDRESSBOARD
#define ADDRESSBOARD_PART0 0
#define ADDRESSBOARD_PART1 0
#define ADDRESSBOARD_PART2 8

#define ADDR_PART0	 ADDRESSBOARD/64516
#define ADDR_PART1	(ADDRESSBOARD-ADDR_PART0*64516)/254
#define ADDR_PART2	(ADDRESSBOARD-ADDR_PART0*64516-ADDR_PART1*254)
#define NEWLINE						255
// dùng thay thế tên cho mảng 2 chi�?u
#define PERIOD 	0
#define PULSES 	1

//#define DEBUGSCAN
#define CMD_ENABLE_CALIBASE_MODE 	'c'
#define CMD_DISABLE_CALIBASE_MODE 	'd'
#define CMD_CALIBASE_SET_TYPE		'C'
#define CMD_SET_REAL_TIME			'T'
#define CMD_READ_REAL_TIME			't'
#define CMD_FRAM_ERASE				'E'
#define CMD_SCAN_DEVICE				'S'
#define CMD_GET_DATA				'G'
#define CMD_FRAM_READ				'F'
#define CMD_CALIBASE_UPDATE_THRESHOLD_A 'r'
#define CMD_CALIBASE_UPDATE_THRESHOLD_B 's'
#define CMD_CALIBASE_WRITE_THRESHOLD 'R'

// code Error system & status
// code = 0baaabbbbb  aaa là status, bbbbb là code Error
#define BOARD_ERROR_NONE              	  0    // No error
#define BOARD_I2C_ERROR_BERR              1    // BERR error
#define BOARD_I2C_ERROR_ARLO              2    // ARLO error
#define BOARD_I2C_ERROR_AF                3    // AF error
#define BOARD_I2C_ERROR_OVR               4    // OVR error
#define BOARD_I2C_ERROR_DMA               5    // DMA transfer error
#define BOARD_I2C_ERROR_TIMEOUT           6    // Timeout Error
#define BOARD_I2C_ERROR_SIZE              7    // Size Management error
#define BOARD_I2C_ERROR_DMA_PARAM         8    // DMA Parameter Error
#define BOARD_I2C_WRONG_START             9    // Wrong start Error

#define BOARD_UART_ERROR_PE                10   // Parity error
#define BOARD_UART_ERROR_NE                11   // Noise error
#define BOARD_UART_ERROR_FE                12   // Frame error
#define BOARD_UART_ERROR_ORE               13   // Overrun error
#define BOARD_UART_ERROR_DMA               14   // DMA transfer error

#define BOARD_PVD_ERROR					15 		// Điện áp xuống thấp dưới 2.8V
#define BOARD_SYS_ERROR 			   	16 		// Lỗi hệ thống, được gọi trong Error_Handler(void)
// status
#define BOARD_CHESSCLOCK_DISCONNECT		32 		//
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
