#include "StdAfx.h"
#include "BTHService.h"


BTHService::BTHService()
{
}

BTHService::BTHService(SOCKET *sock)
{
	this->bthSocket = sock;
}
BTHService::BTHService(BTHConnection *connection)
{
	this->bthConnection = connection;
	this->iswriting = false;
	this->isreading = false;
	this->bthSocket = &(this->bthConnection->ClientSOCKET);	
	this->hWnd = this->bthConnection->hWnd;
	
}

BTHService::BTHService(BTHConnection *connection, bool startRead)
{
	this->bthConnection = connection;
	this->iswriting = false;
	this->isreading = false;
	this->bthSocket = &(this->bthConnection->ClientSOCKET);	
	this->hWnd = this->bthConnection->hWnd;
	this->reading = false;
	
	onInit();
	if (startRead) {
		this->type = 1;
		//this->Read();
		//onStopReading();
	} else {
		type = 0;
	}
}
BTHService::BTHService(BTHConnection *c, int ToSvc)
{
	this->bthConnection = c;
	this->iswriting = false;
	this->isreading = false;
	this->bthSocket = &(this->bthConnection->ClientSOCKET);	
	this->hWnd = this->bthConnection->hWnd;	
	this->type = ToSvc;
	onInit();
}
BTHService::~BTHService()
{
	//int ir;
	//ir = shutdown(*bthSocket, SD_BOTH);
	//if (ir == SOCKET_ERROR) {

	//}
	//closesocket(*bthSocket);
	//this->bthConnection = 0;
	//this->bthSocket = 0;
}

int BTHService::Read()
{
	int iResult = 0;
	this->reading = true;
	this->isreading = true;
	this->onStartReading();
	do {
		char recvbuf[BUFLEN] = "";
		iResult = recv(*bthSocket, recvbuf, (int)BUFLEN, 0);
		if (iResult > 0) {
			if (!this->iswriting) {
				//this->DecodeFrame(recvbuf, iResult);
				//this->OnRead(recvbuf, iResult);				
				
				this->onReadTxt((byte*)recvbuf);
			}
		} else {
			//this->clientEXIT();
			//onStopReading();
			reading = false;
			//return 1;			
		}
	} while (this->reading);
	this->isreading = false;
	this->onStopReading();
	return 0;
}

int BTHService::Write(byte* writeBuf)
{
	int result = 0;
	//char buffer[1024] = (char*)writeBuf;
	int lenght = strlen((char*)writeBuf);
	this->iswriting = true;	
	result = send(*bthSocket, (char*)writeBuf, lenght, 0);
	if (result == SOCKET_ERROR) {
		this->bthConnection->eLog(L"Error during writing");
		this->bthConnection->PrintWSAError();
		closesocket(*bthSocket);
		return 1;
	}	
	this->iswriting = false;
	return 0;
}

