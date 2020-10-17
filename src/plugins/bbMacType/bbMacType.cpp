/*---------------------------------------------------------------------------------
 bbMacType is a plugin for bbLean
 ----------------------------------------------------------------------------------
 bbMacType is free software, released under the GNU General Public License,
 version 2, as published by the Free Software Foundation.  It is distributed
 WITHOUT ANY WARRANTY--without even the implied warranty of MERCHANTABILITY
 or FITNESS FOR A PARTICULAR PURPOSE.  Please see the GNU General Public
 License for more details:  [http://www.fsf.org/licenses/gpl.html].
 --------------------------------------------------------------------------------*/
#include "bbMacType.h"

// --------------------------------------------------------------------------------
BOOL WINAPI DllMainCRTStartup(HANDLE hModule, DWORD dwFunction, LPVOID lpNot){
	return TRUE;
}

// BOOL CALLBACK EnumWindowProc(HWND hWnd, LPARAM lParam){
// 	return TRUE;
// }

static void read_settings(LPCSTR RCFilePath, RCSETTINGS *pRCSettings){
	int (*pReadInt)(LPCSTR, LPCSTR, int) =
		(int(*)(LPCSTR, LPCSTR, int))GetProcAddress((HINSTANCE)GetModuleHandle(NULL), "ReadInt");
	if (pReadInt){
		pRCSettings->dwHookType = pReadInt(RCFilePath, RC_KEY_HOOK_TYPE, WH_GETMESSAGE);
	}

	LPCSTR (*pReadString)(LPCSTR, LPCSTR, LPCSTR) =
		(LPCSTR(*)(LPCSTR, LPCSTR, LPCSTR))GetProcAddress((HINSTANCE)GetModuleHandle(NULL), "ReadString");
	if (pReadString){
		LPCSTR buf = pReadString(RCFilePath, RC_KEY_HOOK_LIBRARY, szMacType);
		pRCSettings->pszHookLibrary = (LPSTR)malloc((strlen(buf)+1)*sizeof(char));
		strcpy((LPSTR)pRCSettings->pszHookLibrary, buf);
		buf = pReadString(RCFilePath, RC_KEY_HOOK_PROC, "GetMsgProc");
		pRCSettings->pszHookProc = (LPSTR)malloc((strlen(buf)+1)*sizeof(char));
		strcpy((LPSTR)pRCSettings->pszHookProc, buf);
    }
}

static void free_settings(RCSETTINGS *pRCSettings){
	if (pRCSettings->pszHookLibrary != NULL) free(pRCSettings->pszHookLibrary);
	if (pRCSettings->pszHookProc    != NULL) free(pRCSettings->pszHookProc);
	ZeroMemory((void*)&g_RCSettings, sizeof(RCSETTINGS));
}

// --------------------------------------------------------------------------------
int beginPlugin(HINSTANCE hPluginInstance){
	WNDCLASS wc;
	ZeroMemory((void*)&wc, sizeof(WNDCLASS));
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

	HWND (*pGetBBWnd)() = (HWND(*)())GetProcAddress((HINSTANCE)GetModuleHandle(NULL), "GetBBWnd");
	g_BBhWnd = pGetBBWnd();
	SendMessage(g_BBhWnd, BB_REGISTERMESSAGE, (WPARAM)g_hPluginWnd, (LPARAM)msgs);

	// backup current directory
	char szCurDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, szCurDir);

	int nLen;
	// get DLL path
	char szMyPath[MAX_PATH];
	nLen = GetModuleFileName(hPluginInstance, szMyPath, MAX_PATH);
	while (nLen && szMyPath[nLen-1] != '\\') nLen--; szMyPath[nLen] = '\0';

	// change current directory
	SetCurrentDirectory(szMyPath);

	// read settings
	ZeroMemory((void*)&g_RCSettings, sizeof(RCSETTINGS));
	// now, current directory is plugin's directory
	read_settings(szAppName ".rc", &g_RCSettings);

	// load MacType.dll
	int nResult = 1;
	g_hMacType = LoadLibrary(g_RCSettings.pszHookLibrary);
	if (g_hMacType){
		HOOKPROC pHookProc = (HOOKPROC)GetProcAddress(g_hMacType, g_RCSettings.pszHookProc);
		if (pHookProc){
			g_hHook = SetWindowsHookEx(g_RCSettings.dwHookType, pHookProc, g_hMacType, 0);
			// EnumWindows(EnumWindowProc, 0);
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
	free_settings(&g_RCSettings);

	if (g_hHook) UnhookWindowsHookEx(g_hHook);
	if (g_hMacType) FreeLibrary(g_hMacType);

	SendMessage(HWND_BROADCAST, WM_NULL, 0, 0);
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

