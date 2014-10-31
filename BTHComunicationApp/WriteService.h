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

#define WS_COMMAND		0x121
#define WS_MESSAGE		0x122
#define WS_STATE_CHANGE	0x123
#define WS_ERROR		0x124
#define WS_WRITE		0x125

#define BUFLEN 512
namespace comunication
{
	class WriteService
	{
	public:
		WriteService(void);
		WriteService(SOCKET* _sock) { mSocket = _sock; }
		~WriteService(void);
		int Write(ByteBuffer*);	
		void setState(int _state)
		{
			mState = _state;
			onStateChange(mState);
		};	
		int getState(void)
		{
			return mState;
		};

		static const int STATE_WRITING = 1;
		static const int STATE_READY = 0;
		static const int STATE_ERROR_READING = 4;
	
		static const int MSG_WRITE_START = 10;
		static const int MSG_WRITE_STOP = 11;
	
		static const int COMMAND_EXIT = 0;

	private:
		SOCKET*	mSocket;
		HWND	mHWND;
		int		mState;
		
		void onStateChange(int _state)
		{
			PostMessage(mHWND, WM_USER, (WPARAM)_state, (LPARAM)WS_STATE_CHANGE);
		}

		void onMessage(int _msg)
		{
			PostMessage(mHWND, WM_USER, (WPARAM)_msg, (LPARAM)WS_MESSAGE);
		}
		
		void onError(int _err)
		{
			PostMessage(mHWND, WM_USER, (WPARAM)_err, (LPARAM)WS_ERROR);
		}
	
		void onCommand(int _cmd)
		{
			PostMessage(mHWND, WM_USER, (WPARAM)_cmd, (LPARAM)WS_COMMAND);
		}
	};
}
