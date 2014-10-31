// BTHComunicationApp.cpp : Defines the entry point for the application.
//
#pragma once

#include "stdafx.h"
#include "BTHComunicationApp.h"
#include "BTHConnection.h"
#include "BTHReadService.h"
#include "TCPIPConnection.h"
#include "ReadService.h"
#include "WriteService.h"
#include "BTHWriteService.h"
#include "HTTPConnection.h"
#include "MMDevice.h"
#include "Frame.h"
#include "VolumeControl.h"
#include "Service.h"


// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
HANDLE CreateNewThread(LPVOID, LPVOID);
DWORD CreateNewThread(LPVOID, LPVOID, HANDLE);
void InitSystemTray(HINSTANCE hI, HWND hWin);
void InitServices(LPVOID data);
void InitConnection(void);

BTHConnection *bthConnection;   //pointer to BTHConnection to manage the connection
HTTPConnection *httpConnection;
TCPIPConnection *tcpConnection;

//ReadService	*svcReadTCP;
//WriteService *svcWriteTCP;
//BTHWriteService *bthWrite;           //poiinter to BTHService for writing
//BTHReadService *bthRead;            //pointer to BTHService for sending


MMDevice *mmDevice;
ComService* tcpWrite;
ComService* bthWrite;

ComService* svcReadTCP;
ComService* svcRead;


DWORD		dSENDSvc;           //Handle to Send Service Thread
DWORD		dWRITESvc;          //Handle to Write Service Thread
DWORD		dConnection;

HANDLE hLog;
HANDLE hEventInit;
HANDLE hEventInitRead;
HANDLE hEventReadContinue;

HWND hListView;
HWND hBtn;
HWND hMainWin;
PNOTIFYICONDATA pTrayIcon;
PNOTIFYICONDATA pIcon0;
PNOTIFYICONDATA pIcon1;
NOTIFYICONDATA iconData;
NOTIFYICONDATA Icon0;
NOTIFYICONDATA Icon1;

const int APP_INITIALIZED = 0;


bool isInHandshake = true;


int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_BTHCOMUNICATIONAPP, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}	
	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_BTHCOMUNICATIONAPP));
	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return TRUE;

	//return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_BTHCOMUNICATIONAPP));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_BTHCOMUNICATIONAPP);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      0, 0, 0, 0, NULL, NULL, hInstance, NULL);
   

   if (!hWnd)
   {
      return FALSE;
   }
   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
void eLog(char* text)
{
	Log::Write(text);
}

HANDLE CreateNewThread(LPVOID procedure, LPVOID data)
{
	HANDLE h;
	DWORD d;
	h = CreateThread(
		NULL,
		0,
		(LPTHREAD_START_ROUTINE) procedure,
		data,
		0,
		&d);
	return h;

}

DWORD CreateNewThread(LPVOID procedure, LPVOID data, HANDLE h)
{
	//HANDLE h;
	DWORD d;
	h = CreateThread(
		NULL,
		0,
		(LPTHREAD_START_ROUTINE) procedure,
		data,
		0,
		&d);
	return d;

}


