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

#include "IsWindowsVersion.h"



namespace Ambiesoft {
	static bool IsWindowsXXXOrAboveOrBelow(DWORD ma, DWORD mi, bool bAbove)
	{
		static DWORD major = (DWORD)-1;
		static DWORD minor = (DWORD)-1;
		if (major == -1)
		{
			OSVERSIONINFOA osvi = { 0 };
			osvi.dwOSVersionInfoSize = sizeof(osvi);
#pragma warning(push)
#pragma warning(disable: 4996)
			if (GetVersionExA(&osvi))
#pragma warning(pop)
			{
				major = osvi.dwMajorVersion;
				minor = osvi.dwMinorVersion;
			}
		}
		return bAbove ? (major >= ma && minor >= mi) : (major <= ma && minor <= mi);
	}
	bool IsWindowsXPOrAbove()
	{
		return IsWindowsXXXOrAboveOrBelow(5, 1, true);
	}
	bool IsWindowsXPOrBelow()
	{
		return IsWindowsXXXOrAboveOrBelow(5, 1, false);
	}

	bool IsWindowsVistaOrAbove()
	{
		return IsWindowsXXXOrAboveOrBelow(6, 0, true);
	}
	bool IsWindowsVistaOrBelow()
	{
		return IsWindowsXXXOrAboveOrBelow(6, 0, false);
	}

	bool IsWindows7OrAbove()
	{
		return IsWindowsXXXOrAboveOrBelow(6, 1, true);
	}
	bool IsWindows7OrBelow()
	{
		return IsWindowsXXXOrAboveOrBelow(6, 1, false);
	}

	bool IsWindows8OrAbove()
	{
		return IsWindowsXXXOrAboveOrBelow(6, 2, true);
	}
	bool IsWindows8OrBelow()
	{
		return IsWindowsXXXOrAboveOrBelow(6, 2, false);
	}

#ifndef STATUS_SUCCESS
#define STATUS_SUCCESS (0x00000000)
#endif
	static bool GetRealOSVersion(DWORD ma, DWORD mi, bool bAbove)
	{
		static DWORD major = (DWORD)-1;
		static DWORD minor = (DWORD)-1;
		if (major == -1)
		{
			major = minor = 0;
			// typedef NTSTATUS (WINAPI* RtlGetVersionPtr)(PRTL_OSVERSIONINFOW);
			typedef LONG(WINAPI* RtlGetVersionPtr)(PRTL_OSVERSIONINFOW);

			HMODULE hMod = ::GetModuleHandleA("ntdll.dll");
			if (hMod)
			{
				RtlGetVersionPtr fxPtr = (RtlGetVersionPtr)::GetProcAddress(hMod, "RtlGetVersion");
				if (fxPtr)
				{
					RTL_OSVERSIONINFOW rovi = { 0 };
					rovi.dwOSVersionInfoSize = sizeof(rovi);
					if (STATUS_SUCCESS == fxPtr(&rovi))
					{
						major = rovi.dwMajorVersion;
						minor = rovi.dwMinorVersion;
					}
				}
			}
		}
		return bAbove ? (major >= ma && minor >= mi) : (major <= ma && minor <= mi);
	}


	bool IsWindows10OrAbove()
	{
		return GetRealOSVersion(10, 0, true);
	}
	bool IsWindows10OrBelow()
	{
		return GetRealOSVersion(10, 0, false);
	}

}