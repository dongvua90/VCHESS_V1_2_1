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
#include "MB85RS256.h"
#include "QueueData.h"
#include "Utilities.h"
#include "calibase.h"

#ifdef DEBUGSCAN
uint8_t debug_type=1,debug_lengthOut=10,debug_outCoil=2,debug_inCoil;
extern UART_HandleTypeDef huart1;
#endif

/* biến chứa trạng thái thay đổi của Fen & ChessClock*/
extern bool chessclock_interrup;
extern bool isCalibaseMode;
extern uint8_t datamain_old[64];
extern uint8_t data_chessclock[15];
extern uint8_t globalMode;
extern bool flagReviewFullData;

/* Queue for Data Main */
uint8_t queueData[QUEUE_MAX][QUEUE_ITEMLENGTH];
uint16_t tikReviewData = 0;	// biến đếm để thêm 1 lần scan sau 1 khoảng thời gian khi ở chế độ Recording

void PushData(bool isClockChanged);

/* có 3 chế độ getdata:
 * 1_Calibase : board sẽ scan và gửi data lên theo 1 vòng lặp vô tận
 * 2_Recording: board nhận tín hiệu chessclock_sidechanged sau đó scan -> gửi data vào queue -> chờ cmd đến lấy dữ liệu từ queue
 * 		- lệnh đầu tiên sẽ trả về fulldata , sau đó sẽ scanpiece 1 lần sau đó
 * 		- các lệnh tiếp theo sẽ chỉ trả về fulldata nếu chessclock hoặc Fen thay đổi
 * 		- các lệnh được gửi theo chu kỳ khoảng 500ms, nếu quá thời gian này (khoảng 1000ms) thì
 * 		   coi như đó là lệnh đầu tiên( trả về fulldata và scanpiece 1 lần sau đó)
 * 3_Review: mỗi lần nhận review
 * 		- board sẽ chỉ scan khi chessclock_side thay đổi sau đó trả về fulldata */

void StartTaskScanPiece(void const *argument) {
	ScanPiecesInit();
	MB85RS_getPointSave();
	Thresholdinit();
	while (1) {
#ifdef DEBUGSCAN
		PulseOut(debug_type, debug_lengthOut, debug_outCoil);
		//SelectReadCoil(debug_inCoil);
		//Measure(debug_type, debug_outCoil, debug_inCoil);
		//sensorPiece->Scan();
		//RoamingData();

		osDelay(1);
#else
#ifdef CALIBASE
		if (isCalibaseMode == true) {
			Scan(false);
			UsbSendDataCalibase();
			IWDG->KR=0xAAAA;// Reload Counter Watchdog
			osDelay(1);
		} else
#endif
		if (globalMode == MODE_REVIEW) {
			SCANRESULT result = Scan(true);
			if (result == SCAN_FEN_CHANGE) {
				flagReviewFullData = true;
			} else if (result == SCAN_NO_FEN_CHANGE) {
				flagReviewFullData = false;
			} else if (result == SCAN_SIDE_CHANGED) {
				LED_ON;
				Scan(false); // scanpiece không ngắt bởi clockside
				flagReviewFullData = true;
				handlerMb85rs();
				LED_OFF;
			}
			IWDG->KR=0xAAAA;// Reload Counter Watchdog
			osDelay(500);
		} else if (globalMode == MODE_RECORDING) {
			if (chessclock_interrup) {
				LED_ON;
				Scan(false);
				chessclock_interrup = false;
				PushData(true);
				handlerMb85rs();
				LED_OFF;
				tikReviewData = 0; 		//đặt lại biến đếm
			} else {
				tikReviewData++;
				if (tikReviewData > 2000) { // sau chu kỳ 1s sẽ tiến hành scan để lấy dữ liệu review
					tikReviewData = 0;
					SCANRESULT result = Scan(true);
					if (result != SCAN_SIDE_CHANGED) {
						flagReviewFullData = true;
					}else if (result == SCAN_SIDE_CHANGED) {
						chessclock_interrup = true;  // đặt lại biến để tiến hành scan ở phần trên
					}
				}
			}
			IWDG->KR=0xAAAA;// Reload Counter Watchdog
		}
		osDelay(1);
#endif
	}
}

/* đẩy data vào queue */
void PushData(bool isClockChanged) {
	uint8_t pieceA, pieceB, pie = 0;
	uint16_t crc;

	int8_t rear = QueueGetRear();
	queueData[rear][0] = CMD_BEGIN;
	queueData[rear][1] = ADDRESSBOARD_PART0;
	queueData[rear][2] = ADDRESSBOARD_PART1;
	queueData[rear][3] = ADDRESSBOARD_PART2;
	if (isClockChanged) {
		queueData[rear][4] = RES_DATACLOCKCHANGE;
	} else {
		queueData[rear][4] = RES_DATAFENCHANGE;
	}

	/* gắn datamain vào 32 byte tiếp theo */
	for (int i = 0; i < 32; i++) {
		pieceA = byteToPiece(datamain_old[pie]);
		pie++;
		pieceB = byteToPiece(datamain_old[pie]);
		pie++;
		queueData[rear][i + 5] = (pieceA << 4) + pieceB;
	}
	for (int i = 0; i < 14; i++) {
		queueData[rear][37 + i] = data_chessclock[i];
	}
	crc = CalculateCRC(queueData[rear], 1, 50);
	queueData[rear][51] = (uint8_t) ((crc & 0xFF00) >> 8);
	queueData[rear][52] = (uint8_t) (crc & 0x00FF);
}

