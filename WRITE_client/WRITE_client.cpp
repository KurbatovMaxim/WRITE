// WRITE_client.cpp: определяет точку входа для приложения.
//


#include "stdafx.h"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <ctime>


#include <windows.h>
#include <WtsApi32.h>
#include <winsock2.h> // Включаем использование сокетов
#include <Shellapi.h>
#include <strsafe.h>

#include "WRITE_client.h"

#define MAX_LOADSTRING 100


// Отправить объявления функций, включенных в этот модуль кода:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

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
	LoadString(hInstance, IDC_WRITE_CLIENT, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDI_WORKSTATISTIC));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}




//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  НАЗНАЧЕНИЕ: регистрирует класс окна.
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
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WRITE_CLIENT));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_WRITE_CLIENT);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   НАЗНАЧЕНИЕ: сохраняет обработку экземпляра и создает главное окно.
//
//   КОММЕНТАРИИ:
//
//        В данной функции дескриптор экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится на экран главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Сохранить дескриптор экземпляра в глобальной переменной

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }
     setlocale(LC_ALL, ".1251");
   ShowWindow(hWnd, SW_HIDE);
   Shell_CreateIcon(hInst, hWnd);
   //UpdateWindow(hWnd);
   startTimer(hWnd);

   OpenProgram_handler(hWnd);
   return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  НАЗНАЧЕНИЕ:  обрабатывает сообщения в главном окне.
//
//  WM_COMMAND	- обработка меню приложения
//  WM_PAINT	-Закрасить главное окно
//  WM_DESTROY	 - ввести сообщение о выходе и вернуться.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		CloseProgram_handler(hWnd);
		PostQuitMessage(0);
		break;
	case WM_TIMER:     // обработка сообщения WM_TIMER
		if( wParam == TIMER_SEC_ID ) {
			SessChange_Handler(hWnd, EVT_TIMER);
		}	
		break;

	case WM_WTSSESSION_CHANGE:
		switch( wParam )
		{
			case WTS_CONSOLE_CONNECT:
				MessageBox(hWnd, TEXT("WTS_CONSOLE_CONNECT"), 
						TEXT("WM_WTSSESSION_CHANGE"), MB_OK );
				break;
			case WTS_CONSOLE_DISCONNECT:
				MessageBox(hWnd, TEXT("WTS_CONSOLE_DISCONNECT"), 
						TEXT("WM_WTSSESSION_CHANGE"), MB_OK );
				break;
			case WTS_SESSION_LOCK:
				SessChange_Handler(hWnd,EVT_LOGOFF); //sess lock
				break;
			case WTS_SESSION_UNLOCK:
				SessChange_Handler(hWnd,EVT_LOGIN); //sess unlock
				break;
			default:
				MessageBox(hWnd, TEXT("WTS_SESSION_UNLOCK"), 
						TEXT("WM_WTSSESSION_CHANGE"), MB_OK );
				break;
		}
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

void SessChange_Handler(HWND hWnd, int flag) {  //0 - start program, 1 - unlock, 2 - lock, 3 - close program

	time_t seconds = time(NULL);
	int timeslot = 0;

	if( Times.lastAction == 0 ) {
		Times.lastAction = seconds;
	}

	timeslot = seconds - Times.lastAction;
	Times.lastAction = seconds;

	switch ( flag )
	{
		case EVT_START: { // start of prog
			break;
		}
		case EVT_LOGIN: { // login
			startTimer(hWnd);
			break;
		}
		case EVT_LOGOFF: { // logout
			//handleCurrentTitle(timeslot,EVT_LOGOFF);
			KillTimer (hWnd,TIMER_SEC_ID);
			break;
		}
		case EVT_STOP: { // stop prog
			//handleCurrentTitle(timeslot,EVT_STOP);
			KillTimer (hWnd,TIMER_SEC_ID);
			break;
		}
		case EVT_TIMER: { // timer triggered
			handleCurrentTitle(timeslot,EVT_TIMER);
			break;
		}		
	}
	return;
}


void startTimer(HWND hWnd) {
	SetTimer(hWnd,TIMER_SEC_ID,TIMER_SEC_QUANTITY,NULL);
}

void CloseProgram_handler(HWND hWnd) {
	SessChange_Handler(hWnd,EVT_STOP);
}

void OpenProgram_handler(HWND hWnd) {
	SessChange_Handler(hWnd,EVT_START);
}

void handleCurrentTitle(int timeslot, int reason) {

	if( timeslot == 0 ) {
		return;
	}

	HWND CurWin = GetForegroundWindow();

	char temptitle[TITLE_SIZE] = {0};
	char restitle[TITLE_SIZE+10] = {0};
	char outtitle[TITLE_SIZE] = {0};
	char buft[TITLE_SIZE] = {0};

	GetWindowText(CurWin,(LPWSTR)temptitle,TITLE_SIZE);
	wcstombs ( buft, (LPWSTR)temptitle, TITLE_SIZE );

	sprintf(restitle, "%d %s\n",timeslot,buft);

	parseTitle(restitle,outtitle);
	sendToServer(restitle);

	return;
}

void parseTitle( char* bufin, char* bufout ) {
	return;
}

void sendToServer( char* strs )
{
WSADATA wsa_data; // Информация о сокетах
SOCKET s; // Серверный сокет

s = NULL;

// 1. Инициализируем сокеты (требуем версию 1.1 как минимум)
if (WSAStartup(0x101,&wsa_data) != 0) return;

// 2. Открываем серверный сокет
s = socket(AF_INET, SOCK_STREAM, 0);
if (s == INVALID_SOCKET) return;

// 3. Привязываем сокет к адресу
sockaddr_in addr; // Для хранения адреса
memset(&addr, 0, sizeof(addr));
addr.sin_family = AF_INET;
addr.sin_addr.S_un.S_addr = inet_addr(SERVER_IP);
addr.sin_port = htons(SERVER_PORT); // Наш порт, который будем открывать для коннектов
if (bind(s, (LPSOCKADDR)&addr, sizeof(addr)) != SOCKET_ERROR) return;

// 4. Соединяемся
if (connect(s, (LPSOCKADDR)&addr, sizeof(addr)) == SOCKET_ERROR)
return;

// 5. Обмениваемся данными
// (получаем приветствие)
char buf[1024];
int len = recv(s, buf, 1024, 0);
if (len == SOCKET_ERROR) return;

buf[len] = 0;

// Отсылаем ответ серверу
if( strs != NULL )
	send(s, strs, (int)strlen(strs), 0);

// 6. Закрываем сокет
closesocket(s);

// 7. Завершаем работу с сокетами
WSACleanup();
}


void Shell_CreateIcon(HINSTANCE hInstance, HWND hWnd)
{
    // Add a Shell_NotifyIcon notificaion
    NOTIFYICONDATA nid = {0};
    nid.cbSize         = sizeof(nid);
    nid.uID            = 100;      // Per Windows Embedded CE docs, values from 0 to 12 are reserved and should not be used.
    nid.hWnd = hWnd;
	nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
    nid.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	nid.uCallbackMessage = WM_TRAY_ICON_MSG;
	StringCchCopy(nid.szTip, ARRAYSIZE(nid.szTip), L"HC statistics");
    // Add the notification to the tray.
    Shell_NotifyIcon(NIM_ADD, &nid);
    return;
}

void RegWrite() {
	return;
}