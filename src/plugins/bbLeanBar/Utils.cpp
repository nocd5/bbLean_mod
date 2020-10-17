//===========================================================================
// some utilities
//===========================================================================
// Function: arrow_bullet
// Purpose:  draw the triangle bullet
// In:       HDC, position x,y, direction -1 or 1
// Out:
//===========================================================================

bool pSettings_arrowUnix;

void arrow_bullet (HDC buf, int x, int y, int d)
{
	int s = pSettings_bulletUnix ? 1 : 2; int e = d;

	if (pSettings_arrowUnix)
		x-= d*s, d+=d, e = 0;
	else
		s++, x -= d*s/2, d = 0;

	for (int i=-s, j=0; i<=s; i++)
	{
		j+=d;
		MoveToEx(buf, x,   y+i, NULL);
		j+=e;
		LineTo  (buf, x+j, y+i);
		if (0==i) d=-d, e=-e;
	}
}

//===========================================================================
// Function: get_fontheight
//===========================================================================
int get_fontheight(HFONT hFont)
{
	TEXTMETRIC TXM;
	HDC hdc = CreateCompatibleDC(NULL);
	HGDIOBJ other = SelectObject(hdc, hFont);
	GetTextMetrics(hdc, &TXM);
	SelectObject(hdc, other);
	DeleteDC(hdc);
	return TXM.tmHeight;// - TXM.tmInternalLeading - TXM.tmExternalLeading;
}

//===========================================================================

void dbg_printf (const char *fmt, ...)
{
	char buffer[256]; va_list arg;
	va_start(arg, fmt);
	vsprintf (buffer, fmt, arg);
	strcat(buffer, "\n");
	OutputDebugString(buffer);
}

//===========================================================================

#if 1
void EnumTasks (TASKENUMPROC lpEnumFunc, LPARAM lParam)
{
	struct tasklist *tl;
	dolist (tl, GetTaskListPtr())
		if (FALSE == lpEnumFunc(tl, lParam))
			break;
}
#endif

#if 0
void EnumTray (TRAYENUMPROC lpEnumFunc, LPARAM lParam)
{
	for (int i = 0, s = GetTraySize(); i < s; i++)
		if (FALSE == lpEnumFunc(GetTrayIcon(i), lParam))
			break;
}

void EnumDesks (DESKENUMPROC lpEnumFunc, LPARAM lParam)
{
	DesktopInfo info;
	GetDesktopInfo(&info);
	string_node *p = info.deskNames;
	for (int n = 0; n < info.ScreensX; n++)
	{
		DesktopInfo DI;
		DI.number = n;
		DI.deskNames = info.deskNames;
		DI.isCurrent = n == info.number;
		DI.name[0] = 0;
		if (p) strcpy(DI.name, p->str), p = p->next;

		if (FALSE == lpEnumFunc(&DI, lParam))
			break;
	}
}

#endif

bool find_node(void *a0, const void *e0)
{
	list_node *a = (list_node*)a0, *e = (list_node*)e0;
	for ( ; a; a=a->next) if (a->v == e) return true;
	return false;
}

