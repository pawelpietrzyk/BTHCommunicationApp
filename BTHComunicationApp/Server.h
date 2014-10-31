#pragma once


class Server
{
public:
	Server(void);
	~Server(void);
	bool OnInit(Object _address);	
	bool OnConnect(Object _address);
	void OnServiceRun(void);
	void OnStop();

private:
	void initServer(void);
	void setupServerSocket(void);
	void initListenSocket(void);
	void bindListenSocket(void);
	void setWSAService(void);
	void waitForAccept(void);
};

