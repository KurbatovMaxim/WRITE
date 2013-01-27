#pragma once
#pragma comment(lib, "wsock32.lib")

#include "resource.h"
#include <shellapi.h>
#include <strsafe.h>

// Defined messages
#define WM_TRAY_ICON_MSG (WM_USER + 1)

#define MAX_LOADSTRING 100
#define EVT_START  0
#define EVT_LOGIN  1
#define EVT_LOGOFF 2
#define EVT_STOP   3
#define EVT_TIMER  4

#define TITLE_SIZE 500

#define SERVER_IP "192.168.106.84"
#define SERVER_PORT 65211
#define TIMER_SEC_QUANTITY 60000  // 60 secs

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
void                SessChange_Handler(HWND, int);
void                CloseProgram_handler(HWND);
void                OpenProgram_handler(HWND);
void                updateStat(time_t, bool);
void                startTimer(HWND);
void                handleCurrentTitle(int, int);
void                parseTitle( char*, char* );
void                sendToServer( char* );

void				Shell_CreateIcon(HINSTANCE hInstance, HWND hWnd);
void				RegWrite();

const unsigned int TIMER_SEC_ID = 1;


class TimesClass {
public:
	int lastAction; //really this value gives information about last checked moment in active comp
	TimesClass() {
		lastAction = 0;
	}
} Times;

void Shell_CreateIcon(HINSTANCE hInstance, HWND hWnd);