#include <psapi.h>
#include <tlhelp32.h>
//
// �E�B���h�E�n���h������A���̃��W���[���̃t���p�X���擾���܂��B
//
// �p�����[�^
//      hWnd
//          ���ׂ�E�B���h�E�̃n���h��
//      lpszFileName
//          ���W���[���̊��S�C���p�X���󂯎��o�b�t�@�ւ̃|�C���^
//      nSize
//          �擾�����������̍ő�̒���
//
// �߂�l
//      ����ɃE�B���h�E���쐬�������W���[���̃t���p�X�����擾�ł�
//      ���� TRUE ���Ԃ�܂��B����ȊO�� FALSE ���Ԃ�܂��B
//
BOOL GetFileNameFromHwnd(HWND hWnd, LPTSTR lpszFileName, DWORD nSize)
{
	BOOL bResult = FALSE;
	// �E�B���h�E���쐬�����v���Z�X�� ID ���擾
	DWORD dwProcessId;
	GetWindowThreadProcessId(hWnd , &dwProcessId);

	// ���ݎ��s���Ă��� OS �̃o�[�W���������擾
	OSVERSIONINFO osverinfo;
	osverinfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	if ( !GetVersionEx(&osverinfo) )
		return FALSE;

	// �v���b�g�t�H�[���� Windows NT/2000 �̏ꍇ
	if ( osverinfo.dwPlatformId == VER_PLATFORM_WIN32_NT )
	{
		// PSAPI �֐��̃|�C���^
		BOOL (WINAPI *lpfEnumProcessModules)
							(HANDLE, HMODULE*, DWORD, LPDWORD);
		DWORD (WINAPI *lpfGetModuleFileNameEx)
							(HANDLE, HMODULE, LPTSTR, DWORD);

		// PSAPI.DLL ���C�u���������[�h
		HINSTANCE hInstLib = LoadLibrary("PSAPI.DLL");
		if ( hInstLib == NULL )
			return FALSE ;

		// �v���V�[�W���̃A�h���X���擾
		lpfEnumProcessModules = (BOOL(WINAPI *)
			(HANDLE, HMODULE *, DWORD, LPDWORD))GetProcAddress(
						hInstLib, "EnumProcessModules");
		lpfGetModuleFileNameEx =(DWORD (WINAPI *)
			(HANDLE, HMODULE, LPTSTR, DWORD))GetProcAddress(
						hInstLib, "GetModuleFileNameExA");

		if ( lpfEnumProcessModules && lpfGetModuleFileNameEx )
		{
			// �v���Z�X�I�u�W�F�N�g�̃n���h�����擾
			HANDLE hProcess;
			hProcess = OpenProcess(
					PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
					FALSE, dwProcessId);
			if ( hProcess )
			{
				// .EXE ���W���[���̃n���h�����擾
				HMODULE hModule;
				DWORD dwNeed;
				if (lpfEnumProcessModules(hProcess,
							&hModule, sizeof(hModule), &dwNeed))
				{
					// ���W���[���n���h������t���p�X�����擾
					if ( lpfGetModuleFileNameEx(hProcess, hModule,
											lpszFileName, nSize) )
						bResult = TRUE;
				}
				// �v���Z�X�I�u�W�F�N�g�̃n���h�����N���[�Y
				CloseHandle( hProcess ) ;
			}
		}
		// PSAPI.DLL ���C�u�������J��
		FreeLibrary( hInstLib ) ;
	}
	// �v���b�g�t�H�[���� Windows 9x �̏ꍇ
	else if ( osverinfo.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS )
	{
		// ToolHelp �֐��|�C���^
		HANDLE (WINAPI *lpfCreateSnapshot)(DWORD, DWORD);
		BOOL (WINAPI *lpfProcess32First)(HANDLE, LPPROCESSENTRY32);
		BOOL (WINAPI *lpfProcess32Next)(HANDLE, LPPROCESSENTRY32);

		// �v���V�[�W���̃A�h���X���擾
		lpfCreateSnapshot =
			(HANDLE(WINAPI*)(DWORD,DWORD))GetProcAddress(
								GetModuleHandle("kernel32.dll"),
								"CreateToolhelp32Snapshot" );
		lpfProcess32First=
			(BOOL(WINAPI*)(HANDLE,LPPROCESSENTRY32))GetProcAddress(
								GetModuleHandle("kernel32.dll"),
								"Process32First" );
		lpfProcess32Next=
			(BOOL(WINAPI*)(HANDLE,LPPROCESSENTRY32))GetProcAddress(
								GetModuleHandle("kernel32.dll"),
								"Process32Next" );
		if ( !lpfCreateSnapshot ||
			!lpfProcess32First ||
			!lpfProcess32Next)
			return FALSE;

		// �V�X�e�� �v���Z�X�� Toolhelp �X�i�b�v�V���b�g���쐬
		HANDLE hSnapshot;
		hSnapshot = lpfCreateSnapshot(TH32CS_SNAPPROCESS , 0);
		if (hSnapshot != (HANDLE)-1)
		{
			// �ŏ��̃v���Z�X�Ɋւ�������擾
			PROCESSENTRY32 pe;
			pe.dwSize = sizeof(PROCESSENTRY32);
			if ( lpfProcess32First(hSnapshot, &pe) )
			{
				do {
					// �����v���Z�XID �ł���΁A�t�@�C�������擾
					if (pe.th32ProcessID == dwProcessId)
					{
						lstrcpy(lpszFileName, pe.szExeFile);
						bResult = TRUE;
						break;
					}
				} while ( lpfProcess32Next(hSnapshot, &pe) );
			}
			// �X�i�b�v�V���b�g��j��
			CloseHandle(hSnapshot);
		}
	}
	else
		return FALSE;
	GetLongPathName(lpszFileName, lpszFileName, nSize);
	return bResult;
}

LPCSTR set_my_path(char *path, char *fname)
{
	int nLen = GetModuleFileName(g_PI->hInstance, path, MAX_PATH);
	while (nLen && path[nLen-1] != '\\') nLen--;
	strcpy(path+nLen, fname);

	return path;
}

