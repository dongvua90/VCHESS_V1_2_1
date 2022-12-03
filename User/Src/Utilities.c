/*
 * Utilities.c
 *
 *  Created on: Sep 16, 2022
 *      Author: goku
 */

#include "Utilities.h"



uint32_t ByteToAddress(uint8_t byteA,uint8_t byteB,uint8_t byteC)
{
	return ((uint32_t)byteA)*64516 +  ((uint32_t)byteB)*254 +  (uint32_t)byteC;
}

uint16_t CalculateCRC(uint8_t* buffer,uint16_t offset,uint16_t count)
{
	uint16_t crc = 0xFFFF;

	for(uint16_t j=offset;j<(count+offset);j++)
	{
		crc ^= (uint16_t)buffer[j];
		for (int i = 0; i < 8; i++)
		{
			if ((crc & 0x0001) != 0)
			{
				crc >>= 1;
				crc ^= 0xA001;
			}
			else
			{
				crc >>= 1;
			}
		}
	}
	return crc;
}





