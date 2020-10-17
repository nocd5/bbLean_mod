/* BBAltTray - a BB4W plugin to access the system tray by bro@ms
   Copyright (C) 2004 kana <nicht AT s8 DOT xrea DOT con>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#include "BBApi.h"
#include "BBAltTray.h"

// ===================================================================================
extern "C" BOOL WINAPI DllMainCRTStartup(HANDLE hModule, DWORD dwFunction, LPVOID lpNot){
    return TRUE;
}

// ===================================================================================
int beginPlugin(HINSTANCE hplugininstance){
    g_PluginModule = hplugininstance;
    g_hBBWnd = GetBBWnd();

    read_settings();

    WNDCLASS wc;
    ZeroMemory(&wc, sizeof(wc));
    wc.lpfnWndProc = BBTK_WndProc;
    wc.hInstance = hplugininstance;
    wc.lpszClassName = BBTK_CLASS_NAME;
    if (!RegisterClass(&wc))
        goto E_REGISTER_CLASS;

    g_PluginWnd = CreateWindowEx( WS_EX_TOOLWINDOW | WS_EX_TOPMOST,
                                  BBTK_CLASS_NAME,
                                  NULL,
                                  WS_POPUP,
                                  0, 0, 0, 0,
                                  NULL,
                                  NULL,
                                  hplugininstance,
                                  NULL );
    if (g_PluginWnd == NULL)
        goto E_CREATE_WINDOW;

    return 0;

E_CREATE_WINDOW:
    UnregisterClass(BBTK_CLASS_NAME, hplugininstance);
E_REGISTER_CLASS:
    return !0;
}

// ===================================================================================
void endPlugin(HINSTANCE hplugininstance){
    /* write_settings(); */

    DestroyWindow(g_PluginWnd);
    UnregisterClass(BBTK_CLASS_NAME, hplugininstance);
}

// ===================================================================================
LPCSTR pluginInfo(int field){
    switch (field){
        default: /* FALLTHRU */
        case PLUGIN_NAME:    return BBTK_NAME;
        case PLUGIN_VERSION: return BBTK_VERSION;
        case PLUGIN_AUTHOR:  return BBTK_AUTHOR;
        case PLUGIN_RELEASE: return BBTK_RELEASE;
        case PLUGIN_LINK:    return BBTK_LINK;
        case PLUGIN_EMAIL:   return BBTK_EMAIL;
    }
}


