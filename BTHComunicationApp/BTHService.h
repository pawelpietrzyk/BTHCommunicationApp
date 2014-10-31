#pragma once

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <Ws2bth.h>
#include <Nspapi.h>
#include "BTHConnection.h"

//typedef ULONGLONG bt_addr, *pbt_addr, BT_ADDR, *PBT_ADDR;

#define BTHS_FRAME_READ		0x500
#define BTHS_FRAME_READTXT	0x510
#define BTHS_VOLUMEUP		0x501
#define BTHS_VOLUMEDOWN		0x502
#define BTHS_VOLUMECHANGE	0x503
#define BTHS_MUTE			0x504
#define BTHS_EXIT			0x520
#define BTHS_INIT			0x530
#define BTHS_START_READ		0x505
#define BTHS_STOP_READ		0x506


class BTHService
{
public:
	BTHService(void);
	BTHService(SOCKET *bthSck);
	BTHService(BTHConnection *c);
	BTHService(BTHConnection *c, int typeOfSvc);
	BTHService(BTHConnection *connection, bool startRead);
		
	~BTHService(void);
	int Read();
	int Write(byte* buf);
	
	void OnRead(char[], int size);	
	
	SOCKET *bthSocket;	
	HWND hWnd;
	bool reading;
private:
	BTHConnection *bthConnection;
	int type;
	bool iswriting;
	bool isreading;
	
	void onInit() {
		PostMessage(hWnd, WM_USER, (WPARAM)&type, (LPARAM)BTHS_INIT);
	};

	void onReadTxt(byte* buf)
	{
		PostMessage(hWnd, WM_USER, (WPARAM)buf, (LPARAM)BTHS_FRAME_READTXT);
	}
	void onStartReading() {
		PostMessage(hWnd, WM_USER, (WPARAM)&type, (LPARAM)BTHS_START_READ);
	};
	void onStopReading() {
		PostMessage(hWnd, WM_USER, (WPARAM)&type, (LPARAM)BTHS_STOP_READ);
	};
	void onExit(void) {
		PostMessage(hWnd, WM_USER, NULL, (LPARAM)BTHS_EXIT);
	};
	void volumeUP(void) { SendMessage(hWnd, WM_USER, NULL, (LPARAM)BTHS_VOLUMEUP); };
	void volumeDOWN(void) { SendMessage(hWnd, WM_USER, NULL, (LPARAM)BTHS_VOLUMEDOWN); };
	void volumeSET(float vol) { SendMessage(hWnd, WM_USER, (WPARAM)&vol, (LPARAM)BTHS_VOLUMECHANGE); };
	void volumeMUTE(void) { PostMessage(hWnd, WM_USER, NULL, (LPARAM)BTHS_MUTE); };
	
	
	
};