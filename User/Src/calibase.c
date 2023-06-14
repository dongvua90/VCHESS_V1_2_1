/*
 * calibase.c
 *
 *  Created on: Mar 19, 2022
 *      Author: goku
 */

#include "calibase.h"
#include "string.h"



uint8_t threshold[THRESHOLD_ROW_SIZE][THRESHOLD_COLUMN_SIZE];

const uint8_t mark_pulse0[64]={
12,12,12,12,12,12,12,12,
12,12,12,12,12,12,12,12,
12,12,12,12,12,12,12,12,
12,12,12,12,12,12,12,12,
12,12,12,12,12,12,12,12,
12,12,12,12,12,12,12,12,
12,12,12,12,12,12,12,12,
12,12,12,12,12,12,12,12
};

const uint8_t mark_pulse1[64]={
16,16,16,16,16,16,16,16,
16,16,16,16,16,16,16,16,
16,16,16,16,16,16,16,16,
16,16,16,16,16,16,16,16,
16,16,16,16,16,16,16,16,
16,16,16,16,16,16,16,16,
16,16,16,16,16,16,16,16,
16,16,16,16,16,16,16,16
};

const uint8_t mark_pulse2[64]={
16,16,16,16,16,16,16,16,
16,16,16,16,16,16,16,16,
16,16,16,16,16,16,16,16,
16,16,16,16,16,16,16,16,
16,16,16,16,16,16,16,16,
16,16,16,16,16,16,16,16,
16,16,16,16,16,16,16,16,
16,16,16,16,16,16,16,16
};

const uint8_t mark_pulse3[64]={
18,18,18,18,18,18,18,18,
18,18,18,18,18,18,18,18,
18,18,18,18,18,18,18,18,
18,18,18,18,18,18,18,18,
18,18,18,18,18,18,18,18,
18,18,18,18,18,18,18,18,
18,18,18,18,18,18,18,18,
18,18,18,18,18,18,18,18
};

const uint8_t mark_pulse4[64]={
18,18,18,18,18,18,18,18,
18,18,18,18,18,18,18,18,
18,18,18,18,18,18,18,18,
18,18,18,18,18,18,18,18,
21,18,18,18,18,18,18,18,
18,18,18,18,18,18,18,18,
18,18,18,18,18,18,18,18,
18,18,18,18,18,18,18,18
};

const uint8_t mark_pulse5[64]={
16,16,16,16,16,16,16,16,
16,16,16,16,16,16,16,16,
16,16,16,16,16,16,16,16,
16,16,16,16,16,16,16,16,
16,16,16,16,16,16,16,16,
16,16,16,16,16,16,16,16,
16,16,16,16,16,16,16,16,
16,16,16,16,16,16,16,16
};

const uint8_t mark_pulse6[64]={
16,16,16,16,16,16,16,16,
16,16,16,16,16,16,16,16,
16,16,16,16,16,16,16,16,
16,16,16,16,16,16,16,16,
16,16,16,16,16,16,16,16,
16,16,16,16,16,16,16,16,
16,16,16,16,16,16,16,16,
16,16,16,16,16,16,16,16
};

const uint8_t mark_pulse7[64]={
19,19,19,19,19,19,19,19,
19,19,19,19,19,19,19,19,
19,19,19,19,19,19,19,19,
19,19,19,19,19,19,19,19,
19,19,19,19,19,19,19,19,
19,19,19,19,19,19,19,19,
19,19,19,19,19,19,19,19,
19,19,19,19,19,19,19,19
};

const uint8_t mark_pulse8[64]={
20,20,20,20,20,20,20,20,
20,20,20,20,20,20,20,20,
20,20,20,20,20,20,20,20,
20,20,20,20,20,20,20,20,
20,20,20,20,20,20,20,20,
20,20,20,20,20,20,20,20,
20,20,20,20,20,20,20,20,
20,20,20,20,20,20,20,20
};

const uint8_t mark_pulse9[64]={
16,16,16,16,16,16,16,16,
16,16,16,16,16,16,16,16,
16,16,16,16,16,16,16,16,
16,16,16,16,16,16,16,16,
16,16,16,16,16,16,16,16,
16,16,16,16,16,16,16,16,
16,16,16,16,16,16,16,16,
16,16,16,16,16,16,16,16
};

