/*
 * ChessClock.h
 *
 *  Created on: Nov 9, 2022
 *      Author: goku
 */

#ifndef INC_CHESSCLOCK_H_
#define INC_CHESSCLOCK_H_
/* DATA FRAME INTERFACE I2C */
/*
 old =[4BYTE_TIME_WHITE] [4BYTE_TIME_BLACK] [1BYTE_SIDE] [1BYTE_STATUS]  : SUM=10BYTE
[4BYTE_TIME_WHITE] [4BYTE_TIME_BLACK] [1BYTE_SIDE] [1BYTE_STATUS] [2BYTE_MOVED_WHITE] [2BYTE_MOVED_BLACK] [1BYTE_CRC] :15BYTE

-4BYTE_TIME_WHITE 	: Total time for White
-4BYTE_TIME_BLACK 	: Total time for Black
-1BYTE_SIDE 				: 0b00000abx
											a: White Flag Over Time
											b: Black Flag Over Time
											x=0: side = Black
											x=1: side = White
-1BYTE_STATUS{ READY=0,BEGIN_PLAY=1,PLAYING=2,PLAY_TO_PAUSE=3,PAUSE_TO_PLAY=4,ONE_SIDE_OVER_TIME=5,ALL_SIDE_OVER_TIME=6 }
	TaskDefault sẽ thiết lập STATUS =  DISCONNECT(55)
-2BYTE_MOVED_WHITE 	: White Moved
-2BYTE_MOVED_BLACK 	: Black Moved
-1BYTE_CRC					: Check SUM
*/

#include "main.h"

#define STATUS_DISCONNECT_CLOCK 55
#define SIDE_WHITE 1
#define SIDE_BLACK 2

void ChessClock_Init();


#endif /* INC_CHESSCLOCK_H_ */
