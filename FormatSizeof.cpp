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
#include <string>
#include "DebugNew.h"
#include "FormatSizeof.h"

namespace Ambiesoft {
	std::wstring FormatSizeof(double num)
	{
		static LPCWSTR units[] = { L"", L"K", L"M", L"G", L"T", L"P", L"E", L"Z" };

		std::wstring ret;
		wchar_t szT[64];
		for (int i = 0; i < _countof(units); ++i)
		{
			LPCWSTR pUnit = units[i];
			if (abs(num) < 1024.0)
			{
				swprintf_s(szT, L"%3.1f%s", num, pUnit);
				ret = szT;
				return ret;
			}
			num /= 1024.0;
		}

		// TODO(this looks not corrent)
		_snwprintf_s(szT, _countof(szT), _countof(szT) - 1, L"%.1f%s", num, L"Y");
		ret = szT;
		return ret;
	}
	std::wstring FormatSizeof(long long ll)
	{
		if (ll < 1024)
		{
			wchar_t buff[32] = { 0 };
			_i64tow_s(ll, buff, _countof(buff), 10);
			return buff;
		}

		return FormatSizeof((double)ll);
	}
	std::wstring FormatSizeof(unsigned long long ll)
	{
		if (ll < 1024)
		{
			wchar_t buff[32] = { 0 };
			_i64tow_s(ll, buff, _countof(buff), 10);
			return buff;
		}

		return FormatSizeof((double)ll);
	}
}