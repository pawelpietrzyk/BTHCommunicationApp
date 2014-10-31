#include "StdAfx.h"
#include "TCPIPConnection.h"
#include "Protocol.h"


TCPIPConnection::TCPIPConnection(void)
{
}

TCPIPConnection::~TCPIPConnection(void)
{
}


int TCPIPConnection::Run()
{

    WSADATA wsaData;
    int iResult;
	int loops = 0;

    SOCKET ListenSocket = INVALID_SOCKET;
    SOCKET ClientSocket = INVALID_SOCKET;

    struct addrinfo *result = NULL;
    struct addrinfo hints;

    int iSendResult;
    char recvbuf[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;
    
    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    if ( iResult != 0 ) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Create a SOCKET for connecting to server
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    // Setup the TCP listening socket
    iResult = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    freeaddrinfo(result);

    iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    // Accept a client socket
	do 
	{
		ClientSocket = accept(ListenSocket, NULL, NULL);
		
		if (ClientSocket == INVALID_SOCKET) 
		{
	        //printf("accept failed with error: %d\n", WSAGetLastError());
			closesocket(ListenSocket);
			WSACleanup();
			return 1;
		}
		//if (handShake(&ClientSocket) == 0)
		//{
			onAccept(&ClientSocket);
		//}

		loops++;
	} while (loops > 0);

    // No longer need server socket
    closesocket(ListenSocket);

    shutDown(&ClientSocket);

    return 0;

}

SOCKET* TCPIPConnection::waitForAccept(SOCKET* _listenSock)
{
	SOCKET client = INVALID_SOCKET;
	// Accept a client socket
    client = accept(*_listenSock, NULL, NULL);
    if (client == INVALID_SOCKET) 
	{        
        closesocket(*_listenSock);
        WSACleanup();        
    }
	return &client;
}

int TCPIPConnection::handShake(SOCKET* _sock)
{
	int iResult;
	int iSendResult;
    char recvbuf[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;


	do {

        iResult = recv(*_sock, recvbuf, recvbuflen, 0);
        if (iResult > 0) 
		{
			iSendResult = send(* _sock, Protocol::respOK(), iResult, 0 );

            if (iSendResult == SOCKET_ERROR) 
			{               
                closesocket(*_sock);
                WSACleanup();
                return 1;
            }
            return 0;
        }
        else if (iResult == 0)
		{
			return 1;
		}            
        else  
		{            
            closesocket(*_sock);
            WSACleanup();
            return 1;
        }

    } while (iResult > 0);
	return 1;
}

int TCPIPConnection::shutDown(SOCKET* _sock)
{
	int iResult;
	
    iResult = shutdown(*_sock, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        //printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(*_sock);
        WSACleanup();
        return 1;
    }

    // cleanup
    closesocket(*_sock);
    WSACleanup();
	return 0;
}