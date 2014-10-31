#pragma once

class Command
{
public:
	Command(void);
	Command(char* _type, char* _cmd);	
	~Command(void);
	void setCmdType(char* _cmdType) { this->mCmdType = _cmdType; };
	void setCmd(char* _cmd) { this->mCmd = _cmd; };
	char* getCmd(void) { return this->mCmd; };
	char* getCmdType(void) { return this->mCmdType;	};	

private:
	char* mCmdType;
	char* mCmd;

};

class Protocol
{
public:
	Protocol(void);
	~Protocol(void);
	char* packCommand(Command* _cmdObj, int &count);
	Command* unpackCommand(char* _buffer);
static char* respOK()
{
	char* ch = "TYPE:OK;";
	return ch;
}
	

};




