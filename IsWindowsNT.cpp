//Copyright (C) 2017 Ambiesoft All rights reserved.
//
//Redistribution and use in source and binary forms, with or without
//modification, are permitted provided that the following conditions
//are met:
//1. Redistributions of source code must retain the above copyright
//notice, this list of conditions and the following disclaimer.
//2. Redistributions in binary form must reproduce the above copyright
//notice, this list of conditions and the following disclaimer in the
//documentation and/or other materials provided with the distribution.
//
//THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
//ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
//IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
//ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
//FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
//DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
//OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
//HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
//LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
//OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
//SUCH DAMAGE.

#ifndef PCH_INCLUDE_FILE
// Define PCH_INCLUDE_FILE outside of the code
#define PCH_INCLUDE_FILE "StdAfx.h"
#endif


#include <windows.h>
#include "IsWindowsNT.h"

#pragma warning(disable:4996) // deprecated GetVersionExW

BOOL IsWindowsNT(void)
{
    static BOOL once = FALSE;
    static BOOL isNT = FALSE;

    if (!once)
    {
        OSVERSIONINFO osver;
        osver.dwOSVersionInfoSize = sizeof(osver);
        if (GetVersionEx(&osver))
            if (osver.dwPlatformId == VER_PLATFORM_WIN32_NT)
                isNT = TRUE;
        once = TRUE;
    }
    return isNT;
}

BOOL IsWinXPOrHigher()
{
	
	OSVERSIONINFO osver;
    osver.dwOSVersionInfoSize = sizeof(osver);
    if (!GetVersionEx(&osver))
		return FALSE;
    
	// return (OS.Platform == PlatformID.Win32NT) && ((OS.Version.Major > 5) || ((OS.Version.Major == 5) && (OS.Version.Minor >= 1)));
	
	return (osver.dwPlatformId==VER_PLATFORM_WIN32_NT) && 
		(  (osver.dwMajorVersion > 5) || ( (osver.dwMajorVersion==5) && (osver.dwMinorVersion >= 1) )  );

}

BOOL IsWinVistaOrHigher()
{
	OSVERSIONINFO osver;
    osver.dwOSVersionInfoSize = sizeof(osver);
    if (!GetVersionEx(&osver))
		return FALSE;

    // return (OS.Platform == PlatformID.Win32NT) && (OS.Version.Major >= 6);

	return (osver.dwPlatformId==VER_PLATFORM_WIN32_NT) && (osver.dwMajorVersion >= 6);
}