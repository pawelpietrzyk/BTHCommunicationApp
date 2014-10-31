#pragma once

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <Ws2bth.h>
#include <Nspapi.h>
#include "BTHConnection.h"

//typedef ULONGLONG bt_addr, *pbt_addr, BT_ADDR, *PBT_ADDR;

#define BTHSW_EXIT			0x510
#define BTHSW_INIT			0x511
#define BTHSW_WRITE			0x512
#define BUFLEN				512

class BTHWriteService
{
public:
	BTHWriteService(void);
	BTHWriteService(SOCKET *bthSck);
	BTHWriteService(BTHConnection *c);	
	~BTHWriteService(void);	
	int Write(char*, int);
	int Write(byte*, int);	
	int Write(ByteBuffer*);
	int state;
	static const int STATE_WRITING = 1;
	static const int STATE_READY = 0;
	
private:
	BTHConnection *bthConnection;
	HWND hWnd;
	SOCKET *bthSocket;
	
	void onInit() {
		PostMessage(hWnd, WM_USER, NULL, (LPARAM)BTHSW_INIT);
	};
	void onWrite(int _bytes) {
		PostMessage(hWnd, WM_USER, (WPARAM)&_bytes, (LPARAM)BTHSW_WRITE);
	};	
};