void InitSystemTray(HINSTANCE hI, HWND hWin)
{
	// Show the notification icon
	//NOTIFYICONDATA nid1;
	ZeroMemory(&Icon0,sizeof(NOTIFYICONDATA));
	Icon0.cbSize				=	sizeof(NOTIFYICONDATA);
	Icon0.hWnd				=	hWin;
	Icon0.uID					=	0;
	Icon0.uFlags				=	NIF_ICON | NIF_MESSAGE | NIF_TIP | NIF_INFO;	
	Icon0.uCallbackMessage    =	WM_USER;	
	Icon0.hIcon =	LoadIcon(hI,MAKEINTRESOURCE(IDI_ICON1));
	//Icon0.uTimeout = 10000;
	//Icon0.uVersion = NOTIFYICON_VERSION_4;
	Icon0.dwInfoFlags = NIIF_INFO;
	
	lstrcpy(Icon0.szInfo, L"Uruchomiono ");
	lstrcpy(Icon0.szInfoTitle, L"BTH Comunication");
	lstrcpy(Icon0.szTip, L"Nie po³¹czony. Prawy klawisz zamyka");
	pIcon0 = &Icon0;

	//NOTIFYICONDATA nid2;
	ZeroMemory(&Icon1,sizeof(Icon1));
	Icon1.cbSize				=	sizeof(NOTIFYICONDATA);
	Icon1.hWnd				=	hWin;
	Icon1.uID					=	1;
	Icon1.uFlags				=	NIF_ICON | NIF_MESSAGE | NIF_TIP;	
	Icon1.uCallbackMessage    =	WM_USER;	
	//LoadIconMetric(hI, MAKEINTRESOURCE(IDI_ICON3), LIM_SMALL, &(Icon1.hIcon));
	Icon1.hIcon =	LoadIcon(hI,MAKEINTRESOURCE(IDI_ICON3));

	lstrcpy(Icon1.szTip, L"Po³¹czony. Prawy klawisz zamyka");
	pIcon1 = &Icon1;
}
void notifyStart()
{
	//NOTIFYICONDATA nid;
	ZeroMemory(&iconData,sizeof(NOTIFYICONDATA));
	iconData.cbSize				=	sizeof(NOTIFYICONDATA);
	iconData.hWnd				=	hMainWin;
	iconData.uID					=	0;
	iconData.uFlags				=	NIF_ICON | NIF_MESSAGE | NIF_TIP | NIF_INFO;	
	iconData.uCallbackMessage    =	WM_USER;	
	iconData.dwInfoFlags = NIIF_INFO;
	iconData.hIcon =	LoadIcon(hInst,MAKEINTRESOURCE(IDI_ICON1));
	lstrcpy(iconData.szInfo, L"Uruchomiono");
	lstrcpy(iconData.szInfoTitle, L"BTH Comunication");
	lstrcpy(iconData.szTip, L"Uruchomiony. Prawy klawisz zamyka");
	pTrayIcon = &iconData;	
	Shell_NotifyIcon(NIM_ADD, pTrayIcon);

}
void notifyStateChange(int src, int _type)
{
	bool display = false;
	//HANDLE hEvent = OpenEvent(SYNCHRONIZE, FALSE, L"NOTIFY_CHANGE");
	//WaitForSingleObject(hEvent, INFINITE);

	switch (src)
	{
	case 0:
		switch (_type)
		{
		/*
		case APP_INITIALIZED:
			nid.hIcon =	LoadIcon(hI,MAKEINTRESOURCE(IDI_ICON1));
			lstrcpy(nid.szInfo, L"Uruchomiono");
			lstrcpy(nid.szInfoTitle, L"BTH Comunication");
			lstrcpy(nid.szTip, L"Uruchomiony. Prawy klawisz zamyka");
			break;
		*/
		case BTHConnection::STATE_CONNECTED:		
			lstrcpy(iconData.szInfo, L"Po³¹czono");		
			lstrcpy(iconData.szTip, L"Po³¹czony. Prawy klawisz zamyka");
			display = true;
			break;
		case BTHConnection::STATE_WAITINGFORACCEPT:
			iconData.hIcon =	LoadIcon(hInst,MAKEINTRESOURCE(IDI_ICON1));
			lstrcpy(iconData.szInfo, L"Oczekuje na po³¹czenie");		
			lstrcpy(iconData.szTip, L"Nie po³¹czony. Prawy klawisz zamyka");
			display = true;
			break;
		case BTHConnection::STATE_HANDSHAKE:		
			lstrcpy(iconData.szInfo, L"Negocjacja po³¹czenia");		
			display = true;
			break;
		case BTHConnection::STATE_DISCONNECTED:		
			lstrcpy(iconData.szInfo, L"Roz³¹czono");		
			lstrcpy(iconData.szTip, L"Nie po³¹czony. Prawy klawisz zamyka");
			display = true;
			break;
		case HTTPConnection::STATE_LISTENING:
			lstrcpy(iconData.szInfo, L"Nas³uchiwanie");
			display = true;
			break;
		case 400:			
			lstrcpy(iconData.szInfo, L"B³¹d podczas próby po³¹czenia");		
			lstrcpy(iconData.szTip, L"Nie po³¹czony. Prawy klawisz zamyka");
			display = true;	
		default: break;	
			break;
		}
	case 1:
		switch (_type)
		{
		case ComService::STATE_READING:
			lstrcpy(iconData.szInfo, L"Rozpoczêto odczyt");		
			display = true;
			break;
		case ComService::MSG_READ_STOP:
			lstrcpy(iconData.szInfo, L"Zatrzymano odczyt");		
			display = true;			
			break;
		}
		break;
	}
	if (display)
	{
		Shell_NotifyIcon(NIM_MODIFY, pTrayIcon);
	}
}
void copy(wchar_t* dest, size_t destSize, char* src, int len)
{
	int i, n;
	for (i = 0, n = 0; n < destSize; i++, n++)
	{
		dest[i] = src[i];
		if (i >= len)
		{
			dest[i] = 0;
		}
	}

}
void notifyMessage(ByteBuffer* _bbuf)
{	

	//wchar_t *c = (wchar_t*)_bbuf->getArray();
	//char buf[] = new char[_bbuf->getCapacity()];
	//buf = _bbuf->getArray();
	//StringCbCopyN(iconData.szInfo, ARRAYSIZE(iconData.szInfo), (char*)_bbuf->getArray(), _bbuf->getCapacity());
	//lstrcpy(iconData.szInfo, (char*)_bbuf->getArray();
	copy(iconData.szInfo, ARRAYSIZE(iconData.szInfo), (char*)_bbuf->getArray(), _bbuf->getCapacity());
	//iconData.szInfo = _msg;
	//iconData.szInfo = (WCHAR*)(c);
	//swprintf(iconData.szInfo, _bbuf->getCapacity(), "%s", (wchar_t*)c);
	//StringCchCopy(iconData.szInfo, ARRAYSIZE(iconData.szInfo), c);
	//lstrcpy(iconData.szInfoTitle, L"BTH Comunication");
	Shell_NotifyIcon(NIM_MODIFY, pTrayIcon);	
}


