#include <stdio.h>
#include <windows.h>
#include "../bbMemLimiter.h"

enum controls { ID_MAINWINDOW = 401,
                ID_LSVALUE,
                ID_ITVALUE,
                ID_LSLABEL,
                ID_ITLABEL,
                ID_LSUNIT,
                ID_ITUNIT,
                control_size,
                control_null = 0
};

inline controls& operator++( controls& v ) {
    return v = static_cast<controls>( static_cast<int>( v ) + 1 );
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
bool CheckDllVersion();

