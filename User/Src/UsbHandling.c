/*
 * usbHandling.c
 *
 *  Created on: Aug 26, 2022
 *      Author: goku
 */
#include <UsbHandling.h>
#include "stdbool.h"
#include "usbd_cdc_if.h"
#include "cmsis_os.h"
#include "MB85RS256.h"

uint8_t dataUsb_rxlength=0;
uint8_t dataUsb_rx[64];

extern osSemaphoreId binarySemGetFRAMHandle;
extern uint8_t flag_masterGetData;
/*
#ifdef CALIBASE
	uint8_t calibase_type=0;  // đặt type piece trong chế độ calibase
	bool isCalibaseMode = false;
	uint8_t dataUsbCalibase[257];
	extern uint16_t datapieces[18][64][2];  	// 18-type 	64-square	2-period+length
	extern uint8_t datamain[64];				// chứa dữ liệu chính của pieces
	extern RTC_HandleTypeDef hrtc;
	extern USBD_HandleTypeDef hUsbDeviceFS;
#endif*/
// hàm xử lý data nhận từ USB
void HandlingUsb(){

	/*if(dataUsb_rx[0]==CMD_SCAN_DEVICE || dataUsb_rx[0]==CMD_GET_DATA || dataUsb_rx[0]==CMD_FRAM_READ){  // Scan Code
		uint32_t addr = ByteToAddress(dataUsb_rx[1], dataUsb_rx[2], dataUsb_rx[3]);
		if(addr==ADDRESSBOARD){
			if(dataUsb_rx[0]==CMD_SCAN_DEVICE){
				UsbSendAddress();
			}else if(dataUsb_rx[0]==CMD_GET_DATA){
				flag_masterGetData = 2; 				//interface: 1=RS485 2=USB
			}else if(dataUsb_rx[0]==CMD_FRAM_READ){
				osSemaphoreRelease(binarySemGetFRAMHandle);
			}
		}
	}
#ifdef CALIBASE
	else if(dataUsb_rx[0]==CMD_ENABLE_CALIBASE_MODE){ // Enable Calibase Mode
		isCalibaseMode = true;
	}else if(dataUsb_rx[0]==CMD_CALIBASE_SET_TYPE){  // Calibase Code: 'C' + 'Type'
		calibase_type = dataUsb_rx[1];
	}else if(dataUsb_rx[0]==CMD_SET_REAL_TIME){
		uint8_t datatime[6];
		for(int i=0;i<6;i++){
			datatime[i] = dataUsb_rx[i+1];
		}
		RTC_SetTime(datatime);
	}else if(dataUsb_rx[0]==CMD_READ_REAL_TIME){
		UsbSendDateTime();
	}else if(dataUsb_rx[0]==CMD_FRAM_ERASE){
		MB85RS_erase();
	}
#endif
	*/
}
/*
void UsbSendAddress()
{
	uint8_t res[5];
	res[0] = ADDR_PART0;
	res[1] = ADDR_PART1;
	res[2] = ADDR_PART2;
	res[3] = 'U';	// interface is USB
	res[4] = NEWLINE;
	CDC_Transmit_FS(res,5);
}*/


/*
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

*/

