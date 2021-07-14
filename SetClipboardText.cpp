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
// #include "tstring.h"
// #include <string>
#include "SetClipboardText.h"

namespace Ambiesoft {
	BOOL SetClipboardText(HWND hWnd, LPCSTR pSTR)
	{
		if (!pSTR || pSTR[0] == 0)
		{
			return FALSE;
		}

		size_t strsize = lstrlenA(pSTR);
		HGLOBAL h = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE,
			strsize + sizeof(char));

		if (h == NULL)
			return FALSE;

		BOOL bRet = FALSE;
		LPSTR p = (LPSTR)GlobalLock(h);
		lstrcpyA(p, pSTR);
		if (OpenClipboard(hWnd))
		{
			if (EmptyClipboard())
			{
				SetClipboardData(CF_TEXT, h);
			}
			CloseClipboard();
			GlobalUnlock(h);
			bRet = TRUE;
		}
		else
		{
			GlobalFree(h);
			bRet = FALSE;
		}

		return bRet;
	}

	BOOL SetClipboardText(HWND hWnd, LPCWSTR pSTR)
	{
		if (!pSTR || pSTR[0] == 0)
		{
			return FALSE;
		}

		size_t strsize = lstrlenW(pSTR) * sizeof(WCHAR);
		HGLOBAL h = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE,
			strsize + sizeof(WCHAR));

		if (h == NULL)
			return FALSE;

		BOOL bRet = FALSE;
		LPWSTR p = (LPWSTR)GlobalLock(h);
		lstrcpyW(p, pSTR);
		if (OpenClipboard(hWnd))
		{
			if (EmptyClipboard())
			{
				SetClipboardData(CF_UNICODETEXT, h);
			}
			CloseClipboard();
			GlobalUnlock(h);
			bRet = TRUE;
		}
		else
		{
			GlobalFree(h);
			bRet = FALSE;
		}

		return bRet;
	}

}