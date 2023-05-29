#include "ScanPieces.h"

//										0		1		2		3		4		5		6		7		8		9		10		11		12		13		14		15		16		17
const uint16_t ListFrequency[18]	={  90,		96,  	103,  	110,  	118,  	130,  	140,  	151,  	165,  	180,  	200,  	220,  	242,  	265,  	295,   	320,   	345,  	370};
const uint8_t ListLengthOut[18]		={  10,	 	10,  	10,		10,	  	10,		10,	  	6,		5,	  	5,		7,	   	6,	 	8,    	8,	 	10,    	10,	  	10,	  	12,   	12};
// gia tri period chuan :       	 		933  	873   	813   	763   	713   	648   	602   	556   	510   	466   	421   	381   	347   	317   	285   	262   	243   	227
const uint16_t mark_period_max[18] 	={	950,  	903,  	843,  	788,  	738,  	681,  	620,  	570,  	533,  	480,  	430,  	401,  	364,  	327,  	301,  	283,  	253,  	230};
const uint16_t mark_period_min[18] 	={	903,  	843,  	788,  	738,  	681,  	620,  	570,  	533,  	480,  	430,  	401,  	364,  	327,  	301,  	240,  	220,  	200,  	180};
//                                   	K      	Q     	B    	N     	R     	k     	q     	b     	n     	r     	p1    	p2    	p3    	p4    	P1    	P2    	P3    	P4
const uint16_t ListIC_measure[18]  	={	37000,	43000,	34000,	32000,	25000,	25000,	24000,	25000,	32000,	28000, 	25000,	15000,	15000,	14000,	11000,	10000,	10000,	10000};
const uint8_t offsetMeasure[18]		={	1,		5,		5,		5,		5,		1,		5,		5,		5,		5,		5,		5,		5,		5,		5,		5,		5,		5};
const uint8_t sttscan[18] 			={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17}; // stt scan piece type
const uint8_t sttInOut[8] 			={1,4,7,2,5,8,3,6};								// stt output pulse & input coil

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
extern bool chessclock_interrup;

volatile uint16_t point_start_measure; 		// biến xác định điểm bắt đầu đo đạc PWM
volatile bool is_start_measure = false;  	// flag xác định điểm bắt đầu đo PWM
uint16_t datapieces[18][64][2];  			// 18-type 	64-square	2-period+length
uint8_t datamain[64];						// chứa dữ liệu chính của pieces
uint8_t datamain_old[64];					// chứa dữ liệu cũ để tìm sự khác biêt FEN