void notifyStop()
{
	Shell_NotifyIcon(NIM_DELETE, pTrayIcon);
}




void bthcInitialized() {
	//Icon0.hIcon = LoadIcon(hInst,MAKEINTRESOURCE(IDI_ICON1));
	//lstrcpy(Icon0.szTip, L"Po³¹czony. Prawy klawisz zamyka");	
	//Shell_NotifyIcon(NIM_MODIFY, &Icon0);
	//Shell_NotifyIcon(NIM_ADD, pIcon1);
	eLog("Connection initialized");
}
void doExit() {
	//Icon0.hIcon = LoadIcon(hInst,MAKEINTRESOURCE(IDI_ICON1));
	//lstrcpy(Icon0.szTip, L"Niepo³¹czony. Prawy klawisz zamyka");				
	//Shell_NotifyIcon(NIM_MODIFY, &Icon0);
	if (svcReadTCP != null)
	{
		svcReadTCP->setState(ComService::STATE_EXT_STOP);
		HANDLE hEvent = OpenEvent(SYNCHRONIZE, FALSE, L"TCP_READ_CONTINUE");	
		SetEvent(hEvent);
	}

			//delete bthRead;
			//delete bthWrite;		
}
void destroy() {	
	Shell_NotifyIcon(NIM_DELETE, pTrayIcon);
	//Shell_NotifyIcon(NIM_DELETE, pIcon1);		
	PostQuitMessage(0);
}


DWORD InitReadServiceBTH(LPVOID data)
{
	SOCKET *sock = (SOCKET *)data;
	svcRead = new ComService(hMainWin, sock);	
	HANDLE hEvent = OpenEvent(SYNCHRONIZE, FALSE, L"BTH_INIT_READ");
	WaitForSingleObject(hEvent, INFINITE);
	svcRead->read();	
	return TRUE;	
}

DWORD InitReadServiceTCP(LPVOID data)
{
	SOCKET *sock = (SOCKET *)data;
	svcReadTCP = new ComService(hMainWin, sock);	
	//HANDLE hEvent = OpenEvent(SYNCHRONIZE, FALSE, L"TCP_INIT_READ");
	//WaitForSingleObject(hEvent, INFINITE);
	svcReadTCP->read();	
	return TRUE;
}
DWORD InitWriteServiceTCP(LPVOID data)
{
	SOCKET *sock = (SOCKET *)data;
	tcpWrite = new ComService(hMainWin, sock);	
	return TRUE;
}

DWORD InitWriteServiceBTH(LPVOID data)
{
	SOCKET *sock = (SOCKET *)data;
	bthWrite = new ComService(hMainWin, sock);	
	return TRUE;
}

void InitServicesBTH(LPVOID data)
{
	HANDLE hThread[2];
	//LPVOID lpw = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(BTHConnection));
	LPVOID lpr = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(BTHConnection));
	//lpw = data;
	lpr = data;
	//hEventInit = CreateEvent(NULL, TRUE, FALSE, L"START_WRITING_INIT");
	hEventInitRead = CreateEvent(NULL, TRUE, FALSE, L"INIT_READ");
	
	//dSENDSvc = CreateNewThread(&InitReadService, lpr, hThread[0]);
	//dWRITESvc = CreateNewThread(&InitWriteService, lpw, hThread[1]);
	hThread[0] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&InitReadServiceBTH, lpr, 0, &dSENDSvc);
	InitWriteServiceBTH(lpr);
	//hThread[1] = CreateThread(NULL, 2097152, (LPTHREAD_START_ROUTINE)&InitWriteService, lpw, STACK_SIZE_PARAM_IS_A_RESERVATION, &dWRITESvc);

	//Button_SetText(hBtn, L"Connected");
	//Shell_NotifyIcon(NIM_MODIFY, pIcon0);
	
    DWORD rv;
	//rv = WaitForMultipleObjects(2, hThread, TRUE, INFINITE); 
	switch (rv)
	{
	case WAIT_FAILED:
		eLog("wait failed");
		break;
	case WAIT_OBJECT_0:
		break;
	}

}

