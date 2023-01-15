/*
 * taskScanPiece.cpp
 *
 *  Created on: Aug 17, 2022
 *      Author: goku
 */
#include "main.h"
#include "usb_device.h"
#include "cmsis_os.h"
#include "ScanPieces.h"
#include "Interface.h"
#include "Status.h"
#include "MB85RS256.h"

#ifdef DEBUGSCAN
uint8_t debug_type=0,debug_lengthOut=20,debug_outCoil=2,debug_inCoil;
extern UART_HandleTypeDef huart1;
#endif

extern osSemaphoreId binarySemI2C_StatusHandle;
/* biến chứa trạng thái thay đổi của Fen & ChessClock
 * mainChange=true : khi App lấy dữ liệu sẽ gửi toàn bộ dữ liệu (FEN ,CLOCK),sau đó sẽ reset biến mainChange=false
 * mainChange=false: khi App lấy dữ liệu sẽ gửi những giữ liệu đơn giản */
bool mainChange = true;
extern bool chessclock_interrup;
extern bool isCalibaseMode;
extern bool autoscan;

uint8_t numScanRepair=0;


void StartTaskScanPiece(void const * argument){
	osDelay(1000);
	ScanPiecesInit();
	MB85RS_getPointSave();
	while(1){
#ifdef DEBUGSCAN
		PulseOut(debug_type, debug_lengthOut, debug_outCoil);
		//SelectReadCoil(debug_inCoil);
		//Measure(debug_type, debug_outCoil, debug_inCoil);
		//sensorPiece->Scan();
		//RoamingData();

		osDelay(1);
#else
#ifdef CALIBASE
		if(isCalibaseMode==true){
			Scan(false);
			UsbSendDataCalibase();
		}else
#endif
		if(autoscan==true || numScanRepair>0){
			LED_OFF;
			if(numScanRepair>0) numScanRepair--;
			SCANRESULT result = Scan(true);
			if(result==SCAN_FEN_CHANGE){
				mainChange = true;
			}else if(result==SCAN_SIDE_CHANGED){
				Scan(false); // scanpiece không ngắt bởi clockside
				mainChange = true;
				handlerMb85rs();
			}
			osDelay(10);
			LED_ON;
		}else if(chessclock_interrup==true){
			LED_OFF;
			Scan(false);
			chessclock_interrup=false;
			mainChange=true;
			handlerMb85rs();
			numScanRepair=5; // sau khi sidechange thì sẽ tự động scan thêm 5 lần sau đó
			LED_ON;
		}
	osDelay(1);
#endif
	}
}




