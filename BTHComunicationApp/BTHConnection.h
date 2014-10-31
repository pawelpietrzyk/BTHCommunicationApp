#pragma once

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <Ws2bth.h>
#include <Nspapi.h>
//#include "BTHService.h"

typedef ULONGLONG bt_addr, *pbt_addr, BT_ADDR, *PBT_ADDR;

#define BTHC_INITIALIZED	0x901
#define BTHC_CLOSEERROR		0x910
#define BTHC_NODEVICE		0x911
#define BTH_ACCEPT			0x920
#define BTHC_MESSAGE		0x930
#define BTHC_CONNECTED		0x940
#define BTHC_DISCONNECTED	0x941
#define BTHC_STATUS_CHANGE	0x902


//#define BTHS_VOLUMEUP		0x501
//#define BTHS_VOLUMEDOWN		0x502
//#define BTHS_VOLUMECHANGE	0x503
//#define BTHS_MUTE			0x504
//#define BTHS_EXIT			0x600
//#define BTHS_INIT			0x700




class BTHConnection
{
public:
	/*class BTHService {
	public:
		BTHService(void);
		BTHService(SOCKET *bthSck);
		BTHService(BTHConnection *c);
		~BTHService(void);
		void Read();
		void Write(char[], int);
		void OnRead(char[], int size);	
		void OnExit(void);
		SOCKET *bthSocket;
		static const int BUFLEN = 512;
		HWND hWnd;
	private:
		BTHConnection *bthConnection;
		bool iswriting;
		bool isreading;
		void DecodeFrame(char frame[], int frSize);
		void volumeUP(void);
		void volumeDOWN(void);
		void volumeSET(float vol);
		void volumeMUTE(void);
		void clientEXIT(void);
		void clientINIT(void);
	};*/
	BTHConnection(void);
	BTHConnection(GUID serverGuid);
	BTHConnection(HWND hwnd, GUID serverGuid);
	~BTHConnection(void);
	void PrintWSAError(void);
	void Initialize(void);
	int StartListening(bool init);
	int StopListening(void);
	int WaitForAccept();
	int WaitForAccept(int loops);
	int CloseRemoteSocket();
	int ListenLoop(void);
	void eLog(LPWSTR lpstr);	
	void OnAccept(BTHConnection *c);
	void OnConnected(BTHConnection *c);
	int SetConnection();
	int StopConnection();	
	int Handshake(void);	
	void setConnected(void)
	{
		this->connectionState = BTHConnection::STATE_CONNECTED;
		postOnStatusChange(connectionState);
	}
	void setDisconnected(void)
	{
		this->connectionState = BTHConnection::STATE_DISCONNECTED;
		postOnStatusChange(connectionState);
	}
	void setHandshake(void)
	{
		this->connectionState = BTHConnection::STATE_HANDSHAKE;
		postOnStatusChange(connectionState);
	}
	void setWaitForAccept(void)
	{
		this->connectionState = BTHConnection::STATE_WAITINGFORACCEPT;
		postOnStatusChange(connectionState);
	}

	//BTHConnection::BTHService *readService;
	//BTHConnection::BTHService *writeService;
	SOCKET ListenSOCKET;
	SOCKET ClientSOCKET;	
	HWND hWnd;
	LPWSAQUERYSET wsaQuerySet;
	int connectionState;
	void sendInitialized(void);
	bool isHandshake()
	{
		return (connectionState == BTHConnection::STATE_HANDSHAKE);
	}
	bool isConnected()
	{
		return (connectionState == BTHConnection::STATE_CONNECTED);
	}
	bool isDisconnected()
	{
		return (connectionState == BTHConnection::STATE_DISCONNECTED);
	}
	bool isWaitingForAccept()
	{
		return (connectionState == BTHConnection::STATE_WAITINGFORACCEPT);		
	}


	static const int STATE_DISCONNECTED = 10;
	static const int STATE_WAITINGFORACCEPT = 11;
	static const int STATE_HANDSHAKE = 12;
	static const int STATE_CONNECTED = 13;	
	
private:
	//void InitReadService(LPVOID data);
	/*void InitWriteService(LPVOID data);
	void CloseReadService(void);
	void CloseWriteService(void);
	void CloseThread(DWORD dwID, HANDLE h);
	DWORD createThd(LPVOID procedure, LPVOID data, HANDLE h);*/
	
	void sendCloseError(void);
	int iLastError;
	bool ifwsaInit;
	bool ifStarted;
	bool ifSetup;
	GUID guid;
	DWORD dwReadThread;
	DWORD dwWriteThread;
	HANDLE hReadThread;
	HANDLE hWriteThread;
	void postOnStatusChange(int _status)
	{
		PostMessage(hWnd, WM_USER, (WPARAM)&_status, (LPARAM)BTHC_STATUS_CHANGE);
	}
	void postNODEVICE(void)
	{
		PostMessage(hWnd, WM_USER, NULL, (LPARAM)BTHC_NODEVICE);
	}
	
	
	
};



/*

[event_receiver(native)]
class CReceiver {
public:
   void MyHandler1(int nValue) {
      printf_s("MyHandler1 was called with value %d.\n", nValue);
   }

   void MyHandler2(int nValue) {
      printf_s("MyHandler2 was called with value %d.\n", nValue);
   }

   void hookEvent(CSource* pSource) {
      __hook(&CSource::MyEvent, pSource, &CReceiver::MyHandler1);
      __hook(&CSource::MyEvent, pSource, &CReceiver::MyHandler2);
   }

   void unhookEvent(CSource* pSource) {
      __unhook(&CSource::MyEvent, pSource, &CReceiver::MyHandler1);
      __unhook(&CSource::MyEvent, pSource, &CReceiver::MyHandler2);
   }
};

int main() {
   CSource source;
   CReceiver receiver;

   receiver.hookEvent(&source);
   __raise source.MyEvent(123);
   receiver.unhookEvent(&source);
}
*/