void InitServicesTCP(LPVOID data)
{
	HANDLE hThread[2];
	//LPVOID lpw = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(BTHConnection));
	LPVOID lpr = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(BTHConnection));
	//lpw = data;
	lpr = data;
	//hEventInit = CreateEvent(NULL, TRUE, FALSE, L"START_WRITING_INIT");
	hEventInitRead = CreateEvent(NULL, TRUE, FALSE, L"TCP_INIT_READ");
	
	//dSENDSvc = CreateNewThread(&InitReadService, lpr, hThread[0]);
	//dWRITESvc = CreateNewThread(&InitWriteService, lpw, hThread[1]);
	hThread[0] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&InitReadServiceTCP, lpr, 0, &dSENDSvc);
	InitWriteServiceTCP(lpr);
	//hThread[1] = CreateThread(NULL, 2097152, (LPTHREAD_START_ROUTINE)&InitWriteService, lpw, STACK_SIZE_PARAM_IS_A_RESERVATION, &dWRITESvc);

	//Button_SetText(hBtn, L"Connected");
	//Shell_NotifyIcon(NIM_MODIFY, pIcon0);
	
    DWORD rv;
	//rv = WaitForMultipleObjects(2, hThread, TRUE, INFINITE); 
	switch (rv)
	{
	case WAIT_FAILED:
		eLog("wait failed");
		break;
	case WAIT_OBJECT_0:
		break;
	}

}

void InitConnectionBTH()
{
	
	bthConnection = new BTHConnection(hMainWin, guid);
	bthConnection->WaitForAccept(1);	
}

void initHTTPConnection()
{
	httpConnection = new HTTPConnection(hMainWin);
	wchar_t *uri[1];
	uri[0] = L"http://+:8082/";
	httpConnection->run(1, uri);
}
void initTCPConnection()
{
	tcpConnection = new TCPIPConnection(hMainWin);
	tcpConnection->Run();
}

void create(HWND hwnd) {
	hMainWin = hwnd;	
	DWORD tmpDWORD;
	hEventReadContinue = CreateEvent(NULL, FALSE, FALSE, L"TCP_READ_CONTINUE");
	//HANDLE hMMDevice = CreateNewThread(&InitMMDevice, NULL);
	mmDevice = new MMDevice();
	//dConnection = CreateNewThread(&InitConnectionBTH, NULL, 0);			
	tmpDWORD = CreateNewThread(&initTCPConnection, NULL, 0);

	//InitSystemTray(hInst, hwnd);
	//Shell_NotifyIcon(NIM_ADD, &Icon0);
	notifyStart();
	//eLog("Application started");
	//tmpDWORD = CreateNewThread(&initHTTPConnection, NULL, 0);	
}




void onServiceReadInit(WPARAM wParam)
{
	UNREFERENCED_PARAMETER(wParam);
	SetEvent(hEventInitRead);
}
void onCntStatusChange(WPARAM wParam)
{
	int param = *(int*)wParam;
	notifyStateChange(0, param);
	switch (param)
	{
		case BTHConnection::STATE_CONNECTED: 			
			break;

		default: break;
	}
		
}
void onSvcStatusChange(WPARAM wParam)
{
	LPVOID lpr = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(int));
	lpr = (LPVOID)wParam;
	int param = *(int*)lpr;
	notifyStateChange(0, param);

}
bool dispatchBuffer(ByteBuffer *buffer)
{
	//Frame* frm;
	//frm = new Frame(buffer->getArray(), buffer->getCapacity());

	//VolumeControl* vc = VolumeControl::createFromFrame(frm);
	VolumeControl* vc = &VolumeControl::unpack((char*)buffer->getArray(), buffer->getCapacity());
	if (mmDevice == null)
	{
		return false;
	}
	
	switch (vc->state)
	{
	case Initialize:
		break;
	case Message:
		break;
	case Action:
		switch (vc->volumeAction)
		{
		case Up:
			mmDevice->SetVolumeUP();
			break;
		case Down:
			mmDevice->SetVolumeDOWN();
			break;
		case Mute:
			mmDevice->Mute();
			break;
		case Exit:
			doExit();
			break;
		default: break;
		}
		break;
	}
	
	return true;
	
}

