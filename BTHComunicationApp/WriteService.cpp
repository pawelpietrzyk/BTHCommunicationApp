#include "StdAfx.h"
#include "WriteService.h"

using namespace comunication;

WriteService::WriteService(void)
{
}

WriteService::~WriteService(void)
{
}


int WriteService::Write(ByteBuffer* writeBuf)
{
	int result = 0;	
	
	setState(WriteService::STATE_WRITING);
	result = send(*mSocket, (char*)writeBuf->getArray(), writeBuf->getCapacity(), 0);
	if (result == SOCKET_ERROR) {		
		closesocket(*mSocket);
		setState(WriteService::STATE_READY);
		return -1;
	}		
	setState(WriteService::STATE_READY);
	return 0;
}