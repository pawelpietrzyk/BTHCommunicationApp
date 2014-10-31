#pragma once
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

#define RS_COMMAND		0x101
#define RS_MESSAGE		0x102
#define RS_STATE_CHANGE	0x103
#define RS_ERROR		0x109
#define RS_READ			0x110

#define BUFLEN 512

class ReadService
{
public:
	ReadService(void);
	ReadService(SOCKET* _sock) { mSocket = _sock; }
	~ReadService(void);
	int Read(void);	
	void SetState(int);	
	int GetState(void);

	static const int STATE_READING = 1;
	static const int STATE_READY = 0;
	static const int STATE_ERROR_READING = 4;

	static const int MSG_READ_START = 1;
	static const int MSG_READ_STOP = 1;

	static const int COMMAND_EXIT = 0;

private:
	SOCKET*	mSocket;
	HWND	mHWND;
	int		mState;


	void onRead(ByteBuffer* _buff)
	{
		PostMessage(mHWND, WM_USER, (WPARAM)_buff, (LPARAM)RS_READ);
	}
	
	void onStateChange(int _state)
	{
		PostMessage(mHWND, WM_USER, (WPARAM)_state, (LPARAM)RS_STATE_CHANGE);
	}

	void onMessage(int _msg)
	{
		PostMessage(mHWND, WM_USER, (WPARAM)_msg, (LPARAM)RS_MESSAGE);
	}
	
	void onError(int _err)
	{
		PostMessage(mHWND, WM_USER, (WPARAM)_err, (LPARAM)RS_ERROR);
	}

	void onCommand(int _cmd)
	{
		PostMessage(mHWND, WM_USER, (WPARAM)_cmd, (LPARAM)RS_COMMAND);
	}
};
