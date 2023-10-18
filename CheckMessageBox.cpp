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
#include <shlwapi.h>
#pragma	comment(lib,"shlwapi.lib")

#include <cassert>

#include "Registory.h"
#include "CheckMessageBox.h"

namespace Ambiesoft {
	int CheckMessageBox(
		HWND hWnd,
		LPCWSTR pText,
		LPCWSTR pCaption,
		UINT uType,
		int iDefault,
		bool* pApplyAll)
	{
		assert(
			(uType & 0x0F) == MB_OKCANCEL ||
			(uType & 0x0F) == MB_YESNO ||
			(uType & 0x0F) == MB_OK);
		assert(
			(uType & 0xF0) == 0 ||
			(uType & 0xF0) == MB_ICONHAND ||
			(uType & 0xF0) == MB_ICONQUESTION ||
			(uType & 0xF0) == MB_ICONEXCLAMATION ||
			(uType & 0xF0) == MB_ICONINFORMATION ||
			(uType & 0xF0) == MB_ICONHAND);

		constexpr wchar_t szRegVal[] = L"{992523B4-E389-4023-8C4D-5522484AEC1A}";
		constexpr wchar_t szSubKey[] = L"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\DontShowMeThisDialogAgain";

		{
			Registory r;
			if (r.Open(HKEY_CURRENT_USER, szSubKey))
			{
				r.Delete(szRegVal);
			}
		}

		int ret = ::SHMessageBoxCheck(
			hWnd,
			pText,
			pCaption,
			uType,
			iDefault,
			szRegVal);

		std::wstring regValue;
		*pApplyAll = !!TrxRegGetValue(HKEY_CURRENT_USER, szSubKey, szRegVal, &regValue);

		return ret;
	}

}