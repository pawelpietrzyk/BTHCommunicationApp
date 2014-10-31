#include "StdAfx.h"
#include "Service.h"

ComService::ComService(void)
{
}

ComService::~ComService(void)
{
}


int ComService::read()
{
	//HANDLE hEvent = OpenEvent(SYNCHRONIZE, FALSE, L"TCP_READ_CONTINUE");	
	int iResult = 0;
	ByteBuffer* buffer;		
	setState(ComService::STATE_READING);	
	char recvbuf[BUFLEN] = "";
	do {		
		iResult = recv(*mSocket, recvbuf, (int)BUFLEN, 0);
		if (iResult > 0) 
		{
			if ((iResult == 1) && (recvbuf[0] == 0))
			{
				iResult = send(*mSocket, recvbuf, iResult, 0);
				if (iResult == 1)
				{
					setState(ComService::MSG_READ_STOP);	
					return 1;	
				}

			}
			buffer = new ByteBuffer((byte*)recvbuf, iResult);		
			onRead(buffer);			
		}
		else
		{
			if (iResult == 0)
			{				
				setState(ComService::MSG_READ_STOP);	
				return 1;	
			}
			else 
			{	
				setState(ComService::STATE_ERROR_READING);				
				return -1;
			}
		}
		//WaitForSingleObject(hEvent, INFINITE);
	} while (mState == ComService::STATE_READING);	
	setState(ComService::MSG_READ_STOP);
	return 0;
}

int ComService::write(ByteBuffer* writeBuf)
{
	int result = 0;	
	
	//setState(ComService::STATE_WRITING);
	result = send(*mSocket, (char*)writeBuf->getArray(), writeBuf->getCapacity(), 0);
	if (result == SOCKET_ERROR) {		
		closesocket(*mSocket);
		//setState(ComService::STATE_READY);
		return -1;
	}		
	//setState(ComService::STATE_READY);
	return 0;
}