void onServiceRead(WPARAM wParam)
{	
	ByteBuffer* mBuf = (ByteBuffer*)wParam;
	dispatchBuffer(mBuf);

	/*	
	char* text = "ODPOWIEDZ";
	ByteBuffer* mBuf2 = new ByteBuffer(mBuf->getArray(), mBuf->getCapacity());
	svcReadTCP->write(mBuf2);
	VolumeControl* c;
	c = &VolumeControl::unpack((char*)mBuf->getArray(), mBuf->getCapacity());
	*/

	//svcReadTCP->setState(ComService::STATE_EXT_STOP);
	//HANDLE hEvent = OpenEvent(SYNCHRONIZE, FALSE, L"TCP_READ_CONTINUE");
	//WaitForSingleObject(hEvent, INFINITE);
	//SetEvent(hEvent);
	//svcReadTCP->write(

	//dispatchBuffer(mBuf);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
		case WM_CREATE:	
		create(hWnd); 
		return (INT_PTR)TRUE;
	case WM_COMMAND: break;		
	case WM_PAINT: break;
	case WM_DESTROY:
		//PostQuitMessage(0);
		destroy();
		break;
	case WM_USER:
		switch(lParam)
		{
		case WM_LBUTTONDBLCLK:	break;
		case WM_LBUTTONUP: break;
		case WM_RBUTTONUP: DestroyWindow(hWnd);	break;
		//case BTHS_VOLUMEUP:	setVolumeUP(); 	break;
		//case BTHS_VOLUMEDOWN: setVolumeDOWN(); break;
		//case BTHS_VOLUMECHANGE:	setVolumeLevel(wParam);	break;
		//case BTHS_MUTE:	setVolumeMUTE(); break;
		// Gdy BTHService zostanie zakonczona przez przerwanie polaczenia
		//case BTHS_EXIT: Exit(); break;
		//Gdy BTHService poprawnie odczyta dane z socket
		//case BTHSR_READ: onServiceReadRead(wParam);	break;			
		//Gdy BTHService zostanie zaincjowana poprawnie
		//case BTHSR_INIT: onServiceReadInit(wParam);	break;			
		// Gdy BTHService zostanie zakonczona przez przerwanie polaczenia
		//case BTHSR_EXIT: Exit(); break;
		//case BTHSR_STOP_READ: notifyStateChange(400); break;
		//Gdy zostanie zakonczona normalnie procedura wysylania ramek inicjujacych
		//case BTHC_INITIALIZED:
			//bthcInitialized();
		//	break;
		// Gdy BTHConnection zaakceptuje polaczenie przychodzace (funckja accept() zwroci socket)
		case BTH_ACCEPT: 
			InitServicesBTH((LPVOID)wParam); 
			break;
		case TCP_ACCEPT:
			InitServicesTCP((LPVOID)wParam); 
			break;
		case RS_READ:
			onServiceRead(wParam);
			break;
		case COMS_STATE_CHANGE:			
			notifyStateChange(1, wParam);
			break;
			
		case BTHC_CONNECTED: 
			//InitServices((LPVOID)wParam); 
			break;
		case BTHC_STATUS_CHANGE:
			onCntStatusChange(wParam);
			break;
		case TCP_STATUS_CHANGE:
			onCntStatusChange(wParam);
			break;
		case HTTPC_STATUS_CHANGE:
			onCntStatusChange(wParam);
			break;
		default: break;
		}		
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

HRESULT STDMETHODCALLTYPE CAudioEndpointVolumeCallback::OnNotify(PAUDIO_VOLUME_NOTIFICATION_DATA pNotify)
{
	if (pNotify == NULL) {
		return E_INVALIDARG;
	}

	if (pNotify->guidEventContext != guid) {
		//mmDevice->SendMsgVolume(bthWrite);
		PostMessage(hMainWin, WM_USER, (WPARAM)pNotify, (LPARAM)MMD_ONNOTIFY);
	}
     
		
		/*
        if (g_hDlg != NULL && pNotify->guidEventContext != g_guidMyContext)
        {
            PostMessage(GetDlgItem(g_hDlg, IDC_CHECK_MUTE), BM_SETCHECK,
                        (pNotify->bMuted) ? BST_CHECKED : BST_UNCHECKED, 0);

            PostMessage(GetDlgItem(g_hDlg, IDC_SLIDER_VOLUME),
                        TBM_SETPOS, TRUE,
                        LPARAM((UINT32)(MAX_VOL*pNotify->fMasterVolume + 0.5)));
        }
		*/
	return S_OK;
}







