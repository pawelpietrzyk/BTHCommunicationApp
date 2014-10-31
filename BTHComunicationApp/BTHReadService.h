#pragma once

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <Ws2bth.h>
#include <Nspapi.h>
#include "BTHConnection.h"

//typedef ULONGLONG bt_addr, *pbt_addr, BT_ADDR, *PBT_ADDR;

#define BTHSR_INIT			0x500
#define BTHSR_START_READ	0x501
#define BTHSR_STOP_READ		0x502
#define BTHSR_EXIT			0x504
#define BTHSR_READ			0x505
#define BTHSR_ERROR			0x506
#define BUFLEN				512

struct package {
	byte* bytes;
	int count;
};

class BTHReadService
{
public:
	BTHReadService(void);
	BTHReadService(SOCKET *bthSck);
	BTHReadService(BTHConnection *c);		
	~BTHReadService(void);
	int Read(void);	
	int state;
	static const int STATE_READING = 1;
	static const int STATE_READY = 0;
	static const int STATE_ERROR_READING = 4;
	

private:	
	BTHConnection *bthConnection;	
	SOCKET *bthSocket;
	HWND hWnd;	
	void onInit() {
		PostMessage(hWnd, WM_USER, NULL, (LPARAM)BTHSR_INIT);
	};

	void onRead(ByteBuffer* _buff)
	{
		PostMessage(hWnd, WM_USER, (WPARAM)_buff, (LPARAM)BTHSR_READ);
	}
	void onStartReading() {
		PostMessage(hWnd, WM_USER, NULL, (LPARAM)BTHSR_START_READ);
	};
	void onStopReading() {
		PostMessage(hWnd, WM_USER, NULL, (LPARAM)BTHSR_STOP_READ);
	};
	void onErrorReading()
	{
		PostMessage(hWnd, WM_USER, NULL, (LPARAM)BTHSR_ERROR);
	}
	void onExit(void) {
		PostMessage(hWnd, WM_USER, NULL, (LPARAM)BTHSR_EXIT);
	};	
};
