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

#define COMS_COMMAND		0x101
#define COMS_MESSAGE		0x102
#define COMS_STATE_CHANGE	0x1203
#define COMS_ERROR			0x109
#define COMS_READ			0x110
#define COMS_WRITE			0x111

#define BUFLEN 512

class ComService
{
public:
	ComService(void);
	ComService(HWND _hwnd, SOCKET* _sock) { mSocket = _sock; mHWND = _hwnd; }
	~ComService(void);
	int read(void);	
	int write(ByteBuffer*);
	void setState(int _state)
	{
		mState = _state;
		onStateChange(mState);
	};
	int getState()
	{
		return mState;
	};

	static const int STATE_READING = 1;
	static const int STATE_WRITING = 2;
	static const int STATE_READY = 0;
	static const int STATE_ERROR_READING = 4;
	static const int STATE_EXT_STOP = 5;


	static const int MSG_READ_START = 1;
	static const int MSG_READ_STOP = 2;
	static const int MSG_WRITE_START = 1;
	static const int MSG_WRITE_STOP = 2;

	static const int COMMAND_EXIT = 0;

private:
	SOCKET*	mSocket;
	HWND	mHWND;
	int		mState;
	


	void onRead(ByteBuffer* _buff)
	{
		PostMessage(mHWND, WM_USER, (WPARAM)_buff, (LPARAM)COMS_READ);
	}
	
	void onStateChange(int _state)
	{
		PostMessage(mHWND, WM_USER, (WPARAM)_state, (LPARAM)COMS_STATE_CHANGE);
	}

	void onMessage(int _msg)
	{
		PostMessage(mHWND, WM_USER, (WPARAM)_msg, (LPARAM)COMS_MESSAGE);
	}
	
	void onError(int _err)
	{
		PostMessage(mHWND, WM_USER, (WPARAM)_err, (LPARAM)COMS_ERROR);
	}

	void onCommand(int _cmd)
	{
		PostMessage(mHWND, WM_USER, (WPARAM)_cmd, (LPARAM)COMS_COMMAND);
	}
};
