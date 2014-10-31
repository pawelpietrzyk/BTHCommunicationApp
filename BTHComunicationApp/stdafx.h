// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once
#include "targetver.h"
#include "resource.h"
#include "Object.h"
#include "ByteBuffer.h"
#include "Log.h"
#include "Frame.h"
#include "Protocol.h"
//#include "json/json_spirit.h"
//#include "Json.h"

#include "Enums.h"

//#include "Frame.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#include <Windowsx.h>
#include <commctrl.h>

// C RunTime Header Files

#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <stdio.h>
#include <Rpc.h>
#include <Shellapi.h>
#include <Strsafe.h>
//#include <iostream>

//#include <vector>



//#include <winsock2.h>
//#include <ws2tcpip.h>
#include <iphlpapi.h>

//#include <Ws2bth.h>
//#include <Nspapi.h>


//#include <std>
//#include <list>
#include <string>

//#pragma comment(lib, "Winmm.lib")
//#pragma comment(lib, "comsuppw.lib")
//#pragma comment(lib, "Ws2_32.lib")
//#pragma comment(lib, "Rpcrt4.lib")
//#pragma comment(lib, "MSVCMRT.lib")




#define MAX_LOADSTRING	100
#define null			0
#define null_ptr		0xbaadf00d
#define null_cptr		(char*)0xbaadf00d



static const GUID guid = 
{ 0xe82f3b8b, 0x8dc0, 0x4669, { 0x9c, 0xe4, 0xc0, 0xbb, 0xc1, 0xb8, 0xd, 0xed } };
// {DD2BA902-ACFF-44ac-AF04-272344E1F1EE}
//static const GUID guid = 
//{ 0xdd2ba902, 0xacff, 0x44ac, { 0xaf, 0x4, 0x27, 0x23, 0x44, 0xe1, 0xf1, 0xee } };
