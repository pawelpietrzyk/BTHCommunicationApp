#include "StdAfx.h"
#include "Connection.h"

Connection::Connection(void)
{
}

Connection::~Connection(void)
{
}

Connection::initWSA()
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
			this->eLog(L"The version of Windows Sockets support requested is not provided by this particular Windows Sockets implementation.");
			break;
		case WSAEINPROGRESS:			
			this->eLog(L"A blocking Windows Sockets 1.1 operation is in progress.");
			break;
		case WSAEPROCLIM:			
			this->eLog(L"A limit on the number of tasks supported by the Windows Sockets implementation has been reached.");
			break;
		case WSAEFAULT:	
			this->eLog(L"The lpWSAData parameter is not a valid pointer.");
			break;
		default: break;
		}
		WSACleanup();
	} else {	
		this->ifwsaInit = true;
		this->eLog(L"WinSock initialized");
	}
}

void Connection::PrintWSAError()
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