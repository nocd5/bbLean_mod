#include "bbp42ch.h"

BOOL WINAPI DllMainCRTStartup(HANDLE hModule, DWORD dwFunction, LPVOID lpNot){
    return TRUE;
}

int beginPlugin(HINSTANCE hPluginInstance){
	WNDCLASS wc;
	ZeroMemory((void*)&wc, sizeof(wc));
	wc.lpszClassName = strVersion;
	wc.hInstance     = hPluginInstance;
	wc.lpfnWndProc   = MainWinProc;
	RegisterClass(&wc);

	g_hPluginWnd = CreateWindow(
		strVersion,
		NULL,
		WS_POPUP,
		0, 0, 0, 0,
		NULL,
		NULL,
		hPluginInstance,
		NULL
	);

	g_BBhWnd = GetBBWnd();
	SendMessage(g_BBhWnd, BB_REGISTERMESSAGE, (WPARAM)g_hPluginWnd, (LPARAM)msgs);

	return 0;
}

// -----------------------------------------------
void endPlugin(HINSTANCE hPluginInstance){
	SendMessage(g_BBhWnd, BB_UNREGISTERMESSAGE, (WPARAM)g_hPluginWnd, (LPARAM)msgs);
	DestroyWindow(g_hPluginWnd);
	UnregisterClass(strVersion, hPluginInstance);
}

// -----------------------------------------------
LPCSTR pluginInfo(int field){
	switch (field)
	{
		default:
		case 0: return strVersion;
		case 1: return strAppName;
		case 2: return strInfoVersion;
		case 3: return strInfoAuthor;
		case 4: return strInfoRelDate;
		case 5: return strInfoLink;
		case 6: return strInfoEmail;
	}
}

// -----------------------------------------------
BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lParam)
{
	TCHAR szClassName[MAX_PATH];
	GetClassName(hWnd, szClassName, MAX_PATH);
	if (!StrCmp("BBMenu", szClassName))
		PostMessage(hWnd, WM_KEYDOWN, VK_DELETE, 0);
	return TRUE;
}

LRESULT CALLBACK MainWinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
	LPSTR message = (LPSTR)lParam;
	LPSTR command = message + 8;
	switch (uMsg){
		case BB_BROADCAST :
			if (!StrCmpNI(message+1, strAppName, 7)){ // compare bro@m to "@bbp42ch"
				// HideAllMenu
				if (!StrCmpI(".HideAllMenu", command)){
					EnumWindows(EnumWindowsProc, 0);
				}
				// SetText
				if (!StrCmpNI(".SetText", command, 8)){
					LPSTR option = command + 8;
					while (*option == ' ') option++;
					HWND hControl = GetFocus();
					if (NULL != hControl){
						PathUnquoteSpaces(option);
						SetWindowText(hControl, option);
						PostMessage(hControl, WM_KEYDOWN, VK_END, 0);
					}
				}
				if (!StrCmpI(".BBQuitDirect", command)){
					SendMessage(g_BBhWnd, BB_EXITTYPE, 0, 0);
					SendMessage(g_BBhWnd, WM_QUERYENDSESSION, 0, 0);
					PostMessage(g_BBhWnd, WM_ENDSESSION, true, 0);
					PostQuitMessage(0);
				}
			}
			break;
		case BB_RECONFIGURE:
			break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

