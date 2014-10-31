#include "StdAfx.h"
#include "BTHWriteService.h"

BTHWriteService::BTHWriteService(SOCKET *sock)
{
	this->bthSocket = sock;
}
BTHWriteService::BTHWriteService(BTHConnection *connection)
{
	this->bthConnection = connection;	
	this->bthSocket = &(this->bthConnection->ClientSOCKET);	
	this->hWnd = this->bthConnection->hWnd;
	this->onInit();	
	state = BTHWriteService::STATE_READY;
}


BTHWriteService::~BTHWriteService()
{
	//int ir;
	//ir = shutdown(*bthSocket, SD_BOTH);
	//if (ir == SOCKET_ERROR) {

	//}
	//closesocket(*bthSocket);
	//this->bthConnection = 0;
	//this->bthSocket = 0;
}

int BTHWriteService::Write(char* writeBuf, int lenght)
{
	int result = 0;
	//char* buffer = (char*)writeBuf;
//	char b[] = *buffer;
	//int lenght = strlen((char*)writeBuf);
	state = BTHWriteService::STATE_WRITING;
	result = send(*bthSocket, writeBuf, lenght, 0);
	if (result == SOCKET_ERROR) {		
		closesocket(*bthSocket);
		return -1;
	}		
	return 0;
}
int BTHWriteService::Write(byte* writeBuf, int lenght)
{
	int result = 0;
	//char* buffer = (char*)writeBuf;
//	char b[] = *buffer;
	//int lenght = strlen((char*)writeBuf);
	state = BTHWriteService::STATE_WRITING;
	result = send(*bthSocket, (char*)writeBuf, lenght, 0);
	if (result == SOCKET_ERROR) {		
		closesocket(*bthSocket);
		return -1;
	}		
	return 0;
}
int BTHWriteService::Write(ByteBuffer* writeBuf)
{
	int result = 0;
	//char* buffer = (char*)writeBuf;
//	char b[] = *buffer;
	//int lenght = strlen((char*)writeBuf);
	state = BTHWriteService::STATE_WRITING;
	result = send(*bthSocket, (char*)writeBuf->getArray(), writeBuf->getCapacity(), 0);
	if (result == SOCKET_ERROR) {		
		closesocket(*bthSocket);
		return -1;
	}		
	return 0;
}


