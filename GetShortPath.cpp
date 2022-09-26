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
#include <memory>

#include "GetShortPath.h"

namespace Ambiesoft {

	using namespace std;

	static bool isLongpath(const std::wstring& ws)
	{
		if (ws.size() > 4 &&
			ws[0] == L'\\' &&
			ws[1] == L'\\' &&
			ws[2] == L'?' &&
			ws[3] == L'\\')
		{
			return true;
		}
		return false;
	}
	static std::wstring removeLongpath(const std::wstring& ws)
	{
		if (!isLongpath(ws))
			return ws;

		return (ws.c_str() + 4);
	}
	std::wstring stdGetShortPath(const wchar_t* pIN)
	{
		if (!pIN)
			return std::wstring();

		wstring longname = pIN;
		if (isLongpath(longname))
		{
			// already has prepend
		}
		else
		{
			longname = L"\\\\?\\" + longname;
		}

		std::unique_ptr<wchar_t> pOut(new wchar_t[longname.size() * 2]);
		DWORD ret = GetShortPathNameW(longname.c_str(),
			pOut.get(),
			DWORD(longname.size() * 2));
		if (!ret || (ret > (longname.size() * 2)))
			return pIN;

		wstring retstring = pOut.get();
		retstring = removeLongpath(retstring);

		return retstring;
	}
	std::wstring stdGetShortPath(const std::wstring& ws)
	{
		return stdGetShortPath(ws.c_str());
	}

} // namespace Ambiesoft
