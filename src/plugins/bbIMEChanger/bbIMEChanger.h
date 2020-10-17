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

#define ID_HOTKEY 5

#ifndef _WIN32_WINNT
	#define _WIN32_WINNT 0x0500
#endif

#ifndef DLL_EXPORT
  #define DLL_EXPORT __declspec(dllexport)
#endif

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
//-------------------------------------------------------------------------------------
#define szVersion     "bbIMEChanger 0.0.5"
#define szAppName     "bbIMEChanger"
#define szInfoVersion "0.0.5"
#define szInfoAuthor  "nocd5"
#define szInfoRelDate "2008-11-23"

extern "C"{
	BOOL WINAPI DllMainCRTStartup(HANDLE hModule, DWORD dwFunction, LPVOID lpNot);
	DLL_EXPORT void endPlugin(HINSTANCE);
	DLL_EXPORT int beginPlugin(HINSTANCE);
	DLL_EXPORT LPCSTR pluginInfo(int index);
};

//---------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
HWND BBhWnd = NULL;
HWND hPluginWnd = NULL;
HINSTANCE hHookDll = NULL;
char *set_my_path(HINSTANCE hInstance, char *path, char *fname);
//---------------------------------------------------------------------------------