// ===================================================================================
LRESULT CALLBACK BBTK_WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
    switch (msg){
        default:
            break;

        case BB_BROADCAST:
            if (!strnicmp((char*)lParam, "@BBAltTray.", 11)){
                if (!stricmp((char*)lParam+11, "Show")){
                    if (g_status.State == STATE_NOT_SELECTING){
                        action_non_selecting_to_selecting();
                        break;
                    }
                    InvalidateRect(g_PluginWnd, NULL, TRUE);
                }
//                 else {
//                     notice("Unknown bro@m: `%s'", (char*)lParam);
//                 }
            }
            break;

        case BB_RECONFIGURE:
            read_settings();
            break;

        case WM_KILLFOCUS:
            if (g_status.State == STATE_SELECTING) action_selecting_to_non_selecting();
            break;

        case WM_KEYDOWN:
            if (g_status.State == STATE_SELECTING){
                bool bWithShift = GetKeyState(VK_SHIFT) & 0x8000;
                switch (wParam){
                    // -- DOWN -------------------------------------------------------------------------------------------------
                    case 'J' : case VK_DOWN   : move_cursor(+1);                                                          break;
                    // -- UP ---------------------------------------------------------------------------------------------------
                    case 'K' : case VK_UP     : move_cursor(-1);                                                          break;
                    // -- LEFT CLICK -------------------------------------------------------------------------------------------
                    case 'H' : case VK_LEFT   :
                               case VK_RETURN :
                               case VK_SPACE  : send_click_message(bWithShift ? SCM_LEFTDOUBLECLICK   : SCM_LEFTCLICK);   break;
                    // -- RIGHT CLICK ------------------------------------------------------------------------------------------
                    case 'L' : case VK_RIGHT  :
                               case VK_APPS   : send_click_message(bWithShift ? SCM_RIGHTDOUBLECLICK  : SCM_RIGHTCLICK);  break;
                    // -- MIDDLE CLICK -----------------------------------------------------------------------------------------
                    case 'M' :                  send_click_message(bWithShift ? SCM_MIDDLEDOUBLECLICK : SCM_MIDDLECLICK); break;
                    // -- HIDE BBAltTray ---------------------------------------------------------------------------------------
                    case 'Q' : case VK_ESCAPE :
                               case VK_DELETE :
                                                action_selecting_to_non_selecting();                                      break;
                }
            }
            break;

        case WM_CREATE:
            SendMessage(g_hBBWnd, BB_REGISTERMESSAGE, (WPARAM)hWnd, (LPARAM)BBTK_MESSAGES);
            break;

        case WM_DESTROY:
            SendMessage(g_hBBWnd, BB_UNREGISTERMESSAGE, (WPARAM)hWnd, (LPARAM)BBTK_MESSAGES);
            break;

        case WM_PAINT: 
            StyleItem* base_style = (StyleItem*)GetSettingPtr(g_config.BaseStyle);
            StyleItem* task_style = (StyleItem*)GetSettingPtr(g_config.TipStyle);
            StyleItem* pSI;

            PAINTSTRUCT ps;
            HDC hDC = BeginPaint(hWnd, &ps);
            HDC hMemDC = CreateCompatibleDC(hDC);
            SetBkMode(hMemDC, TRANSPARENT);
            HBITMAP hBitmap = CreateCompatibleBitmap(hDC, ps.rcPaint.right, ps.rcPaint.bottom); 
            HBITMAP hBitmapOld = (HBITMAP)SelectObject(hMemDC, hBitmap);
            HFONT hFontOld = (HFONT)SelectObject(hMemDC, CreateStyleFont(base_style));

            RECT r = ps.rcPaint; // RECT r is destroyed
            MakeStyleGradient(hMemDC, &r, base_style, g_config.BaseBorder);

            int height = (((r.bottom - r.top)
                        - (g_config.BasePadding.top + g_config.BasePadding.bottom))
                        / g_status.TraySize)
                        - (g_config.TipPadding.top + g_config.TipPadding.bottom);

            r.left += g_config.BasePadding.left + g_config.TipPadding.left;
            r.top += g_config.BasePadding.top;
            r.right -= g_config.BasePadding.right + g_config.TipPadding.right;
            /* r.bottom */

            for (int i = 0; i < g_status.TraySize; i++){
                systemTray* icon = GetTrayIcon(i);
                if (icon == NULL) break;

                const char* tip;
                char buf[NUMBER_OF(icon->szTip)];
                if (g_config.DoConvertNewLine)
                    tip = convert_new_line(buf,icon->szTip,NUMBER_OF(buf));
                else
                    tip = icon->szTip;

                if (i == g_status.SelectedIcon){
                    pSI = task_style;
                    RECT hilite_rect = r;
                    hilite_rect.left -= g_config.TipPadding.left;
                    hilite_rect.right += g_config.TipPadding.right;
                    hilite_rect.bottom = g_config.TipPadding.top
                                       + hilite_rect.top
                                       + height
                                       + g_config.TipPadding.bottom;
                    MakeStyleGradient( hMemDC, &hilite_rect, pSI, g_config.TipBorder);
                }
                else{
                    pSI = base_style;
                }

                r.top += g_config.TipPadding.top;
                r.bottom = r.top + height;

                int left_old = r.left;
                if (0 != g_config.IconSize){
                    DrawIconEx( hMemDC, r.left, r.top,
                            icon->hIcon,
                            g_config.IconSize, g_config.IconSize,
                            0, NULL, DI_NORMAL );
                    r.left += g_config.IconSize + g_config.IconTipPadding;
                }
                BBDrawText(hMemDC, tip, -1, &r, DT_NOPREFIX | DT_SINGLELINE | DT_VCENTER, pSI);
                r.left = left_old;

                // r.top of next item
                r.top += height + g_config.TipPadding.bottom;

            }
            // hDC <- hMemDC
            BitBlt(hDC, 0, 0, ps.rcPaint.right, ps.rcPaint.bottom, hMemDC, 0, 0, SRCCOPY);
            // delete objects
            DeleteObject(SelectObject(hMemDC, hFontOld));
            DeleteObject(SelectObject(hMemDC, hBitmapOld));
            DeleteDC(hMemDC);
            EndPaint(hWnd, &ps);
            break;
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}

