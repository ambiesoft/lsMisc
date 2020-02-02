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

#include "stdafx.h"

#ifndef _WINDOWS_
#pragma message("including windows.h")
#include <windows.h>
#endif

#include <shellapi.h>

namespace Ambiesoft {


	BOOL AddTrayIcon(HWND hWnd, UINT dwIDandCallbackMessage, HICON hIcon, LPCTSTR pszTip)
	{
		NOTIFYICONDATA tnd = { 0 };
		tnd.cbSize = sizeof(tnd);
		tnd.hWnd = hWnd;
		tnd.uID = dwIDandCallbackMessage;
		tnd.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
		tnd.uCallbackMessage = dwIDandCallbackMessage;
		tnd.hIcon = hIcon;
		if (pszTip)
			lstrcpyn(tnd.szTip, pszTip, _countof(tnd.szTip) - 1);

		return Shell_NotifyIcon(NIM_ADD, &tnd);
	}

	BOOL PopupTrayIcon(HWND hWnd, UINT dwIDandCallbackMessage, HICON hIcon, LPCTSTR pAppName, LPCTSTR pszTip)
	{
		NOTIFYICONDATA tnd = { 0 };
		tnd.cbSize = sizeof(tnd);
		tnd.hWnd = hWnd;
		tnd.uID = dwIDandCallbackMessage;
		tnd.uFlags = NIF_ICON | NIF_INFO;
		tnd.hIcon = hIcon;
		if (pszTip)
		{
			lstrcpyn(tnd.szInfo, pszTip, _countof(tnd.szTip) - 1);
		}
		lstrcpyn(tnd.szInfoTitle, pAppName, _countof(tnd.szInfoTitle) - 1);
		tnd.dwInfoFlags = NIIF_INFO;

		return Shell_NotifyIcon(NIM_MODIFY, &tnd);
	}

	BOOL RemoveTrayIcon(HWND hWnd, UINT dwIDandCallbackMessage)
	{
		NOTIFYICONDATA tnd = { 0 };
		tnd.cbSize = sizeof(tnd);
		tnd.hWnd = hWnd;
		tnd.uID = dwIDandCallbackMessage;
		tnd.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
		tnd.uCallbackMessage = dwIDandCallbackMessage;

		return Shell_NotifyIcon(NIM_DELETE, &tnd);
	}

} // namespace