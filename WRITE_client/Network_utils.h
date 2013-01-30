
#pragma comment(lib, "wsock32.lib") //link WinSock

#include "stdafx.h"
#include <winsock2.h> // SOCKET lib

#define SERVER_IP "192.168.106.84"
#define SERVER_PORT 65211
#define TIMER_SEC_QUANTITY 60000  // 60 secs

void SendToServer(char*);