//pwmdata là duty của pwm được truyền qua DMA
uint16_t pwmdata[40];	// dùng cho timer 16bit
uint32_t pwmdata32[40]; // dùng cho timer 32bit
uint16_t inputCapture_data[IC_MAX]; // chứa dữ liệu InputPwm
uint16_t startmeasure,stopmeasure;
uint8_t  responce_length;
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
	for(int i=0;i<_length;i++)
	{
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


void Measure(uint8_t type,uint8_t outCoil,uint8_t readCoil)
{
	/* Phát xung và thiết lập Cuộn cảm nhận tín hiệu */
	PulseOut(type,ListLengthOut[type], outCoil);
	SelectReadCoil(readCoil);
	/* Đưa các biến sử dụng trong đo đạc về giá trị ban đầu */
	is_start_measure = true;
	point_start_measure=0;
	startmeasure=0;
	stopmeasure=0;
	responce_length=0;
	responce_period=0;
	for(int i=0;i<IC_MAX;i++){
		inputCapture_data[i]=0;	// Clear ic_data
	}
	uint8_t sq = FileRankToSquare(outCoil, readCoil);
	bool firstcheck=true;
	uint16_t period = 84000/ListFrequency[type]; 	// tính toán chu kỳ

	/*	Thiết lập Timer để đo đạc tín hiệu trả về	*/
	TIM4->ARR = ListIC_measure[type];	// xác định khoảng đo tối đa
	HAL_TIM_IC_Start_DMA(&htim4, TIM_CHANNEL_2,(uint32_t*)inputCapture_data, IC_MAX);  	// giá trị đo đạc lưu vào ic_data

    /* chờ cho quá trình đo PWM hoàn thành */
	while(is_start_measure){
		/*	Kiểm tra liên tục xem có xung phản hồi hay không? nếu không thì dừng quá trình đo lại luôn để tiết kiệm thời gian
		 * khi quét ô cuối cùng thì không bỏ qua quá trình scan vì nó sẽ gây nhiễu đến piece type sau đó */
		if(firstcheck && outCoil!=sttInOut[7] && readCoil!=sttInOut[7]){
			if(TIM4->CNT > (period*18)){
				if(inputCapture_data[10]==0){
					/* Nếu không có xung phản hồi thì reset biến và đặt lại timer */
					firstcheck=false;
					responce_length=0;
					responce_period=0;
					datapieces[type][sq][PULSES]=0;
					datapieces[type][sq][PERIOD]=0;
					TIM4->CNT=0;
					HAL_TIM_IC_Stop_DMA(&htim4, TIM_CHANNEL_2);
					is_start_measure=false;
					return;
				}
			}
		}

	};

	/* Pulse input được đo từ lúc phát PulseOut đến timesIC_measure[tupe]
	 * để xác định số Pulses ta chỉ tính Pulse phản hồi (từ lúc phát Pulse kết thúc) đến giới hạn thời gian đo
	 *
	 * 	  điểm bắt đầu Measure ⏤⏤⏤⏤┓						    ┎⏤⏤⏤⏤⏤⏤ điểm kết thúc Measure
	 * 	    						│					        │
	 * 	PulseOut:	___⎍⎍⎍⎍⎍⎍⎍⎍⎍⎍⎍⎍⎍⎍⎍
	 * 	ReadCoil:	__________⎍⎍⎍⎍⎍⎍⎍⎍⎍⎍⎍⎍⎍⎍⎍⎍⎍⎍⎍⎍⎍⎍⎍⎍⎍⎍⎍⎍⎍⎍⎍⎍⎍⎍⎍⎍⎍⎍⎍_
	 * 								│___││___│
	 * 							   	  │    │
	 * 	bỏ cách 1 khoảng trước khi đo ┘	   │
	 * 	sử dụng 1 khoảng để đo tần số ⏤⏤⏤⏤┘
	 *
	 *  */

	/* xác định điểm đầu Measure */
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

	/* xác định điểm cuối Measure */
	for(int i=startmeasure;i<IC_MAX-1;i++){
		stopmeasure=i;
		if(inputCapture_data[i+1] < inputCapture_data[i])break;
	}
	if(inputCapture_data[IC_MAX-1]!=0) stopmeasure=IC_MAX;

	/* tính toán số lượng Pulses phản hồi */
	responce_length = stopmeasure-startmeasure;

	// tính period phản hồi
	if(responce_length > offsetMeasure[type]+RANGE_MEASURE){
		responce_period = (inputCapture_data[startmeasure+offsetMeasure[type]+RANGE_MEASURE] - inputCapture_data[startmeasure + offsetMeasure[type]]) / RANGE_MEASURE;
	}
#ifdef ONLYTYPE
	if(responce_period <= mark_period_max[type] && responce_period >mark_period_min[type]){
		datapieces[type][sq][PULSES]=responce_length;
		datapieces[type][sq][PERIOD]=responce_period;
	}else{
		datapieces[type][sq][PULSES]=0;
		datapieces[type][sq][PERIOD]=0;
	}
#else
	datapieces[type][sq][PULSES]=responce_length;
	datapieces[type][sq][PERIOD]=responce_period;
#endif
}
/* hàm quét Pieces
 * Return 0: quá trình quét hoàn tất và không phát hiện sự thay đổi của FEN
 * Return 1: quá trình quét hoàn tất và phát hiện có sự thay đổi FEN
 * Return 2: quá trình quét có xảy ra ChessClock Side Thay đổi
 *  enableClockSideInterrupt: có cho phép return lại khi chessclock thay đổi hay không */
SCANRESULT Scan(bool enableClockSideInterrup)
{
	/* Reset lại các biến */
	for(int i=0;i<64;i++){
		datamain[i]=0;
	}
	for(int t=0;t<18;t++){
		for(int s =0;s<64;s++){
			datapieces[t][s][PULSES]=0;
			datapieces[t][s][PERIOD]=0;
		}
	}
	/* Quét tất cả các Piece */
	for(int i=0;i<18;i++){
		int type = sttscan[i];
		int outCoil =0;
		int readCoil =0;
		for(int j=0;j<8;j++){
			readCoil = sttInOut[j];
			for(int k=0;k<8;k++){
				outCoil = sttInOut[k];
				if(chessclock_interrup==true && enableClockSideInterrup==true){
					chessclock_interrup=false;
					return SCAN_SIDE_CHANGED;
				}
				uint8_t sq = FileRankToSquare(outCoil, readCoil);
				if(datamain[sq]==0){  // nếu ô quét chưa thấy piece nào thì tiến hành đo đạc
					Measure(type,outCoil,readCoil);

				}
			}
		}
		// edit new
		int8_t sqDetects[10]={-5,-5,-5,-5,-5,-5,-5,-5,-5,-5}; // mảng các ô có pulseLength > MinValue và có period nằm trong ngưỡng detect
		uint8_t lengthValid =0; // số lượng các ô hợp lệ
		/* tìm các ô có pulseLength và period hợp lệ  */
		for (int8_t sq = 0; sq < 64; sq++) {
			// số xung phải lớn hơn ngưỡng calibase
			if (datapieces[type][sq][PULSES] >= getMarkSq(type, sq)) {// period phải nằm trong khoảng của type piece
				if (datapieces[type][sq][PERIOD] <= mark_period_max[type] && datapieces[type][sq][PERIOD] > mark_period_min[type])
				{
					sqDetects[lengthValid] = sq;
					lengthValid++;
					if(lengthValid > 10) break; // nếu có lớn hơn 10 ô hợp lệ thì break
				}
			}
		}
		/* nếu có 1 ô hợp lệ thì đó là piece  */
		if(lengthValid==1)
		{
			datamain[sqDetects[0]]=type+1;
		}else if(lengthValid > 1)	// tìm ra 2 ô có giá trị lớn nhất
		{
			SQData firstMax,secondMax;
			firstMax.pulses=-1;firstMax.square=-1;
			secondMax.pulses=-2;secondMax.square=-2;

			for(int v=0;v<lengthValid;v++) // lặp lại tất cả các giá trị
			{
				if(datapieces[type][sqDetects[v]][PULSES] >= firstMax.pulses)
				{
					secondMax.pulses = firstMax.pulses;
					secondMax.square = firstMax.square;
					firstMax.pulses  = datapieces[type][sqDetects[v]][PULSES];
					firstMax.square = sqDetects[v];
				}else if(datapieces[type][sqDetects[v]][PULSES] >= secondMax.pulses){
					secondMax.pulses = datapieces[type][sqDetects[v]][PULSES];
					secondMax.square = sqDetects[v];
				}
			}
			/* nếu type là King thì chỉ chọn firstMax là ô có piece */
			/* nếu không thì kiểm tra xem 2 ô có phải là 2 ô liền kề hay không
			 * 4 ô liền kề với ô firstMax là : (sq+1;sq-1;sq+8;sq-8) */
			if(type == 0 || type==5)
			{
				if(firstMax.square >=0)
				{
					datamain[firstMax.square] = type+1;
				}
			}
#ifdef USING_ADJACENT
			else if(secondMax.square==firstMax.square+1 || secondMax.square==firstMax.square-1
					|| secondMax.square==firstMax.square+8 || secondMax.square==firstMax.square-8) // là ô liền kề
			{
				/* kiểm tra giá trị pulses của secondMax có >= firstMax*2/3 hay không
				 * nếu vượt ngưỡng đó thì nó là 1 Piece không thì đó là nhiễu */
				if(datapieces[type][secondMax.square][PULSES] >= datapieces[type][firstMax.square][PULSES]*2/3){
					datamain[firstMax.square]=type+1;
					datamain[secondMax.square]=type+1;
				}else		// nếu không thì chỉ có 1 ô first là piece
				{
					datamain[firstMax.square]=type+1;
				}
			}
#endif
			else	// nếu không phải là 2 ô liền kề thì cả 2 ô đều là Piece
			{
				datamain[firstMax.square] = type+1;
				datamain[secondMax.square] = type+1;
			}
		}
	}
	if(DetectFenChange()) return SCAN_FEN_CHANGE;
	return SCAN_NO_FEN_CHANGE;
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


