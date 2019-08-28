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
#include <cassert>
#include <map>
#include <set>
#include "EnableTextTripleClickSelectAll.h"

#include "DebugNew.h"

using namespace std;

namespace Ambiesoft {

	static set<HWND> gTargetEdits;
	static map<HWND, int> gDCTickMap;
	static const UINT gDBTime = GetDoubleClickTime();
	static HHOOK gTripleClickHook;
	static LRESULT CALLBACK tripleClickMouseProc(
		_In_ int    nCode,
		_In_ WPARAM wParam,
		_In_ LPARAM lParam
		)
	{
		if (nCode == HC_ACTION)
		{
			do {
				MOUSEHOOKSTRUCT* pMS = (MOUSEHOOKSTRUCT*)lParam;
				if (gTargetEdits.find(pMS->hwnd) == gTargetEdits.end())
					break;  // not registered

				if (wParam == WM_LBUTTONDOWN)
				{
					if (gDCTickMap.find(pMS->hwnd) == gDCTickMap.end())
						break;

					int dbLastTick = gDCTickMap[pMS->hwnd];
					if (gDBTime > (GetTickCount() - dbLastTick))
					{
						PostMessage(pMS->hwnd, EM_SETSEL, 0, -1);
					}
				}
				else if (wParam == WM_LBUTTONDBLCLK)
				{
					gDCTickMap[pMS->hwnd] = GetTickCount();
				}
			} while (false);
		}
		return CallNextHookEx(gTripleClickHook, nCode, wParam, lParam);
	}

	void EnableTextTripleClickSelectAll(HWND hEditORCombo)
	{
		static DWORD gTargetThreadId = GetCurrentThreadId();
		if (gTargetThreadId != GetCurrentThreadId())
		{
			// only one thread can call this function
			assert(false);
			return;
		}
		
		HWND hEdit = hEditORCombo;
		TCHAR szClass[512] = {};
		GetClassName(hEditORCombo, szClass, _countof(szClass));
		if (lstrcmp(szClass, _T("ComboBox"))==0)
			hEdit = ::GetDlgItem(hEditORCombo, 1001);

		gTargetEdits.insert(hEdit);
		if (!gTripleClickHook)
		{
			gTripleClickHook = SetWindowsHookEx(WH_MOUSE, tripleClickMouseProc, NULL, GetCurrentThreadId());
			assert(gTripleClickHook);
		}
	}
}