/*---------------------------------------------------------------------------------
 bbGdi++ is a plugin for bbLean
 ----------------------------------------------------------------------------------
 bbGdi++ is free software, released under the GNU General Public License,
 version 2, as published by the Free Software Foundation.  It is distributed
 WITHOUT ANY WARRANTY--without even the implied warranty of MERCHANTABILITY
 or FITNESS FOR A PARTICULAR PURPOSE.  Please see the GNU General Public
 License for more details:  [http://www.fsf.org/licenses/gpl.html].
 --------------------------------------------------------------------------------*/
#ifndef _BBGDIPP_H
#define _BBGDIPP_H
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

// Blackbox messages
#define BB_REGISTERMESSAGE   10001
#define BB_UNREGISTERMESSAGE 10002

// --------------------------------------------------------------------------------
#define szVersion     "bbGdi++ 0.0.0"
#define szAppName     "bbGdi++"
#define szInfoVersion "0.0.0"
#define szInfoAuthor  ""
#define szInfoRelDate "2010-11-17"

extern "C"{
	BOOL WINAPI DllMainCRTStartup(HANDLE hModule, DWORD dwFunction, LPVOID lpNot);
	DLL_EXPORT void endPlugin(HINSTANCE);
	DLL_EXPORT int beginPlugin(HINSTANCE);
	DLL_EXPORT LPCSTR pluginInfo(int index);
};

// --------------------------------------------------------------------------------
static int msgs[] = { 0 };
// --------------------------------------------------------------------------------
HINSTANCE g_hGdipp     = NULL;
HHOOK     g_hHook      = NULL;
HWND      g_hPluginWnd = NULL;
HWND      g_BBhWnd     = NULL;
// ================================================================================
#endif

