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
#include <vector>
#include <string>

#include "GetAllClipboardFormats.h"

#include "DebugNew.h"

namespace Ambiesoft {
	bool GetAllClipboardFormats(std::vector<UINT>* pFormats)
	{
		if (!pFormats)
			return false;

		if (!OpenClipboard(nullptr))
			return false;

		UINT format = 0;
		while ((format = EnumClipboardFormats(format)) != 0) {
			pFormats->push_back(format);
		}

		CloseClipboard();
		return true;
	}
	bool GetAllClipboardFormatsAsString(std::vector<std::wstring>* pFormats)
	{
		if (!pFormats)
			return false;
		if (!OpenClipboard(nullptr))
			return false;
		UINT format = 0;
		while ((format = EnumClipboardFormats(format)) != 0) {
			wchar_t name[256]{};
			int len = GetClipboardFormatNameW(format, name, _countof(name));
			if (len > 0) {
				pFormats->push_back(name);
			}
			else {
				pFormats->push_back(L"");
			}
		}
		CloseClipboard();
		return true;
	}
}
