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
#define LED_Pin GPIO_PIN_13
#define LED_GPIO_Port GPIOC
#define BLE_STA_Pin GPIO_PIN_0
#define BLE_STA_GPIO_Port GPIOA
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
#define CALIBASE
#define ADDRESSBOARD 1
#define ADDRESSBOARD_PART0 0
#define ADDRESSBOARD_PART1 0
#define ADDRESSBOARD_PART2 1
#define ADDR_PART0	 ADDRESSBOARD/64516
#define ADDR_PART1	(ADDRESSBOARD-ADDR_PART0*64516)/254
#define ADDR_PART2	(ADDRESSBOARD-ADDR_PART0*64516-ADDR_PART1*254)
#define NEWLINE						255
// dùng thay thế tên cho mảng 2 chi�?u
#define PERIOD 	0
#define PULSES 	1

//#define DEBUGSCAN
#define CMD_ENABLE_CALIBASE_MODE 	'c'
#define CMD_CALIBASE_SET_TYPE		'C'
#define CMD_SET_REAL_TIME			'T'
#define CMD_READ_REAL_TIME			't'
#define CMD_FRAM_ERASE				'E'
#define CMD_SCAN_DEVICE				'S'
#define CMD_GET_DATA				'G'
#define CMD_FRAM_READ				'F'
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
