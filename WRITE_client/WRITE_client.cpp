// WRITE_client.cpp: _tWinMain entry point
//

#include "WRITE_client.h"
#pragma warning
using namespace std;


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

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDI_ICON1));

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
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Register Window class
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
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_WRITE_CLIENT);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON1));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves descriptor and creates a Window

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   //InitCommonControls();
   hInst = hInstance; // Save descriptor of the instance in global var

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);
   //UpdateHostFile();
   if (!hWnd)
   {
      return FALSE;
   }

   setlocale(LC_ALL, ".1251");
   ShowWindow(hWnd, SW_HIDE);
   Shell_CreateIcon(hInst, hWnd);
   startTimer(hWnd);

   OpenProgram_handler(hWnd);
   return TRUE;
}


BOOL OnInitDialog(HWND hwnd)
{
	HMENU hmenu = GetSystemMenu(hwnd,FALSE);
	if(hmenu)
	{

	}
	return FALSE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Process messages

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
		case WM_GOTO_WEB:
			GoToSite();
			break;
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case WM_EXIT:
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		//***
		//  No need to add anything here at this point
		//***
		EndPaint(hWnd, &ps);
		break;
	case WM_TRAY_ICON_MSG:
		{
			switch(lParam)
			{
		case WM_RBUTTONDOWN:
		case WM_CONTEXTMENU:
				ShowContextMenu(hWnd);
				break;
			default:
				return DefWindowProc(hWnd, message, wParam, lParam);
			}
			break;
		}
	case WM_DESTROY:
		CloseProgram_handler(hWnd);
		PostQuitMessage(0);
		break;
	case WM_TIMER:     // TIMER Message
		if( wParam == TIMER_SEC_ID ) {
			SessChange_Handler(hWnd, EVT_TIMER);
		}	
		break;
		

	case WM_WTSSESSION_CHANGE:
		switch( wParam )
		{
			case WTS_CONSOLE_CONNECT:
				//MessageBox(hWnd, TEXT("WTS_CONSOLE_CONNECT"), 
					//	TEXT("WM_WTSSESSION_CHANGE"), MB_OK );
				break;
			case WTS_CONSOLE_DISCONNECT:
				//MessageBox(hWnd, TEXT("WTS_CONSOLE_DISCONNECT"), 
					//	TEXT("WM_WTSSESSION_CHANGE"), MB_OK );
				break;
			case WTS_SESSION_LOCK:
				SessChange_Handler(hWnd,EVT_LOGOFF); //session lock
				break;
			case WTS_SESSION_UNLOCK:
				SessChange_Handler(hWnd,EVT_LOGIN); //session unlock
				break;
			default:
				//MessageBox(hWnd, TEXT("WTS_SESSION_UNLOCK"), 
					//	TEXT("WM_WTSSESSION_CHANGE"), MB_OK );
				break;
		}
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// About callback
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


void Shell_CreateIcon(HINSTANCE hInstance, HWND hWnd)
{
    // Add a Shell_NotifyIcon notificaion
    NOTIFYICONDATA nid = {0};
    nid.cbSize         = sizeof(nid);
    nid.uID            = TRAYICON_ID;      // Per Windows Embedded CE docs, values from 0 to 12 are reserved and should not be used.
    nid.hWnd = hWnd;
	nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
    nid.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	nid.uCallbackMessage = WM_TRAY_ICON_MSG;
	StringCchCopy(nid.szTip, ARRAYSIZE(nid.szTip), L"HC statistics");
    // Add the notification to the tray.
    Shell_NotifyIcon(NIM_ADD, &nid);
    return;
}

void Shell_IconModified(HINSTANCE hInstance, HWND hWnd)
{

NOTIFYICONDATA nid;
nid.cbSize = sizeof(NOTIFYICONDATA);
nid.hWnd = hWnd;nid.uID = TRAYICON_ID;
wcscpy_s(nid.szTip, L"Modified Tip");
nid.uFlags = NIF_TIP;
Shell_NotifyIcon(NIM_MODIFY, &nid);


}

void ShowContextMenu(HWND hWnd)
{
	POINT pt;
	GetCursorPos(&pt);
	HMENU hMenu = CreatePopupMenu();

	if(hMenu)
	{
		InsertMenu(hMenu,-1,MF_BYPOSITION,WM_GOTO_WEB,_T("Open Site"));
		InsertMenu(hMenu,-1,MF_BYPOSITION,WM_EXIT,_T("Close"));
	}
	SetForegroundWindow(hWnd);
	TrackPopupMenu(hMenu, TPM_BOTTOMALIGN,pt.x, pt.y, 0, hWnd, NULL );
	DestroyMenu(hMenu);
	return;
}

void GoToSite()
{
	ShellExecute(NULL, L"open", WRITE_SITE_URL,NULL, NULL, SW_SHOWNORMAL);
}

void UpdateHostFile()
{

  fstream filestr;
  filestr.open ("C:\\Windows\\System32\\drivers\\etc\\hosts", fstream::in | fstream::out | fstream::app);
  if (filestr.is_open())
  {
    filestr << HOST_FILE_UPDATE;
	filestr << "\n";

  }
  filestr.close();




}