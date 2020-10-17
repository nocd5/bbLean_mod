/*---------------------------------------------------------------------------------
 bbMacType is a plugin for bbLean
 ----------------------------------------------------------------------------------
 bbMacType is free software, released under the GNU General Public License,
 version 2, as published by the Free Software Foundation.  It is distributed
 WITHOUT ANY WARRANTY--without even the implied warranty of MERCHANTABILITY
 or FITNESS FOR A PARTICULAR PURPOSE.  Please see the GNU General Public
 License for more details:  [http://www.fsf.org/licenses/gpl.html].
 --------------------------------------------------------------------------------*/
#ifndef _BBMACTYPE_H
#define _BBMACTYPE_H
// ================================================================================
#ifndef _WIN32_WINNT
	#define _WIN32_WINNT 0x0500
#endif

#ifndef DLL_EXPORT
	#define DLL_EXPORT __declspec(dllexport)
#endif

#ifdef WIN32_LEAN_AND_MEAN
	#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>

#undef malloc
#define malloc(cb) (GlobalLock(GlobalAlloc((GHND), (cb))))

#undef free
#define free(p) (GlobalUnlock(GlobalHandle(p)), GlobalFree(GlobalHandle(p)))

// Blackbox messages
#define BB_REGISTERMESSAGE   10001
#define BB_UNREGISTERMESSAGE 10002

// --------------------------------------------------------------------------------
#define szVersion     "bbMacType 0.0.1"
#define szAppName     "bbMacType"
#define szInfoVersion "0.0.1"
#define szInfoAuthor  "nocd5"
#define szInfoRelDate "2012-07-07"

extern "C"{
	BOOL WINAPI DllMainCRTStartup(HANDLE hModule, DWORD dwFunction, LPVOID lpNot);
	DLL_EXPORT void endPlugin(HINSTANCE);
	DLL_EXPORT int beginPlugin(HINSTANCE);
	DLL_EXPORT LPCSTR pluginInfo(int index);
};

typedef struct RCSettings {
	DWORD dwHookType;
	LPSTR pszHookLibrary;
	LPSTR pszHookProc;
} RCSETTINGS;

// --------------------------------------------------------------------------------
static int msgs[] = { 0 };
// --------------------------------------------------------------------------------
HINSTANCE  g_hMacType   = NULL;
HHOOK      g_hHook      = NULL;
HWND       g_hPluginWnd = NULL;
HWND       g_BBhWnd     = NULL;
RCSETTINGS g_RCSettings;
// ================================================================================

// DLL NAME
#define szMacType  "MacType.dll"

#define RC_KEY(s) szAppName "." s ":"
#define RC_KEY_HOOK_TYPE RC_KEY("HookType")
#define RC_KEY_HOOK_LIBRARY RC_KEY("HookLibrary")
#define RC_KEY_HOOK_PROC RC_KEY("HookProc")

#endif

