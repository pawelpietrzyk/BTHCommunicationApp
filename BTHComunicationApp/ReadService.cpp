#include "StdAfx.h"
#include "ReadService.h"

ReadService::ReadService(void)
{
}

ReadService::~ReadService(void)
{
}
void ReadService::SetState(int _state)
{
	mState = _state;
	onStateChange(mState);
}
int ReadService::GetState()
{
	return mState;
}


int ReadService::Read()
{
	int iResult = 0;
	ByteBuffer* buffer;		
	SetState(ReadService::STATE_READING);	
	char recvbuf[BUFLEN] = "";
	do {		
		iResult = recv(*mSocket, recvbuf, (int)BUFLEN, 0);
		if (iResult > 0) 
		{
			buffer = new ByteBuffer((byte*)recvbuf, iResult);		
			onRead(buffer);			
		}
		else
		{
			if (iResult == 0)
			{				
				SetState(ReadService::MSG_READ_STOP);	
				return 1;	
			}
			else 
			{	
				SetState(ReadService::STATE_ERROR_READING);				
				return -1;
			}
		}
		
	} while (mState == ReadService::STATE_READING);	
	SetState(ReadService::MSG_READ_STOP);
	return 0;
}

