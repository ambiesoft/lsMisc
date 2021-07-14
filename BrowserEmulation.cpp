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
#include "Registory.h"

#include "BrowserEmulation.h"

#include "DebugNew.h"

namespace Ambiesoft {
	namespace {
		static const wchar_t* FEATURE_BROWSER_EMULATION = L"Software\\Microsoft\\Internet Explorer\\Main\\FeatureControl\\FEATURE_BROWSER_EMULATION";
	}

	bool GetBrowserEmulation(LPCWSTR pName, DWORD& mode)
	{
		mode = -1;
		Registory reg;
		if (!reg.Open(HKEY_CURRENT_USER, FEATURE_BROWSER_EMULATION))
			return false;

		return reg.Get(pName, mode);
	}
	bool SetBrowserEmulation(LPCWSTR pName, DWORD mode)
	{
		Registory reg;
		reg.OpenOrCreate(HKEY_CURRENT_USER, FEATURE_BROWSER_EMULATION);
		if (!reg)
			return false;
		return reg.Set(pName, mode);
	}
	bool UnsetBrowserEmulation(LPCWSTR pName)
	{
		Registory reg;
		reg.Open(HKEY_CURRENT_USER, FEATURE_BROWSER_EMULATION);
		return reg.Delete(pName);
	}
}