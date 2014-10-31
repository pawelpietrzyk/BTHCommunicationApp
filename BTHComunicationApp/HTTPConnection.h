#pragma once

#ifndef UNICODE
#define UNICODE
#endif

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#define HTTPC_STATUS_CHANGE		0x800

//#include <windows.h>
#include <http.h>
//#include <stdio.h>

#pragma comment(lib, "httpapi.lib")

class HTTPConnection
{
public:
	HTTPConnection(void);
	HTTPConnection(HWND _hWnd)
	{
		hWnd = _hWnd;
	};
	~HTTPConnection(void);
	HWND hWnd;
	DWORD DoReceiveRequests(HANDLE hReqQueue);

DWORD
SendHttpResponse(
    IN HANDLE        hReqQueue,
    IN PHTTP_REQUEST pRequest,
    IN USHORT        StatusCode,
    IN PSTR          pReason,
    IN PSTR          pEntity
    );

DWORD
SendHttpPostResponse(
    IN HANDLE        hReqQueue,
    IN PHTTP_REQUEST pRequest
    );

int run(int argc, wchar_t * argv[]);


void postOnStatusChange(int _status)
{
	PostMessage(hWnd, WM_USER, (WPARAM)&_status, (LPARAM)HTTPC_STATUS_CHANGE);
}

static const int STATE_LISTENING = 20;
static const int STATE_CONNECTED = 21;

};



