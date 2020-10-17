/*---------------------------------------------------------------------------------
 bbIMEChanger (© 2008 nocd5)
 ----------------------------------------------------------------------------------
 bbMemLimiter is a plugin for bbLean
 ----------------------------------------------------------------------------------
 bbMemLimiter is free software, released under the GNU General Public License,
 version 2, as published by the Free Software Foundation.  It is distributed
 WITHOUT ANY WARRANTY--without even the implied warranty of MERCHANTABILITY
 or FITNESS FOR A PARTICULAR PURPOSE.  Please see the GNU General Public
 License for more details:  [http://www.fsf.org/licenses/gpl.html].
 --------------------------------------------------------------------------------*/

#include "bbIMEChanger.h"

//-------------------------------------------------------------------------------------
BOOL WINAPI DllMainCRTStartup(HANDLE hModule, DWORD dwFunction, LPVOID lpNot){
    return TRUE;
}

//-------------------------------------------------------------------------------------
int beginPlugin(HINSTANCE hPluginInstance){
	WNDCLASS wc;
	ZeroMemory((void*)&wc, sizeof(wc));
	wc.lpszClassName = szVersion;
	wc.hInstance = hPluginInstance;
	wc.lpfnWndProc = WndProc;
	RegisterClass(&wc);

	hPluginWnd = CreateWindow(
		szVersion,
		NULL,
		WS_POPUP,
		0, 0, 0, 0,
		HWND_MESSAGE,
		NULL,
		hPluginInstance,
		NULL
	);

	return 0;
}

//-------------------------------------------------------------------------------------
void endPlugin(HINSTANCE hPluginInstance){
	DestroyWindow(hPluginWnd);
	UnregisterClass(szVersion, hPluginInstance);
}

//-------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){
	static void (*LoadSettings)(char*);
	static bool (*SetHook)();
	static void (*UnHook)();
	switch (message){
		case WM_CREATE:
			// set hot-key Ctrl-SPACE
			RegisterHotKey(hWnd, ID_HOTKEY, MOD_CONTROL, VK_SPACE);

			// load SetCaretColor_dll.dll
			char szMyPathBuff[MAX_PATH];
			set_my_path(((LPCREATESTRUCT)lParam)->hInstance, szMyPathBuff, "SetCaretColor_dll.dll");
			if (hHookDll = LoadLibrary(szMyPathBuff)){
				// get SetCaretColor_dll's Function
				*(FARPROC*)&LoadSettings = GetProcAddress(hHookDll, "LoadSettings");
				*(FARPROC*)&SetHook      = GetProcAddress(hHookDll, "SetHook");
				*(FARPROC*)&UnHook       = GetProcAddress(hHookDll, "UnHook");
				// for DLL-load error message
				int (*BBMessageBox)(int flg, const char *fmt, ...);
				*(FARPROC*)&BBMessageBox = GetProcAddress(GetModuleHandle(NULL), "BBMessageBox");

				if (!(LoadSettings && SetHook && UnHook)){ 
					// some function don't find
					FreeLibrary(hHookDll);
					hHookDll = NULL;
					if (BBMessageBox)
						BBMessageBox(MB_OK, "Error: Unable to load DLL \"%s\"", szMyPathBuff);
				}
				else{
					// Set Hook
					if (SetHook()){
						// Load Settings
						set_my_path(((LPCREATESTRUCT)lParam)->hInstance, szMyPathBuff, "SetCaretColor.ini");
						LoadSettings(szMyPathBuff);
					}
					else{
						// can't do SetHook
						FreeLibrary(hHookDll);
						hHookDll = NULL;
						if (BBMessageBox)
							BBMessageBox(MB_OK, "Error: Unable to hook DLL \"%s\"", szMyPathBuff);
					}
				}
			}
			break;

		case WM_DESTROY:
			// unload SetCaretColor_dll.dll
			if (hHookDll){
				UnHook();
				FreeLibrary(hHookDll);
				hHookDll = NULL;
			}
			// unset hot-key
			UnregisterHotKey(hPluginWnd, ID_HOTKEY);
			break;

		case WM_HOTKEY:
			keybd_event(VK_KANJI, 0, 0, 0);
			return 0;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

//-------------------------------------------------------------------------------------
char *set_my_path(HINSTANCE hInstance, char *path, char *fname){
	int nLen = GetModuleFileName(hInstance, path, MAX_PATH);
	while (nLen && path[nLen-1] != '\\') nLen--;
	lstrcpy(path+nLen, fname);
	return path;
}

//-------------------------------------------------------------------------------------
LPCSTR pluginInfo (int field){
	switch (field){
		case 0  : return szVersion;
		case 1  : return szAppName;
		case 2  : return szInfoVersion;
		case 3  : return szInfoAuthor;
		case 4  : return szInfoRelDate;
		default : return "";
	}
}
