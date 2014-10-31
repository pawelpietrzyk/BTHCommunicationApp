#include "StdAfx.h"
#include "Protocol.h"

Protocol::Protocol(void)
{
}

Protocol::~Protocol(void)
{
}

char* Protocol::packCommand(Command* _cmdObj, int &count)
{
	
	if (_cmdObj != 0)
	{
		char *buffer = (char*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, 200);

		sprintf_s(buffer, 200, "|%s;%s|", _cmdObj->getCmdType(), _cmdObj->getCmd());

		count = strlen(buffer);
		return buffer;
	}
	return 0;	
}

Command* Protocol::unpackCommand(char *_buffer)
{
	int i;
	bool in = false;
	char* str1;
	char* str2;
	for (i = 0; _buffer[i] != 0; i++)
	{
		if ((_buffer[i] == '|') && (in == true))
		{
			in = false;
		}
		if ((_buffer[i] == '|') && (in == false))
		{
			in = true;
		}
	}
	return 0;
}

Command::Command(char *_type, char *_cmd)
{
	this->mCmd = _cmd;
	this->mCmdType = _type;
}

