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

#include "CHandle.h"
#include "CloseConsoleWin.h"

namespace Ambiesoft {

	namespace {
		class stEP {
			DWORD pid_;
			HWND h_;
		public:
			stEP(DWORD pid) :pid_(pid), h_(nullptr) {
			}
			DWORD pid() const {
				return pid_;
			}
			HWND hwnd() const {
				return h_;
			}
			void setHwnd(HWND h) {
				h_ = h;
			}
		};

		BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
		{
			if (hwnd)
			{
				stEP* pStep = (stEP*)lParam;

				TCHAR szT[256];
				if (0 != GetClassName(hwnd, szT, sizeof(szT) / sizeof(szT[0])))
				{
					if (lstrcmp(szT, L"ConsoleWindowClass") == 0)
					{
						DWORD pid = 0;
						GetWindowThreadProcessId(hwnd, &pid);
						if (pStep->pid() == pid)
						{
							pStep->setHwnd(hwnd);
							return FALSE;
						}
					}
				}
			}
			return TRUE;
		}
	} // namespace


	BOOL CloseConsoleWin(DWORD pid, DWORD maxwait)
	{
		CHandle process(OpenProcess(SYNCHRONIZE, FALSE, pid));
		if (!process)
			return FALSE;

		stEP step(pid);

		EnumWindows(EnumWindowsProc, (LPARAM)&step);

		if (!step.hwnd())
			return FALSE;

		if (!IsWindow(step.hwnd()))
			return FALSE;

		if (!::PostMessage(step.hwnd(), WM_CLOSE, 0, 0))
			return FALSE;

		switch (WaitForSingleObject(process, maxwait))
		{
		case WAIT_OBJECT_0: // signaled
		case WAIT_ABANDONED:
			return TRUE;
		case WAIT_TIMEOUT:
		case WAIT_FAILED:
			break;
		}
		return FALSE;
	}
}