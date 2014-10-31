#pragma once
#include "stdafx.h"
#include <WinSock2.h>

class Connection
{
public:
	bool OnInit(Object _address);	
	bool OnConnect(Object _address);
	void OnServiceRun(void);
	void OnStop();

	Connection(void);
	~Connection(void);	
	void InitWSA(void);
	void PrintWSAError(void);
	void InitConnection(void);
	void BindServerSocket(SOCKET);

	void SetHandler(HWND _h)
	{
		this->hWnd = _h;
	}
public:
	SOCKET			listenSOCKET;
	SOCKET			clientSOCKET;	
	HWND			hWnd;
	LPWSAQUERYSET	wsaQuerySet;
private:
	void sendCloseError(void);
private:
	int				iLastError;
	bool			ifwsaInit;
	bool			ifStarted;
	bool			ifSetup;
	GUID			mGuid;
};
