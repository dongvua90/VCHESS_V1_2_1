/*
 * ScanPieces.c
 *
 *  Created on: Sep 17, 2022
 *      Author: goku
 */
#include "ScanPieces.h"

//										0     1    2     3     4     5     6     7     8     9    10     11   12     13    14     15     16    17
const uint16_t ListFrequency[18]	={  90,  96,  103,  110,  118,  130,  140,  151,  165,  180,  200,  220,  242,  265,  295,   320,   345,  370};
//const uint8_t ListLengthOut[18]		={  10,	 10,  10,	10,	  10,	10,	  10,	15,	  10,	10,	  7,	7,	  7,	 7,	    7,	  7,	 7,	   7};
// gia tri period chuan :       	        933   873   813   763   713   648   602   556   510   466   421   381   347   317   285   262   243   227
const uint16_t mark_period[19] 		= {  950,  903,  828,  788,  738,  681,  625,  570,  533,  480,  430,  401,  364,  332,  301,  273,  253,  235,  200};
//                                   	  K      Q     B    N     R     k     q     b     n     r     p1    p2    p3    p4    P1    P2    P3    P4
//										  0		 1     2    3     4     5     6     7     8     9     10    11    12    13    14    15    16    17
const uint16_t ListIC_measure[18]  	={25000,24000,22000,20000,20000,18000,15000,14000,18000,10000, 9000, 8000, 8000 ,8000 ,8000  ,8000  ,8000 ,8000}; // new


extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;

extern bool chessclock_interrup;

uint16_t point_start_measure; 		// biến xác định điểm bắt đầu đo đạc PWM
bool is_start_measure = false;  	// flag xác định điểm bắt đầu đo PWM
uint16_t datapieces[18][64][2];  	// 18-type 	64-square	2-period+length
uint8_t datamain[64];				// chứa dữ liệu chính của pieces
uint8_t datamain_old[64];			// chứa dữ liệu cũ để tìm sự khác biêt FEN

//pwmdata là duty của pwm được truyền qua DMA
uint16_t pwmdata[40];	// dùng cho timer 16bit
uint32_t pwmdata32[40]; // dùng cho timer 32bit
uint16_t inputCapture_data[IC_MAX]; // chứa dữ liệu InputPwm
uint16_t startmeasure,stopmeasure;
uint8_t responce_length;
uint16_t responce_period;


/* hàm callback khi phát pulse hoàn thành */
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim){
	if(is_start_measure ==true){
		point_start_measure = TIM4->CNT;  // điểm bắt đầu đo đạc là giá trị TIM4->CNT hiện tại
	}
}