/* Misc. functions */
// ===================================================================================
// void notice(const char* format, ...){
//     char buf[80 * 25];
//     va_list va;
// 
//     va_start(va, format);
//     vsnprintf(buf, NUMBER_OF(buf), format, va);
//     buf[NUMBER_OF(buf) - 1] = '\0';
//     va_end(va);
// 
//     MessageBox( g_PluginWnd, buf, BBTK_NAME,
//                 MB_OK | MB_SETFOREGROUND | MB_TOPMOST );
// }

// ===================================================================================
static BOOL get_rc_path(HINSTANCE hInstance, char *buf, int size){
    int nLen = GetModuleFileName(hInstance, buf, size);
    while (nLen && buf[nLen-1] != '.') nLen--;
    if ((0 == nLen) || (size < nLen + 2 + /* strlen("rc") */ + 1))
        return FALSE;
    strcpy(buf+nLen, "rc");
    return TRUE;
}

// ===================================================================================
const char* convert_new_line(char* _dest, const char* src, int size){
    char* last = _dest + (size - 1);
    char* dest = _dest;

    while (*src != '\0' && dest < last){
        switch (*src){
            default:
                *dest = *src;
                dest++;
                break;

            case '\n':
                if ((last - dest)/sizeof(*dest) - 1 < strlen(g_config.ConvertedNewLine)) goto END;
                strcpy(dest, g_config.ConvertedNewLine);
                dest += strlen(g_config.ConvertedNewLine);
                break;

            case '\r':
                break;
        }
        src++;
    }
END:
    *dest = '\0';
    return _dest;
}

// ===================================================================================
static int get_screen_width(void){
    int w = GetSystemMetrics(SM_CXSCREEN);
    if (!(0 < w))
        w = g_config.DefaultScreenWidth;

    return w;
}

// ===================================================================================
static int get_screen_height(void){
    int h = GetSystemMetrics(SM_CYSCREEN);
    if (!(0 < h))
        h = g_config.DefaultScreenHeight;

    return h;
}

// ===================================================================================
static Padding read_padding(LPCSTR rc, LPCSTR key, LPCSTR default_value){
    LPCSTR s;
    Padding p;

    s = ReadString(rc, key, default_value);

    switch (sscanf( s, "%d %d %d %d",
                &(p.top), &(p.right), &(p.bottom), &(p.left) ))
    {
        default:
            p.top = 0;
        case 1:
            p.right = p.top;
        case 2:
            p.bottom = p.top;
        case 3:
            p.left = p.right;
        case 4:
            break;
    }

    return p;
}

// ===================================================================================
static const struct {const char* name; int number;} STYLE_TABLE[] = {
    {"toolbar", SN_TOOLBAR},
    {"toolbar.label", SN_TOOLBARLABEL},
    {"toolbar.windowlabel", SN_TOOLBARWINDOWLABEL},
    {"toolbar.clock", SN_TOOLBARCLOCK},
    {"menu.title", SN_MENUTITLE},
    {"menu.frame", SN_MENUFRAME},
    {"menu.hilite", SN_MENUHILITE},
};
static int read_style(LPCSTR rc, LPCSTR key, LPCSTR default_value){
    LPCSTR s = ReadString(rc, key, default_value);
    for (size_t i = 0; i < NUMBER_OF(STYLE_TABLE); i++){
        if (!stricmp(s, STYLE_TABLE[i].name))
            return STYLE_TABLE[i].number;
    }

    return SN_TOOLBAR;
}

