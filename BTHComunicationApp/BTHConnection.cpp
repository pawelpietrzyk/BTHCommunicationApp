#include "StdAfx.h"
#include "BTHConnection.h"


BTHConnection::BTHConnection(void)
{
	this->Initialize();
	if (!ifwsaInit) {

	}
}
BTHConnection::BTHConnection(GUID serverGuid)
{
	this->guid = serverGuid;
	this->ifSetup = false;
	this->ifwsaInit = false;
	this->ifStarted = false;
	this->Initialize();
	if (!ifwsaInit) {

	} else {		
		this->SetConnection();
		this->StartListening(true);

		
	}

}
BTHConnection::BTHConnection(HWND hwnd, GUID serverGuid)
{
	this->guid = serverGuid;
	this->hWnd = hwnd;
	this->ifSetup = false;
	this->ifwsaInit = false;
	this->ifStarted = false;
	this->Initialize();
	if (!ifwsaInit) {

	} else {		
		int ir;
		ir = this->SetConnection();
		if (ir == SOCKET_ERROR) { return; }
		ir = this->StartListening(true);
		if (ir == SOCKET_ERROR) { return; }
		/*ir = this->WaitForAccept();
		if (ir == SOCKET_ERROR) { return; }*/
				
	}
}

BTHConnection::~BTHConnection(void)
{
	StopListening();
}

int BTHConnection::ListenLoop()
{
	if (ifSetup) {

	}
	return 0;
}

int BTHConnection::StopListening()
{
	int iResult;
	//this->ifSetup = false;
	
	//iResult = WSASetService(wsaQuerySet, RNRSERVICE_DEREGISTER, 0);
	//if (iResult == SOCKET_ERROR) {
	//	this->eLog(L"WSASetService Deregister Error");
	//	PrintWSAError();		
	//	//return 1;
	//}
	
	
	//iResult = shutdown(this->ClientSOCKET, SD_BOTH);
	//if (iResult == SOCKET_ERROR) {
	//	this->eLog(L"Client Socket Shutdown error");
	//	PrintWSAError();		
	//	//return 1;
	//}
	//closesocket(this->ListenSOCKET);

	//iResult = shutdown(this->ListenSOCKET, SD_BOTH);
	//if (iResult == SOCKET_ERROR) {
	//	this->eLog(L"Listen Socket Shutdown error");
	//	PrintWSAError();		
	//	//return 1;
	//}
	//closesocket(this->ClientSOCKET);

	//iResult = WSASetService(wsaQuerySet, RNRSERVICE_DELETE, 0);
	//if (iResult == SOCKET_ERROR) {
	//	this->eLog(L"WSASetService Delete Error");
	//	PrintWSAError();		
	//	//return 1;
	//}
	//WSACleanup();
	//this->ifwsaInit = false;
	//this->ifStarted = false;
	return 0;
}

int BTHConnection::StartListening(bool init)
{			
	if (!ifSetup) {
		//eLog(L"Connection are not set");
		
		return 1;
	}
	if (listen(ListenSOCKET, SOMAXCONN) == SOCKET_ERROR) {
		//this->eLog(L"listen error");
		PrintWSAError();
		closesocket(ListenSOCKET);
		WSACleanup();

		return 1;
	}
	//this->eLog(L"listen is running");

	
	



	return 0;
}

