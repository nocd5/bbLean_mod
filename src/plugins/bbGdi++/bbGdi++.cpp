/*---------------------------------------------------------------------------------
 bbGdi++ is a plugin for bbLean
 ----------------------------------------------------------------------------------
 bbGdi++ is free software, released under the GNU General Public License,
 version 2, as published by the Free Software Foundation.  It is distributed
 WITHOUT ANY WARRANTY--without even the implied warranty of MERCHANTABILITY
 or FITNESS FOR A PARTICULAR PURPOSE.  Please see the GNU General Public
 License for more details:  [http://www.fsf.org/licenses/gpl.html].
 --------------------------------------------------------------------------------*/
#include "bbGdi++.h"

// --------------------------------------------------------------------------------
BOOL WINAPI DllMainCRTStartup(HANDLE hModule, DWORD dwFunction, LPVOID lpNot){
	return TRUE;
}

// --------------------------------------------------------------------------------
int beginPlugin(HINSTANCE hPluginInstance){
	WNDCLASS wc;
	ZeroMemory((void*)&wc, sizeof(wc));
	wc.lpszClassName = szVersion;
	wc.hInstance     = hPluginInstance;
	wc.lpfnWndProc   = DefWindowProc;
	RegisterClass(&wc);

	g_hPluginWnd = CreateWindow(
		szVersion,
		NULL,
		WS_POPUP,
		0, 0, 0, 0,
		HWND_MESSAGE,
		NULL,
		hPluginInstance,
		NULL
	);

	HWND (*GetBBWnd)() = (HWND(*)())GetProcAddress((HINSTANCE)GetModuleHandle(NULL), "GetBBWnd");
	g_BBhWnd = GetBBWnd();
	SendMessage(g_BBhWnd, BB_REGISTERMESSAGE, (WPARAM)g_hPluginWnd, (LPARAM)msgs);

	// backup current directory
	char szCurDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, szCurDir);

	// get DLL's path
	char szMyPath[MAX_PATH+(sizeof("gdi++.dll")/sizeof(char)-1)];
	int nLen = GetModuleFileName(hPluginInstance, szMyPath, MAX_PATH);
	while (nLen && szMyPath[nLen-1] != '\\') nLen--; szMyPath[nLen] = '\0';

	// change current directory
	SetCurrentDirectory(szMyPath);
	// load gdi++.dll
	int nResult = 1;
	g_hGdipp = LoadLibrary(lstrcat(szMyPath, "gdi++.dll"));
	if (g_hGdipp){
		HOOKPROC pHookProc = (HOOKPROC)GetProcAddress(g_hGdipp, "GetMsgProc");
		if (pHookProc){
			g_hHook = SetWindowsHookEx(WH_GETMESSAGE, pHookProc, g_hGdipp, 0);
			RedrawWindow(NULL, NULL, NULL, RDW_INVALIDATE | RDW_ERASE | RDW_ALLCHILDREN);
			nResult = 0;
		}
	}
	// restore current directory
	SetCurrentDirectory(szCurDir);
	if (nResult == 1) endPlugin(hPluginInstance);
	return nResult;
}

// --------------------------------------------------------------------------------
void endPlugin(HINSTANCE hPluginInstance){
	if (g_hHook){
		UnhookWindowsHookEx(g_hHook);
		PostMessage(HWND_BROADCAST, WM_NULL, 0, 0);
	}
	if (g_hGdipp) FreeLibrary(g_hGdipp);
	RedrawWindow(NULL, NULL, NULL, RDW_INVALIDATE | RDW_ERASE | RDW_ALLCHILDREN);
	SendMessage(g_BBhWnd, BB_UNREGISTERMESSAGE, (WPARAM)g_hPluginWnd, (LPARAM)msgs);
	DestroyWindow(g_hPluginWnd);
	UnregisterClass(szVersion, hPluginInstance);
}

// --------------------------------------------------------------------------------
LPCSTR pluginInfo (int field){
	switch (field){
		case 1  : return szAppName;
		case 2  : return szInfoVersion;
		case 3  : return szInfoAuthor;
		case 4  : return szInfoRelDate;
		default : return "";
	}
}

