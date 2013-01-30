#include "Network_utils.h"


//
// Network fuctions
//


void sendToServer( char* strs )
{
WSADATA wsa_data; // SOCKET info
SOCKET s; // SERVER SOCKET

s = NULL;

// 1. Initialization. 1.1 version requiered
if (WSAStartup(0x101,&wsa_data) != 0) return;

// 2. Opening connection
s = socket(AF_INET, SOCK_STREAM, 0);
if (s == INVALID_SOCKET) return;

// 3. Binding
sockaddr_in addr; // ADDRESS storage
memset(&addr, 0, sizeof(addr));
addr.sin_family = AF_INET;
addr.sin_addr.S_un.S_addr = inet_addr(SERVER_IP);
addr.sin_port = htons(SERVER_PORT); // SERVER port
if (bind(s, (LPSOCKADDR)&addr, sizeof(addr)) != SOCKET_ERROR) return;

// 4. COnnecting...
if (connect(s, (LPSOCKADDR)&addr, sizeof(addr)) == SOCKET_ERROR)
return;

// 5.Get's HELLO
char buf[1024];
int len = recv(s, buf, 1024, 0);
if (len == SOCKET_ERROR) return;

buf[len] = 0;

// Send response to server
if( strs != NULL )
	send(s, strs, (int)strlen(strs), 0);

// 6. Close socket
closesocket(s);

// 7. CLEANING up
WSACleanup();
}