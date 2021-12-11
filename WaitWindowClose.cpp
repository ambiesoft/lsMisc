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








#include <windows.h>
#include <tchar.h>
#include <vector>
#include "DebugNew.h"
#define WAITWINDOW_CLASS _T("WaitWindowClass")

namespace Ambiesoft
{
	struct EWPROCST
	{
		std::vector<HWND> vHwnds;
		DWORD dwPID;
	};


	static BOOL CALLBACK ewproc(HWND hwnd, LPARAM lParam)
	{
		if (!IsWindowVisible(hwnd))
			return TRUE;

		EWPROCST& ewst = *((EWPROCST*)lParam);

		DWORD dwPIDwin = 0;
		GetWindowThreadProcessId(hwnd, &dwPIDwin);

		if (ewst.dwPID == dwPIDwin)
		{
			ewst.vHwnds.push_back(hwnd);
		}
		return TRUE;
	}


	static HWND sHwnd;
	static LRESULT CALLBACK WaitWindowProc(
		HWND   hwnd,
		UINT   uMsg,
		WPARAM wParam,
		LPARAM lParam
	)
	{
		static UINT_PTR sTimer;

		switch (uMsg)
		{
		case WM_CREATE:
			sHwnd = hwnd;
			sTimer = SetTimer(hwnd, 1, 5000, NULL);
			if (sTimer == 0)
			{
				DestroyWindow(hwnd);
				break;
			}
			break;

		case WM_TIMER:
		{
			EWPROCST ewst;
			ewst.dwPID = GetCurrentProcessId();
			ewproc(sHwnd, (LPARAM)& ewst);
			if (!EnumWindows(ewproc, (LPARAM)& ewst))
			{
				DestroyWindow(hwnd);
				break;
			}

			if (ewst.vHwnds.size() == 0)
			{
				DestroyWindow(hwnd);
				break;
			}
		}
		break;

		case WM_DESTROY:
			KillTimer(hwnd, sTimer);
			break;

		case WM_NCDESTROY:
			PostQuitMessage(0);
			break;
		}
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	DWORD WaitWindowClose()
	{
		EWPROCST ewst;
		ewst.dwPID = GetCurrentProcessId();

		if (!EnumWindows(ewproc, (LPARAM)& ewst))
		{
			return 0;
		}

		if (ewst.vHwnds.size() == 0)
		{
			return 0;
		}


		WNDCLASS wndcls = { 0 };
		wndcls.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
		wndcls.cbClsExtra = 0;
		wndcls.cbWndExtra = 0;
		wndcls.hInstance = GetModuleHandle(NULL);
		wndcls.hIcon = NULL;
		wndcls.hCursor = LoadCursor(NULL, IDC_IBEAM);
		wndcls.hbrBackground = HBRUSH(COLOR_GRAYTEXT + 1);
		wndcls.lpszMenuName = NULL;
		wndcls.lpszClassName = WAITWINDOW_CLASS;
		wndcls.lpfnWndProc = WaitWindowProc;
		if (0 == RegisterClass(&wndcls))
			return GetLastError();

		sHwnd = CreateWindowEx(
			0, // exstyle
			WAITWINDOW_CLASS,
			_T(""), // window name
			WS_OVERLAPPEDWINDOW,
			0, 0, 0, 0,
			NULL, // parent
			NULL, // menu
			NULL, // inst
			NULL // lparam
		);
		if (!sHwnd)
			return GetLastError();

		MSG msg;
		while (GetMessage(&msg, NULL, 0, 0) > 0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		return 0;
	}
}