// ===================================================================================
static BOOL read_bool(LPCSTR rc, LPCSTR key, LPCSTR default_value){
    return !stricmp("true", ReadString(rc, key, default_value));
}

// ===================================================================================
static void read_string(LPCSTR rc, LPCSTR key, LPCSTR default_value, LPSTR buf, int size){
    LPCSTR s = ReadString(rc, key, default_value);
    int l = strlen(s);
    if ((s[0] == '"') && (s[l-1] == '"')){
        s++;
        l -= 2;
    }

    int m = imin(l+1, size);
    strncpy(buf, s, m);
    buf[m-1] = '\0';
}

/* Main functions */
// ===================================================================================
void read_settings(void){
    char rc[MAX_PATH];

    if (!get_rc_path(g_PluginModule, rc, NUMBER_OF(rc))){
//         notice("Failed to get the path to %s", BBTK_RC_NAME);
        return;
    }

    g_config.DefaultScreenWidth = ReadInt(rc, RC_KEY_SCREEN_WIDTH, RC_DEF_SCREEN_WIDTH);
    g_config.DefaultScreenHeight = ReadInt(rc, RC_KEY_SCREEN_HEIGHT, RC_DEF_SCREEN_HEIGHT);
    g_config.BasePadding = read_padding(rc, RC_KEY_BASE_PADDING, RC_DEF_BASE_PADDING);
    g_config.BaseStyle = read_style(rc, RC_KEY_BASE_STYLE, RC_DEF_BASE_STYLE);
    g_config.BaseBorder = read_bool(rc, RC_KEY_BASE_BORDER, RC_DEF_BASE_BORDER);
    g_config.TipPadding = read_padding(rc, RC_KEY_TIP_PADDING, RC_DEF_TIP_PADDING);
    g_config.TipStyle = read_style(rc, RC_KEY_TIP_STYLE, RC_DEF_TIP_STYLE);
    g_config.TipBorder = read_bool(rc, RC_KEY_TIP_BORDER, RC_DEF_TIP_BORDER);
    g_config.IconTipPadding = ReadInt(rc, RC_KEY_ICON_TIP_PADDING, RC_DEF_ICON_TIP_PADDING);
    g_config.IconSize = imax(0, ReadInt(rc, RC_KEY_ICON_SIZE, RC_DEF_ICON_SIZE));
    g_config.DoConvertNewLine = read_bool(rc, RC_KEY_DO_CONVERT_NEW_LINE, RC_DEF_DO_CONVERT_NEW_LINE);
    read_string(rc, RC_KEY_CONVERTED_NEW_LINE, RC_DEF_CONVERTED_NEW_LINE, g_config.ConvertedNewLine, NUMBER_OF(g_config.ConvertedNewLine));
    g_config.AlphaEnable = read_bool(rc, RC_KEY_ALPHA_ENABLE, RC_DEF_ALPHA_ENABLE);
    g_config.AlphaValue = iminmax(0, 255, ReadInt(rc, RC_KEY_ALPHA_VALUE, RC_DEF_ALPHA_VALUE));
}

