#include "StdAfx.h"
#include "BTHReadService.h"

BTHReadService::BTHReadService(SOCKET *sock)
{
	this->bthSocket = sock;
}
BTHReadService::BTHReadService(BTHConnection *connection)
{
	this->bthConnection = connection;	
	this->bthSocket = &(this->bthConnection->ClientSOCKET);	
	this->hWnd = this->bthConnection->hWnd;	
	onInit();
}

BTHReadService::~BTHReadService()
{
	//int ir;
	//ir = shutdown(*bthSocket, SD_BOTH);
	//if (ir == SOCKET_ERROR) {

	//}
	//closesocket(*bthSocket);
	//this->bthConnection = 0;
	//this->bthSocket = 0;
}

int BTHReadService::Read()
{
	int iResult = 0;
	ByteBuffer* buffer;
	//Buffer buffer;
	state = BTHReadService::STATE_READING;	
	onStartReading();
	char recvbuf[BUFLEN] = "";
	do {
		
		iResult = recv(*bthSocket, recvbuf, (int)BUFLEN, 0);
		if (iResult > 0) 
		{
			buffer = new ByteBuffer((byte*)recvbuf, iResult);		
			//buffer.mBuff = recvbuf;
			//buffer.mLenght = iResult;

			onRead(buffer);			
		}
		else
		{
			if (iResult == 0)
			{
				onStopReading();			
				return 1;	
			}
			else 
			{		
				onErrorReading();
				return -1;
			}
		}
		
	} while (state == BTHReadService::STATE_READING);	
	onStopReading();
	return 0;
}