const uint8_t mark_pulse10[64]={
30,30,30,30,30,30,30,30,
30,30,30,30,30,30,30,30,
30,30,30,30,30,30,30,30,
30,30,30,30,30,30,30,30,
30,30,30,30,30,30,30,30,
30,30,30,30,30,30,30,30,
30,30,30,30,30,30,30,30,
30,30,30,30,30,30,30,30
};

const uint8_t mark_pulse11[64]={
16,16,16,16,16,16,16,16,
16,16,16,16,16,16,16,16,
16,16,16,16,16,16,16,16,
16,16,16,16,16,16,16,16,
16,16,16,16,16,16,16,16,
16,16,16,16,16,16,16,16,
16,16,16,16,16,16,16,16,
16,16,16,16,16,16,16,16
};

const uint8_t mark_pulse12[64]={
16,16,16,16,16,16,16,16,
16,16,16,16,16,16,16,16,
16,16,16,16,16,16,16,16,
16,16,16,16,16,16,16,16,
16,16,16,16,16,16,16,16,
16,16,16,16,16,16,16,16,
16,16,16,16,16,16,16,16,
16,16,16,16,16,16,16,16
};

const uint8_t mark_pulse13[64]={
20,20,20,20,20,20,20,20,
20,20,20,20,20,20,20,20,
20,20,20,20,20,20,20,20,
20,20,20,20,20,20,20,20,
20,20,20,20,20,20,20,20,
20,20,20,20,20,20,20,20,
20,20,20,20,20,20,20,20,
20,20,20,20,20,20,20,20
};

const uint8_t mark_pulse14[64]={
22,22,22,22,22,22,22,22,
22,22,22,22,22,22,22,22,
22,22,22,22,22,22,22,22,
22,22,22,22,22,22,22,22,
22,22,22,22,22,22,22,22,
22,22,22,22,22,22,22,22,
22,22,22,22,22,22,22,22,
22,22,22,22,22,22,22,22
};

const uint8_t mark_pulse15[64]={
18,18,18,18,18,18,18,18,
18,18,18,18,18,18,18,18,
18,18,18,18,18,18,18,18,
18,18,18,18,18,18,18,18,
18,18,18,18,18,18,18,18,
18,18,18,18,18,18,18,18,
18,18,18,18,18,18,18,18,
18,18,18,18,18,18,18,18
};

const uint8_t mark_pulse16[64]={
26,26,26,26,26,26,26,26,
26,26,26,26,26,26,26,26,
26,26,26,26,26,26,26,26,
26,26,26,26,26,26,26,26,
26,26,26,26,26,26,26,26,
26,26,26,26,26,26,26,26,
26,26,26,26,26,26,26,26,
26,26,26,26,26,26,26,26
};

const uint8_t mark_pulse17[64]={
25,25,25,25,25,25,25,25,
25,25,25,25,25,25,25,25,
25,25,25,25,25,25,25,25,
25,25,25,25,25,25,25,25,
25,25,25,25,25,25,25,25,
25,25,25,25,25,25,25,25,
25,25,25,25,25,25,25,25,
25,25,25,25,25,25,25,25
};




















uint8_t getMarkSq(uint8_t type, uint8_t squamark) {
	uint8_t pulses_mark = 0;
	if(squamark>=64) return 15;
	pulses_mark = threshold[type][squamark];

//	switch (type) {
//	case 0:
//		pulses_mark = mark_pulse0[squamark];
//		break;
//	case 1:
//		pulses_mark = mark_pulse1[squamark];
//		break;
//	case 2:
//		pulses_mark = mark_pulse2[squamark];
//		break;
//	case 3:
//		pulses_mark = mark_pulse3[squamark];
//		break;
//	case 4:
//		pulses_mark = mark_pulse4[squamark];
//		break;
//	case 5:
//		pulses_mark = mark_pulse5[squamark];
//		break;
//	case 6:
//		pulses_mark = mark_pulse6[squamark];
//		break;
//	case 7:
//		pulses_mark = mark_pulse7[squamark];
//		break;
//	case 8:
//		pulses_mark = mark_pulse8[squamark];
//		break;
//	case 9:
//		pulses_mark = mark_pulse9[squamark];
//		break;
//	case 10:
//		pulses_mark = mark_pulse10[squamark];
//		break;
//	case 11:
//		pulses_mark = mark_pulse11[squamark];
//		break;
//	case 12:
//		pulses_mark = mark_pulse12[squamark];
//		break;
//	case 13:
//		pulses_mark = mark_pulse13[squamark];
//		break;
//	case 14:
//		pulses_mark = mark_pulse14[squamark];
//		break;
//	case 15:
//		pulses_mark = mark_pulse15[squamark];
//		break;
//	case 16:
//		pulses_mark = mark_pulse16[squamark];
//		break;
//	case 17:
//		pulses_mark = mark_pulse17[squamark];
//		break;
//	}
	return pulses_mark;
}

