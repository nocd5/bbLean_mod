/*
 ============================================================================
  Copyright © 2019 nocd5
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

#ifndef __VOLUMECONTROL_H
#define __VOLUMECONTROL_H

#ifndef DLL_EXPORT
#    define DLL_EXPORT __declspec(dllexport)
#endif

enum GSV {
    GSV_SET_VOLUME,
    GSV_GET_VOLUME,
    GSV_SET_MUTE,
    GSV_GET_MUTE
};

extern "C" {
    BOOL WINAPI DllMainCRTStartup(HANDLE hModule, DWORD dwFunction, LPVOID lpNot);
    DLL_EXPORT void SetVolume(int nVol);
    DLL_EXPORT int  GetVolume();
    //     DLL_EXPORT void ToggleMute();
    DLL_EXPORT void SetMute(bool bMute);
    DLL_EXPORT bool GetMute();
}

int  GetSetValue(int nVol, GSV func);

#endif
