#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#pragma warning (disable: 4995)

#include "stdafx.h"
#include "resource.h"
#include "Network_utils.h"
#include <shellapi.h>
#include <commctrl.h>
#include <strsafe.h>

#include <iostream>
#include <stdlib.h>
#include <fstream>

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <ctime>
#include <windows.h>
#include <WtsApi32.h>

#define MAX_LOADSTRING 100
#define EVT_START  0
#define EVT_LOGIN  1
#define EVT_LOGOFF 2
#define EVT_STOP   3
#define EVT_TIMER  4

#define TRAYICON_ID 100

#define WRITE_SITE_URL L"http://n102851"
#define HOST_FILE_UPDATE "n102851 www.ccweekler.com"

const unsigned int TIMER_SEC_ID = 1;


#include <strsafe.h>

// Defined messages
#define WM_TRAY_ICON_MSG	(WM_USER + 1)
#define WM_GOTO_WEB			(WM_APP + 1)
#define WM_EXIT				(WM_APP+ 2 )


#define TITLE_SIZE 500



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
void				Shell_IconModified(HINSTANCE hInstance, HWND hWnd);
void				ShowContextMenu(HWND hwnd);
void				RegWrite();
void				GoToSite();
void				UpdateHostFile();

class TimesClass {
public:
	int lastAction; //really this value gives information about last checked moment in active comp
	TimesClass() {
		lastAction = 0;
	}
} Times;