void Thresholdinit() {
	//ThresholdRead();
	/* Nếu dữ liệu lỗi thì gán ngưỡng bằng giá trị mặc định */
	//if (ThresholdCheckError() == true) {
		for (int type = 0; type < 18; type++) {
			for (int i = 0; i < 64; i++) {
				switch(type){
				case 0:threshold[type][i] = mark_pulse0[i]; break;
				case 1:threshold[type][i] = mark_pulse1[i]; break;
				case 2:threshold[type][i] = mark_pulse2[i]; break;
				case 3:threshold[type][i] = mark_pulse3[i]; break;
				case 4:threshold[type][i] = mark_pulse4[i]; break;
				case 5:threshold[type][i] = mark_pulse5[i]; break;
				case 6:threshold[type][i] = mark_pulse6[i]; break;
				case 7:threshold[type][i] = mark_pulse7[i]; break;
				case 8:threshold[type][i] = mark_pulse8[i]; break;
				case 9:threshold[type][i] = mark_pulse9[i]; break;
				case 10:threshold[type][i] = mark_pulse10[i]; break;
				case 11:threshold[type][i] = mark_pulse11[i]; break;
				case 12:threshold[type][i] = mark_pulse12[i]; break;
				case 13:threshold[type][i] = mark_pulse13[i]; break;
				case 14:threshold[type][i] = mark_pulse14[i]; break;
				case 15:threshold[type][i] = mark_pulse15[i]; break;
				case 16:threshold[type][i] = mark_pulse16[i]; break;
				case 17:threshold[type][i] = mark_pulse17[i]; break;
				}
			}
		}
		/* Lưu lại vào bộ nhớ Flash */
		//ThresholdWrite();
	//}
}

void ThresholdRead() {
	// Đọc mảng 2 chiều từ flash
	//memcpy(threshold, (uint8_t*) THRESHOLD_FLASH_ARRAY_START_ADDR,THRESHOLD_ROW_SIZE * THRESHOLD_COLUMN_SIZE);
}

bool ThresholdCheckError() {
	// Kiểm tra giá trị có hợp lệ không
	for (int type = 0; type < 18; type++) {
		for (int i = 0; i < 64; i++) {
			if (threshold[type][i] > 50)
				return true;
		}
	}
	return false;
}

uint32_t sectorError = 0;
HAL_StatusTypeDef flash_err;

void ThresholdWrite() {
	 // Xóa sector chứa vùng nhớ flash
//	HAL_FLASH_Unlock();
//	  FLASH_EraseInitTypeDef eraseConfig;
//	  eraseConfig.TypeErase = FLASH_TYPEERASE_SECTORS;
//	  eraseConfig.Sector = FLASH_SECTOR_5;  // Ví dụ: Xóa sector 5
//	  eraseConfig.NbSectors = 1;
//	  eraseConfig.VoltageRange = FLASH_VOLTAGE_RANGE_1;  //  Điện áp từ 2.7V đến 3.6V.
//
//	  HAL_FLASHEx_Erase(&eraseConfig, &sectorError);
//	  HAL_Delay(1000);
	// Lưu mảng 2 chiều vào flash
	//uint32_t tempArray[THRESHOLD_ROW_SIZE][THRESHOLD_COLUMN_SIZE/4];
//	  for (int i = 0; i < THRESHOLD_ROW_SIZE; i++)
//	  {
//	    for (int j = 0; j < THRESHOLD_COLUMN_SIZE/4; j++)
//	    {
//	      //uint32_t value = *((uint32_t*)&threshold[i][j*4]);
//	     // tempArray[i][j] = value;
//	    // flash_err = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, THRESHOLD_FLASH_ARRAY_START_ADDR + ((i * (THRESHOLD_COLUMN_SIZE/4) + j) * 4), tempArray[i][j]);
//	     flash_err = HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, THRESHOLD_FLASH_ARRAY_START_ADDR + (i * (THRESHOLD_COLUMN_SIZE) + j), threshold[i][j]);
//	    }
//	  }
	 // HAL_Delay(1000);
//	HAL_FLASH_Lock();
}

