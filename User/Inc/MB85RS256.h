/*
 * MB85RS256.h
 *
 *  Created on: Aug 22, 2022
 *      Author: goku
 */

#ifndef INC_MB85RS256_H_
#define INC_MB85RS256_H_

#include "main.h"
#include "usbd_cdc_if.h"

void WriteProtect();
void MB85RS_erase();
uint8_t ReadStatus();
void WriteEnable();
void MB85RS_read(unsigned int address, unsigned char *buffer, unsigned int size);
void MB85RS_write(unsigned int address, unsigned char *buffer, unsigned int size);

void MB85RS_getPointSave();
void MB85RS_WritePointSave();
void MB85RS_WriteNewGame();
void MB85RS_saveFen();
void MB85RS_readFen();

#endif /* INC_MB85RS256_H_ */
