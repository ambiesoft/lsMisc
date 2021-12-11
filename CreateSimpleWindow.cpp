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
#include "DebugNew.h"
#include "tstring.h"
#include "CreateSimpleWindow.h"

namespace Ambiesoft {
	static tstring getClassName(LPCTSTR pBase)
	{
		static int sI;
		tstring ret(pBase);
		if (sI > 0)
		{
			TCHAR szT[16];
			wsprintf(szT, _T("%d"), sI);
			ret += szT;
		}
		++sI;
		return ret;
	}
	HWND CreateSimpleWindow(HINSTANCE hInst,
		LPCTSTR pClassName,
		LPCTSTR pWinName,
		WNDPROC WndProc,
		DWORD dwStyles,
		DWORD dwExStyles,
		void* param)
	{
		hInst = hInst ? hInst : GetModuleHandle(NULL);
		tstring className = pClassName ? pClassName : getClassName(TEXT("CreateSimpleWindowClass"));

		WNDCLASSEX wcex;
		if (GetClassInfoEx(hInst, className.c_str(), &wcex))
			UnregisterClass(className.c_str(), hInst);

		memset(&wcex, 0, sizeof(wcex));
		wcex.cbSize = sizeof(WNDCLASSEX);

		wcex.style = 0; //CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = WndProc ? WndProc : DefWindowProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = hInst;
		wcex.hIcon = NULL;
		wcex.hCursor = NULL;
		wcex.hbrBackground = NULL; //(HBRUSH)(COLOR_WINDOW+1);
		wcex.lpszMenuName = NULL;
		wcex.lpszClassName = className.c_str();
		wcex.hIconSm = NULL;

		if (!RegisterClassEx(&wcex))
			return NULL;

		return CreateWindowEx(
			dwExStyles,
			className.c_str(),
			pWinName,
			dwStyles,
			0,
			0,
			0,
			0,
			NULL,
			NULL,
			hInst,
			param);
	}
	HWND CreateSimpleWindow(WNDPROC WndProc)
	{
		return CreateSimpleWindow(NULL,
			NULL,
			NULL,
			WndProc,
			0L,
			0L,
			NULL);
	}
	int WaitSimpleWindowClose(HWND hWnd)
	{
		BOOL bRet;
		MSG msg;

		while ((bRet = GetMessage(&msg, NULL, 0, 0)) != 0)
		{
			if (bRet == -1)
			{
				return -1;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		// Return the exit code to the system. 
		return (int)msg.wParam;
	}
}