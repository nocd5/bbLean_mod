#define NUMBER_OF(array) (sizeof(array) / sizeof((array)[0]))

#ifdef __GNUC__
  #define stricmp   _stricmp
  #define strnicmp  _strnicmp
  #define vsnprintf _vsnprintf
#endif
#define imax(a,b) ((a) > (b) ? (a) : (b))
#define imin(a,b) ((a) < (b) ? (a) : (b))
#define iminmax(a,b,c) (imin(imax((a),(b)),(c)))
#define _CopyRect(lprcDst,lprcSrc) (*lprcDst) = (*lprcSrc)
#define _InflateRect(lprc,dx,dy) (*(lprc)).left -= (dx), (*(lprc)).right += (dx), (*(lprc)).top -= (dy), (*(lprc)).bottom += (dy)
#define _OffsetRect(lprc,dx,dy) (*(lprc)).left += (dx), (*(lprc)).right += (dx), (*(lprc)).top += (dy), (*(lprc)).bottom += (dy)
#define _SetRect(lprc,xLeft,yTop,xRight,yBottom) (*(lprc)).left = (xLeft), (*(lprc)).right = (xRight), (*(lprc)).top = (yTop), (*(lprc)).bottom = (yBottom)
#define _CopyOffsetRect(lprcDst,lprcSrc,dx,dy) (*(lprcDst)).left = (*(lprcSrc)).left + (dx), (*(lprcDst)).right = (*(lprcSrc)).right + (dx), (*(lprcDst)).top = (*(lprcSrc)).top + (dy), (*(lprcDst)).bottom = (*(lprcSrc)).bottom + (dy)

#define RC_KEY(s) "bbalttray." s ":"
#define RC_KEY_SCREEN_WIDTH RC_KEY("screen_width")
#define RC_KEY_SCREEN_HEIGHT RC_KEY("screen_height")
#define RC_KEY_BASE_PADDING RC_KEY("base_padding")
#define RC_KEY_BASE_STYLE RC_KEY("base_style")
#define RC_KEY_BASE_BORDER RC_KEY("base_border")
#define RC_KEY_TIP_PADDING RC_KEY("tip_padding")
#define RC_KEY_TIP_STYLE RC_KEY("tip_style")
#define RC_KEY_TIP_BORDER RC_KEY("tip_border")
#define RC_KEY_ICON_TIP_PADDING RC_KEY("icon_tip_padding")
#define RC_KEY_ICON_SIZE RC_KEY("icon_size")
#define RC_KEY_DO_CONVERT_NEW_LINE RC_KEY("do_convert_new_line")
#define RC_KEY_CONVERTED_NEW_LINE RC_KEY("converted_new_line")
#define RC_KEY_ALPHA_ENABLE RC_KEY("alpha_enable")
#define RC_KEY_ALPHA_VALUE RC_KEY("alpha_value")

#define RC_DEF_SCREEN_WIDTH         640
#define RC_DEF_SCREEN_HEIGHT        480
#define RC_DEF_BASE_PADDING         "0 0 0 0"
#define RC_DEF_BASE_STYLE           "menu.frame"
#define RC_DEF_BASE_BORDER          "false"
#define RC_DEF_TIP_PADDING          "2 13 2 4"
#define RC_DEF_TIP_STYLE            "menu.hilite"
#define RC_DEF_TIP_BORDER           "false"
#define RC_DEF_ICON_TIP_PADDING     4
#define RC_DEF_ICON_SIZE            16
#define RC_DEF_DO_CONVERT_NEW_LINE  "true"
#define RC_DEF_CONVERTED_NEW_LINE   " / "
#define RC_DEF_ALPHA_ENABLE         "false"
#define RC_DEF_ALPHA_VALUE          255

typedef struct {
    int top;
    int right;
    int bottom;
    int left;
} Padding;

/* These are set in read_settings(). */
typedef struct {
    int     DefaultScreenWidth;
    int     DefaultScreenHeight;
    Padding BasePadding;
    int     BaseStyle;
    BOOL    BaseBorder;
    Padding TipPadding;
    int     TipStyle;
    BOOL    TipBorder;
    int     IconTipPadding;
    int     IconSize;
    BOOL    DoConvertNewLine;
    char    ConvertedNewLine[16];
    BOOL    AlphaEnable;
    int     AlphaValue;
} Config;

/**
 * The current state of BBAltTray.
 *
 *                 Next | Prev    Next | Prev
 *                    ---->          ----.
 * ----> (not selecting) (selecting)     |
 * start              <----          <---'
 *                    Cancel
 *
 *                    <----
 *                     Send
 */
enum STATE {
    STATE_NOT_SELECTING,
    STATE_SELECTING
} State;

typedef struct {
    STATE State;
    /* The index of the current selected icon (0 ... TraySize-1). */
    int SelectedIcon;
    /* The number of icons in the system tray (0 <= TraySize). */
    int TraySize;
} Status;
/* Constants */

enum SCM_XXX {
    SCM_LEFTCLICK = 0,
    SCM_RIGHTCLICK,
    SCM_MIDDLECLICK,
    SCM_LEFTDOUBLECLICK = SCM_LEFTCLICK + 4,
    SCM_RIGHTDOUBLECLICK,
    SCM_MIDDLEDOUBLECLICK
};

const char BBTK_NAME[]       = "BBAltTray";
const char BBTK_VERSION[]    = "0.0.2";
const char BBTK_AUTHOR[]     = "kana";
const char BBTK_RELEASE[]    = "2004-12-20/2005-04-06";
const char BBTK_LINK[]       = "http://nicht.s8.xrea.com/2004/12/BBAltTray";
const char BBTK_EMAIL[]      = "nicht AT s8 DOT xrea DOT con";

const char BBTK_CLASS_NAME[] = "BBAltTray";
const UINT BBTK_MESSAGES[]   = {BB_BROADCAST, BB_RECONFIGURE, 0};

/* Global variables */

/**
 * These are set in beginPlugin().
 * And these will never be changed until endPlugin().
 */
HWND    g_PluginWnd = NULL;
HMODULE g_PluginModule = NULL;
HWND    g_hBBWnd;
Config  g_config;
Status  g_status;

// Prototype Declaration
LRESULT CALLBACK BBTK_WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
// void notice(const char* format, ...);
void read_settings(void);
void move_cursor(int nDistance);
void action_non_selecting_to_selecting(void);
void action_selecting_to_non_selecting(void);
void send_click_message(SCM_XXX scm_xxx);
const char* convert_new_line(char* _dest, const char* src, int size);
int BBDrawText(HDC hDC, LPCTSTR lpString, int nCount, LPRECT lpRect, UINT uFormat, StyleItem* pSI);

