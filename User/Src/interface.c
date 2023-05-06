/*
 * interface.c
 *
 *  Created on: Aug 29, 2022
 *      Author: goku
 */
#include "Interface.h"
#include "usbd_cdc_if.h"
#include "stdbool.h"
#include "MB85RS256.h"
#include "usbd_conf.h"
#include "cmsis_os.h"
#include "Utilities.h"
#include "RtcRealTime.h"
#include "QueueData.h"

#define UART1_BUFFER_LENGTH	32

extern USBD_HandleTypeDef hUsbDeviceFS;
extern UART_HandleTypeDef huart1;
extern DMA_HandleTypeDef hdma_usart1_rx;
extern RTC_HandleTypeDef hrtc;
extern uint8_t queueData[QUEUE_MAX][QUEUE_ITEMLENGTH];
extern uint8_t datamain_old[64];
extern uint8_t data_chessclock[15];

uint8_t uart1_rx_buf[UART1_BUFFER_LENGTH];
uint8_t uart1_data_length;
// new
uint8_t globalMode = MODE_REVIEW;
uint32_t lastCmdReview = 0;
bool flagReviewFullData = true;


#ifdef CALIBASE
	uint8_t calibase_type=0;  // đặt type piece trong chế độ calibase
	bool isCalibaseMode = false;
	TypeConnection typeCalibaseConnect = USB;
	uint8_t dataUsbCalibase[263];
	extern uint16_t datapieces[18][64][2];  	// 18-type 	64-square	2-period+length
	extern uint8_t datamain[64];				// chứa dữ liệu chính của pieces
#endif

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size){
	if(huart->Instance==USART1){
		uart1_data_length=Size;
		AnalyserCMD(uart1_rx_buf,uart1_data_length, RS485);
		HAL_UARTEx_ReceiveToIdle_DMA(&huart1, uart1_rx_buf, UART1_BUFFER_LENGTH);
	}
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart){
	if(huart->Instance==USART1){
		HAL_NVIC_SystemReset();
	}
}

void initReceiverUart(){
	HAL_UARTEx_ReceiveToIdle_DMA(&huart1, uart1_rx_buf, UART1_BUFFER_LENGTH);
	 __HAL_DMA_DISABLE_IT(&hdma_usart1_rx,DMA_IT_HT);
}


/*	Data: 0bxxxxyyyy (32byte <=> 64 square)
 *  xxxx: 0->12 piece A
 *  yyyy: 0->12 piece B
 *  0:EMTY, 1:WKING, 2:WQUEEN, 3:WBISHOP, 4:WKNIGHT, 5:WROOK, 6:WPAWN,
 *  		7:BKING, 8:BQUEEN, 9:BBISHOP,10:BKNIGHT,11:BROOK,12:BPAWN.
 * 		*/
uint8_t byteToPiece(uint8_t piece){
	uint8_t pi;
	switch(piece){
	case 0: pi=EMTY;	break;
	case 1: pi=WKING; 	break;
	case 2: pi=WQUEEN; 	break;
	case 3: pi=WBISHOP; break;
	case 4: pi=WKNIGHT; break;
	case 5: pi=WROOK; 	break;
	case 6: pi=BKING; 	break;
	case 7: pi=BQUEEN; 	break;
	case 8: pi=BBISHOP; break;
	case 9: pi=BKNIGHT; break;
	case 10:pi=BROOK; 	break;
	case 11:pi=BPAWN; 	break;
	case 12:pi=BPAWN; 	break;
	case 13:pi=BPAWN; 	break;
	case 14:pi=BPAWN; 	break;
	case 15:pi=WPAWN; 	break;
	case 16:pi=WPAWN; 	break;
	case 17:pi=WPAWN; 	break;
	case 18:pi=WPAWN; 	break;
	}
	return pi;
}

// new

/*phân tích dữ liệu nhận được */