// ===================================================================================
int BBDrawText(HDC hDC, LPCTSTR lpString, int nCount, LPRECT lpRect, UINT uFormat, StyleItem* pSI){
    if (pSI->validated & VALID_OUTLINECOLOR){ // draw shadow with outline
        if (pSI->validated & VALID_SHADOWCOLOR){ // draw shadow
            SetTextColor(hDC, pSI->ShadowColor);
            _OffsetRect(lpRect,  2,  0); DrawText(hDC, lpString, nCount, lpRect, uFormat);
            _OffsetRect(lpRect,  0,  1); DrawText(hDC, lpString, nCount, lpRect, uFormat);
            _OffsetRect(lpRect,  0,  1); DrawText(hDC, lpString, nCount, lpRect, uFormat);
            _OffsetRect(lpRect, -1,  0); DrawText(hDC, lpString, nCount, lpRect, uFormat);
            _OffsetRect(lpRect, -1,  0); DrawText(hDC, lpString, nCount, lpRect, uFormat);
            _OffsetRect(lpRect,  0, -2);
        }
        SetTextColor(hDC, pSI->OutlineColor);
        _OffsetRect(lpRect,  1,  0); DrawText(hDC, lpString, nCount, lpRect, uFormat);
        _OffsetRect(lpRect,  0,  1); DrawText(hDC, lpString, nCount, lpRect, uFormat);
        _OffsetRect(lpRect, -1,  0); DrawText(hDC, lpString, nCount, lpRect, uFormat);
        _OffsetRect(lpRect, -1,  0); DrawText(hDC, lpString, nCount, lpRect, uFormat);
        _OffsetRect(lpRect,  0, -1); DrawText(hDC, lpString, nCount, lpRect, uFormat);
        _OffsetRect(lpRect,  0, -1); DrawText(hDC, lpString, nCount, lpRect, uFormat);
        _OffsetRect(lpRect,  2,  0); DrawText(hDC, lpString, nCount, lpRect, uFormat);
        _OffsetRect(lpRect, -1,  0); DrawText(hDC, lpString, nCount, lpRect, uFormat);
        _OffsetRect(lpRect,  0,  1);
    }
    else if (pSI->validated & VALID_SHADOWCOLOR){ // draw shadow
        SetTextColor(hDC, pSI->ShadowColor);
        _OffsetRect(lpRect,  1,  1); DrawText(hDC, lpString, nCount, lpRect, uFormat);
        _OffsetRect(lpRect, -1, -1);
    }
    // draw text
    SetTextColor(hDC, pSI->TextColor);
    return DrawText(hDC, lpString, nCount, lpRect, uFormat);
}

/*
static void
write_settings(void)
{
    char rc[MAX_PATH];

    if (!get_rc_path(rc, NUMBER_OF(rc), BBTK_RC_NAME)){
        notice("Failed to get the path to %s", BBTK_RC_NAME);
        return;
    }

    ...
}
*/

/**
 *  ,----------- g_config.BasePadding (left)
 *  | ,--------- g_config.TipPadding (left)
 *  | |   ,----- g_config.IconSize
 *  | |   |  ,-- g_config.IconTipPadding
 *  | |   |  |               ,---- g_config.TipPadding (right)
 *  | |   |  |               | ,-- g_config.BasePadding (right)
 *  | |   |  |               | |
 *  v v   v  v               v v
 * ,----------------------------. ___ g_config.BasePadding (top)
 * | ,------------------------. | ___ g_config.TipPadding (top)
 * | | ,--------------------. | |
 * | | |    | |             | | |
 * | | |icon| | tip...      | | |
 * | | `--------------------' | | ___ g_config.TipPadding (bottom)
 * | |------------------------| | ___ g_config.TipPadding (top)
 * | | ,--------------------. | |
 * | | |    | |             | | |
 * : : :    : :             : : :
 */

// ===================================================================================
static void update_size_and_position(void){
    HDC hDC;
    RECT rect;
    int max_width;
    int max_height;
    int screen_width;
    int screen_height;
    int width;
    int height;
    HGDIOBJ old_font;
    StyleItem* base_style = (StyleItem*)GetSettingPtr(g_config.BaseStyle);

    hDC = GetDC(g_PluginWnd);
    if (hDC == NULL)
        return;
    old_font = SelectObject(hDC, CreateStyleFont(base_style));

    ZeroMemory(&rect, sizeof(rect));
    max_height = g_config.IconSize;
    max_width = 0;
    for (int i = 0; i < g_status.TraySize; i++){
        systemTray* icon;
        char buf[NUMBER_OF(icon->szTip)];
        const char* tip;

        icon = GetTrayIcon(i);
        if (icon == NULL)
            break;

        if (g_config.DoConvertNewLine)
            tip = convert_new_line(buf,icon->szTip,NUMBER_OF(buf));
        else
            tip = icon->szTip;
        DrawText( hDC, tip, -1, &rect,
              DT_CALCRECT | DT_NOPREFIX | DT_SINGLELINE);
        max_height = imax(max_height, rect.bottom);
        max_width  = imax(max_width,  rect.right);
    }

    DeleteObject(SelectObject(hDC, old_font));
    ReleaseDC(g_PluginWnd, hDC);

    width = g_config.BasePadding.left
              + g_config.IconSize + g_config.IconTipPadding
              + (g_config.TipPadding.left + max_width + g_config.TipPadding.right)
              + g_config.BasePadding.right;
    height = g_config.BasePadding.top
               + (g_config.TipPadding.top + max_height + g_config.TipPadding.bottom)
                 * g_status.TraySize
               + g_config.BasePadding.bottom;

    screen_width = get_screen_width();
    screen_height = get_screen_height();

    SetWindowPos(
      g_PluginWnd,
      NULL,
      (screen_width - width) >> 1,
      (screen_height - height) >> 1,
      width,
      height,
      SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOZORDER
    );
}

