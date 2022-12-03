/*
 * taskScanPiece.cpp
 *
 *  Created on: Aug 17, 2022
 *      Author: goku
 */
#include <UsbHandling.h>
#include "main.h"
#include "usb_device.h"
#include "cmsis_os.h"
#include "ScanPieces.h"
#include "Interface.h"
#include "Status.h"

#ifdef DEBUGSCAN
uint8_t debug_type=0,debug_lengthOut=20,debug_outCoil=2,debug_inCoil;
extern UART_HandleTypeDef huart1;
#endif

extern osSemaphoreId binarySemI2C_StatusHandle;
/* biến chứa trạng thái thay đổi của Fen & ChessClock
 * mainChange=true : khi App lấy dữ liệu sẽ gửi toàn bộ dữ liệu (FEN ,CLOCK),sau đó sẽ reset biến mainChange=false
 * mainChange=false: khi App lấy dữ liệu sẽ gửi những giữ liệu đơn giản */
bool mainChange = true;

uint16_t tik3=0;
void StartTaskScanPiece(void const * argument){
	osDelay(1000);
	ScanPiecesInit();
	while(1){
#ifdef DEBUGSCAN
		PulseOut(debug_type, debug_lengthOut, debug_outCoil);
		//SelectReadCoil(debug_inCoil);
		//Measure(debug_type, debug_outCoil, debug_inCoil);
		//sensorPiece->Scan();
		//RoamingData();

		osDelay(1);
		tik3++;
		if(tik3>500){
		//	HAL_GPIO_TogglePin(LED_STATUS_GPIO_Port, LED_STATUS_Pin);
			tik3=0;
		}
#else
	uint8_t resuft = Scan();
	if(resuft==1){	// có thay đổi Fen
		mainChange = true;
	}else if(resuft==2){ // có thay đổi ChessClock
		mainChange=true;
	}else if(resuft==0){ // ko có thay đổi gì

	}
	UsbSendDataCalibase();
	osDelay(1);
#endif
	}
}


