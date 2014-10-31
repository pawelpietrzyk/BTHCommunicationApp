#include "StdAfx.h"
#include "Log.h"

Log::Log(void)
{
}

Log::~Log(void)
{
}

void Log::Write(char* buffer)
{
	DWORD  dwBytesWritten, dwPos;
	HANDLE hLog;
	//char *buff = (char*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, 1024);

	hLog = CreateFile(
		TEXT(FILEPATH),
		FILE_APPEND_DATA,
		FILE_SHARE_READ,
		NULL,
		OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (hLog == INVALID_HANDLE_VALUE) {
		//InsertItem(hListView, L"Error during writin");
		return;
	}
	//int cc = LPCSTRtoCHARA(text, buff);
	int cc;
	//buff = 0buffer;
	//sprintf_s(buff, 1024, "%s| %s", Log::Time(), buffer);
	cc = strlen(buffer);
	LONG hord;
	dwPos = SetFilePointer(hLog, 0, NULL, FILE_END);
    LockFile(hLog, dwPos, 0, cc, 0);
    WriteFile(hLog, buffer, cc, &dwBytesWritten, NULL);
    UnlockFile(hLog, dwPos, 0, cc, 0);
	CloseHandle(hLog);
	//HeapFree(GetProcessHeap(), NULL, (LPVOID)buff);
	
}

char* Log::Time()
{
	char ctime[26] = "";
	time_t rawtime;
	struct tm* timeinfo;
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	strftime(ctime, 26, "%d-%m-%Y %H:%M:%S", timeinfo);
	return ctime;
}
