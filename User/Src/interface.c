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


#define UART2_BUFFER_LENGTH	32
#define UART1_BUFFER_LENGTH	32

uint8_t data_mainpiece[64];
uint8_t flag_masterGetData = 0;

extern uint8_t datamain[64];
extern uint8_t datamain_old[64];
extern USBD_HandleTypeDef hUsbDeviceFS;
extern UART_HandleTypeDef huart1;
extern DMA_HandleTypeDef hdma_usart1_rx;

extern uint8_t data_chessclock[15];
extern bool mainChange;


uint8_t compression_board[41]; // Address:3byte databoard:32byte + timePlay:4byte + switchClock:1byte + Newline:1byte
uint8_t uart1_rx_buf[UART1_BUFFER_LENGTH];
uint8_t uart1_main_buf[UART1_BUFFER_LENGTH];
bool uart1_onData=false;
uint8_t uart1_data_length;

#ifdef CALIBASE
	uint8_t calibase_type=0;  // đặt type piece trong chế độ calibase
	bool isCalibaseMode = false;
	uint8_t dataUsbCalibase[257];
	extern uint16_t datapieces[18][64][2];  	// 18-type 	64-square	2-period+length
	extern uint8_t datamain[64];				// chứa dữ liệu chính của pieces
	extern RTC_HandleTypeDef hrtc;
	extern USBD_HandleTypeDef hUsbDeviceFS;
#endif

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size){
	if(huart->Instance==USART1){
		memcpy(uart1_main_buf,uart1_rx_buf,UART1_BUFFER_LENGTH);
		uart1_onData=true;
		uart1_data_length=Size;

		if(uart1_main_buf[0]=='S' || uart1_main_buf[0]=='G'){  // Scan Code
			uint32_t addr = ((uint32_t)(uart1_main_buf[1]))*64516 +  ((uint32_t)uart1_main_buf[2])*254 +  (uint32_t)uart1_main_buf[3];
			if(addr==ADDRESSBOARD){
				if(uart1_main_buf[0]=='S'){
					uint8_t res[5];
					res[0] = ADDR_PART0;
					res[1] = ADDR_PART1;
					res[2] = ADDR_PART2;
					res[3] = 'R';	// interface is RS485
					res[4] = 255;
					HAL_UART_Transmit(&huart1, res, 5, 100);
				}else if(uart1_main_buf[0]=='G'){
					flag_masterGetData = 1;	//interface: 1=RS485 2=USB
				}
				uart1_main_buf[0]=0;
			}
		}
		HAL_UARTEx_ReceiveToIdle_DMA(&huart1, uart1_rx_buf, UART1_BUFFER_LENGTH);
	}
#ifdef USING_BLUETOOTH
	else if(huart->Instance==USART2){
			memcpy(uart2_main_buf,uart2_rx_buf,UART2_BUFFER_LENGTH);
			uart2_onData=true;
			uart2_data_length=Size;
			HAL_UARTEx_ReceiveToIdle_DMA(&huart2, uart2_rx_buf, UART2_BUFFER_LENGTH);
	}
#endif
}


void initReceiverUart(){
	HAL_UARTEx_ReceiveToIdle_DMA(&huart1, uart1_rx_buf, UART1_BUFFER_LENGTH);
	 __HAL_DMA_DISABLE_IT(&hdma_usart1_rx,DMA_IT_HT);
}



// 32bytePiece + 2byteWhiteTime + 2byteBlackTime +1byteSide
// vị trí board_start ở dạng nén
const uint8_t position_start[64]={11,10,9,8,7,9,10,11,12,12,12,12,12,12,12,12,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,6,6,6,6,6,6,6,5,4,3,2,1,3,4,5};
uint8_t position_old[64];
bool isNewGame=false;


 void RoamingData()
 {
	 // gắn Address vào 3 Byte đầu
	compression_board[0] = ADDR_PART0;
	compression_board[1] = ADDR_PART1;
	compression_board[2] = ADDR_PART2;
	// gắn datamain vào 32 byte tiếp theo
	uint8_t pieceA,pieceB,pie=0;
	for(int i=0;i<32;i++){
		pieceA = byteToPiece(datamain[pie]);
		data_mainpiece[pie] = pieceA;
		pie++;
		pieceB = byteToPiece(datamain[pie]);
		data_mainpiece[pie] = pieceB;
		pie++;
		compression_board[i+3]=(pieceA<<4)+pieceB;
	}
	compression_board[40]=NEWLINE;
	// nếu có lệnh gửi compression từ UART
	if(flag_masterGetData==1){ // nếu có lệnh gửi compression từ UART
		HAL_UART_Transmit(&huart1, compression_board, 41, 1000);
	}else if(flag_masterGetData==2){ 						// nếu có lệnh gửi compression từ USB
		if(hUsbDeviceFS.dev_state == USBD_STATE_CONFIGURED){
			CDC_Transmit_FS(compression_board, 41);
		}
	}
	flag_masterGetData=0;

	if(isNewGame==false){
		isNewGame = CheckFenIsPositionStart();
		if(isNewGame==true){
			// write a new game fo Fram
			//uint8_t dat[]="NewGame\r\n";
			//CDC_Transmit_FS(dat, 9);
			MB85RS_WriteNewGame();
		}
	}
}