int BTHConnection::WaitForAccept()
{
	SOCKET ClientSocket;
	ClientSocket = INVALID_SOCKET;
	//this->eLog(L"Wait For Accept");
	// Accept a client socket
	ClientSocket = accept(ListenSOCKET, NULL, NULL);
	if (ClientSocket == INVALID_SOCKET) {
		//printf("accept failed: %d\n", WSAGetLastError());
		//this->eLog(L"accept failed");
		PrintWSAError();
		closesocket(ListenSOCKET);
		WSACleanup();
		return 1;
	}
	this->ifStarted = true;
	//this->eLog(L"Socket accepted");
	this->ClientSOCKET = ClientSocket;
	this->OnAccept(this);
	return 0;
}
int BTHConnection::WaitForAccept(int loops)
{
	SOCKET ClientSocket;
	if (loops == 0) { return 1; }
	ClientSocket = INVALID_SOCKET;
	//this->eLog(L"Wait For Accept");
	// Accept a client socket
	ClientSocket = accept(ListenSOCKET, NULL, NULL);
	if (ClientSocket == INVALID_SOCKET) {
		//printf("accept failed: %d\n", WSAGetLastError());
		//this->eLog(L"accept failed");
		postNODEVICE();
		PrintWSAError();
		closesocket(ListenSOCKET);
		WSACleanup();
		return 1;
	}
	this->ifStarted = true;
	this->setHandshake();
	
	//this->eLog(L"Socket accepted");
	this->ClientSOCKET = ClientSocket;
	loops++;
	this->OnAccept(this);
	//if (this->Handshake() == 0)
	//{
	//	this->OnConnected(this);
	//}
	this->WaitForAccept(loops);
	return 0;
}
int BTHConnection::CloseRemoteSocket()
{
	int ir;
	ir = shutdown(this->ClientSOCKET, SD_BOTH);
	if (ir == SOCKET_ERROR) {
		//eLog(L"shutdown remote socket error");		
	}
	closesocket(this->ClientSOCKET);
	//eLog(L"Remote socket shutdown");	
	return 0;
}
/*
int BTHConnection::Handshake(SOCKET s)
{
	int result = 0;
	int runs = 0;
	int maxruns = 10;	
	do {
		char recvbuf[BUFLEN] = "";
		result = recv(s, recvbuf, (int)BUFLEN, 0);
		if (result > 0)
		{
			result = send(s, recvbuf, result, 0);
			if (result == SOCKET_ERROR)
			{
				closesocket(s);
				return -1;
			}		
			return 0;
		}
		Sleep(1000);
		runs++;
	} while (runs < maxruns);
	return -2;
}
*/
void BTHConnection::OnAccept(BTHConnection *bthcon)
{
	bool check;
	LPVOID lp = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(BTHConnection));
	lp = (LPVOID) bthcon;
	this->setHandshake();
	PostMessage(hWnd, WM_USER, (WPARAM)lp, (LPARAM)BTH_ACCEPT);	
}
void BTHConnection::OnConnected(BTHConnection *bthcon)
{
	bool check;
	LPVOID lp = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(BTHConnection));
	lp = (LPVOID) bthcon;
	PostMessage(hWnd, WM_USER, (WPARAM)lp, (LPARAM)BTHC_CONNECTED);	
}



