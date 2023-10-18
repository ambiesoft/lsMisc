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

#include "tstring.h"

namespace Ambiesoft
{
	BOOL TrxIsRegKeyExists(HKEY hRoot, LPCTSTR pSub);
	BOOL TrxRegGetValue(HKEY hRoot, LPCTSTR pSub, LPCTSTR pName, tstring* value);


	class Registory
	{
		HKEY hKey_ = nullptr;
	public:
		Registory(){}
		~Registory()
		{
			Close();
		}

		bool Open(HKEY hRoot, LPCWSTR pSub)
		{
			if (hKey_)
				Close();

			RegOpenKeyEx(hRoot,
				pSub,
				0,
				KEY_ALL_ACCESS,
				&hKey_);
			return hKey_ != nullptr;
		}
		bool OpenOrCreate(HKEY hRoot, LPCWSTR pSub)
		{
			if (Open(hRoot, pSub))
				return true;

			RegCreateKey(hRoot,
				pSub,
				&hKey_);
			return hKey_ != nullptr;
		}
		bool Close()
		{
			bool ret = RegCloseKey(hKey_) == ERROR_SUCCESS;
			hKey_ = nullptr;
			return ret;
		}
		bool Get(LPCWSTR pName, DWORD& value) const
		{
			DWORD size = sizeof(value);
			return RegQueryValueEx(hKey_,
				pName,
				0,
				nullptr, // type
				(LPBYTE)&value,
				&size) == ERROR_SUCCESS;
		}
		bool Set(LPCWSTR pName, DWORD value) const
		{
			return RegSetValueEx(hKey_,
				pName,
				0,
				REG_DWORD,
				(const BYTE*)&value,
				sizeof(value)) == ERROR_SUCCESS;
		}
		bool Delete(LPCWSTR pName)
		{
			return RegDeleteValue(hKey_, pName) == ERROR_SUCCESS;
		}
		operator bool() const
		{
			return hKey_ != nullptr;
		}
	};
}