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
#include <windows.h>
#include <mmsystem.h>
#include "MasterVolume.h"

#ifdef NOSTDLIB
//===========================================================================
BOOL WINAPI DllMainCRTStartup(HANDLE hModule, DWORD dwFunction, LPVOID lpNot){
    return TRUE;
}
#endif

//===========================================================================
// SetVolume
// Volume in percents (0-100[%])
//===========================================================================
void SetVolume(int nVol){
    GetSetValue(nVol, GSV_SET_VOLUME);
}

//===========================================================================
// GetVolume
// Volume in percents (0-100[%])
//===========================================================================
int GetVolume(){
    return GetSetValue(0, GSV_GET_VOLUME);
}

//===========================================================================
// SetMute
// bMute : true  => set mute
//         false => unset mute
//===========================================================================
void SetMute(bool bMute){
    GetSetValue(bMute, GSV_SET_MUTE);
}

//===========================================================================
// GetMute
// Return : true  => volume is mute
//          false => volume is not mute
//===========================================================================
bool GetMute(){
    return GetSetValue(0, GSV_GET_MUTE);
}

//===========================================================================
int GetSetValue(int nValue, GSV func){
    int nResult = -1;
    HMIXER m_hMixer = NULL;
    MIXERCAPS m_mxcaps;

    MMRESULT mmr;
    // open MIXER 
    mmr = mixerOpen(&m_hMixer, 0, 0, 0, MIXER_OBJECTF_MIXER);
    if (MMSYSERR_NOERROR == mmr){
        mmr = mixerGetDevCaps(reinterpret_cast<UINT>(m_hMixer), &m_mxcaps, sizeof(MIXERCAPS));
        if (MMSYSERR_NOERROR == mmr){
            // get dwLineID
            MIXERLINE mxl;
            mxl.cbStruct = sizeof(MIXERLINE);
            mxl.dwComponentType = MIXERLINE_COMPONENTTYPE_DST_SPEAKERS;
            mmr = mixerGetLineInfo(reinterpret_cast<HMIXEROBJ>(m_hMixer), &mxl, MIXER_OBJECTF_HMIXER | MIXER_GETLINEINFOF_COMPONENTTYPE);
            if (MMSYSERR_NOERROR == mmr){
                // get dwControlID
                MIXERCONTROL mxc;
                MIXERLINECONTROLS mxlc;
                mxlc.cbStruct = sizeof(MIXERLINECONTROLS);
                mxlc.dwLineID = mxl.dwLineID;
                mxlc.dwControlType = ((GSV_SET_VOLUME == func) || (GSV_GET_VOLUME == func)) ? MIXERCONTROL_CONTROLTYPE_VOLUME : MIXERCONTROL_CONTROLTYPE_MUTE;
                mxlc.cControls = 1;
                mxlc.cbmxctrl = sizeof(MIXERCONTROL);
                mxlc.pamxctrl = &mxc;
                mmr = mixerGetLineControls(reinterpret_cast<HMIXEROBJ>(m_hMixer), &mxlc, MIXER_OBJECTF_HMIXER | MIXER_GETLINECONTROLSF_ONEBYTYPE);
                if (MMSYSERR_NOERROR == mmr){
                    DWORD m_dwMinimum = mxc.Bounds.dwMinimum;
                    DWORD m_dwMaximum = mxc.Bounds.dwMaximum;
                    MIXERCONTROLDETAILS_UNSIGNED mxcdVolume;
                    MIXERCONTROLDETAILS mxcd;
                    mxcd.cbStruct = sizeof(MIXERCONTROLDETAILS);
                    mxcd.dwControlID = mxc.dwControlID;
                    mxcd.cChannels = 1;
                    mxcd.cMultipleItems = 0;
                    mxcd.cbDetails = sizeof(MIXERCONTROLDETAILS_UNSIGNED);
                    mxcd.paDetails = &mxcdVolume;
                    if ((GSV_SET_VOLUME == func) || (GSV_SET_MUTE == func)){ // Set Volume/Mute
                        if (GSV_SET_VOLUME == func)
                            mxcdVolume.dwValue = (m_dwMinimum + nValue*(m_dwMaximum-m_dwMinimum)+1)/100;
                        else
                            mxcdVolume.dwValue = nValue;
                        mixerSetControlDetails(reinterpret_cast<HMIXEROBJ>(m_hMixer), &mxcd, MIXER_OBJECTF_HMIXER | MIXER_SETCONTROLDETAILSF_VALUE);
                    }
                    else{ // Get Volume/Mute
                        mmr = mixerGetControlDetails(reinterpret_cast<HMIXEROBJ>(m_hMixer), &mxcd, MIXER_OBJECTF_HMIXER | MIXER_GETCONTROLDETAILSF_VALUE);
                        if (MMSYSERR_NOERROR == mmr){
                            if (GSV_GET_VOLUME == func)
                                nResult = (100*(mxcdVolume.dwValue-m_dwMinimum+1))/(m_dwMaximum-m_dwMinimum);
                            else
                                nResult = mxcdVolume.dwValue;
                        }
                    }
                }
            }
        }
        mixerClose(m_hMixer);
    }
    return nResult;
}