void ScanPiecesInit()
{
	/*	mặc định kênh TIM_CCxN bị tắt, cần phải bật nó lên để có thể xuất xung PWM trên nó	*/
	TIM_CCxChannelCmd(htim1.Instance, TIM_CHANNEL_3, TIM_CCxN_ENABLE);
	TIM_CCxChannelCmd(htim1.Instance, TIM_CHANNEL_2, TIM_CCxN_ENABLE);
	HAL_TIM_Base_Start_IT(&htim4);
}
// độ dài tối đa của length=20
void PulseOut(uint8_t type,uint8_t length,uint8_t outCoil)
{
	uint16_t _length = length;
	uint16_t period = 84000/ListFrequency[type]; // tính toán chu kỳ
	uint16_t duty	 = period/2;
	for(int i=0;i<_length;i++){
			 pwmdata[i] = duty;
			 pwmdata32[i] = (uint32_t)duty;
	}
	pwmdata[_length] =0;
	pwmdata32[_length]=0;
	switch(outCoil){
	case 1:	TIM3->ARR = period; 			HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_1,(uint32_t *)pwmdata, _length+1); 	break;
	case 2:	TIM2->ARR = (uint32_t)period; 	HAL_TIM_PWM_Start_DMA(&htim2, TIM_CHANNEL_2,(uint32_t *)pwmdata32, _length+1); 	break;
	case 3:	TIM1->ARR = period; 			HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_1,(uint32_t *)pwmdata, _length+1); 	break;
	case 4: TIM1->ARR = period; 			HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_3,(uint32_t *)pwmdata, _length+1); 	break;
	case 5:	TIM1->ARR = period; 			HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_2,(uint32_t *)pwmdata, _length+1); 	break;
	case 6:	TIM2->ARR = (uint32_t)period; 	HAL_TIM_PWM_Start_DMA(&htim2, TIM_CHANNEL_3,(uint32_t *)pwmdata32, _length+1); 	break;
	case 7:	TIM3->ARR = period; 			HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_4,(uint32_t *)pwmdata, _length+1); 	break;
	case 8:	TIM3->ARR = period; 			HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_3,(uint32_t *)pwmdata, _length+1); 	break;
	}
}
void SelectReadCoil(uint8_t readCoil)
{
	switch(readCoil){
		case 1: CD4051_A_1; CD4051_B_0; CD4051_C_1;break; //coil_1 <-> X5
		case 2: CD4051_A_1; CD4051_B_1; CD4051_C_1;break; //coil_2 <-> X7
		case 3: CD4051_A_0; CD4051_B_1; CD4051_C_1;break; //coil_3 <-> X6
		case 4: CD4051_A_0; CD4051_B_0; CD4051_C_1;break; //coil_4 <-> X4
		case 5: CD4051_A_0; CD4051_B_1; CD4051_C_0;break; //coil_5 <-> X2
		case 6: CD4051_A_1; CD4051_B_0; CD4051_C_0;break; //coil_6 <-> X1
		case 7: CD4051_A_0; CD4051_B_0; CD4051_C_0;break; //coil_7 <-> X0
		case 8: CD4051_A_1; CD4051_B_1; CD4051_C_0;break; //coil_8 <-> X3
	}
}
void StartInputPWM(uint8_t type)
{
	is_start_measure = true;  															// bật flag measure
	memset(inputCapture_data,0,sizeof(inputCapture_data)); 								// clear ic_data
	TIM4->ARR = ListIC_measure[type];  													// xác định khoảng đo tối đa
	HAL_TIM_IC_Start_DMA(&htim4, TIM_CHANNEL_2,(uint32_t*)inputCapture_data, IC_MAX);  	// giá trị đo đạc lưu vào ic_data
}
void Measure(uint8_t type,uint8_t outCoil,uint8_t readCoil)
{
	PulseOut(type,10, outCoil);
	SelectReadCoil(readCoil);
	StartInputPWM(type);
	startmeasure=0;
	stopmeasure=0;
	responce_length=0;
	responce_period=0;
	uint8_t sq = FileRankToSquare(outCoil, readCoil);

	while(is_start_measure){};  // chờ cho quá trình đo PWM hoàn thành

	/* Pulse input được đo từ lúc phát PulseOut đến timesIC_measure[tupe]
	 * để xác định số Pulses ta chỉ tính Pulse phản hồi (từ lúc phát Pulse kết thúc) đến giới hạn thời gian đo */

	// xác định điểm đầu Measure
	for(int i=0;i<IC_MAX;i++){
		if(inputCapture_data[i] >point_start_measure){
			startmeasure=i;
			break;
		}else if(inputCapture_data[i]==0){ // nếu ko có xung nào nhận được
			responce_length=0;
			responce_period=0;
			datapieces[type][sq][PULSES]=0;
			datapieces[type][sq][PERIOD]=0;
			return;
		}
	}

	// xác định điểm cuối Measure
	for(int i=startmeasure;i<IC_MAX-1;i++){
		stopmeasure=i;
		if(inputCapture_data[i+1] < inputCapture_data[i])break;
	}
	if(inputCapture_data[IC_MAX-1]!=0) stopmeasure=IC_MAX;

	// tính toán số lượng Pulses phản hồi
	responce_length = stopmeasure-startmeasure;

	// tính period phản hồi
	if(responce_length>=5){
		responce_period = (inputCapture_data[startmeasure+5] - inputCapture_data[startmeasure]) / 5;
	}else if(responce_length>3){
		responce_period = (inputCapture_data[startmeasure+responce_length-1]-inputCapture_data[startmeasure])/(responce_length-1);
	}
	// đặt số xung phản hồi vào từng ô
	datapieces[type][sq][PULSES]=responce_length;
	datapieces[type][sq][PERIOD]=responce_period;
}
/* hàm quét Pieces
 * Return 0: quá trình quét hoàn tất và không phát hiện sự thay đổi của FEN
 * Return 1: quá trình quét hoàn tất và phát hiện có sự thay đổi FEN
 * Return 2: quá trình quét có xảy ra ChessClock Side Thay đổi */
