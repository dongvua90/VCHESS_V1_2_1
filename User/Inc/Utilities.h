/*
 * Utilities.h
 *
 *  Created on: Sep 16, 2022
 *      Author: goku
 */

#ifndef INC_UTILITIES_H_
#define INC_UTILITIES_H_
#include "main.h"

uint32_t ByteToAddress(uint8_t byteA,uint8_t byteB,uint8_t byteC);

uint16_t CalculateCRC(uint8_t* buffer,uint16_t offset,uint16_t count);

#endif /* INC_UTILITIES_H_ */
