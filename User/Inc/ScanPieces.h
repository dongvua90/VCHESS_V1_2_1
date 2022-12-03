/*
 * ScanPieces.h
 *
 *  Created on: Sep 17, 2022
 *      Author: goku
 */

#ifndef INC_SCANPIECES_H_
#define INC_SCANPIECES_H_

#include "main.h"
#include <stdbool.h>
#include <string.h>
#include "calibase.h"

#define IC_MAX	50	// số pulse max để đo đạc trong Input Capture
#define CD4051_A_1  HAL_GPIO_WritePin(CD4051_A_GPIO_Port,CD4051_A_Pin,GPIO_PIN_SET)
#define CD4051_A_0  HAL_GPIO_WritePin(CD4051_A_GPIO_Port,CD4051_A_Pin,GPIO_PIN_RESET)
#define CD4051_B_1  HAL_GPIO_WritePin(CD4051_B_GPIO_Port,CD4051_B_Pin,GPIO_PIN_SET)
#define CD4051_B_0  HAL_GPIO_WritePin(CD4051_B_GPIO_Port,CD4051_B_Pin,GPIO_PIN_RESET)
#define CD4051_C_1  HAL_GPIO_WritePin(CD4051_C_GPIO_Port,CD4051_C_Pin,GPIO_PIN_SET)
#define CD4051_C_0  HAL_GPIO_WritePin(CD4051_C_GPIO_Port,CD4051_C_Pin,GPIO_PIN_RESET)

typedef struct {
	int8_t  pulses;
	int8_t 	square;
}SQData;

void ScanPiecesInit();
void PulseOut(uint8_t type,uint8_t length,uint8_t outCoil);
void SelectReadCoil(uint8_t readCoil);
void StartInputPWM(uint8_t type);
void Measure(uint8_t type,uint8_t outCoil,uint8_t readCoil);
uint8_t Scan();
bool DetectFenChange();
uint8_t FileRankToSquare(uint8_t file,uint8_t rank);
void FindMax(uint8_t type, SQData *sqFirstMax,SQData *sqSecondMax);

#endif /* INC_SCANPIECES_H_ */