uint8_t Scan()
{
	bool isChessclockInterrupt = false;
	CHESSCLOCKINTERRUPT:
	memset(datamain,0,sizeof(datamain)); // clear data
	for(int type=17;type>=0;type--){
		for(int outCoil=1;outCoil<=8;outCoil++){
			for(int readCoil=1;readCoil<=8;readCoil++){

				if(chessclock_interrup==true){
					chessclock_interrup=false;
					isChessclockInterrupt = true;
					goto CHESSCLOCKINTERRUPT; // tiến hành quét lại từ đầu
				}

				uint8_t sq = FileRankToSquare(outCoil, readCoil);
				if(datamain[sq]==0){  // nếu ô quét chưa thấy piece nào thì tiến hành đo đạc
					Measure(type,outCoil,readCoil);
				}
			}
		}
		// chọn piece mạnh nhất
		SQData firstMax,secondMax;
		FindMax(type, &firstMax, &secondMax);

		// WKING & BKING chỉ chọn 1 giá trị lớn nhất
		if(type == 0 || type==5){
			if(firstMax.square >=0){
				datamain[firstMax.square] = type+1;
			}
		}else{
			if(firstMax.square >=0){
				datamain[firstMax.square] = type+1;
			}
			if(secondMax.square >=0){
				datamain[secondMax.square] = type+1;
			}
		}
	}
	if(DetectFenChange()) return 1;
	if(isChessclockInterrupt) return 2;
	return 0;
}


// file:1->8 rank:1->8 (file=outCoil , rank=readCoil)
uint8_t FileRankToSquare(uint8_t file,uint8_t rank)
{
	return (8-file)*8 + rank-1;
}

/* Tìm sự thay đổi của FEN */
bool DetectFenChange()
{
	bool resuft=false;
	for(int i=0;i<64;i++){
		if(datamain[i] != datamain_old[i]){
			resuft = true;
			datamain_old[i] = datamain[i];
		}
	}
	return resuft;
}


void FindMax(uint8_t type, SQData *sqFirstMax,SQData *sqSecondMax)
{
	// hàm tìm ra 2 ô có tín hiệu trong khoảng tần số và và số xung phản hồi đạt ngưỡng
	sqFirstMax->pulses	= -1;	sqFirstMax->square 	= -1;
	sqSecondMax->pulses = -2;	sqSecondMax->square = -2;
	for(int sq=0;sq<64;sq++){
		// số xung phải lớn hơn ngưỡng calibase
		if(datapieces[type][sq][PULSES] >= getMarkSq(type, sq))
		{	// period phải nằm trong khoảng của type piece
			if(datapieces[type][sq][PERIOD] <= mark_period[type] && datapieces[type][sq][PERIOD] >mark_period[type+1])
			{	// nếu NewValue >= FirstMax thì FirstMax = NewValue và SecondMax = FirstMax
				if(datapieces[type][sq][PULSES] >= sqFirstMax->pulses){
					sqSecondMax->pulses = sqFirstMax->pulses;
					sqSecondMax->square = sqFirstMax->square;
					sqFirstMax->pulses = datapieces[type][sq][PULSES];
					sqFirstMax->square = sq;
				}else if(datapieces[type][sq][PULSES] > sqSecondMax->pulses){ // nếu NewValue Chỉ > SecondMax thì SecondMax=NewValue
					sqSecondMax->pulses = datapieces[type][sq][PULSES];
					sqSecondMax->square = sq;
				}
			}
		}
	}
}