int BTHConnection::SetConnection()
{
	if (!this->ifwsaInit) {
		//this->eLog(L"WSAService not initialized");
		//this->Initialize();
		return 1;
	}
	int iResult = 0;

	//Setup server Socket needed for bind with ListenSocket
	SOCKADDR_BTH serverSocket;
	serverSocket.addressFamily = AF_BTH;
	serverSocket.btAddr = 0;
	serverSocket.serviceClassId = guid;
	serverSocket.port = BT_PORT_ANY;

	//Init Listen Socket with required parameters for BTH device
	SOCKET ListenSocket = INVALID_SOCKET;
	ListenSocket = socket(AF_BTH, SOCK_STREAM, BTHPROTO_RFCOMM);
	if (ListenSocket == INVALID_SOCKET)
	{
		//this->eLog(L"Invalid Socket");
		return 1;
	}
	//Bind Listen Socket
	iResult = bind( ListenSocket, (SOCKADDR*) &serverSocket, sizeof(SOCKADDR_BTH));
    if (iResult == SOCKET_ERROR) {        
		//this->eLog(L"bind failed with error");
		PrintWSAError();
        //freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }
	//this->eLog(L"Listen Socket binded");
		
	//get SOCKADDR_BTH structure for listen socket needed to WSASetService
	SOCKADDR_BTH listenSockA;
	int socksize = sizeof(listenSockA);
	iResult = getsockname(ListenSocket, (SOCKADDR *) &listenSockA, &socksize);
	if (iResult == SOCKET_ERROR)
	{
		//this->eLog(L"Error in getsockname");
		PrintWSAError();
		return 1;
	}
	
	//Setting CSADDR_INFO structure
	CSADDR_INFO addrInfo;
	addrInfo.LocalAddr.lpSockaddr = (SOCKADDR *) &listenSockA;
	addrInfo.iSocketType = SOCK_STREAM;
	addrInfo.iProtocol = BTHPROTO_RFCOMM;

	WSAQUERYSET wsaQS;
	wsaQS.dwSize = sizeof(WSAQUERYSET);
	wsaQS.lpszServiceInstanceName = L"BLUEWIN32WSA2";
	wsaQS.dwNameSpace = NS_BTH;
	wsaQS.dwNumberOfCsAddrs = 1;
	wsaQS.lpcsaBuffer = &addrInfo;
	wsaQS.lpServiceClassId = (LPGUID) &guid;
	

	iResult = WSASetService(&wsaQS, RNRSERVICE_REGISTER, 0);
	if (iResult == SOCKET_ERROR) {
		//this->eLog(L"WSASetService Error");
		PrintWSAError();
		WSASetService(&wsaQS, RNRSERVICE_DELETE, 0);
		return 1;
	}
	this->wsaQuerySet = &wsaQS;
	//this->eLog(L"WSAsetService registered");
	this->ListenSOCKET = ListenSocket;
	this->ifSetup = true;
	return 0;
}

void BTHConnection::Initialize()
{
	WSADATA wsaDATA;	
	this->ifwsaInit = false;
	iLastError = WSAStartup(MAKEWORD(2,2), &wsaDATA);	
	if (iLastError != 0) {
		switch (iLastError)
		{
		case WSASYSNOTREADY:			
			this->eLog(L"The underlying network subsystem is not ready for network communication.");
			break;
		case WSAVERNOTSUPPORTED:
			//InsertItem(hListView, L"The version of Windows Sockets support requested is not provided by this particular Windows Sockets implementation.");
			this->eLog(L"The version of Windows Sockets support requested is not provided by this particular Windows Sockets implementation.");
			break;
		case WSAEINPROGRESS:
			//InsertItem(hListView, L"A blocking Windows Sockets 1.1 operation is in progress.");
			this->eLog(L"A blocking Windows Sockets 1.1 operation is in progress.");
			break;
		case WSAEPROCLIM:
			//InsertItem(hListView, L"A limit on the number of tasks supported by the Windows Sockets implementation has been reached.");
			this->eLog(L"A limit on the number of tasks supported by the Windows Sockets implementation has been reached.");
			break;
		case WSAEFAULT:
			//InsertItem(hListView, L"The lpWSAData parameter is not a valid pointer.");
			this->eLog(L"The lpWSAData parameter is not a valid pointer.");
			break;
		default: break;
		}
		WSACleanup();
	} else {
		//InsertItem(hListView, L"WinSock initialized");
		this->ifwsaInit = true;
		this->eLog(L"WinSock initialized");
		//wsprintf(version, _T("Version: %s"), wsaDATA.wVersion);
		//InsertItem(hListView, L"Version");
		//CreateSocket();

	}
	
}


void BTHConnection::PrintWSAError()
{
	LPWSTR msgt = L"";
	switch (WSAGetLastError())
		{
		case WSANOTINITIALISED:			
			this->eLog(L"A successful WSAStartup call must occur before using this function.");
			break;
		case WSAENETDOWN:			
			this->eLog(L"The network subsystem has failed.");
			break;
		case WSAEADDRINUSE:
			this->eLog(L"Only one usage of each socket address (protocol/network address/port) is normally permitted.");
			break;
		case WSAEINPROGRESS:			
			this->eLog(L"A blocking Windows Sockets 1.1 call is in progress, or the service provider is still processing a callback function.");
			break;
		case WSAEINVAL:			
			this->eLog(L"One or more required parameters were invalid or missing.");
			break;
		case WSAEISCONN:
			this->eLog(L"The socket is already connected.");
			break;
		case WSAEMFILE:
			this->eLog(L"No more socket descriptors are available.");
			break;
		case WSAENOBUFS:
			this->eLog(L"No buffer space is available.");
			break;
		case WSAENOTSOCK:
			this->eLog(L"The descriptor is not a socket.");
			break;
		case WSAEOPNOTSUPP:
			this->eLog(L"The referenced socket is not of a type that supports the listen operation.");
			break;
		case WSAEACCES:
			this->eLog(L"The calling routine does not have sufficient privileges to install the Service.");
			break;
		case WSA_NOT_ENOUGH_MEMORY:
			this->eLog(L"There was insufficient memory to perform the operation.");
			break;
		case WSAEADDRNOTAVAIL:			
			this->eLog(L"The requested address is not valid in its context.");
			break;
		case WSAEFAULT:
			this->eLog(L"The system detected an invalid pointer address in attempting to use a pointer argument in a call.");
			break;
		case WSAENETRESET:
			this->eLog(L"The connection has timed out when SO_KEEPALIVE is set.");
			break;
		case WSAENOPROTOOPT:
			this->eLog(L"The option is unknown or unsupported for the specified provider or socket (see SO_GROUP_PRIORITY limitations).");
			break;
		case WSAENOTCONN:			
			this->eLog(L"The connection has been reset when SO_KEEPALIVE is set.");			
			break;
		default: 
			this->eLog(L"Other errors");
			break;
		}
	
}






void BTHConnection::sendInitialized()
{
	PostMessage(hWnd, WM_USER, NULL, (LPARAM)BTHC_INITIALIZED);
}
void BTHConnection::sendCloseError()
{
	PostMessage(hWnd, WM_USER, NULL, (LPARAM)BTHC_CLOSEERROR);
}

void BTHConnection::eLog(LPWSTR lpstr)
{
	//mListView->InsertItem(lpstr);
	//InsertItem(hListView, lpstr);
	//Log::Write((char*)lpstr);
	PostMessage(hWnd, WM_USER, (WPARAM)lpstr, (LPARAM)BTHC_MESSAGE);
	
}
