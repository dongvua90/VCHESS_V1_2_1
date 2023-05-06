#include <QueueData.h>

uint8_t 	qfront = 0;
int8_t 		qrear =-1;
uint8_t 	qcount =0;

/* trả về rear mà sau đó sẽ push data vào nó ( data[rear] ) */
int8_t QueueGetRear()
{
	if (qcount == QUEUE_MAX)  // nếu queue đầy thì xóa phần tử đầu front
	{
		qfront++;						 // tang vi tri phan dau tien len, neu dang o Max-1 thi ve 0
		if(qfront ==QUEUE_MAX) qfront=0;
		qcount--;
	}
	//tang Rear len va gan phan tu vao, Neu Rear dang o vi tri Max-1 thi tang ve vi tri 0
	qrear++;
	if(qrear==QUEUE_MAX) qrear=0;
	qcount++; //tang so phan tu len
	return qrear;
}
/* trả về front, mà sau đó sẽ lấy data ra ( data[front] ) */
int8_t QueueGetFront()
{
	return qfront;

}
/* sau khi lấy dữ liệu từ front thì cần thay đổi qfront và qcount  */
void QueueGetFrontAfter()
{
	qfront++;
	if(qfront==QUEUE_MAX)qfront = 0; // tang vi tri phan dau tien len, neu dang o Max-1 thi ve 0
	qcount--;//giam so phan tu xuong
}

uint8_t QueueGetCount()
{
	return qcount;
}
void QueueClear()
{
	qfront=0;
	qrear=-1;
	qcount=0;
}
