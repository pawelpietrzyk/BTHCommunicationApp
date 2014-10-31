#pragma once
#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "8082"

#define TCP_STATUS_CHANGE	0x1001
#define TCP_ACCEPT			0x1002


class TCPIPConnection
{
public:
	TCPIPConnection(void);
	TCPIPConnection(HWND _hWnd) { mHWND = _hWnd; }
	~TCPIPConnection(void);
	int Run(void);
	bool isHandshake()
	{
		return (connectionState == TCPIPConnection::STATE_HANDSHAKE);
	}
	
	bool isConnected()
	{
		return (connectionState == TCPIPConnection::STATE_CONNECTED);
	}
	bool isDisconnected()
	{
		return (connectionState == TCPIPConnection::STATE_DISCONNECTED);
	}
	bool isWaitingForAccept()
	{
		return (connectionState == TCPIPConnection::STATE_WAITINGFORACCEPT);		
	}
	
	static const int STATE_DISCONNECTED = 10;
	static const int STATE_WAITINGFORACCEPT = 11;
	static const int STATE_HANDSHAKE = 12;
	static const int STATE_CONNECTED = 13;	

private:
	HWND mHWND;
	int handShake(SOCKET* _sock);
	int shutDown(SOCKET* _sock);
	SOCKET* waitForAccept(SOCKET*);
	int connectionState;
	void postOnStatusChange(int _status)
	{
		PostMessage(mHWND, WM_USER, (WPARAM)&_status, (LPARAM)TCP_STATUS_CHANGE);
	}	

	void onAccept(SOCKET* _sock)
	{	
		PostMessage(mHWND, WM_USER, (WPARAM)_sock, (LPARAM)TCP_ACCEPT);	
	}
	
};