void AnalyserCMD(uint8_t *data,uint8_t length,TypeConnection typeconnect )
{
	uint16_t crc;
	uint8_t crcval[2];
#ifdef CALIBASE
	if(data[0]==CMD_ENABLE_CALIBASE_MODE){ // Enable Calibase Mode
		isCalibaseMode = true;
		typeCalibaseConnect = typeconnect;
	}else if(data[0]==CMD_CALIBASE_SET_TYPE){  // Calibase Code: 'C' + 'Type'
		calibase_type = data[1];
	}else if(data[0]==CMD_SET_REAL_TIME){
		uint8_t datatime[6];
		for(int i=0;i<6;i++){
			datatime[i] = data[i+1];
		}
		RTC_SetTime(datatime);
	}else if(data[0]==CMD_READ_REAL_TIME){
		UsbSendDateTime();
	}else if(data[0]==CMD_FRAM_ERASE){
		MB85RS_erase();
	}
	if(isCalibaseMode) return;
#endif

	/* kiểm tra khớp địa chỉ */
	if(data[1] != ADDRESSBOARD_PART0 || data[2] != ADDRESSBOARD_PART1 || data[3] != ADDRESSBOARD_PART2) return;
	/* Kiểm tra CRC */
	if(length==7)	// các lệnh Get có độ dài 7byte
	{
		crc = CalculateCRC(data, 1, 4);
		crcval[0] = (uint8_t)((crc & 0xFF00)>>8);
		crcval[1] = (uint8_t)(crc & 0x00FF);
		if(crcval[0] != data[5] || crcval[1] != data[6]) return;
	}else if(length==13) // lệnh Set Time RTC có độ dài 13 byte
	{
		crc = CalculateCRC(data, 1, 10);
		crcval[0] = (uint8_t)((crc & 0xFF00)>>8);
		crcval[1] = (uint8_t)(crc & 0x00FF);
		if(crcval[0] != data[11] || crcval[1] != data[12]) return;
	}

	if(data[4]==CMD_CHECKBOARD)
	{
		Response_CheckAddress(typeconnect);
	}else if(data[4]==CMD_GETBOARDREVIEW)
	{
		globalMode  = MODE_REVIEW;
		if(HAL_GetTick()- lastCmdReview >1000){
			flagReviewFullData = true;
		}
		lastCmdReview = HAL_GetTick();
		Responce_GetBoardReview(typeconnect);
	}else if(data[4] == CMD_GETBOARDRECORDING)
	{
		if(globalMode==MODE_REVIEW) // nếu mode trước đó là review thì clear queue data
		{
			QueueClear();
		}
		globalMode = MODE_RECORDING;
		Responce_GetBoardRecording(typeconnect);
	}else if(data[4] == CMD_SETTIMERTC)
	{
		Responce_SetTimeRTC(data);
	}else if(data[4] >= CMD_GETGAMESAVEBASE){
		Responce_GetDataGameSave(typeconnect,data[4]-CMD_GETGAMESAVEBASE);
	}else if(data[4] == CMD_ERASEALLGAME){
		Responce_EraseGameSave();
	}
}


void Response_CheckAddress(TypeConnection typeconnect)
{
	uint8_t data[7];
	uint16_t crc;
	data[0] = CMD_BEGIN;
	data[1] = ADDRESSBOARD_PART0;
	data[2] = ADDRESSBOARD_PART1;
	data[3] = ADDRESSBOARD_PART2;
	data[4] = CMD_CHECKBOARD;
	crc = CalculateCRC(data, 1, 4);
	data[5] = (uint8_t)((crc & 0xFF00)>>8);
	data[6] = (uint8_t)(crc & 0x00FF);

	if(typeconnect ==RS485){
		HAL_UART_Transmit(&huart1, data, 7,1000);
	}else if(typeconnect == USB){
		CDC_Transmit_FS(data, 7);
	}else if(typeconnect == BLE){

	}
}
void Responce_GetBoardReview(TypeConnection typeconnect) {
	uint8_t data[53];
	uint16_t crc;
	uint8_t pieceA,pieceB,pie=0;
	data[0] = CMD_BEGIN;
	data[1] = ADDRESSBOARD_PART0;
	data[2] = ADDRESSBOARD_PART1;
	data[3] = ADDRESSBOARD_PART2;
	data[4] = RES_DATAFENCHANGE;

	if (flagReviewFullData == true) {
		flagReviewFullData = false;
		/* gắn datamain vào 32 byte tiếp theo */
		for(int i=0;i<32;i++){
			pieceA = byteToPiece(datamain_old[pie]);
			pie++;
			pieceB = byteToPiece(datamain_old[pie]);
			pie++;
			data[i+5]=(pieceA<<4)+pieceB;
		}
		for(int i=0;i<14;i++)
		{
			data[37+i] = data_chessclock[i];
		}
		crc = CalculateCRC(data, 1, 50);
			data[51] = (uint8_t)((crc &0xFF00)>>8);
			data[52] = (uint8_t)(crc &0x00FF);
		if (typeconnect == RS485) {
			HAL_UART_Transmit(&huart1, data, 53, 1000);
		} else if (typeconnect == USB) {
			CDC_Transmit_FS(data, 53);
		} else if (typeconnect == BLE) {

		}
	}else {
		crc = CalculateCRC(data, 1, 4);
		data[5] = (uint8_t) ((crc & 0xFF00) >> 8);
		data[6] = (uint8_t) (crc & 0x00FF);
		/* Send Data*/
		if (typeconnect == RS485) {
			HAL_UART_Transmit(&huart1, data, 7, 1000);
		} else if (typeconnect == USB) {
			CDC_Transmit_FS(data, 7);
		} else if (typeconnect == BLE) {

		}
	}
}

