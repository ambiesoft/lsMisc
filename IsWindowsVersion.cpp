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




#include <Windows.h>
#include <tuple>

#include "IsWindowsVersion.h"

namespace Ambiesoft {
	static bool IsWindowsXXXOrAbove_Old(DWORD maCheck, DWORD miCheck, DWORD bnCheck)
	{
		static bool ok = false;
		static OSVERSIONINFOA thisVer = []()
		{
			OSVERSIONINFOA osvi = { 0 };
			osvi.dwOSVersionInfoSize = sizeof(osvi);
#pragma warning(push)
#pragma warning(disable: 4996)
			if (GetVersionExA(&osvi))
#pragma warning(pop)
			{
				ok = true;
			}
			return osvi;
		}();

		return
			std::tie(thisVer.dwMajorVersion, thisVer.dwMinorVersion, thisVer.dwBuildNumber) >=
			std::tie(maCheck, miCheck, bnCheck);
	}

#ifndef STATUS_SUCCESS
#define STATUS_SUCCESS (0x00000000)
#endif
	static bool IsWindowsXXXOrAbove(DWORD maCheck, DWORD miCheck, DWORD bnCheck)
	{
		static bool ok = false;
		static RTL_OSVERSIONINFOW thisVer = []()
		{
			RTL_OSVERSIONINFOW rovi = { 0 };
			typedef LONG(WINAPI* RtlGetVersionPtr)(PRTL_OSVERSIONINFOW);
			HMODULE hMod = ::GetModuleHandleA("ntdll.dll");
			if (hMod)
			{
				RtlGetVersionPtr fxPtr = (RtlGetVersionPtr)::GetProcAddress(hMod, "RtlGetVersion");
				if (fxPtr)
				{
					rovi.dwOSVersionInfoSize = sizeof(rovi);
					if (STATUS_SUCCESS == fxPtr(&rovi))
					{
						ok = true;
					}
				}
			}
			return rovi;
		}();

		if (!ok)
		{
			return IsWindowsXXXOrAbove_Old(maCheck, miCheck, bnCheck);
		}

		return
			std::tie(thisVer.dwMajorVersion, thisVer.dwMinorVersion, thisVer.dwBuildNumber) >=
			std::tie(maCheck, miCheck, bnCheck);
	}

	bool IsWindowsXPOrAbove()
	{
		return IsWindowsXXXOrAbove(5, 1, 0);
	}

	bool IsWindowsVistaOrAbove()
	{
		return IsWindowsXXXOrAbove(6, 0, 0);
	}

	bool IsWindows7OrAbove()
	{
		return IsWindowsXXXOrAbove(6, 1, 0);
	}

	bool IsWindows8OrAbove()
	{
		return IsWindowsXXXOrAbove(6, 2, 0);
	}

	bool IsWindows10OrAbove()
	{
		return IsWindowsXXXOrAbove(10, 0, 0);
	}

	bool IsWindows11OrAbove()
	{
		return IsWindowsXXXOrAbove(10, 0, 22000);
	}
}