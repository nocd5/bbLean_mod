#include <windows.h>
#include <commctrl.h>
#include "bbMLpe.h"

#define DLL_NAME "bbMemLimiter.dll"

bool bLock = false;
/////////////////////////////////////////////////////////////
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
	if (!CheckDllVersion()){
		MessageBox(NULL, "DLL is not invalid version", "Load Error", MB_OK | MB_ICONERROR);
		return 0;
	}

	if (FILE *fp = fopen(DLL_NAME, "rb+")){
		fclose(fp);
	}
	else{
//         MessageBox(NULL, "DLL is not writable.\n\nif you load bbMemLimiter.dll, please unload it.", "Load Error", MB_OK | MB_ICONERROR);
		bLock = true;
	}

	WNDCLASS winc;
	winc.style         = CS_HREDRAW | CS_VREDRAW;
	winc.lpfnWndProc   = WndProc;
	winc.cbClsExtra    = 0;
	winc.cbWndExtra    = 0;
	winc.hInstance     = hInstance;
	winc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
	winc.hCursor       = LoadCursor(NULL, IDC_ARROW);
	winc.hbrBackground = GetSysColorBrush(COLOR_MENU);
	winc.lpszMenuName  = NULL;
	winc.lpszClassName = "DefWndCls";

	RegisterClass(&winc);
	HWND hWnd = CreateWindow(winc.lpszClassName,
		szAppName,
		WS_OVERLAPPED,
		0, 0, 210, 160,
		NULL,
		NULL,
		winc.hInstance,
		NULL);

	MSG msg;
	while(BOOL bRet = GetMessage(&msg, NULL, 0, 0)){
		if (bRet == 0 || bRet == -1){
			break;
		}
		else if (!IsDialogMessage(hWnd, &msg)){
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return msg.wParam;
}

/////////////////////////////////////////////////////////////
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
	static RECT rect;

	fpos_t posLimitSize    = POS_LIMIT_SIZE;
	fpos_t posIntervalTime = POS_INTERVAL_TIME;

	unsigned char byLS[3], byTI[2];

	switch(uMsg)
	{
		case WM_CREATE: {
			HFONT hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);

			CreateWindowEx(0, WC_EDIT, 0,
				WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL | WS_TABSTOP | (bLock ? ES_READONLY : 0),
				26, 23, 130, 20,
				hWnd,
				(HMENU)ID_LSVALUE,
				((LPCREATESTRUCT)lParam)->hInstance,
				NULL
				);

			CreateWindowEx(0, WC_EDIT, 0,
				WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL | WS_TABSTOP | (bLock ? ES_READONLY : 0),
				26, 75, 130, 20,
				hWnd,
				(HMENU)ID_ITVALUE,
				((LPCREATESTRUCT)lParam)->hInstance,
				NULL
				);

			CreateWindowEx(0, WC_STATIC, "limit size (0 - 65535)",
				WS_CHILD | WS_VISIBLE | SS_LEFT,
				6, 3, 130, 20,
				hWnd,
				(HMENU)ID_LSLABEL,
				((LPCREATESTRUCT)lParam)->hInstance,
				NULL
				);

			CreateWindowEx(0, WC_STATIC, "interval time (0 - 65535)",
				WS_CHILD | WS_VISIBLE | SS_LEFT,
				6, 55, 130, 20,
				hWnd,
				(HMENU)ID_ITLABEL,
				((LPCREATESTRUCT)lParam)->hInstance,
				NULL
				);

			CreateWindowEx(0, WC_STATIC, "[KB]",
				WS_CHILD | WS_VISIBLE | SS_LEFT,
				160,  25, 50, 20,
				hWnd,
				(HMENU)ID_LSUNIT,
				((LPCREATESTRUCT)lParam)->hInstance,
				NULL
				);
			CreateWindowEx(0, WC_STATIC, "[ms]",
				WS_CHILD | WS_VISIBLE | SS_LEFT,
				160,  77, 50, 20,
				hWnd,
				(HMENU)ID_ITUNIT,
				((LPCREATESTRUCT)lParam)->hInstance,
				NULL
				);

			CreateWindowEx(0, WC_BUTTON, "OK",
				WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_TABSTOP,
				46, 100, 70, 20,
				hWnd,
				(HMENU)IDOK,
				((LPCREATESTRUCT)lParam)->hInstance,
				NULL
				);

			CreateWindowEx(0, WC_BUTTON, "CANCEL",
				WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_TABSTOP,
				126, 100, 70, 20,
				hWnd,
				(HMENU)IDCANCEL,
				((LPCREATESTRUCT)lParam)->hInstance,
				NULL
				);

			for(controls i = control_null; i < control_size; ++i) {
				SendMessage(GetDlgItem(hWnd, i), WM_SETFONT, (WPARAM)hFont, true);
			}

			if (FILE *fp = fopen(DLL_NAME, "rb")){
				fsetpos(fp, &posLimitSize);
				fread(byLS, sizeof(byLS[0]), sizeof(byLS)/sizeof(byLS[0]), fp);
				fsetpos(fp, &posIntervalTime);
				fread(byTI, sizeof(byTI[0]), sizeof(byTI)/sizeof(byTI[0]), fp);
				fclose(fp);
			}
			else{
				MessageBox(NULL, "DLL is not invalid version", "load error", MB_OK | MB_ICONERROR);
			}

			char buf[6];
			SetDlgItemText(hWnd, ID_LSVALUE, _itoa((byLS[2]<<14) + (byLS[1]<<6) + (byLS[0]>>2), &buf[0], 10));
			SetDlgItemText(hWnd, ID_ITVALUE, _itoa((byTI[1]<<8) + byTI[0], &buf[0], 10));

			RECT rc;
			GetWindowRect(hWnd, &rc);
			int nCX = GetSystemMetrics(SM_CXSCREEN);
			int nCY = GetSystemMetrics(SM_CYSCREEN);
			SetWindowPos(hWnd, NULL, (nCX-rc.right+rc.left)/2, (nCY-rc.bottom+rc.top)/2, rc.right, rc.bottom, false);
			ShowWindow(hWnd, SW_SHOW);
			break;
		}

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		case WM_COMMAND:
			switch(LOWORD(wParam)){
				case IDOK: {
					char strLimitSize[10+1];
					char strIntervalTime[10+1];
					GetDlgItemText (hWnd, ID_LSVALUE, strLimitSize,    10+1);
					GetDlgItemText (hWnd, ID_ITVALUE, strIntervalTime, 10+1);
					unsigned long ulLimitSize = strtoul(strLimitSize, NULL, 0);
					unsigned long ulIntervalTime = strtoul(strIntervalTime, NULL, 0);
					if (
							(ulLimitSize     < 0 | 0xffffL < ulLimitSize) |
							(ulIntervalTime  < 0 | 0xffffL < ulIntervalTime)
					   ){
						MessageBox(NULL, "invalid value", "Error", MB_OK | MB_ICONERROR);
						break;
					}

					if (FILE *fp = fopen(DLL_NAME, "rb+")){
						byLS[0] =  ulLimitSize<<2 & 0x0000ff;
						byLS[1] = (ulLimitSize<<2 & 0x00ff00) >> 8;
						byLS[2] = (ulLimitSize<<2 & 0xff0000) >> 16;
						byTI[0] =  ulIntervalTime & 0x00ff;
						byTI[1] = (ulIntervalTime & 0xff00) >> 8;

						fsetpos(fp, &posLimitSize);
						fwrite((const void*)byLS, sizeof(byLS[0]), sizeof(byLS)/sizeof(byLS[0]), fp);
						fsetpos(fp, &posIntervalTime);
						fwrite((const void*)byTI, sizeof(byTI[0]), sizeof(byTI)/sizeof(byTI[0]), fp);
						fclose(fp);
					}
					PostQuitMessage(0);
					break;
				}

				case IDCANCEL:
					PostQuitMessage(0);
					break;

				default:
					break;
			}
			break;

		case WM_LBUTTONDOWN:
			// タイトルバー以外のところでもウィンドウの移動ができる
			PostMessage(hWnd, WM_SYSCOMMAND, SC_MOVE | 0x02, 0);
			break;

		default:
			break;
	}
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

/////////////////////////////////////////////////////////////
bool CheckDllVersion(){
	bool bDllVersion = false;
	if (HINSTANCE hInstDLL = LoadLibrary(DLL_NAME)){
		if (LPCSTR (*pPluginInfo)(int) = (LPCSTR(*)(int))GetProcAddress(hInstDLL,"pluginInfo")){
			if (0 == strcmp(szVersion, pPluginInfo(0))){
				bDllVersion = true;
			}
		}
		FreeLibrary(hInstDLL);
	}
	return bDllVersion;
}
