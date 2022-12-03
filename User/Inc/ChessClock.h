/*
 * ChessClock.h
 *
 *  Created on: Nov 9, 2022
 *      Author: goku
 */

#ifndef INC_CHESSCLOCK_H_
#define INC_CHESSCLOCK_H_
/*
[4BYTE_TIME_WHITE] [4BYTE_TIME_BLACK] [1BYTE_SIDE] [1BYTE_STATUS] [2BYTE_MOVED_WHITE] [2BYTE_MOVED_BLACK] [1BYTE_CRC] :15BYTE

-4BYTE_TIME_WHITE 	: Total time for White
-4BYTE_TIME_BLACK 	: Total time for Black
-1BYTE_SIDE 				: 0b00000abx
											a: White Flag Over Time
											b: Black Flag Over Time
											x=0: side = White
											x=1: side = Black
-1BYTE_STATUS				: = PLAYING,
											PAUSED,
											STOP,
											WHIE_BLACK_OVERTIME
-2BYTE_MOVED_WHITE 	: White Moved
-2BYTE_MOVED_BLACK 	: Black Moved
-1BYTE_CRC					: Check SUM
*/

#include "main.h"

void ChessClock_Init();


#endif /* INC_CHESSCLOCK_H_ */
