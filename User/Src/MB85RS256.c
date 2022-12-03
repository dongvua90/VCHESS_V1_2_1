/*
 * MB85RS256.c
 *
 *  Created on: Aug 22, 2022
 *      Author: goku
 */
#include "MB85RS256.h"
#include "stdio.h"

#define MB85RS_CS_LOW 	HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, GPIO_PIN_RESET)
#define MB85RS_CS_HIGH 	HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, GPIO_PIN_SET)

// OP-CODE
#define WREN       0x06		// Set Write Enable Latch
#define WRDI       0x04  	// Reset Write Enable Latch
#define RDSR       0x05		// Read Status Register
#define WRSR       0x01		// Write Status Register
#define READ       0x03		// Read Memory Code
#define WRITE      0x02		// Write Memory Code
#define RDID       0x9F
#define SLEEP      0xB9

uint16_t pointsave=0;
extern uint8_t compression_board[41];
extern RTC_HandleTypeDef hrtc;

uint8_t tik=0;
uint8_t Mb85rs_dataTx[50];
uint8_t Mb85rs_dataRx[50];
uint8_t data_rrx[37];
uint8_t data_tx[32];
uint8_t Mb85rs_cmd[3];
uint8_t cmd=0;
extern SPI_HandleTypeDef hspi1;
HAL_StatusTypeDef status;

// Thiết lập lại trạng thái Write ( bảo vệ , không cho phép Write)
void WriteProtect(){
	uint8_t cmd[1];
	cmd[0]= WRDI; // Reset Write Enable Latch
	MB85RS_CS_LOW;
	status = HAL_SPI_Transmit(&hspi1, cmd, 1, 1);
	MB85RS_CS_HIGH;
}
uint8_t ReadStatus(){
	uint8_t reg[2],cmd[2];
	cmd[0] = RDSR;// Read Status Register
	MB85RS_CS_LOW;
	HAL_SPI_TransmitReceive(&hspi1,cmd, reg, 2, 1);
	MB85RS_CS_HIGH;
	return reg[1];
}
void WriteEnable(){
	uint8_t cmd[1];
	cmd[0]=WREN;
	MB85RS_CS_LOW;
	status = HAL_SPI_Transmit(&hspi1, cmd, 1, 1);
	MB85RS_CS_HIGH;
}

void MB85RS_write(unsigned int address, unsigned char *buffer, unsigned int size)
{
	Mb85rs_dataTx[0] = WRITE;
	Mb85rs_dataTx[1] = (char)(address >> 8);
	Mb85rs_dataTx[2] = (char)(address);
	for(int i=0;i<size;i++){
		Mb85rs_dataTx[3+i] = buffer[i];
	}
	WriteEnable();
	MB85RS_CS_LOW;
	HAL_SPI_Transmit(&hspi1, Mb85rs_dataTx, size+3, 10);
	MB85RS_CS_HIGH;
	WriteProtect();
}
void MB85RS_read(unsigned int address, unsigned char *buffer, unsigned int size)
{
	uint8_t cmd[3];
	cmd[0]=READ;
	cmd[1]=(char)(address >> 8);
	cmd[2] = (char)(address);
	MB85RS_CS_LOW;
	HAL_SPI_Transmit(&hspi1, cmd, 3, 10);
	HAL_SPI_Receive(&hspi1, buffer, size, 10);
	MB85RS_CS_HIGH;
}

void MB85RS_erase()
{
	uint8_t cmd[3];
	uint8_t datanull[128];
	//setStatus(0x00);
	cmd[0]=WRITE;
	cmd[1] = 0;
	cmd[2] = 0;
	WriteEnable();
	MB85RS_CS_LOW;
	for(int i=0;i<128;i++){
		datanull[i]=0;
	}
	HAL_SPI_Transmit(&hspi1, cmd, 3, 100);
	for(int i = 0; i <= 256; i++)
	{
		HAL_SPI_Transmit(&hspi1, datanull, 128, 100);
	}
	MB85RS_CS_HIGH;
	WriteProtect();
}


// get Điểm lưu FEN gần nhất ( thông tin được lưu ở Address cuối cùng)
void MB85RS_getPointSave(){
	uint8_t point[2];
	MB85RS_read(32766, point, 2);
	pointsave = point[0]*255 +point[1];
}
void MB85RS_WritePointSave(){
	if(pointsave >1022) pointsave=0;
	uint8_t point[2];
	point[0] = pointsave/255;
	point[1] = pointsave%255;
	MB85RS_write(32766,point, 2);
}

void MB85RS_WriteNewGame(){
	uint8_t dat[37];
	RTC_TimeTypeDef sTime;
	RTC_DateTypeDef sDate;
	HAL_RTC_GetTime(&hrtc, &sTime,RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
	dat[0]=254; // code for HeaderGame
	dat[1] = sTime.Seconds;
	dat[2] = sTime.Minutes;
	dat[3] = sTime.Hours;
	dat[4] = sDate.Date;
	dat[5] = sDate.Month;
	dat[6] = sDate.Year;
	for(int i=7;i<37;i++){
		dat[i]=0;
	}
	MB85RS_write(pointsave*37, dat, 37);
	pointsave++;
	MB85RS_WritePointSave();
}

// 32bytePiece + 2byteWhiteTime + 2byteBlackTime +1byteSide
void MB85RS_saveFen(){
	//uint8_t dat[11];
	//snprintf(dat,sizeof(dat),"Save:%d\r\n",pointsave);
	//CDC_Transmit_FS(dat, 11);
	uint8_t dat[37];
	for(int i=0;i<37;i++){
		dat[i]=compression_board[i+3];
	}
	MB85RS_write(pointsave*37,dat, 37);
	pointsave++;
	MB85RS_WritePointSave();
}
void MB85RS_readFen(){
	MB85RS_read(pointsave*37,data_rrx,37);
}