bool CheckFenIsPositionStart(){
	for(int i=0;i<64;i++){
		if(data_mainpiece[i]!=position_start[i]){
			return false;
		}
	}
	return true;
}
void CopyFenToOldFen(){
	for(int i=0;i<64;i++){
		position_old[i] = data_mainpiece[i];
	}
}




// FRAM lưu được 35*25 = 875 Fen
uint8_t data_fram[926]; // 25*37 byte
void sendFenData(){
	for(int i=0;i<35;i++){
		MB85RS_read(i*925, data_fram, 925);
		if(i<34){
			CDC_Transmit_FS(data_fram, 925);
		}else if(i==34){
			data_fram[925]=255;
			CDC_Transmit_FS(data_fram, 926);
		}
		//osDelay(1);
	}
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
	}else if(data[4]==CMD_GETBOARDDATA)
	{
		Responce_GetBoardData(true,typeconnect);
	}else if(data[4] == CMD_GETBOARDDATAFULL)
	{
		Responce_GetBoardData(false, typeconnect);
	}else if(data[4] == CMD_SETTIMERTC)
	{
		Responce_SetTimeRTC(data,USB);
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

	}else if(typeconnect == USB){
		CDC_Transmit_FS(data, 7);
	}else if(typeconnect == BLE){

	}
}
/* autoSend==true : chỉ gửi dữ liệu khi có sự thay đổi (mainChange==true)
 * 					nếu mainChange==false thì gửi lại Response thôi
 * autoSend==false : gửi toàn bộ dữ liệu bất chấp mainChange */
void Responce_GetBoardData(bool autoSend, TypeConnection typeconnect)
{
	uint8_t data[53];
	uint8_t pieceA,pieceB,pie=0;
	uint16_t crc;

	data[0] = CMD_BEGIN;
	data[1] = ADDRESSBOARD_PART0;
	data[2] = ADDRESSBOARD_PART1;
	data[3] = ADDRESSBOARD_PART2;
	data[4] = CMD_GETBOARDDATA;
	if(mainChange==true || autoSend==false)
	{
	/* gắn datamain vào 32 byte tiếp theo */
		for(int i=0;i<32;i++){
			pieceA = byteToPiece(datamain_old[pie]);
			//data_mainpiece[pie] = pieceA;
			data[i+5] = pieceA;
			pie++;
			pieceB = byteToPiece(datamain_old[pie]);
			//data_mainpiece[pie] = pieceB;
			data[i+5] = pieceB;
			pie++;
			data[i+5]=(pieceA<<4)+pieceB;
		}
		for(int i=0;i<14;i++){
			data[37+i] = data_chessclock[i];
		}
		crc = CalculateCRC(data, 1, 50);
		data[51] = (uint8_t)((crc &0xFF00)>>8);
		data[52] = (uint8_t)(crc &0x00FF);
		/* Send Data*/
		if(typeconnect ==RS485){

		}else if(typeconnect == USB){
			CDC_Transmit_FS(data, 53);
		}else if(typeconnect == BLE){

		}
		mainChange=false;
	}else
	{
		crc = CalculateCRC(data, 1, 4);
		data[5] = (uint8_t)((crc &0xFF00)>>8);
		data[6] = (uint8_t)(crc &0x00FF);
		/* Send Data*/
		if(typeconnect ==RS485){

		}else if(typeconnect == USB){
			CDC_Transmit_FS(data, 7);
		}else if(typeconnect == BLE){

		}
	}
}
void Responce_SetTimeRTC(uint8_t *dat, TypeConnection typeconnect)
{
	uint8_t datatime[6];
	uint8_t data[7];
	for(int i=0;i<6;i++){
		datatime[i] = dat[i+5];
	}
	RTC_SetTime(datatime);
	uint16_t crc;
	data[0] = CMD_BEGIN;
	data[1] = ADDRESSBOARD_PART0;
	data[2] = ADDRESSBOARD_PART1;
	data[3] = ADDRESSBOARD_PART2;
	data[4] = CMD_SETTIMERTC;
	crc = CalculateCRC(data, 1, 4);
	data[5] = (uint8_t)((crc & 0xFF00)>>8);
	data[6] = (uint8_t)(crc & 0x00FF);

	if(typeconnect ==RS485){

	}else if(typeconnect == USB){
			CDC_Transmit_FS(data, 7);
	}else if(typeconnect == BLE){

	}
}
void Responce_GetInfoGameSave();
void Responce_GetDataGameSave();
void Responce_EraseGameSave();


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
	// nếu USB không được kết nối thì return
	if(hUsbDeviceFS.dev_state != USBD_STATE_CONFIGURED)
	{
		isCalibaseMode = false;
		return;
	}

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
		dataUsbCalibase[256]=255;
		CDC_Transmit_FS(dataUsbCalibase,257);
	}
#endif

}



