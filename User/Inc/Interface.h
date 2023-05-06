/*
 * Interface.h
 *
 *  Created on: Aug 31, 2022
 *      Author: goku
 */

#ifndef INC_INTERFACE_H_
#define INC_INTERFACE_H_
#include "main.h"
#include "stdbool.h"
#include "RtcRealTime.h"

enum {EMTY=0,WKING,WQUEEN,WBISHOP,WKNIGHT,WROOK,WPAWN,BKING,BQUEEN,BBISHOP,BKNIGHT,BROOK,BPAWN};

#define MODE_REVIEW					0
#define MODE_RECORDING				1

#define CMD_BEGIN 					58	// ':'
#define CMD_CHECKBOARD 				49	// '1'
#define CMD_GETBOARDDATA 			50
#define CMD_GETBOARDREVIEW 			50 	// '2'
#define CMD_GETBOARDRECORDING 		51 	// '3'
#define CMD_SETTIMERTC				52	// '4'
#define CMD_ERASEALLGAME   			55  // '7'

#define CMD_GETGAMESAVEBASE	   		65	// 'A'
#define GAMESAVELENGTH  			512
#define GAMESAVEPARTS				62

#define RES_DATAFENCHANGE			56	// '8'
#define RES_DATACLOCKCHANGE			57  // '9'




typedef enum {RS485=0,USB,BLE}TypeConnection;

void sendFenData();
void saveGame();
bool CheckFenIsPositionStart();
void RoamingData();

uint8_t byteToPiece(uint8_t piece);

// new
void AnalyserCMD(uint8_t *data,uint8_t length,TypeConnection typeconnect );
void Response_CheckAddress(TypeConnection typeconnect);
void Responce_GetBoardReview(TypeConnection typeconnect);
void Responce_GetBoardRecording(TypeConnection typeconnect);
void Responce_SetTimeRTC(uint8_t *dat);

void Responce_GetDataGameSave(TypeConnection typeconnect,uint8_t part);
void Responce_EraseGameSave();

void UsbSendDateTime();
void UsbSendDataCalibase();

#endif /* INC_INTERFACE_H_ */
