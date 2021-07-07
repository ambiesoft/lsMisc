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
#include PCH_INCLUDE_FILE

#include <windows.h>

#include "GetHotkeyString.h"

using namespace std;

wstring GetHotkeyStringW(WORD wKey)
{
	wstring ret;
	if(wKey==0)
		return ret;

	if(HIBYTE(wKey) & MOD_CONTROL)
		ret += L"Ctrl+";
	if(HIBYTE(wKey) & MOD_SHIFT)
		ret += L"Shift+";
//	if(HIBYTE(wKey) & MOD_NOREPEAT)
//		ret += L"NoRepeat+";
	if(HIBYTE(wKey) & MOD_WIN)
		ret += L"Win+";
	if(HIBYTE(wKey) & MOD_ALT)
		ret += L"Alt+";

	unsigned int scanCode = MapVirtualKey(LOBYTE(wKey), 0); //MAPVK_VK_TO_VSC);

    // because MapVirtualKey strips the extended bit for some keys
    switch (wKey)
    {
        case VK_LEFT: case VK_UP: case VK_RIGHT: case VK_DOWN: // arrow keys
        case VK_PRIOR: case VK_NEXT: // page up and page down
        case VK_END: case VK_HOME:
        case VK_INSERT: case VK_DELETE:
        case VK_DIVIDE: // numpad slash
        case VK_NUMLOCK:
        {
            scanCode |= 0x100; // set extended bit
            break;
        }
    }

    wchar_t keyName[64] = {0};
    if (GetKeyNameText(scanCode << 16, keyName, sizeof(keyName)/sizeof(keyName[0]) != 0))
    {
        ret += keyName;
    }
    else
    {
        ret += L"[Error]";
    }


	return ret;
}
