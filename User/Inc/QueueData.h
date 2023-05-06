/*
 * QueueData.h
 *
 *  Created on: Jan 17, 2023
 *      Author: Pika
 */

#ifndef INC_QUEUEDATA_H_
#define INC_QUEUEDATA_H_

#include "main.h"
#define QUEUE_MAX 20
#define QUEUE_ITEMLENGTH 53

int8_t QueueGetRear();
int8_t QueueGetFront();
void QueueGetFrontAfter();
uint8_t QueueGetCount();
void QueueClear();


#endif /* INC_QUEUEDATA_H_ */