//===========================================================================
static const short vk_codes[] = { VK_SHIFT, VK_CONTROL, VK_LBUTTON, VK_RBUTTON, VK_MBUTTON };
static const char mk_mods[] = { MK_SHIFT, MK_CONTROL, MK_LBUTTON, MK_RBUTTON, MK_MBUTTON };
static const char modkey_strings_r[][6] = { "Shift", "Ctrl", "Left", "Right", "Mid"};
static const char modkey_strings_l[][6] = { "Shift", "Ctrl", "Right", "Left", "Mid"};
static const char button_strings[][10] = { "Left", "Right", "Mid", "Double", "WheelUp", "WheelDown" };
//===========================================================================
unsigned get_modkeys(void){
	unsigned modkey = 0;
	for (int i = 0; i < (int)(sizeof(vk_codes)/sizeof(vk_codes[0])); i++)
		if (0x8000 & GetAsyncKeyState(vk_codes[i])) modkey |= mk_mods[i];

	return modkey;
}

LPCSTR ReadMouseAction(LPARAM button){
	char rc_key[80] = "bbleanbar.tasks.";

	const char (*modkey_strings)[6] = GetSystemMetrics(SM_SWAPBUTTON) ? &modkey_strings_l[0] : &modkey_strings_r[0];

	unsigned modkey = get_modkeys();
	for (int i = 0; i < (int)(sizeof(mk_mods)/sizeof(mk_mods[0])); i++)
		if (mk_mods[i] & modkey) strcat(rc_key, modkey_strings[i]);

	if (button >= (int)(sizeof(button_strings)/sizeof(button_strings[0]))) return false;
	if (button >= 4)
		sprintf(strchr(rc_key, 0), "%s:", button_strings[button]);
	else
		sprintf(strchr(rc_key, 0), "%sClick:", button_strings[button]);

	return ReadString(g_PI->rcpath, rc_key, "");
}

//===========================================================================
MenuItem *MakeMenuSEP(Menu *PluginMenu){
	MenuItem* (*_MakeMenuSEP)(Menu* PluginMenu);
	_MakeMenuSEP = (MenuItem*(*)(Menu*))GetProcAddress((HINSTANCE)GetModuleHandle(NULL),"MakeMenuSEP");
	return _MakeMenuSEP ?  _MakeMenuSEP(PluginMenu) : MakeMenuNOP(PluginMenu);
}
//===========================================================================

//===========================================================================
// Function: BBDrawText
// Purpose: draw text with shadow and/or outline
// In:
// Out:
//===========================================================================
int BBDrawText(HDC hDC, LPCTSTR lpString, int nCount, LPRECT lpRect, UINT uFormat, StyleItem* pSI){
    if (pSI->validated & VALID_SHADOWCOLOR){ // draw shadow
        RECT rcShadow;
        SetTextColor(hDC, pSI->ShadowColor);
        if (pSI->validated & VALID_OUTLINECOLOR){ // draw shadow with outline
            _CopyOffsetRect(&rcShadow, lpRect, 2, 0);
            DrawText(hDC, lpString, nCount, &rcShadow, uFormat); _OffsetRect(&rcShadow,  0, 1);
            DrawText(hDC, lpString, nCount, &rcShadow, uFormat); _OffsetRect(&rcShadow,  0, 1);
            DrawText(hDC, lpString, nCount, &rcShadow, uFormat); _OffsetRect(&rcShadow, -1, 0);
            DrawText(hDC, lpString, nCount, &rcShadow, uFormat); _OffsetRect(&rcShadow, -1, 0);
            DrawText(hDC, lpString, nCount, &rcShadow, uFormat);
        }
        else{
            _CopyOffsetRect(&rcShadow, lpRect, 1, 1);
            DrawText(hDC, lpString, nCount, &rcShadow, uFormat);
        }
    }
    if (pSI->validated & VALID_OUTLINECOLOR){ // draw outline
        RECT rcOutline;
        SetTextColor(hDC, pSI->OutlineColor);
        _CopyOffsetRect(&rcOutline, lpRect, 1, 0);
        DrawText(hDC, lpString, nCount, &rcOutline, uFormat); _OffsetRect(&rcOutline,   0,  1);
        DrawText(hDC, lpString, nCount, &rcOutline, uFormat); _OffsetRect(&rcOutline,  -1,  0);
        DrawText(hDC, lpString, nCount, &rcOutline, uFormat); _OffsetRect(&rcOutline,  -1,  0);
        DrawText(hDC, lpString, nCount, &rcOutline, uFormat); _OffsetRect(&rcOutline,   0, -1);
        DrawText(hDC, lpString, nCount, &rcOutline, uFormat); _OffsetRect(&rcOutline,   0, -1);
        DrawText(hDC, lpString, nCount, &rcOutline, uFormat); _OffsetRect(&rcOutline,   1,  0);
        DrawText(hDC, lpString, nCount, &rcOutline, uFormat); _OffsetRect(&rcOutline,   1,  0);
        DrawText(hDC, lpString, nCount, &rcOutline, uFormat);
    }
    // draw text
    SetTextColor(hDC, pSI->TextColor);
    return DrawText(hDC, lpString, nCount, lpRect, uFormat);
}
