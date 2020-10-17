#ifndef __BBP42CH_H__
#define __BBP42CH_H__

#ifndef _WIN32_WINNT
	#define _WIN32_WINNT 0x0500
#endif

#include <shlwapi.h>
#include "BBApi.h"

#define DLL_EXPORT __declspec(dllexport)

#define BB_REGISTERMESSAGE   10001
#define BB_UNREGISTERMESSAGE 10002
#define BB_RECONFIGURE       10103
#define BB_BROADCAST         10901

extern "C"{
	BOOL WINAPI DllMainCRTStartup(HANDLE hModule, DWORD dwFunction, LPVOID lpNot);
}

int msgs[] = {BB_BROADCAST, BB_RECONFIGURE, 0};
HWND g_hPluginWnd = NULL;
HWND g_BBhWnd = NULL;

LRESULT CALLBACK MainWinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

LPCSTR strVersion     = "0.0.0";
LPCSTR strAppName     = "bbp42ch";
LPCSTR strInfoVersion = "0.0.0";
LPCSTR strInfoAuthor  = "2ch";
LPCSTR strInfoRelDate = "Jan. 1st, 20XX";
LPCSTR strInfoLink    = "";
LPCSTR strInfoEmail   = "sage";

#endif // __BBP42CH_H__
