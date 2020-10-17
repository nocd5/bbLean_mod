/*
 ============================================================================

  This file is part of the bbLean source code
  Copyright � 2001-2003 The Blackbox for Windows Development Team
  Copyright � 2004 grischka

  http://bb4win.sourceforge.net/bblean
  http://sourceforge.net/projects/bb4win

 ============================================================================

  bbLean and bb4win are free software, released under the GNU General
  Public License (GPL version 2 or later), with an extension that allows
  linking of proprietary modules under a controlled interface. This means
  that plugins etc. are allowed to be released under any license the author
  wishes. For details see:

  http://www.fsf.org/licenses/gpl.html
  http://www.fsf.org/licenses/gpl-faq.html#LinkingOverControlledInterface

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
  or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
  for more details.

 ============================================================================
*/

#ifndef BBSETTINGS_H_
#define BBSETTINGS_H_

#ifndef BBSETTING
#define BBSETTING extern
#endif

#include "stylestruct.h"

//===========================================================================
// Settings.cpp functions

void Settings_ReadRCSettings();
void Settings_ReadStyleSettings();
void Settings_WriteRCSetting(const void *);
int Settings_ItemSize(int w);

//=====================================================
// Style definitions

BBSETTING StyleStruct mStyle;

//====================
// Toolbar Config
BBSETTING bool Settings_toolbarOnTop;
BBSETTING bool Settings_toolbarAutoHide;
BBSETTING bool Settings_toolbarPluginToggle;
BBSETTING bool Settings_toolbarEnabled;
BBSETTING char Settings_toolbarPlacement[20];
BBSETTING int Settings_toolbarWidthPercent;
BBSETTING char Settings_toolbarStrftimeFormat[40];
BBSETTING bool Settings_toolbarAlphaEnabled;
BBSETTING int Settings_toolbarAlphaValue;
BBSETTING int Settings_toolbarAlpha;

//====================
// Menu Config
BBSETTING int Settings_menuPositionX;
BBSETTING int Settings_menuPositionY;
BBSETTING bool Settings_menusOnTop;
BBSETTING bool Settings_menuspluginToggle;
BBSETTING int  Settings_menuPopupDelay;
BBSETTING int Settings_menuMousewheelfac;
BBSETTING int Settings_menuMaxWidth;
BBSETTING bool Settings_menusSnapWindow;
BBSETTING char Settings_menuBulletPosition_cfg[16];
BBSETTING bool Settings_menusBroamMode;
BBSETTING bool Settings_menusExtensionSort;
BBSETTING bool Settings_menuAlphaEnabled;
BBSETTING int Settings_menuAlphaValue;
BBSETTING int Settings_menuAlpha;

//====================
// workspaces
BBSETTING bool Settings_workspacesPCo;
BBSETTING bool Settings_restoreToCurrent;
BBSETTING bool Settings_followActive;
BBSETTING bool Settings_altMethod;
BBSETTING int  Settings_workspaces;
BBSETTING char Settings_workspaceNames[256];
BBSETTING bool Settings_followMoved;

//====================
// Snap
BBSETTING int Settings_edgeSnapThreshold;
BBSETTING int Settings_edgeSnapPadding;
BBSETTING bool Settings_edgeSnapPlugins;

//====================
// Desktop Margins
BBSETTING bool Settings_fullMaximization;
BBSETTING RECT Settings_desktopMargin;

//====================
// Background
BBSETTING bool Settings_desktopHook;
BBSETTING bool Settings_background_enabled;
BBSETTING bool Settings_smartWallpaper;
//BBSETTING bool Settings_desktopWheel;

//====================
// Other
BBSETTING bool Settings_opaqueMove;
BBSETTING bool Settings_imageDither;
BBSETTING char Settings_focusModel[32];
BBSETTING int Settings_autoRaiseDelay;
BBSETTING bool Settings_shellContextMenu;
BBSETTING int Settings_LogFlag;

BBSETTING char Settings_preferredEditor[MAX_PATH];
BBSETTING bool Settings_usedefCursor;
BBSETTING bool Settings_arrowUnix;
BBSETTING bool Settings_globalFonts;

BBSETTING bool Settings_newMetrics;

// settngs for bbLean mod
BBSETTING int  Settings_menuVolumeWidth;
BBSETTING int  Settings_menuVolumeHeight;
BBSETTING bool Settings_menuVolumeHilite;
BBSETTING bool Settings_menuKeyBindVI;
BBSETTING int  Settings_menuIconSize;
BBSETTING int  Settings_menuMaxHeightRatio;
BBSETTING bool Settings_menuKeepHilite;
BBSETTING char Settings_recentMenu[MAX_PATH];
BBSETTING int  Settings_recentItemKeepSize;
BBSETTING int  Settings_recentItemSortSize;
BBSETTING bool Settings_recentBeginEnd;
BBSETTING int  Settings_separatorMargin;
BBSETTING char Settings_separatorStyle[12];

typedef struct ext_list
{
    struct ext_list *next;
    char ext[32];
    char key[32];
    StyleItem si;
} ext_list;

BBSETTING ext_list *Settings_menuExtStyle;
//====================
// --- unused *nix settings ---
//BBSETTING bool Settings_focusLastWindow;
//BBSETTING bool Settings_focusNewWindows;
//BBSETTING char Settings_windowPlacement[80];
//BBSETTING char Settings_colPlacementDirection[80];
//BBSETTING char Settings_rowPlacementDirection[80];

//===========================================================================
#endif // BBSETTINGS_H_