void Responce_GetBoardRecording(TypeConnection typeconnect)
{
	if (QueueGetCount() > 0) {
		uint8_t front = QueueGetFront();
		if (typeconnect == RS485) {
			HAL_UART_Transmit(&huart1, queueData[front], 53, 1000);
		} else if (typeconnect == USB) {
			CDC_Transmit_FS(queueData[front], 53);
		} else if (typeconnect == BLE) {

		}
		QueueGetFrontAfter();
	}else if(flagReviewFullData){
		Responce_GetBoardReview(typeconnect);
	} else {
		uint8_t data[53];
		uint16_t crc;

		data[0] = CMD_BEGIN;
		data[1] = ADDRESSBOARD_PART0;
		data[2] = ADDRESSBOARD_PART1;
		data[3] = ADDRESSBOARD_PART2;
		data[4] = CMD_GETBOARDDATA;
		crc = CalculateCRC(data, 1, 4);
		data[5] = (uint8_t) ((crc & 0xFF00) >> 8);
		data[6] = (uint8_t) (crc & 0x00FF);
		/* Send Data*/
		if (typeconnect == RS485) {
			HAL_UART_Transmit(&huart1, data, 7, 1000);
		} else if (typeconnect == USB) {
			CDC_Transmit_FS(data, 7);
		} else if (typeconnect == BLE) {

		}
	}
}
void Responce_SetTimeRTC(uint8_t *dat)
{
	uint8_t datatime[6];
	for(int i=0;i<6;i++){
		datatime[i] = dat[i+5];
	}
	RTC_SetTime(datatime);
}

void Responce_GetDataGameSave(TypeConnection typeconnect,uint8_t part)
{
	uint8_t data[GAMESAVELENGTH];
	if(part > GAMESAVEPARTS) return;
	MB85RS_read(part*GAMESAVELENGTH, data, GAMESAVELENGTH);

	if(typeconnect ==RS485){
			HAL_UART_Transmit(&huart1, data, GAMESAVELENGTH, 1000);
	}else if(typeconnect == USB){
		CDC_Transmit_FS(data, GAMESAVELENGTH);
	}else if(typeconnect == BLE){

	}
}
void Responce_EraseGameSave()
{
	MB85RS_erase();
}


void UsbSendDateTime()
{
#ifdef CALIBASE
	uint8_t dat[7];
	RTC_TimeTypeDef 	sTime;
	RTC_DateTypeDef 	sDate;
	HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
	dat[0] = sTime.Seconds;
	dat[1] = sTime.Minutes;
	dat[2] = sTime.Hours;
	dat[3] = sDate.Date;
	dat[4] = sDate.Month;
	dat[5] = sDate.Year;
	dat[6] = NEWLINE;
	CDC_Transmit_FS(dat, 7);
#endif
}

void UsbSendDataCalibase()
{
#ifdef CALIBASE
	if(isCalibaseMode==true){
		if(calibase_type>17) calibase_type=0;
		for(int i=0;i<64;i++){
			dataUsbCalibase[i]= (uint8_t)datapieces[calibase_type][i][PULSES];
		}
		int inx =0;
		for(int i=0;i<64;i++){
			dataUsbCalibase[64+inx] = datapieces[calibase_type][i][PERIOD]/254;
			inx++;
			dataUsbCalibase[64+inx] = datapieces[calibase_type][i][PERIOD]%254;
			inx++;
		}
		for(int i=0;i<64;i++){
			dataUsbCalibase[192+i] = datamain[i];
		}

		RTC_TimeTypeDef sTime = {0};
		RTC_DateTypeDef sDate = {0};
		HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
		HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);

		dataUsbCalibase[256] = sTime.Seconds;
		dataUsbCalibase[257] = sTime.Minutes;
		dataUsbCalibase[258] = sTime.Hours;
		dataUsbCalibase[259] = sDate.Date;
		dataUsbCalibase[260] = sDate.Month;
		dataUsbCalibase[261] = sDate.Year;

		dataUsbCalibase[262]=255;
		if(typeCalibaseConnect==USB){
			CDC_Transmit_FS(dataUsbCalibase,263);
		}else if(typeCalibaseConnect==RS485){
			HAL_UART_Transmit(&huart1, dataUsbCalibase, 263, 1000);
		}
	}
#endif

}