// ===================================================================================
static int _GetTraySize(void){
    int size = imax(0, GetTraySize());
    for (int i = 0; i < size; i++){
        systemTray* icon = GetTrayIcon(i);
        if ((icon == NULL) || !IsWindow(icon->hWnd)){
            SendMessage(g_hBBWnd, BB_CLEANTRAY, 0, 0);
            size = imax(0, GetTraySize());
            break;
        }
    }

    return size;
}

// ===================================================================================
void action_non_selecting_to_selecting(void){
    g_status.TraySize = _GetTraySize();
    if (0 == g_status.TraySize){
        action_selecting_to_non_selecting();
        return;
    }
    g_status.State = STATE_SELECTING;
    g_status.SelectedIcon = 0;

    update_size_and_position();
    if (g_config.AlphaEnable) SetTransparency(g_PluginWnd, g_config.AlphaValue);
    ShowWindow(g_PluginWnd, SW_SHOW);
    SetForegroundWindow(g_PluginWnd);
}

// ===================================================================================
void action_selecting_to_non_selecting(void){
    g_status.State = STATE_NOT_SELECTING;

    ShowWindow(g_PluginWnd, SW_HIDE);
}

// ===================================================================================
#define POST(icon, msg)                     \
    PostMessage( (icon)->hWnd, (icon)->uCallbackMessage,    \
             (WPARAM)((icon)->uID), (LPARAM)(msg) )
void send_click_message(SCM_XXX scm_xxx){
    systemTray* icon = GetTrayIcon(g_status.SelectedIcon);
    if (icon == NULL) return;

    switch (scm_xxx & 3){
        case SCM_LEFTCLICK:
            POST(icon, WM_LBUTTONDOWN);
            POST(icon, WM_LBUTTONUP);
            if (scm_xxx & 4) {
                POST(icon, WM_LBUTTONDBLCLK);
                POST(icon, WM_LBUTTONUP);
            }
            break;

        case SCM_RIGHTCLICK:
            POST(icon, WM_RBUTTONDOWN);
            POST(icon, WM_RBUTTONUP);
            if (scm_xxx & 4) {
                POST(icon, WM_RBUTTONDBLCLK);
                POST(icon, WM_RBUTTONUP);
            }
            break;

        case SCM_MIDDLECLICK:
            POST(icon, WM_MBUTTONDOWN);
            POST(icon, WM_MBUTTONUP);
            if (scm_xxx & 4) {
                POST(icon, WM_MBUTTONDBLCLK);
                POST(icon, WM_MBUTTONUP);
            }
            break;

        default:
            break;
    }

    SetForegroundWindow(icon->hWnd);
}
#undef POST

// ===================================================================================
void move_cursor(int nDistance){
    g_status.SelectedIcon += nDistance;
    if (g_status.TraySize <= g_status.SelectedIcon)
        g_status.SelectedIcon -= g_status.TraySize;
    else if (g_status.SelectedIcon < 0)
        g_status.SelectedIcon += g_status.TraySize;
    InvalidateRect(g_PluginWnd, NULL, TRUE);
}

/* __END__ */
