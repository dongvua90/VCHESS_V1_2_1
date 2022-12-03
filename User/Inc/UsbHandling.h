/*
 * usbHandling.h
 *
 *  Created on: Sep 16, 2022
 *      Author: goku
 */

#ifndef INC_USBHANDLING_H_
#define INC_USBHANDLING_H_

#include "main.h"
#include "RtcRealTime.h"
#include "Utilities.h"

// định nghĩa các mã code chức năng
#define CMD_ENABLE_CALIBASE_MODE 	'c'
#define CMD_CALIBASE_SET_TYPE		'C'
#define CMD_SET_REAL_TIME			'T'
#define CMD_READ_REAL_TIME			't'
#define CMD_FRAM_ERASE				'E'
#define CMD_SCAN_DEVICE				'S'
#define CMD_GET_DATA				'G'
#define CMD_FRAM_READ				'F'

//void UsbSendDateTime();
void HandlingUsb();
void UsbSendAddress();
//void UsbSendDataCalibase();


#endif /* INC_USBHANDLING_H_ */
