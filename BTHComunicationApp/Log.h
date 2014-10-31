#pragma once
#define FILEPATH "eLog"
class Log
{
public:
	Log(void);
	~Log(void);
	
	static void Write(char* buf);
	static char* Time(void);
};
